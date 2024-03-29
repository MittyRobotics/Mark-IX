/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "TKOGyro.h"
#include "AnalogChannel.h"
#include "AnalogModule.h"
#include "Timer.h"
#include "WPIErrors.h"

const UINT32 TKOGyro::kOversampleBits;
const UINT32 TKOGyro::kAverageBits;
const float TKOGyro::kSamplesPerSecond;
const float TKOGyro::kCalibrationSampleTime;
const float TKOGyro::kDefaultVoltsPerDegreePerSecond;

/**
 * Initialize the TKOGyro.
 * Calibrate the TKOGyro by running for a number of samples and computing the center value for this
 * part. Then use the center value as the Accumulator center value for subsequent measurements.
 * It's important to make sure that the robot is not moving while the centering calculations are
 * in progress, this is typically done when the robot is first turned on while it's sitting at
 * rest before the competition starts.
 */
void TKOGyro::InitGyro()
{
	if (!m_analog->IsAccumulatorChannel())
	{
		wpi_setWPIErrorWithContext(ParameterOutOfRange,
				"moduleNumber and/or channel (must be accumulator channel)");
		if (m_channelAllocated)
		{
			delete m_analog;
			m_analog = NULL;
		}
		return;
	}

	m_voltsPerDegreePerSecond = kDefaultVoltsPerDegreePerSecond;
	m_analog->SetAverageBits(kAverageBits);
	m_analog->SetOversampleBits(kOversampleBits);
	float sampleRate = kSamplesPerSecond * 
		(1 << (kAverageBits + kOversampleBits));
	m_analog->GetModule()->SetSampleRate(sampleRate);
	Wait(1.0);

	m_analog->InitAccumulator();
	Wait(kCalibrationSampleTime);

	INT64 value;
	UINT32 count;
	m_analog->GetAccumulatorOutput(&value, &count);
	
	UINT32 center = (UINT32)((float)value / (float)count + .5);

	m_offset = ((float)value / (float)count) - (float)center;
	
	m_analog->SetAccumulatorCenter(center);
	m_analog->SetAccumulatorDeadband(0); ///< TODO: compute / parameterize this
	m_analog->ResetAccumulator();
}

/**
 * TKOGyro constructor given a slot and a channel.
 * 
 * @param moduleNumber The analog module the TKOGyro is connected to (1).
 * @param channel The analog channel the TKOGyro is connected to (1 or 2).
 */
TKOGyro::TKOGyro(UINT8 moduleNumber, UINT32 channel)
{
	m_analog = new AnalogChannel(moduleNumber, channel);
	m_channelAllocated = true;
	InitGyro();
}

/**
 * TKOGyro constructor with only a channel.
 * 
 * Use the default analog module slot.
 * 
 * @param channel The analog channel the TKOGyro is connected to.
 */
TKOGyro::TKOGyro(UINT32 channel)
{
	m_analog = new AnalogChannel(channel);
	m_channelAllocated = true;
	InitGyro();
}

/**
 * TKOGyro constructor with a precreated analog channel object.
 * Use this constructor when the analog channel needs to be shared. There
 * is no reference counting when an AnalogChannel is passed to the TKOGyro.
 * @param channel The AnalogChannel object that the TKOGyro is connected to.
 */
TKOGyro::TKOGyro(AnalogChannel *channel)
{
	m_analog = channel;
	m_channelAllocated = false;
	if (channel == NULL)
	{
		wpi_setWPIError(NullParameter);
	}
	else
	{
		InitGyro();
	}
}

TKOGyro::TKOGyro(AnalogChannel &channel)
{
	m_analog = &channel;
	m_channelAllocated = false;
	InitGyro();
}

/**
 * Reset the TKOGyro.
 * Resets the TKOGyro to a heading of zero. This can be used if there is significant
 * drift in the TKOGyro and it needs to be recalibrated after it has been running.
 */
void TKOGyro::Reset()
{
	m_analog->ResetAccumulator();
}

/**
 * Delete (free) the accumulator and the analog components used for the TKOGyro.
 */
TKOGyro::~TKOGyro()
{
	if (m_channelAllocated)
		delete m_analog;
}

/**
 * Return the actual angle in degrees that the robot is currently facing.
 * 
 * The angle is based on the current accumulator value corrected by the oversampling rate, the
 * TKOGyro type and the A/D calibration values.
 * The angle is continuous, that is can go beyond 360 degrees. This make algorithms that wouldn't
 * want to see a discontinuity in the TKOGyro output as it sweeps past 0 on the second time around.
 * 
 * @return the current heading of the robot in degrees. This heading is based on integration
 * of the returned rate from the TKOGyro.
 */
float TKOGyro::GetAngle( void )
{
	INT64 rawValue;
	UINT32 count;
	m_analog->GetAccumulatorOutput(&rawValue, &count);

	INT64 value = rawValue - (INT64)((float)count * m_offset);

	double scaledValue = value * 1e-9 * (double)m_analog->GetLSBWeight() * (double)(1 << m_analog->GetAverageBits()) /
		(m_analog->GetModule()->GetSampleRate() * m_voltsPerDegreePerSecond);

	return (float)scaledValue;
}


/**
 * Set the TKOGyro type based on the sensitivity.
 * This takes the number of volts/degree/second sensitivity of the TKOGyro and uses it in subsequent
 * calculations to allow the code to work with multiple gyros.
 * 
 * @param voltsPerDegreePerSecond The type of TKOGyro specified as the voltage that represents one degree/second.
 */
void TKOGyro::SetSensitivity( float voltsPerDegreePerSecond )
{
	m_voltsPerDegreePerSecond = voltsPerDegreePerSecond;
}

/**
 * Get the angle in degrees for the PIDSource base object.
 * 
 * @return The angle in degrees.
 */
double TKOGyro::PIDGet()
{
	return GetAngle();
}
