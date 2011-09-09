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
 * $Id: QuadratureAnalyserAnalyzerSettings.cpp 1033 2011-09-09 08:04:49Z dirkx $
 */

#include "QuadratureAnalyserAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


QuadratureAnalyserAnalyzerSettings::QuadratureAnalyserAnalyzerSettings()
:	mInputChannelA( UNDEFINED_CHANNEL ),
	mInputChannelB( UNDEFINED_CHANNEL ),
	ticksPerRotation( 0 )
{
	mInputChannelAInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelAInterface->SetTitleAndTooltip( "Quadrature A", "Standard Quadrature Decoder - input A (or left/cw/first)" );
	mInputChannelAInterface->SetChannel( mInputChannelA );

	mInputChannelBInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelBInterface->SetTitleAndTooltip( "Quadrature B", "Standard Quadrature Decoder - input Bi (or right/ccw/last)" );
	mInputChannelBInterface->SetChannel( mInputChannelB );

	mTicksPerRotationInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mTicksPerRotationInterface->SetTitleAndTooltip( "Impules/rotation",  
		"Specify the number of rotations per full revolution - or set to '0' to ignore");
	mTicksPerRotationInterface->SetMax( 1e12 );
	mTicksPerRotationInterface->SetMin( 0 );
	mTicksPerRotationInterface->SetInteger( ticksPerRotation);

	AddInterface( mInputChannelAInterface.get() );
	AddInterface( mInputChannelBInterface.get() );
	AddInterface( mTicksPerRotationInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannelA, "Quadrature A", false );
	AddChannel( mInputChannelB, "Quadrature B", false );
}

QuadratureAnalyserAnalyzerSettings::~QuadratureAnalyserAnalyzerSettings()
{
}

bool QuadratureAnalyserAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannelA = mInputChannelAInterface->GetChannel();
	mInputChannelB = mInputChannelBInterface->GetChannel();
	ticksPerRotation = mTicksPerRotationInterface->GetInteger();

	ClearChannels();
	AddChannel( mInputChannelA, "Quadrature A", true);
	AddChannel( mInputChannelB, "Quadrature B", true);

	return true;
}

void QuadratureAnalyserAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelAInterface->SetChannel( mInputChannelA );
	mInputChannelBInterface->SetChannel( mInputChannelB );
	mTicksPerRotationInterface->SetInteger( ticksPerRotation );
}

void QuadratureAnalyserAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannelA;
	text_archive >> mInputChannelB;
	text_archive >> ticksPerRotation;

	ClearChannels();
        AddChannel( mInputChannelA, "Quadrature A", true);
        AddChannel( mInputChannelB, "Quadrature B", true);

	UpdateInterfacesFromSettings();
}

const char* QuadratureAnalyserAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannelA;
	text_archive << mInputChannelB;
	text_archive << ticksPerRotation;

	return SetReturnString( text_archive.GetString() );
}
