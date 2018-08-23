//----------------------------------------------------------------------------
//
//@file  
//	UserModule.h
//
//@date 
//	2009/06/03
//
//@author
//	Martin FLEURENT aka 'martignasse'
//
//@brief  
//	A generic class provided to derive user modules from.
//
//@IMPORTANT
//	This file is part of the Usine CPP SDK Version 5
//
//  Please, report bugs and patch to Usine forum :
//  http://www.sensomusic.com/forums/ 
//  in the 'Module development SDK' section
//
//----------------------------------------------------------------------------

// include once, no more
#ifndef _USERMODULE_H_
#define _USERMODULE_H_

//----------------------------------------------------------------------------
// includes
//----------------------------------------------------------------------------
#include <Windows.h>
#include <WinUser.h>
#include <cmath>

#include "UsineDefinitions.h"

//-----------------------------------------------------------------------------
// class definition
//-----------------------------------------------------------------------------
// generic class to derive the user module from
class TUserModule 
{
    // the gui have direct access to members
    friend class TGuiCanvas;

    //-------------------------------------------------------------------------
    // protected members
    //-------------------------------------------------------------------------
protected:

    // Pointer to TMasterInfo and TModuleInfo provided by usine
    TMasterInfo* m_masterInfo;
    TModuleInfo* m_moduleInfo;

    // Dimension of the module (if mtForm or mtControl, otherwise = 0)
    int	m_moduleWidth;
    int	m_moduleHeight;

    //-------------------------------------------------------------------------
    // public constructors/destructors
    //-------------------------------------------------------------------------
public:

    // constructor
    TUserModule() {
        m_moduleWidth = 0;
        m_moduleHeight = 0;
        m_masterInfo = 0;
    };

    // destructor
    virtual ~TUserModule() {};

    //-------------------------------------------------------------------------
    // public methodes
    //-------------------------------------------------------------------------
public:

    //-----------------------------------------------------------------------------
    // Accessors methodes

    // module size setter
    inline void SetSize( int Width, int Height )
    { 
        this->m_moduleWidth = Width;
        this->m_moduleHeight = Height; 
    };

    // module width getter
    inline int GetWidth() { return this->m_moduleWidth; };

    // module width getter
    inline int GetHeight() { return this->m_moduleHeight; };

    //-----------------------------------------------------------------------------
    // module initialisation
    // IMPORTANT : it's up to you to initialize the user module before further use
    inline void Init(TMasterInfo* pMasterInfo, TModuleInfo* pModuleInfo) 
    { 
        if (pMasterInfo != 0) 
        {
            this->m_masterInfo = pMasterInfo;
        }
        if (pModuleInfo != 0) 
        {
            this->m_moduleInfo = pModuleInfo;
        } 
    };
 
    //-----------------------------------------------------------------------------
    // wrappers for MasterInfo functions
    //-----------------------------------------------------------------------------

    // ask Usine to repaint the module Panel
    inline void RepaintPanel( void )
    { 
        if ( (m_masterInfo != 0) && (m_moduleInfo != 0) ) { m_masterInfo->RepaintPanel( m_moduleInfo ); }; 
    };

    // edit layout
    inline void AddLayoutLineSeparator( void )                                      { m_masterInfo->AddLayoutLineSeparator( m_moduleInfo ); };
    inline void AddLayoutLineColor( TColorUSINE* pVal, PCHAR Caption )              { m_masterInfo->AddLayoutLineColor( m_moduleInfo, pVal, Caption ); };
    inline void AddLayoutLineBoolean( LONGBOOL* pVal, PCHAR Caption )               { m_masterInfo->AddLayoutLineBoolean( m_moduleInfo, pVal, Caption ); };
    inline void AddLayoutLineInteger( int* pVal, PCHAR Caption, int min, int max )  { m_masterInfo->AddLayoutLineInteger( m_moduleInfo, pVal, Caption, min, max ); };

