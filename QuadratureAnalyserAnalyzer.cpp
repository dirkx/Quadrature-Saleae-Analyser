#include "QuadratureAnalyserAnalyzer.h"
#include "QuadratureAnalyserAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

static change_t qe_decode(unsigned char olda, unsigned char oldb, unsigned char newa, unsigned char newb) 
{
     change_t oldnew[16] = {       // AB -> AB  which is (old state) --> (new state)
		     STANDSTILL  , // 00 -> 00
		     CLOCKWISE   , // 00 -> 01 -1
		     COUNTERCW   , // 00 -> 10 +1
		     GLITCH      , // 00 -> 11
		     COUNTERCW   , // 01 -> 00 +1
		     STANDSTILL  , // 01 -> 01
		     GLITCH      , // 01 -> 10 
		     CLOCKWISE   , // 01 -> 11 -1
		     CLOCKWISE   , // 10 -> 00 -1
		     GLITCH      , // 10 -> 01
		     STANDSTILL  , // 10 -> 10
		     COUNTERCW   , // 10 -> 11 +1
		     GLITCH      , // 11 -> 00
		     COUNTERCW   , // 11 -> 01 +1
		     CLOCKWISE   , // 11 -> 10 -1
		     STANDSTILL  , // 11 -> 11
	};
	unsigned char state = 0;
	state += olda ? 1 : 0;
	state += oldb ? 2 : 0;
	state += newa ? 4 : 0;
	state += newb ? 8 : 0;
	return oldnew[state];
}

QuadratureAnalyserAnalyzer::QuadratureAnalyserAnalyzer()
:	Analyzer(),  
	mSettings( new QuadratureAnalyserAnalyzerSettings() )
{
	SetAnalyzerSettings( mSettings.get() );
}

QuadratureAnalyserAnalyzer::~QuadratureAnalyserAnalyzer()
{
	KillThread();
}

void QuadratureAnalyserAnalyzer::WorkerThread()
{
	mResults.reset( new QuadratureAnalyserAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );

	if (mSettings->mInputChannelA != UNDEFINED_CHANNEL)
		mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannelA );
	if (mSettings->mInputChannelB != UNDEFINED_CHANNEL)
		mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannelB );

        AnalyzerChannelData * mChannelA = GetAnalyzerChannelData( mSettings->mInputChannelA );
        AnalyzerChannelData * mChannelB = GetAnalyzerChannelData( mSettings->mInputChannelB );

	U8 mLastA = mChannelA->GetBitState() == BIT_HIGH ? 1 : 0;
	U8 mLastB = mChannelB->GetBitState() == BIT_HIGH ? 1 : 0;

	change_t lastDir = GLITCH;

	U64 lastStart = mChannelA->GetSampleNumber();
	U64 lastEnd = mChannelA->GetSampleNumber();
	U64 curEnd = mChannelA->GetSampleNumber();

	U32 glitchCount = 0;
	U32 tickCount = 0;

	for( ; ; )
	{
		AnalyzerResults::MarkerType m;
		int result = 0;
		// XX fixme - advance to next transition on _either_.
		//
		mChannelA->Advance(1);
		mChannelB->Advance(1);

		U8 mNewA = mChannelA->GetBitState() == BIT_HIGH ? 1 : 0;
		U8 mNewB = mChannelB->GetBitState() == BIT_HIGH ? 1 : 0;

		change_t dir = qe_decode(mLastA, mLastB, mNewA, mNewB);
		mLastA = mNewA;
		mLastB = mNewB;
		if (dir == STANDSTILL)
			continue;

		switch(dir) {
		case 	CLOCKWISE:
		case 	COUNTERCW:
			m = (dir == CLOCKWISE) ? AnalyzerResults::DownArrow : AnalyzerResults::UpArrow;
			mResults->AddMarker( mChannelA->GetSampleNumber(), m, mSettings->mInputChannelA );
			mResults->AddMarker( mChannelB->GetSampleNumber(), m, mSettings->mInputChannelB );
			result ++;
			tickCount++;
			lastEnd = curEnd;
			curEnd = mChannelA->GetSampleNumber(); // fixme - should be A or B if they are not at same pace
			break;
		default:
			glitchCount++;
			break;
		};
		if (dir != lastDir) {
			// skip any initial glitches. We're not sure what they are anyway.
			//
			if (glitchCount != 0 || lastDir != GLITCH) {
				Frame frame;
	
				frame.mData1 = lastDir;
				frame.mData2 = ((U64)tickCount << 32) | (glitchCount << 0);
				frame.mFlags = 0;
	
				frame.mStartingSampleInclusive = lastStart;
				frame.mEndingSampleInclusive = lastEnd;
				mResults->AddFrame( frame );
			}

			lastStart = curEnd;
			lastDir = dir;
			result ++;
			tickCount = 0;
			glitchCount = 0;
		};

		if (result) {
			mResults->CommitResults();
			ReportProgress( mChannelA->GetSampleNumber() );
		};

	}
}

bool QuadratureAnalyserAnalyzer::NeedsRerun()
{
	return false;
}

U32 QuadratureAnalyserAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 QuadratureAnalyserAnalyzer::GetMinimumSampleRateHz()
{
	return SCANRATE;
}

const char* QuadratureAnalyserAnalyzer::GetAnalyzerName() const
{
	return "Quadrature Decoder";
}

const char* GetAnalyzerName()
{
	return "Quadrature Decoder";
}

Analyzer* CreateAnalyzer()
{
	return new QuadratureAnalyserAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
