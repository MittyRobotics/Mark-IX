/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef TKOGYRO_H_
#define TKOGYRO_H_

#include "SensorBase.h"
#include "PIDSource.h"

class AnalogChannel;
class AnalogModule;

/**
 * Use a rate TKOGyro to return the robots heading relative to a starting position.
 * The TKOGyro class tracks the robots heading based on the starting position. As the robot
 * rotates the new heading is computed by integrating the rate of rotation returned
 * by the sensor. When the class is instantiated, it does a short calibration routine
 * where it samples the TKOGyro while at rest to determine the default offset. This is
 * subtracted from each sample to determine the heading. This TKOGyro class must be used 
 * with a channel that is assigned one of the Analog accumulators from the FPGA. See
 * AnalogChannel for the current accumulator assignments.
 */
class TKOGyro : public SensorBase, public PIDSource
{
public:
	static const UINT32 kOversampleBits = 10;
	static const UINT32 kAverageBits = 0;
	static const float kSamplesPerSecond = 1000.0;
	static const float kCalibrationSampleTime = 5.0;
	static const float kDefaultVoltsPerDegreePerSecond = 0.007;

	TKOGyro(UINT8 moduleNumber, UINT32 channel);
	explicit TKOGyro(UINT32 channel);
	explicit TKOGyro(AnalogChannel *channel);
	explicit TKOGyro(AnalogChannel &channel);
	virtual ~TKOGyro();
	virtual float GetAngle();
	void SetSensitivity(float voltsPerDegreePerSecond);
	virtual void Reset();

	// PIDSource interface
	double PIDGet();

private:
	void InitGyro();

	AnalogChannel *m_analog;
	float m_voltsPerDegreePerSecond;
	float m_offset;
	bool m_channelAllocated;
};
#endif
