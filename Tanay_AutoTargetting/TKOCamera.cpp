#include "TKOCamera.h"
#include "Vision/RGBImage.h"

TKOCamera::TKOCamera() {
	m_angle = 0;
}

TKOCamera::~TKOCamera() {}

float TKOCamera::GetAngle() {
	AxisCamera &camera = AxisCamera::GetInstance("10.13.51.11");
	camera.WriteResolution(AxisCamera::kResolution_320x240);
	camera.WriteCompression(10);
	camera.WriteBrightness(15);			
	ParticleFilterCriteria2 criteria[] = {
		{IMAQ_MT_BOUNDING_RECT_WIDTH, 30, 400, false, false},
		{IMAQ_MT_BOUNDING_RECT_HEIGHT, 40, 400, false, false}
	};
	if (camera.IsFreshImage()) {
		RGBImage *cameraImage = new RGBImage();
		camera.GetImage(cameraImage);
		
		Threshold threshold = Threshold(225, 255, 225, 255, 235, 255);
		BinaryImage *thresholdImage = cameraImage->ThresholdRGB(threshold);
		BinaryImage *bigObjectsImage = thresholdImage->RemoveSmallObjects(false, 2);
		BinaryImage *convexHullImage = bigObjectsImage->ConvexHull(false);
		BinaryImage *filteredImage = convexHullImage->ParticleFilter(criteria, 2);	// TODO: change criteria, if necessary
		vector<ParticleAnalysisReport> *particles = filteredImage->GetOrderedParticleAnalysisReports();
		
		Image *img = cameraImage->GetImaqImage();
		ImageInfo info;
		imaqGetImageInfo(img, &info);
		{
			Rect boundingRect = particles->at(0).boundingRect;
			int startingLeftX = boundingRect.left;
			int startingRightX = startingLeftX + boundingRect.width;
			float xvalue = (startingLeftX + startingRightX)/(float)2;
			m_angle = (xvalue)/320 * 54;
			m_angle = m_angle - 27;
		}
		delete particles;
		delete filteredImage;
		delete convexHullImage;
		delete bigObjectsImage;
		delete thresholdImage;
		delete cameraImage;
	}
	return m_angle;
}
