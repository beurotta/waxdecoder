//-----------------------------------------------------------------------------
//@file  
//	WaxDecoder.cpp
//
//@author
//	Arnaud BEUROTTE aka 'naarud'
//
//@brief 
//	Implementation of the WaxDecoder class.
//
//@historic 
//	2012/07/04
//    first release, implement the timecoder used in v1.2 of xwax
//  2018/11/16
//    updated to Usine SDK HH3-7.01.006
//    based on xwax v1.7 sources
//
//@IMPORTANT
// All dependencies are under there own licence.
//
//@LICENCE
//
// Copyright (c) 2012, 2018 Arnaud BEUROTTE
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include "WaxDecoder.h"

//-----------------------------------------------------------------------------
// create, general info and destroy methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void CreateModule (void* &pModule, AnsiCharPtr optionalString, LongBool Flag, MasterInfo* pMasterInfo, AnsiCharPtr optionalContent)
{
	pModule = new WaxDecoder();
}

//-----------------------------------------------------------------------------
void DestroyModule(void* pModule) 
{
	delete ((WaxDecoder*)pModule);
}

//-----------------------------------------------------------------------------
// module constants for browser info and module info
const AnsiCharPtr UserModuleBase::MODULE_NAME = "WaxDecoder";
const AnsiCharPtr UserModuleBase::MODULE_DESC = "Timecoded Vinyl/CD interpreter";
const AnsiCharPtr UserModuleBase::MODULE_VERSION = "20181116";

//-----------------------------------------------------------------------------
void GetBrowserInfo(ModuleInfo* pModuleInfo) 
{
	pModuleInfo->Name				= UserModuleBase::MODULE_NAME;
	pModuleInfo->Description		= UserModuleBase::MODULE_DESC;
	pModuleInfo->Version			= UserModuleBase::MODULE_VERSION;
}


//-----------------------------------------------------------------------------
// module constructors/destructors and description
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
WaxDecoder::WaxDecoder()
{
	usineBlockSize = 0;
	usineSmplRate = 0;
	target_position = TARGET_UNKNOWN;
	pitch = 0.;
    lbxTimecodes = 0;
    lbxRpmSpeed = 0;
    lbxSoftPA = 0;
};

//-----------------------------------------------------------------------------
WaxDecoder::~WaxDecoder()
{
	if (pcm != NULL)
		delete [] pcm;
}

//-----------------------------------------------------------------------------
void WaxDecoder::onGetModuleInfo (MasterInfo* pMasterInfo, ModuleInfo* pModuleInfo)
{
	pModuleInfo->Name				= MODULE_NAME;
	pModuleInfo->Description		= MODULE_DESC;
	pModuleInfo->ModuleType         = mtSimple;
	pModuleInfo->BackColor          = sdkGetUsineColor(clAudioModuleColor);
	pModuleInfo->Version			= MODULE_VERSION;
	pModuleInfo->NumberOfParams     = 4;
}


//-----------------------------------------------------------------------------
// query system and init methods
//-----------------------------------------------------------------------------

// query system not used

//-----------------------------------------------------------------------------
void WaxDecoder::onInitModule (MasterInfo* pMasterInfo, ModuleInfo* pModuleInfo) {
    
	// init timecoder to 'serato_2a' timecode at 33 rpm without using a 'software' preamp
	loadTimecoder(TC_NAMES[0], RPM_SPEED[0], SOFT_PREAMP[0]);

	// usine block size
	usineBlockSize = sdkGetBlocSize();
	pcm = new signed short[usineBlockSize * 2];
}


//----------------------------------------------------------------------------
// parameters and process
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// parameters description
void WaxDecoder::onGetParamInfo (int ParamIndex, TParamInfo* pParamInfo)
{	
	switch (ParamIndex) 
    {
    // audio in left
	case 0:
		pParamInfo->ParamType		= ptAudio;
		pParamInfo->Caption			= "in L";
		pParamInfo->IsInput			= true;
		pParamInfo->IsOutput		= false;
		pParamInfo->ReadOnly		= false;
		pParamInfo->EventPtr        = &audioInputTab[0];
		break;
    // audio in right
	case 1:
		pParamInfo->ParamType		= ptAudio;
		pParamInfo->Caption			= "in R";
		pParamInfo->IsInput			= true;
		pParamInfo->IsOutput		= false;
		pParamInfo->ReadOnly		= false;
		pParamInfo->EventPtr        = &audioInputTab[1];
		break;
	// position output
	case 2:
		pParamInfo->ParamType		= ptDataField;
		pParamInfo->Caption			= "position";
		pParamInfo->IsInput			= false;
		pParamInfo->IsOutput		= true;
		pParamInfo->MinValue		= 0.0;
		pParamInfo->MaxValue		= 1200.0;
		pParamInfo->DefaultValue	= 0.0;
		pParamInfo->Symbol			= "s";
		pParamInfo->Format			= "%g";
		pParamInfo->ReadOnly		= true;
		pParamInfo->EventPtr        = &dtfPositionOut;
		break;
	// pitch output
	case 3:
		pParamInfo->ParamType		= ptDataField;
		pParamInfo->Caption			= "pitch";
		pParamInfo->IsInput			= false;
		pParamInfo->IsOutput		= true;
		pParamInfo->MinValue		= -30.0;
		pParamInfo->MaxValue		= 30.0;
		pParamInfo->DefaultValue	= 0.0;
		pParamInfo->Symbol			= "";
		pParamInfo->Format			= "%g";
		pParamInfo->ReadOnly		= true;
		pParamInfo->EventPtr        = &dtfPitchOut;
		break;
	// default case
	default:
		break;
	}
}


