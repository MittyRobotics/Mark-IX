#include "Definitions.h"
#include "TKOCamera.h"

class RobotDemo : public SimpleRobot {
	Joystick stick;
	int counter;

public:
	RobotDemo(void):
		stick(STICK_1_PORT)
	{}
	
	bool pixelInRGBThreshold(RGBValue *pixel, Threshold &threshold) {
		return (pixel->R >= threshold.plane1Low && pixel->R <= threshold.plane1High
			 && pixel->G >= threshold.plane2Low && pixel->G <= threshold.plane2High
		 	 && pixel->B >= threshold.plane3Low && pixel->B <= threshold.plane3High);
	}
	
	void GetStartingPoint(int &startingX, int &startingY, ImageInfo &info, Rect &boundingRect, Threshold &threshold, bool leftSide) {
		int horizChange = 1, vertChange = info.pixelsPerLine;
		RGBValue *pixel;
		bool flag = false;
		int counter = 0;
		if (leftSide)
			startingX = boundingRect.left;
		else
			startingX = boundingRect.left + boundingRect.width;
		while (!flag) {
			startingY = boundingRect.top;
			pixel = (RGBValue *)info.imageStart + startingY * info.pixelsPerLine + startingX + counter;
			if (leftSide) {
				startingX++;
				counter += horizChange;
			} else {
				startingX--;
				counter -= horizChange;
			}
			while (startingY <= boundingRect.top + boundingRect.height) {
				if (pixelInRGBThreshold(pixel, threshold)) {
					flag = true;
					break;
				}
				pixel += vertChange;
				startingY++;
			}
		}
		printf("Furthest point = (%d, %d)\n", startingX, startingY);
	}
	
	int FindCorner(Threshold &threshold, ImageInfo &info, int startingX, int startingY, bool leftSide, bool topCorner) {
		RGBValue *pixel = (RGBValue *)info.imageStart + startingY * info.pixelsPerLine + startingX + counter;
		int horizChange = 1, vertChange = info.pixelsPerLine;
		int x = startingX, y = startingY;
		while (abs(x - startingX) <= 8) {
			// Change y (row value)
			if (topCorner) {
				pixel -= vertChange;
				y--;
			} else {
				pixel += vertChange;
				y++;
			}
			// Moving right and up
			while (abs(x - startingX) <= 8 && !pixelInRGBThreshold(pixel, threshold)) {
				if (leftSide) {
					pixel += horizChange;
					x++;
				} else {
					pixel -= horizChange;
					x--;
				}
			}
		}
		printf("Corner = (%d, %d)\n", x, y);
		return y;
	}
	
	void Disabled() {
		printf("ragequit");
	}
	
