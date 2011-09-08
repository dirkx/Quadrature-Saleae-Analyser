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
}

QuadratureAnalyserSimulationDataGenerator::~QuadratureAnalyserSimulationDataGenerator()
{
}

void QuadratureAnalyserSimulationDataGenerator::Initialize( U32 simulation_sample_rate, QuadratureAnalyserAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;
	int first_run = 0;

	if (!initialized) {
		initialized = true;
		if (mSettings->mInputChannelA != UNDEFINED_CHANNEL) {
			if (mChannelA == NULL)
				mChannelA = mQuadratureSimulationChannels.Add( mSettings->mInputChannelA, simulation_sample_rate, BIT_LOW );
				first_run++;
		} else {
			mChannelA = NULL;
		}
	
		if (mSettings->mInputChannelB != UNDEFINED_CHANNEL) {
			if (mChannelB == NULL) 
				mChannelB = mQuadratureSimulationChannels.Add( mSettings->mInputChannelB, simulation_sample_rate, BIT_LOW );
				first_run++;
	
		} else {
			mChannelB = NULL;
		}

		mClockGenerator.Init(SCANRATE /* 10 to 5k */, simulation_sample_rate );

		ticks = 10; dir = 0; at = 0;
	};
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

#if 0
	#define DIRSTR(x) (x ? ((x==1) ? "in clockwise direction" : "in counter clockwise direction") : "standing still")
	fprintf(stderr,"Starting at %lld, run till %lld  - %ld ticks from previous run %s left over\n", 
		at, adjusted_largest_sample_requested, ticks, DIRSTR(dir));
#endif
        while( at < adjusted_largest_sample_requested )
        {
		if (ticks <= 0) {
			U32 speed = 5+(rand() % 11);		// 5 .. 15 tocks	
			U32 time = 500 + (rand() %  4500); 	// 0.5 to 5 seconds.
			dir = rand() % 3;			// in random (or non moving) direction.
			ticks = 2 * speed * time / 1000;
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

		mQuadratureSimulationChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod(1));
		at += mClockGenerator.AdvanceByHalfPeriod(1);

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