    // Usine internal messages
    inline void SendUsineMsg( PCHAR Msg )                            { m_masterInfo->SendUsineMsg( m_moduleInfo, Msg ); };

    // events manipuation
    inline bool CompareEvt( TEVT* e1, TEVT* e2 )                     { return m_masterInfo->CompareEvt( e1, e2 ); };
    inline void CopyEvt(TEVT* scr, TEVT* dest)                       { m_masterInfo->CopyEvt( scr, dest ); };
    inline void ConcatEvt( TEVT* i1, TEVT* i2, TEVT* o )             { m_masterInfo->ConcatEvt( i1, i2, o ); };
    inline void ShiftEvt( int n, TEVT* input, TEVT* output )         { m_masterInfo->ShiftEvt( n, input, output ); };
    inline void SetEvtSize( TEVT* ev, int size )                     { m_masterInfo->SetEvtSize( ev, size ); };
    inline int GetEvtSize( TEVT* ev)                                 { return m_masterInfo->GetEvtSize( ev ); };
    inline void SetEvtMaxSize( TEVT* ev, int size )                  { m_masterInfo->SetEvtMaxSize( ev, size ); };
    inline void DestroyEvt( TEVT* &ev )                               { m_masterInfo->DestroyEvt( ev ); };
    inline void CreateEvt( TEVT* &ev, int originalsize )              { m_masterInfo->CreateEvt( ev, originalsize ); };
    inline void MoveBLOC32( float* src, float* dest, int size )      { m_masterInfo->MoveBLOC32( src, dest, size ); };

    // acces to evt values
    inline void	SetEvtData( TEVT* ev, float value )                  { m_masterInfo->SetEvtData( ev, value ); };
    inline float GetEvtData( TEVT* ev )                              { return m_masterInfo->GetEvtData( ev ); };
    inline void	SetEvtArrayData( TEVT* ev, int idx, float value )    { m_masterInfo->SetEvtArrayData( ev, idx, value ); };
    inline float GetEvtArrayData( TEVT* ev, int idx )                { return m_masterInfo->GetEvtArrayData( ev, idx ); };
    
    inline void	SetEvtDWORD( TEVT* ev, DWORD value )                 { m_masterInfo->SetEvtDWORD( ev, value ); };
    inline TDWORD GetEvtDWORD( TEVT* ev )                            { return m_masterInfo->GetEvtDWORD( ev ); };
    inline void	SetEvtArrayDWORD( TEVT* ev, int idx, DWORD value )   { m_masterInfo->SetEvtArrayDWORD( ev, idx, value ); };
    inline TDWORD GetEvtArrayDWORD( TEVT* ev, int idx )              { return m_masterInfo->GetEvtArrayDWORD( ev, idx ); };
    
    inline void	SetEvtArrayMidi( TEVT* ev, int idx, TCodeMidi midi ) { m_masterInfo->SetEvtArrayMidi( ev, idx, midi ); };
    inline TCodeMidi GetEvtArrayMidi( TEVT* ev, int idx )            {	return m_masterInfo->GetEvtArrayMidi( ev, idx ); };
    
    inline void	SetEvtPChar( TEVT* ev,  PCHAR text )                 { m_masterInfo->SetEvtPChar( ev, text ); };
    inline PCHAR GetEvtPChar( TEVT* ev )                             { return m_masterInfo->GetEvtPChar( ev ); };
    
    inline float* GetEvtDataAddr( TEVT*ev )                          { return m_masterInfo->GetEvtDataAddr( ev ); };

