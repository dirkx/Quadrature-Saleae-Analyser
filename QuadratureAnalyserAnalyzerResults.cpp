#include "QuadratureAnalyserAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "QuadratureAnalyserAnalyzer.h"
#include "QuadratureAnalyserAnalyzerSettings.h"
#include <iostream>
#include <fstream>

QuadratureAnalyserAnalyzerResults::QuadratureAnalyserAnalyzerResults( QuadratureAnalyserAnalyzer* analyzer, QuadratureAnalyserAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

QuadratureAnalyserAnalyzerResults::~QuadratureAnalyserAnalyzerResults()
{
}

void QuadratureAnalyserAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	char number_str[128];
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );

	change_t dir = (change_t) (frame.mData1 & 0xF);
	U32 gltchCnt = (frame.mData2 >>  0) & 0xFFFFFFFF;
	U32 tocksCnt = (frame.mData2 >> 32) & 0xFFFFFFFF;

        switch(dir) {
        case    CLOCKWISE:
        case    COUNTERCW:
			snprintf(number_str, sizeof(number_str), "%c",
				(dir == CLOCKWISE) ? '+' : '-');
			AddResultString(number_str);

			snprintf(number_str, sizeof(number_str), "%c%d",
				(dir == CLOCKWISE) ? '+' : '-', tocksCnt);
			AddResultString(number_str);

			snprintf(number_str, sizeof(number_str), "%c%d %s",
				(dir == CLOCKWISE) ? '+' : '-', 
				tocksCnt, 
				(dir == CLOCKWISE) ? "(clockwise)" : "(counterclockwise)");
			AddResultString(number_str);
                        break;
        case    STANDSTILL:
                        break;
        default:
			AddResultString( "!" );

			AddResultString( "err" );

			snprintf(number_str, sizeof(number_str), "%d glitches", gltchCnt);
			AddResultString(number_str);
                        break;
	};
}

void QuadratureAnalyserAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
#if 0
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

		file_stream << time_str << "," << number_str << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
#endif
}

void QuadratureAnalyserAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#if 0
	Frame frame = GetFrame( frame_index );
	ClearResultStrings();

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	AddResultString( number_str );
#endif
}

void QuadratureAnalyserAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	ClearResultStrings();
	AddResultString( "not supported" );
}

void QuadratureAnalyserAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	ClearResultStrings();
	AddResultString( "not supported" );
}
