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
 * $Id: QuadratureAnalyserAnalyzerSettings.h 1033 2011-09-09 08:04:49Z dirkx $
 */

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
