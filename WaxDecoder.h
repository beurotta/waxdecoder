//-----------------------------------------------------------------------------
//
//@file  
//	WaxDecoder.h
//
//@date 
//	2012/06/xx
//
//@author
//	naarud
//
//@brief 
//	Implementation of the timecoder c code used in xwax (xwax.org), vinyl 
//  emulation software for Linux designed by Mark Hills <mark@pogo.org.uk>
//
//@historic 
//	2012/07/04
//    first release, implement the timecoder used in v1.2 of xwax
//
//-----------------------------------------------------------------------------

// include once, no more
#ifndef INCLUDED_WAXDECODER_H
#define INCLUDED_WAXDECODER_H

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include "UserDefinitions.h"
#include "timecoder.h"

//-----------------------------------------------------------------------------
// defines and constantes
//-----------------------------------------------------------------------------

#define TARGET_UNKNOWN INFINITY

// names of handled timecodes
PCHAR const TC_NAMES[7] = {
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

//-----------------------------------------------------------------------------
// structures and typedef
//-----------------------------------------------------------------------------

// structures and typedef goes here

//-----------------------------------------------------------------------------
// class definition
//-----------------------------------------------------------------------------
class TWaxDecoder : public TUserModule
{
    //-------------------------------------------------------------------------
    // public members
    //-------------------------------------------------------------------------
public:

    //-------------------------------------------------------------------------
    // parameters events
    TEVT* m_audioInputTab[2];       // stereo audio input
    TEVT* m_lbxTimecodes;       // listbox of available timecodes
    TEVT* m_lbxRpmSpeed;        // listbox of rotational speed
    TEVT* m_dtfPositionOut;       // position output
    TEVT* m_dtfPitchOut;       // pitch output

    // public constructors/destructors
    //-------------------------------------------------------------------------
public:
    // constructor
    TWaxDecoder()
    {
        usineBlockSize = 0;
        usineSmplRate = 0;
        target_position = TARGET_UNKNOWN;
        pitch = 0.;
    };

    // destructor
    ~TWaxDecoder()
    {
		if (pcm != NULL )
			delete [] pcm;
    };

    //-------------------------------------------------------------------------
    // protected members
    //-------------------------------------------------------------------------
protected:
    // used to iterate into the audio buffer
    int usineBlockSize;

    // used to init the timecoder
    unsigned int usineSmplRate;

    // timecode definition and timecoder
    timecode_def * TimecodeDefinition;
    timecoder TCoder;
    
    // audio samples for timecoder lib
    signed short * pcm;
        
    // output var
    double target_position; // seconds or TARGET_UNKNOWN
    double pitch;

    //-------------------------------------------------------------------------
    // PUBLIC METHODES
    //-------------------------------------------------------------------------
public:

    // init the timecoder (return -1 if fails, 0 otherwise)
    int LoadTimecoder(PCHAR tc_def, double speed)
    {
        // load the definition associated to the provided name
        TimecodeDefinition = timecoder_find_definition(tc_def);
        
        // terminate if the loading failed
        if (TimecodeDefinition == NULL)
            return -1;
        
        // get the current sample rate
        usineSmplRate = m_masterInfo->pTimeInfo->sampleRate;
        
        timecoder_init(&TCoder, TimecodeDefinition, speed, usineSmplRate);
        
        return 0;
    }


    // main process
    void OutputTCoder()
    {
        // usine block size
        usineBlockSize = GetBlocSize();
        
        // get 'xwax compatible' audio from inputs
        m_writeCompatibleAudio(pcm);
        
        // submit block to timecoder
        timecoder_submit(&TCoder, pcm, usineBlockSize);
        
        // decode and output playback infos
        if (m_exportPlaybackParameters() == -1)
        {
            SetEvtData(m_dtfPositionOut, TARGET_UNKNOWN);
            SetEvtData(m_dtfPitchOut, 0.);
        }
        
        delete [] pcm;
    }



    //-------------------------------------------------------------------------
    // PROTECTED METHODES
    //-------------------------------------------------------------------------
protected:

    // pcm conversion from Usine stereo audio inputs to xwax decoder
    void m_writeCompatibleAudio(signed short*& pcm)
    {
        int i;

        // declare an audio samples array of size 'usineBlockSize'
        pcm = new signed short[usineBlockSize * 2];
        
        for(i=0; i < usineBlockSize; ++i)
        {         
            pcm[2*i] = 32768 * GetEvtArrayData(m_audioInputTab[0], i);
            pcm[2*i+1] = 32768 * GetEvtArrayData(m_audioInputTab[1], i);
        }
    }
    
    int m_exportPlaybackParameters()
    {
        /* COPY-PASTE FROM 'sync_to_timecode(struct player)' in player.c */
        /*****************************************************************/
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
        /*****************************************************************/
        
        // set outputs
        SetEvtData(m_dtfPositionOut, target_position);
        SetEvtData(m_dtfPitchOut, pitch);
        
        return 0;
    }

}; // class TWaxDecoder

#endif //INCLUDED_WAXDECODER_H