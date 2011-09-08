#ifndef QUADRATUREANALYSER_ANALYZER_SETTINGS
#define QUADRATUREANALYSER_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class QuadratureAnalyserAnalyzerSettings : public AnalyzerSettings
{
public:
	QuadratureAnalyserAnalyzerSettings();
	virtual ~QuadratureAnalyserAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();
	
	Channel mInputChannelA;
	Channel mInputChannelB;
	U32 ticksPerRotation;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelAInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelBInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mTicksPerRotationInterface;
};

#endif //QUADRATUREANALYSER_ANALYZER_SETTINGS
