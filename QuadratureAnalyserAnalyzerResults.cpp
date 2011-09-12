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
 * $Id: QuadratureAnalyserAnalyzerResults.cpp 1037 2011-09-12 09:49:58Z dirkx $
*/
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

float nunit(char * buff, float f) {
	buff[0] = '\0';
	     if (f < 1e-12) { f *= 1e12; strcpy(buff,"pico"); }
	else if (f < 1e-09) { f *= 1e09; strcpy(buff,"nano"); }
	else if (f < 1e-06) { f *= 1e06; strcpy(buff,"micro"); }
	else if (f < 1e-03) { f *= 1e03; strcpy(buff,"mili"); }
	else if (f > 1e+12) { f /= 1e12; strcpy(buff,"Giga"); }
	else if (f > 1e+06) { f /= 1e06; strcpy(buff,"Mega"); }
	else if (f > 1e+03) { f /= 1e03; strcpy(buff,"Kilo"); }
	return f;
}
		
void QuadratureAnalyserAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
        U32 sample_rate = mAnalyzer->GetSampleRate();
	Frame frame = GetFrame( frame_index );
	char tpsUnit[12];
	char rpsUnit[12];
	char dUnit[12];
	char buff[128];
	char str[512];

	change_t dir = (change_t) (frame.mData1 & 0xF);
	int64_t posCnt =   (frame.mData1 >> 32) & 0xFFFFFFFF;
	U32 gltchCnt = (frame.mData2 >>  0) & 0xFFFFFFFF;
	U32 tocksCnt = (frame.mData2 >> 32) & 0xFFFFFFFF;

	float rotPerSecond;
	float ticksPerSecond;

	U64 delta;
	float dtime, rotation, dutime;

	bzero(tpsUnit,sizeof(tpsUnit));
	bzero(rpsUnit,sizeof(rpsUnit));
	bzero(buff,sizeof(buff));
	bzero(str,sizeof(str));

	ClearResultStrings();

        switch(dir) {
        case    CLOCKWISE:
        case    COUNTERCW:
			snprintf(str, sizeof(str), "%c",
				(dir == CLOCKWISE) ? '+' : '-');
			AddResultString(str);

			snprintf(str, sizeof(str), "%c%d",
				(dir == CLOCKWISE) ? '+' : '-', tocksCnt);
			AddResultString(str);

			snprintf(str, sizeof(str), "%c%d %s",
				(dir == CLOCKWISE) ? '+' : '-', 
				tocksCnt, 
				(dir == CLOCKWISE) ? "(clockwise)" : "(counterclockwise)");
			AddResultString(str);

			delta = frame.mEndingSampleInclusive - frame.mStartingSampleInclusive;
			dtime = (float) delta / sample_rate;
			dutime = nunit(dUnit, dtime);

			snprintf(buff, sizeof(buff), " for %f %sSeconds", dutime, dUnit);
			strncat(str, buff,sizeof(str)-1);
			AddResultString(str);

			if (mSettings->ticksPerRotation != 0) {
			rotation = posCnt / mSettings->ticksPerRotation;

			snprintf(buff, sizeof(buff), " to position %f", rotation);
			strncat(str, buff,sizeof(str)-1);
			AddResultString(str);
			};

			ticksPerSecond = tocksCnt / dtime;
			ticksPerSecond = nunit(tpsUnit, ticksPerSecond);

			snprintf(buff, sizeof(buff), ", speed %f ticks/%sSecond", ticksPerSecond, tpsUnit);
			strncat(str, buff,sizeof(str)-1);
			AddResultString(str);

			if (mSettings->ticksPerRotation != 0) {
			rotPerSecond = tocksCnt / dtime / mSettings->ticksPerRotation;
			rotPerSecond = nunit(rpsUnit, rotPerSecond);

			snprintf(buff, sizeof(buff), " or speed %f degrees/%sSecond", rotPerSecond, rpsUnit);
			strncat(str, buff,sizeof(str)-1);
			AddResultString(str);
			};
                        break;
        case    STANDSTILL:
                        break;
        default:
			AddResultString( "!" );

			AddResultString( "err" );

			snprintf(str, sizeof(str), "%d glitches", gltchCnt);
			AddResultString(str);
                        break;
	};
}

void QuadratureAnalyserAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],till[s], dir,pos,err,count" << std::endl;

	U64 num_frames = GetNumFrames();
	for(U64 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );

                char from[128];
                AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, 0, sample_rate, from, 128 );

                char till[128];
                AnalyzerHelpers::GetTimeString( frame.mEndingSampleInclusive, 0, sample_rate, till, 128 );

		file_stream << from << "," << till;

		change_t dir = (change_t) (frame.mData1 & 0xF);
		int64_t posCnt = (frame.mData1 >> 32) & 0xFFFFFFFF;
		U32 gltchCnt = (frame.mData2 >>  0) & 0xFFFFFFFF;
		U32 tocksCnt = (frame.mData2 >> 32) & 0xFFFFFFFF;

		switch(dir) {
		case CLOCKWISE:  file_stream << ", +1,";
			break;
		case COUNTERCW:  file_stream << ", -1,";
			break;
		case STANDSTILL: file_stream << ",  0,";
			break;
		default:	file_stream << ", err,";
			break;
		};	
		file_stream << posCnt << "," << gltchCnt << "," << tocksCnt << std::endl;
	}

	file_stream.close();
}

void QuadratureAnalyserAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
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