    // evt data manipulation
    inline void  AddEvt3( TEVT* in1, TEVT* in2, TEVT* out )                     { m_masterInfo->AddEvt3( in1, in2, out ); };
    inline void  AddEvt2( TEVT* in1, TEVT* in2 )                                { m_masterInfo->AddEvt2( in1, in2 ); };
    inline void  SubEvt3( TEVT* in1, TEVT* in2, TEVT* out )                     { m_masterInfo->SubEvt3( in1, in2, out ); };
    inline void  SubEvt2( TEVT* in1, TEVT* in2 )                                { m_masterInfo->SubEvt2( in1, in2 ); };
    inline void  DivEvt3( TEVT* in1, TEVT* in2, TEVT* out )                     { m_masterInfo->DivEvt3( in1, in2, out ); };
    inline void  MultEvt3( TEVT* in1, TEVT* in2, TEVT* out )                    { m_masterInfo->MultEvt3( in1, in2, out ); };
    inline void  MultEvt2( TEVT* in1, TEVT* in2 )                               { m_masterInfo->MultEvt2( in1, in2 ); };
    inline void  MultEvt2Audio( TEVT* in1, TEVT* in2 )                         { m_masterInfo->MultEvt2( in1, in2 ); };
    inline void  MultEvt1( float m, TEVT* out )                                 { m_masterInfo->MultEvt1( m, out ); };
    inline void  ModEvt3( TEVT* in1, TEVT* in2, TEVT* out )                     { m_masterInfo->ModEvt3( in1, in2, out ); };
    inline void  PowerEvt3( TEVT* in1, TEVT* in2, TEVT* out )                   { m_masterInfo->PowerEvt3( in1, in2, out ); };
    inline void  ExpEvt1( TEVT* in1 )                                           { m_masterInfo->ExpEvt1( in1 ); };

    inline float MaxEvt1( TEVT* in1 )                                           { return m_masterInfo->MaxEvt1( in1 ); };
    inline float MinEvt1( TEVT* in1 )                                           { return m_masterInfo->MinEvt1( in1 ); };
    inline void  MaxEvt3( TEVT* in1, TEVT* in2, TEVT* out )                     { m_masterInfo->MaxEvt3( in1, in2, out ); };
    inline void  MinEvt3( TEVT* in1, TEVT* in2, TEVT* out )                     { m_masterInfo->MinEvt3( in1, in2, out ); };

    inline void  ThreshEvt2( const TEVT* in1, TEVT* out, float min, float max ) { m_masterInfo->ThreshEvt2( in1, out, min, max ); };
    inline void  ThreshEvt1( TEVT* in1, float min, float max )                  { m_masterInfo->ThreshEvt1( in1, min, max ); };
    inline void  MixMidiEvt3( TEVT* in1, TEVT* in2, TEVT* out )                 { m_masterInfo->MixMidiEvt3( in1, in2, out ); };
    inline void  MixMidiEvt2( TEVT* in1, TEVT* in2 )                            { m_masterInfo->MixMidiEvt2( in1, in2 ); };
    inline void  FadeInAudioEvt( TEVT* TPCMin )                                 { m_masterInfo->FadeInAudioEvt( TPCMin ); };
    inline void  FadeOutAudioEvt( TEVT* TPCMin )                                { m_masterInfo->FadeOutAudioEvt( TPCMin ); };
    inline void  ClearAudioEvt( TEVT* in1 )                                     { m_masterInfo->ClearAudioEvt( in1 ); };