//-----------------------------------------------------------------------------
void WaxDecoder::onCallBack (UsineMessage *Message) 
{
}

//-----------------------------------------------------------------------------
// populate properties tab with hardware settings (disc manufacturer, RPM speed and use of software phono preamp)
void WaxDecoder::onCreateSettings()
{
	sdkAddSettingLineCaption(PROPERTIES_TAB_NAME, "hardware settings");
	sdkAddSettingLineCombobox(PROPERTIES_TAB_NAME, &lbxTimecodes, "timecode", "\"serato_2a\",\"serato_2b\",\"serato_cd\",\"traktor_a\",\"traktor_b\",\"mixvibes_v2\",\"mixvibes_7inch\"");
	sdkAddSettingLineCombobox(PROPERTIES_TAB_NAME, &lbxRpmSpeed, "rpm", "\"33\",\"45\"");
	sdkAddSettingLineCombobox(PROPERTIES_TAB_NAME, &lbxSoftPA, "software phono preamp", "\"no\",\"yes\"");
}

//-----------------------------------------------------------------------------
void WaxDecoder::onSettingsHasChanged()
{
	loadTimecoder(TC_NAMES[lbxTimecodes], RPM_SPEED[lbxRpmSpeed], SOFT_PREAMP[lbxSoftPA]);
} 

//-----------------------------------------------------------------------------
void WaxDecoder::onBlocSizeChange (int BlocSize)     
/* nothing to do ? Usine asks to reboot after a block size change.
 * update the module even if Usine is not restarted */
{
	usineBlockSize = BlocSize;

	if (pcm != NULL)
		delete[] pcm;

	pcm = new signed short[usineBlockSize * 2];
}

//-----------------------------------------------------------------------------
void WaxDecoder::onSampleRateChange (double SampleRate)
{
    loadTimecoder(TC_NAMES[lbxTimecodes], RPM_SPEED[lbxRpmSpeed], SOFT_PREAMP[lbxSoftPA]);
}

//-------------------------------------------------------------------------
void WaxDecoder::onProcess () 
{
	outputTCoder();
}


//-------------------------------------------------------------------------
// private methods implementation
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------------
// init the timecoder (return -1 if fails, 0 otherwise)
int WaxDecoder::loadTimecoder(AnsiCharPtr tc_def, double speed, bool soft_pa)
{
    TimecodeDefinition = timecoder_find_definition(tc_def);
    
    if (TimecodeDefinition == NULL)
        return -1;
    
    usineSmplRate = sdkGetSampleRate();

    timecoder_init(&TCoder, TimecodeDefinition, speed, usineSmplRate, soft_pa);
    
    return 0;
}

//-----------------------------------------------------------------------------
// getting pitch and position from the decoder and writing to the module outputs
int WaxDecoder::exportPlaybackParameters()
{
    /* FROM 'sync_to_timecode(struct player)' in player.c xwax sources */
    /*******************************************************************/
    double when, tcpos;
    signed int timecode;

    timecode = timecoder_get_position(&TCoder, &when);

    /* Instruct the caller to disconnect the timecoder if the needle
     * is outside the 'safe' zone of the record */

    if (timecode != -1 && timecode > timecoder_get_safe(&TCoder))
        return -1;

    /* If the timecoder is alive, use the pitch from the sine wave */

    pitch = timecoder_get_pitch(&TCoder);

    /* If we can read an absolute time from the timecode, then use it */

    if (timecode == -1)
        target_position = TARGET_UNKNOWN;
    else
    {
        tcpos = (double)timecode / timecoder_get_resolution(&TCoder);
        target_position = tcpos + pitch * when;
    }
    /*******************************************************************/
    
    // set outputs
    sdkSetEvtData(dtfPositionOut, target_position);
    sdkSetEvtData(dtfPitchOut, pitch);
    
    return 0;
}

//-----------------------------------------------------------------------------
// pcm conversion from Usine stereo audio inputs to xwax decoder
void WaxDecoder::writeCompatibleAudio(signed short*& pcm)
{
    int i;
    
    for(i=0; i < usineBlockSize; ++i)
    {         
        pcm[2*i] = 32768 * sdkGetEvtArrayData(audioInputTab[0], i);
        pcm[2*i+1] = 32768 * sdkGetEvtArrayData(audioInputTab[1], i);
    }
}

//-----------------------------------------------------------------------------
// main process
void WaxDecoder::outputTCoder()
{
    // get 'xwax compatible' audio from inputs
    writeCompatibleAudio(pcm);
    
    // submit block to timecoder
    timecoder_submit(&TCoder, pcm, usineBlockSize);
    
    // decode and output playback infos
    if (exportPlaybackParameters() == -1)
    {
        sdkSetEvtData(dtfPositionOut, TARGET_UNKNOWN);
        sdkSetEvtData(dtfPitchOut, 0.);
    }
}
