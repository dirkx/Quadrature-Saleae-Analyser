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
 * $Id: QuadratureAnalyserSimulationDataGenerator.cpp 1037 2011-09-12 09:49:58Z dirkx $
 */

#include "QuadratureAnalyserSimulationDataGenerator.h"
#include "QuadratureAnalyserAnalyzerSettings.h"
#include "QuadratureAnalyserAnalyzer.h"

#include <AnalyzerHelpers.h>
#include <assert.h>

QuadratureAnalyserSimulationDataGenerator::QuadratureAnalyserSimulationDataGenerator()
{
	initialized = false;
	mChannelA = NULL;
	mChannelB = NULL;
	ticks = 10; dir = 0; at = 0;
	runtime  = 0; speed = 1;
}

QuadratureAnalyserSimulationDataGenerator::~QuadratureAnalyserSimulationDataGenerator()
{
}

void QuadratureAnalyserSimulationDataGenerator::Initialize( U32 simulation_sample_rate, QuadratureAnalyserAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	if (initialized)
		return;
	initialized = true;


	if (mSettings->mInputChannelA != UNDEFINED_CHANNEL) {
		if (mChannelA == NULL)
			mChannelA = mQuadratureSimulationChannels.Add( mSettings->mInputChannelA, simulation_sample_rate, BIT_LOW );
	} else {
		mChannelA = NULL;
	}

	if (mSettings->mInputChannelB != UNDEFINED_CHANNEL) {
		if (mChannelB == NULL) 
			mChannelB = mQuadratureSimulationChannels.Add( mSettings->mInputChannelB, simulation_sample_rate, BIT_LOW );

	} else {
		mChannelB = NULL;
	}

	mClockGenerator.Init(SCANRATE /* 10 to 5k */, simulation_sample_rate );
	srand(-time(NULL) ^ getpid());
}

U32 QuadratureAnalyserSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
        U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( 
		largest_sample_requested, sample_rate, mSimulationSampleRateHz );
	U64 tA = 0, tB = 0;

#if 0
	// keep getting mChannelA related core dumps. 
	U64 at;
#define _gcsn(a) (((a) == NULL) ? 0 : (a)->GetCurrentSampleNumber())
#define _mmax(a,b) (_gcsn(a) > _gcsn(b) ? _gcsn(a) : _gcsn(b))
	at = _mmax(mChannelA,mChannelB);
#endif

	#define DIRSTR(x) (x ? ((x==1) ? "in clockwise direction" : "in counter clockwise direction") : "standing still")
#if 0
	fprintf(stderr,"Starting at %lld, run till %lld  - %ld ticks from previous run %s left over\n", 
		at, adjusted_largest_sample_requested, ticks, DIRSTR(dir));
#endif
        while( at < adjusted_largest_sample_requested )
        {
		if (ticks <= 0) {
			speed = 5+(rand() % 11);		// 5 .. 15 tocks/second
			runtime  = 500 + (rand() %  4500); 	// 0.5 to 5 seconds.
			dir = rand() % 3;			// in random (or non moving) direction.
			ticks = 2 * speed * runtime  / 1000;
	// fprintf(stderr,"speed %d, length %d, ticks %d, %s\n", speed, runtime , ticks, DIRSTR(dir));
		};

		switch(dir) {
		case 1: if (mChannelA) {
				mChannelA->Transition(); 
				tA++;
			};
			dir = 2;
			break;
		case 2: if (mChannelB) {
				mChannelB->Transition(); 
				tB++;
			};
			dir = 1;
			break;
		case 0: // stand still;
			break;
		default:
			fprintf(stderr,"Eh - nothing\n");
		};
		
		U32 dtime = mClockGenerator.AdvanceByHalfPeriod(1) / (speed+1) * SCANRATE / 10;
		mQuadratureSimulationChannels.AdvanceAll(dtime);
		at += dtime;

		if (ticks) ticks --;
#if 0
		fprintf(stderr,"loop done - we are at %lld - going to %lld\n", at, adjusted_largest_sample_requested);
#endif
	}
#if 0
	fprintf(stderr,"Returning %d channels till sample %lld." 
		"There are %lld transitions (A:%lld, B:%lld) in this run. "
		"Still %ld ticks %s left to do.\n",
		mQuadratureSimulationChannels.GetCount(),
		at, tA+tB, tA, tB, ticks, DIRSTR(dir)
	);
#endif

	*simulation_channel = mQuadratureSimulationChannels.GetArray();
	return mQuadratureSimulationChannels.GetCount();
}

