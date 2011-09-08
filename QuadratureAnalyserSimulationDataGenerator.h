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