	void OperatorControl(void) {
		AxisCamera &camera = AxisCamera::GetInstance("10.13.51.11");
		camera.WriteResolution(AxisCamera::kResolution_320x240);
		camera.WriteCompression(10);
		camera.WriteBrightness(15);
		
		counter = 0;
		ParticleFilterCriteria2 criteria[] = {
											{IMAQ_MT_BOUNDING_RECT_WIDTH, 30, 400, false, false},
											{IMAQ_MT_BOUNDING_RECT_HEIGHT, 40, 400, false, false}
		};
		printf("Starting loop\n");
		SDLog("Camera Status", "Starting loop");
		DSLog(3, "Starting loop");
		while (IsOperatorControl())
		{
			if (stick.GetTrigger() == true) {
				if (camera.IsFreshImage())
				{
					DSLog(1, "Vision Testing");
					RGBImage *cameraImage = new RGBImage();
					camera.GetImage(cameraImage);
					
					if (stick.GetRawButton(3)) {
						char str[100];
						sprintf(str, "/vision/img%d.jpg", counter);
						printf("Saving image: %d\n", imaqWriteJPEGFile(cameraImage->GetImaqImage(), str, 750, NULL));
					}
					
//					Threshold threshold = Threshold(225, 255, 175, 255, 175, 255);
					Threshold threshold = Threshold(225, 255, 225, 255, 235, 255);
					BinaryImage *thresholdImage = cameraImage->ThresholdRGB(threshold);
					BinaryImage *bigObjectsImage = thresholdImage->RemoveSmallObjects(false, 2);
					BinaryImage *convexHullImage = bigObjectsImage->ConvexHull(false);
					BinaryImage *filteredImage = convexHullImage->ParticleFilter(criteria, 2);	// TODO: change criteria, if necessary
					vector<ParticleAnalysisReport> *particles = bigObjectsImage->GetOrderedParticleAnalysisReports();
					
					Image *img = cameraImage->GetImaqImage();
					ImageInfo info;
					imaqGetImageInfo(img, &info);
					
					// How to go through entire image
//					RGBValue *pixel = (RGBValue *)info.imageStart;
//					for (int y = 0; y < info.yRes; y++) {
//						printf("\n%d: ", y);
//						for (int x = 0; x < info.xRes; x++) {
//							printf("{%d %d %d %d} ", pixel->R, pixel->G, pixel->B, pixel->alpha);
//							pixel += 1;
//						}
//						pixel += info.pixelsPerLine - info.xRes;
//					}
					
					// for each particle
					{
						// TODO: For first two, need to get top left - bottom left and top right - bottom right
//						int distanceLeftSidePixels = yay(filteredImage->GetImaqImage(), 0, IMAQ_MT_BOUNDING_RECT_LEFT);
						Rect boundingRect = particles->at(0).boundingRect;
						int distanceLeftSidePixels = boundingRect.left;
						
						int startingLeftX, startingLeftY, startingRightX, startingRightY;
						GetStartingPoint(startingLeftX, startingLeftY, info, boundingRect, threshold, true);
						GetStartingPoint(startingRightX, startingRightY, info, boundingRect, threshold, false);
						int topLeft = FindCorner(threshold, info, startingLeftX, startingLeftY, true, true);
						int bottomLeft = FindCorner(threshold, info, startingLeftX, startingLeftY, true, false);
						int topRight = FindCorner(threshold, info, startingRightX, startingRightY, false, true);
						int bottomRight = FindCorner(threshold, info, startingRightX, startingRightY, false, false);
						
						int targetLeftHeightPixels = bottomLeft - topLeft;
						int targetRightHeightPixels = bottomRight - topRight;
						
						float distanceToLeft = DISTANCE_CONSTANT / targetLeftHeightPixels;
						float distanceToRight = DISTANCE_CONSTANT / targetRightHeightPixels;
						printf("pixels (left, right) = (%d, %d)", targetLeftHeightPixels, targetRightHeightPixels);
						float distanceToCenterSquared = .5*distanceToLeft*distanceToLeft + .5*distanceToRight*distanceToRight - QUARTER_TARGET_WIDTH_SQUARED;
						float distanceToCenter = sqrt(distanceToCenterSquared);
						printf("distance to (left, right, center) = (%f %f %f)\n", distanceToLeft, distanceToRight, distanceToCenter);
						float cos = (distanceToRight*distanceToRight - distanceToLeft*distanceToLeft) / (2.*distanceToCenter*TARGET_WIDTH);
						printf("cos = (%f %f %f)\n", distanceToRight*distanceToRight - distanceToLeft*distanceToLeft, 2.*distanceToCenter*TARGET_WIDTH, cos);
						float distanceToHoopSquared = distanceToCenterSquared + BACKBOARD_TO_HOOP_CENTER_SQUARED - 2. * distanceToCenter * BACKBOARD_TO_HOOP_CENTER * sqrt(1-cos*cos);
						float distanceToHoop = sqrt(distanceToHoopSquared);
						float a = TANGENT_HORIZONTAL_CAMERA_ANGLE * (1 - 2*distanceLeftSidePixels/SCREEN_WIDTH_PIXELS);
						float b = (distanceToLeft*distanceToLeft + distanceToHoopSquared - QUARTER_TARGET_WIDTH_SQUARED - BACKBOARD_TO_HOOP_CENTER_SQUARED) / (2*distanceToLeft*distanceToHoop);
						printf("angle (a, b) = (%f %f)\n", a, b);
						float angleError;
//						float xvalue = (float)(startingLeftX + startingRightX)/2;
//						float secondAngleError = (xvalue)/320 * 54;
//						secondAngleError = secondAngleError - 27;
						if (b == 0)
							angleError = atan(a) - PI / 2;
						else {
							float c = sqrt(1 - b*b) / b;
							if (a >= 0) {
								if (c > 0 || a * c < -1)
									angleError = atan((a - c) / (1 + a*c));
								else
									angleError = atan((a - c) / (1 + a*c)) - PI;
							} else {
								if (c > 0 && a * c > -1)
									angleError = atan((a - c) / (1 + a*c));
								else
									angleError = atan((a - c) / (1 + a*c)) - PI;
							}
						}
						angleError *= DEGREES_PER_RADIAN;
						printf("Distance to hoop = %f, angle error = %f\n", distanceToHoop, angleError);
						DSLog (3, "Angle: %f", angleError);
						DSLog (4, "Distance: %f", distanceToHoop);
					}
					
					DSLog(2, "Camera status = %d %d", particles->size(), counter);
					SDLog("Camera Status", "%d %d", particles->size(), counter);
//					printf("%d\n", particles->size());
//					for (unsigned i = 0; i < particles->size(); i++) {
//						printf("%d %d\n", particles->at(i).boundingRect.height, particles->at(i).boundingRect.width);
//					}
					
					// Delete statements: VERY IMPORTANT
					delete particles;
					delete filteredImage;
					delete convexHullImage;
					delete bigObjectsImage;
					delete thresholdImage;
					delete cameraImage;
					printf("counter = %d\n\n", counter++);
					Wait(0.5);
//					printf("ragequit");
				}
			}
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
