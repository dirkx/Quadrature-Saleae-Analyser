#ifndef QUADRATUREANALYSER_ANALYZER_H
#define QUADRATUREANALYSER_ANALYZER_H

#include <Analyzer.h>
#include "QuadratureAnalyserAnalyzerResults.h"
#include "QuadratureAnalyserSimulationDataGenerator.h"

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
