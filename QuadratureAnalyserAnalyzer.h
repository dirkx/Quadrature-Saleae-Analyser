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
 * $Id: QuadratureAnalyserAnalyzer.h 1037 2011-09-12 09:49:58Z dirkx $
 */

#ifndef QUADRATUREANALYSER_ANALYZER_H
#define QUADRATUREANALYSER_ANALYZER_H

#include <Analyzer.h>
#include "QuadratureAnalyserAnalyzerResults.h"
#include "QuadratureAnalyserSimulationDataGenerator.h"

#define MYVERSION "QuadratureAnalyser/1.02"

#define SCANRATE (100)
typedef enum change_t { STANDSTILL = 0, CLOCKWISE, COUNTERCW, GLITCH };

class QuadratureAnalyserAnalyzerSettings;
class ANALYZER_EXPORT QuadratureAnalyserAnalyzer : public Analyzer
{
public:
	QuadratureAnalyserAnalyzer();
	virtual ~QuadratureAnalyserAnalyzer();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< QuadratureAnalyserAnalyzerSettings > mSettings;
	std::auto_ptr< QuadratureAnalyserAnalyzerResults > mResults;

	QuadratureAnalyserSimulationDataGenerator mSimulationDataGenerator;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //QUADRATUREANALYSER_ANALYZER_H
