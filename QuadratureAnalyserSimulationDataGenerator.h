/* Copyright 2011 Dirk-Willem van Gulik, All Rights Reserved.
 *                dirkx(at)webweaving(dot)org
 *
  * Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 * 
 * $Id: QuadratureAnalyserSimulationDataGenerator.h 1033 2011-09-09 08:04:49Z dirkx $
 */

#ifndef QUADRATUREANALYSER_SIMULATION_DATA_GENERATOR
#define QUADRATUREANALYSER_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <AnalyzerHelpers.h>

#include <string>

class QuadratureAnalyserAnalyzerSettings;

class QuadratureAnalyserSimulationDataGenerator
{
public:
	QuadratureAnalyserSimulationDataGenerator();
	~QuadratureAnalyserSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, QuadratureAnalyserAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	QuadratureAnalyserAnalyzerSettings* mSettings;

protected:
	SimulationChannelDescriptorGroup mQuadratureSimulationChannels;

	SimulationChannelDescriptor* mChannelA;
	SimulationChannelDescriptor* mChannelB;

	bool initialized;

	ClockGenerator mClockGenerator;
	U32 mSimulationSampleRateHz;

	// state for ticks - as they span generation blocks.
	U32 ticks;
	int dir;
	U64 at;
};
#endif //QUADRATUREANALYSER_SIMULATION_DATA_GENERATOR
