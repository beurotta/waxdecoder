//-----------------------------------------------------------------------------
//@file  
//	WaxDecoder.h
//
//@author
//	Arnaud BEUROTTE aka 'naarud'
//
//@brief 
//	Implementation of the timecoder c code used in xwax (xwax.org), DJ software
//	with vinyl record emulation for Linux, designed by Mark Hills <mark@pogo.org.uk>
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

// include once, no more
#ifndef INCLUDED_WAXDECODER_H
#define INCLUDED_WAXDECODER_H

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include "./sdk/UserDefinitions.h"  
#include "./xwax_src/timecoder.h"

//-----------------------------------------------------------------------------
// defines and constantes
//-----------------------------------------------------------------------------
#define TARGET_UNKNOWN INFINITY

// names of handled timecodes
AnsiCharPtr const TC_NAMES[7] = {
    "serato_2a",
    "serato_2b",
    "serato_cd",
    "traktor_a",
    "traktor_b",
    "mixvibes_v2",
    "mixvibes_7inch"
};

// turntable rotational speed : 1.0 for 33rpm, 1.35 for 45rpm
double const RPM_SPEED[2] = {1., 1.35};

// 'software preamp' for unamplified phono signal connected to a line-level input
bool const SOFT_PREAMP[2] = {FALSE, TRUE};

//-----------------------------------------------------------------------------
// class definition
//-----------------------------------------------------------------------------
class WaxDecoder : public UserModuleBase
{
    //-------------------------------------------------------------------------
	// module constructors/destructors
	//-------------------------------------------------------------------------
public:
	// constructor
    WaxDecoder();
 
    // destructor
    virtual ~WaxDecoder();


	//-------------------------------------------------------------------------
	// public methods inherited from UserModule
	//-------------------------------------------------------------------------
public:
	//-------------------------------------------------------------------------
	// module info
	void onGetModuleInfo (MasterInfo* pMasterInfo, ModuleInfo* pModuleInfo);

	//-------------------------------------------------------------------------
	// init
	void onInitModule (MasterInfo* pMasterInfo, ModuleInfo* pModuleInfo);

	//-------------------------------------------------------------------------
	// parameters and process
	void onGetParamInfo (int ParamIndex, TParamInfo* pParamInfo);
	void onCallBack (UsineMessage *Message);
 	void onProcess ();

	//-------------------------------------------------------------------------
	// timecoder settings
	void onCreateSettings();
	void onSettingsHasChanged();

 	//-----------------------------------------------------------------------------
	// audio setup update
	void onBlocSizeChange (int BlocSize);
	void onSampleRateChange (double SampleRate);
   
	//-------------------------------------------------------------------------
	// private members
	//-------------------------------------------------------------------------
private:
	//-------------------------------------------------------------------------
	// parameters events
    UsineEventPtr audioInputTab[2];   // stereo audio input
    UsineEventPtr dtfPositionOut;     // position data output
    UsineEventPtr dtfPitchOut;        // pitch data output

	//-------------------------------------------------------------------------
    // Usine and soundcard audio settings
    int usineBlockSize;               // used to translate Usine audio blocks to xwax buffer
    unsigned int usineSmplRate;       // used to init the timecoder

	//-------------------------------------------------------------------------
    // timecode definition and timecoder
    timecode_def * TimecodeDefinition;
    timecoder TCoder;
    
 	//-------------------------------------------------------------------------
    // audio samples for timecoder lib
    signed short * pcm;

	//-------------------------------------------------------------------------
    // output
    double target_position;           // seconds or TARGET_UNKNOWN
    double pitch;
	
	//-------------------------------------------------------------------------
	// hardware settings
	int lbxTimecodes;
    int lbxRpmSpeed;
	int lbxSoftPA;
	
	//-------------------------------------------------------------------------
	// private methods
	//-------------------------------------------------------------------------
private :
    int loadTimecoder(AnsiCharPtr tc_def, double speed, bool soft_pa);
    int exportPlaybackParameters();
    void writeCompatibleAudio(signed short*& pcm);
    void outputTCoder();

}; // class WaxDecoder

#endif // INCLUDED_WAXDECODER_H