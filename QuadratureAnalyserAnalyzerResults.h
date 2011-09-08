#ifndef QUADRATUREANALYSER_ANALYZER_RESULTS
#define QUADRATUREANALYSER_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class QuadratureAnalyserAnalyzer;
class QuadratureAnalyserAnalyzerSettings;

class QuadratureAnalyserAnalyzerResults : public AnalyzerResults
{
public:
	QuadratureAnalyserAnalyzerResults( QuadratureAnalyserAnalyzer* analyzer, QuadratureAnalyserAnalyzerSettings* settings );
	virtual ~QuadratureAnalyserAnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	QuadratureAnalyserAnalyzerSettings* mSettings;
	QuadratureAnalyserAnalyzer* mAnalyzer;
};

#endif //QUADRATUREANALYSER_ANALYZER_RESULTS