    // Audio File manipulation
    inline PAUDIO CreateAudioFile          ()                                                      { return m_masterInfo->CreateAudioFile(); };
    inline void   DestroyAudioFile         (PAUDIO audiofile)                                      { m_masterInfo->DestroyAudioFile( audiofile); };
    inline float  GetSampleAudioFile       (PAUDIO audiofile, int channel, int pos)                { return m_masterInfo->GetSampleAudioFile		( audiofile, channel, pos); };
    inline void	  GetBlocSampleAudioFile   (PAUDIO audiofile, int channel, int pos, TEVT* evt)     { m_masterInfo->GetBlocSampleAudioFile			(audiofile, channel, pos, evt); };
    inline int    GetSizeAudioFile         (PAUDIO audiofile)                                      { return m_masterInfo->GetSizeAudioFile			(audiofile); };
    inline int    GetChannelAudioFile      (PAUDIO audiofile)                                      { return m_masterInfo->GetChannelAudioFile		( audiofile); };
    inline int    GetSampleRateAudioFile   (PAUDIO audiofile)                                      { return m_masterInfo->GetSampleRateAudioFile	( audiofile); };
    inline int    GetBitPerSampleAudioFile (PAUDIO audiofile)                                      { return m_masterInfo->GetBitPerSampleAudioFile	( audiofile); };
    inline void   LoadInMemoryAudioFile    (PAUDIO audiofile, PCHAR name)                          { m_masterInfo->LoadInMemoryAudioFile	( audiofile, name); };
    inline void   LoadStayOnDiskAudioFile  (PAUDIO audiofile, PCHAR name)                          { m_masterInfo->LoadStayOnDiskAudioFile	( audiofile, name); };
    inline void   GetPeaksAudioFile        (PAUDIO audiofile, TEVT* evt)                           { m_masterInfo->GetPeaksAudioFile		( audiofile, evt); };
    inline void   SaveToDiskAudioFile      (PAUDIO audiofile, PCHAR name, int nbChannels)          { m_masterInfo->SaveToDiskAudioFile		( audiofile, name, nbChannels); };
    inline void   SetChannelsAudioFile     (PAUDIO audiofile, int nbChannels)                      { m_masterInfo->SetChannelsAudioFile		( audiofile, nbChannels); };
    inline void   SetSizeAudioFile         (PAUDIO audiofile, int size)                            { m_masterInfo->SetSizeAudioFile			( audiofile, size); };
    inline void   SetSampleAudioFile       (PAUDIO audiofile, int channel, int pos, float sample)  { m_masterInfo->SetSampleAudioFile		( audiofile, channel, pos, sample); };
    inline void   ClearAudioFile           (PAUDIO audiofile)                                      { m_masterInfo->ClearAudioFile			( audiofile); };

    // math util
    inline float LinearInterpolation ( float f, float a, float b )                               { return m_masterInfo->LinearInterpolation	( f, a, b ); };
    inline float CubicInterpolation  ( float fr, float inm1, float inp, float inp1, float inp2 ) { return m_masterInfo->CubicInterpolation	( fr, inm1, inp, inp1, inp2 ); };
    inline float SplineInterpolation ( float fr, float inm1, float inp, float inp1, float inp2 ) { return m_masterInfo->SplineInterpolation	( fr, inm1, inp, inp1, inp2 ); };

    // files manipulation
    inline LONGBOOL	ProcessOpenDialog ( PCHAR* filename, PCHAR initialDir, PCHAR filter ) { return m_masterInfo->ProcessOpenDialog	( filename, initialDir, filter ); };
    inline LONGBOOL	ProcessSaveDialog ( PCHAR* filename, PCHAR initialDir, PCHAR filter ) { return m_masterInfo->ProcessSaveDialog	( filename, initialDir, filter ); };
    inline PCHAR    FindFile          ( PCHAR* filename, PCHAR initialDir )               { return m_masterInfo->FindFile			( filename, initialDir ); };

    // usine path
    inline PCHAR    GetUsineSoundPath   () { return m_masterInfo->SoundPath; };
    inline PCHAR    GetUsinePatchPath  () { return m_masterInfo->PatchPath; };
    inline PCHAR    GetUsineVst1Path   () { return m_masterInfo->Vst1Path; };
    inline PCHAR    GetUsineVst2Path   () { return m_masterInfo->Vst2Path; };
    inline PCHAR    GetUsineRecordPath () { return m_masterInfo->RecordPath; };
    
    // trace functions
    inline void    TracePChar      ( PCHAR s ) { return m_masterInfo->sTrace			( s ); };
    inline void    TraceInt        ( int i )   { return m_masterInfo->iTrace			( i ); };
    inline void    TraceFloat      ( float f ) { return m_masterInfo->fTrace			( f ); };
    inline void    TraceLogPChar   ( PCHAR s ) { return m_masterInfo->TraceLogPChar	( s ); };
    inline void    TraceErrorPChar ( PCHAR s ) { return m_masterInfo->TraceErrorPChar	( s ); };

    // smooth functions
    inline void    SmoothSingle  ( float& oldValue, TEVT* currentEvent, float factor )                  { return m_masterInfo->SmoothSingle ( oldValue, currentEvent, factor ); };
    inline void    SmoothEvent   ( float& oldValue, TEVT* currentEvent, float target, float factor )    { return m_masterInfo->SmoothEvent  ( oldValue, currentEvent, target, factor ); };

    // accessors for MasterInfo members
    inline VstTimeInfo* GetVstTimeInfo()      { return m_masterInfo->pTimeInfo; };

    // size of Usine audio blocs
    inline int GetBlocSize()                  { return m_masterInfo->BlocSize; };

    // default control panel Width
    inline int GetPanelWidth()                { return m_masterInfo->PanelWidth; };	
    
    // Usine mainform handle (THandle)
    inline HWND GetUsineHandle()              { return m_masterInfo->UsineHandle; };	

    // Usine ColorSet
    inline TUsineColorSet* GetUsineColorSet() { return m_masterInfo->UsineColorSet; };

    
    //-----------------------------------------------------------------------------
    // wrappers for ModuleInfo functions
    //-----------------------------------------------------------------------------

    // pointer to Usine internal User Module
    inline void* GetUsineUserModule()                 { return m_moduleInfo->UsineUserModule; };

    // background color accessors
    inline TColorUSINE GetBackgroundColor()           { return m_moduleInfo->BackgroundColor; };
    inline void SetBackgroundColor(TColorUSINE color) { m_moduleInfo->BackgroundColor = color; };
    inline LONGBOOL GetBackgroundRounded()            { return m_moduleInfo->Rounded; };
    inline int GetBackgroundRoundedSize()             { return m_moduleInfo->RoundSize; };
    inline PCHAR GetBackgroundBitMapFilename()        { return m_moduleInfo->BackBitMapFilename; };

    // caption accessors
    inline LONGBOOL GetCaptionShow()                  { return m_moduleInfo->ShowCaption; };
    //inline void SetCaptionShow(LONGBOOL showcaption)  { m_moduleInfo->ShowCaption = showcaption; };
    inline TColorUSINE GetCaptionColor()              { return m_moduleInfo->CaptionColor; };
    inline TColorUSINE GetCaptionTextColor()          { return m_moduleInfo->CaptionTextColor; };
    inline int GetCaptionTextSize()                   { return m_moduleInfo->CaptionTextSize; };
    inline int GetCaptionHeight()                     { return m_moduleInfo->CaptionHeight; };
    inline PCHAR GetCaption()                         { return m_moduleInfo->Caption; };

    // border color accessors
    inline TColorUSINE GetBorderColor()               { return m_moduleInfo->BorderColor; };
    inline void SetBorderColor(TColorUSINE color)     { m_moduleInfo->BorderColor = color; };
    
    // border visibility accessors
    inline LONGBOOL GetBorderShow()                   { return m_moduleInfo->ShowBorder; };
    inline void SetBorderShow(LONGBOOL showborder)    { m_moduleInfo->ShowBorder = showborder; };
    
    // border width accessors
    inline int GetBorderWidth()                       { return m_moduleInfo->BorderWidth; };
    inline void SetBorderWidth(int width)             { m_moduleInfo->BorderWidth = width; };

    // query index accessor
    inline int GetQueryIdx()                          { return m_moduleInfo->QueryIdx; };
    
}; // class TUserModule

#endif // _USERMODULE_H_