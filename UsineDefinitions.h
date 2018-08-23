//-----------------------------------------------------------------------------
//
//@file  
//	UsineDefinitions.h
//
//@date 
//	2009/06/03
//
//@author
//	Martin FLEURENT aka 'martignasse'
//
//@brief  
//	Datas and functions declarations exposed by Usine.
//
//@IMPORTANT
//	This file is part of the Usine CPP SDK Version 5
//
//  Please, report bugs and patch to Usine forum :
//  http://www.sensomusic.com/forums/ 
//  in the 'Module development SDK' section
//
//-----------------------------------------------------------------------------

// include once, no more
#ifndef _USINEDEFINITIONS_H_
#define _USINEDEFINITIONS_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <WinUser.h>
#include <climits> //MODS '<limits>' --> '<climits>' + '<cfloat>'
#include <cfloat> //MODS

//-----------------------------------------------------------------------------
// Datatypes definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// for memory alignment with DELPHI
#pragma pack(push, 4)

//-----------------------------------------------------------------------------
// version of the SDK
// to use in GetSDKVersion function to return the SDK version number
//static int const SDK_VERSION = 5;
static int const SDK_VERSION = 57000;

//-----------------------------------------------------------------------------
// possible type of implementation used to make the module
// to use in GetImplementation function 
enum TImplementation 
{
    DELPHI,
    CPP_VC90,
    CPP_GCC_432,
    CPP_BORLAND_55,
    OTHER = INT_MAX
};

//----------------------------------------------------------------------------
// Usine system message 
// used in CallBack function declaration (see end of file)
struct TMessage 
{
    DWORD message; 
    DWORD wParam;  
    DWORD lParam; 
    DWORD result;
};

//----------------------------------------------------------------------------
// possible TMessage::lparam values
// to use in Callback function (called each time the parameter is modified or clicked)
static DWORD const MSG_CHANGE     = 0;  // the parameter value has changed
static DWORD const MSG_CLICK      = 1;  // the parameter has been clicked
static DWORD const MSG_DBLCLICK   = 2;  // the parameter has been double clicked
static DWORD const MSG_SETCAPTION = 3;  // change the caption of a control
static DWORD const MSG_MOUSEUP    = 4;  // mouse Up
static DWORD const MSG_MOUSEMOVE  = 5;  // mouse Move

//----------------------------------------------------------------------------
// keys state modifiers
// used in MouseMove, MouseUp and MouseDown functions declaration (see end of file)
typedef DWORD TShiftState;

static DWORD const ssShift  = 0x1;
static DWORD const ssAlt    = 0x2;
static DWORD const ssCtrl   = 0x4;
static DWORD const ssLeft   = 0x8;
static DWORD const ssRight  = 0x10;
static DWORD const ssMiddle = 0x20;
static DWORD const ssDouble = 0x40;

//----------------------------------------------------------------------------
// mouse buttons   
// used in MouseUp and MouseDown functions declaration (see end of file)
typedef enum TMouseButton 
{ 
    mbLeft,
    mbRight,
    mbMiddle 
};

//-----------------------------------------------------------------------------
// scale of the fader parameter
// used in TParamInfo structure (see below) 
typedef enum TScale 
{ 
    lsLinear, 
    lsLog, 
    lsExp 
};

//-----------------------------------------------------------------------------
// all parameters type availlable in usine
// used in TParamInfo structure (see below) 
typedef enum TParamType 
{ 
    ptTextField, 
    ptChooseColor, 
    ptMidi, 
    ptGainFader, 
    ptAudio, 
    ptDataField, 
    ptDataFader, 
    ptButton, 
    ptListBox, 
    ptSwitch, 
    ptArray, 
    ptIpAddress, 
    ptSmpte, 
    ptMidiNoteFader, 
    ptPointerBitMap, 
    ptPointer, 
    ptOther 
};

//-----------------------------------------------------------------------------
// Data type of the parameter
// part of the TEVT structure (see below)
// intern, not used in user modules
typedef enum  TFlowType  
{ 
    ftNone, 
    ftPCM,
    ftInternText, 
    ftData, 
    ftMIDI, 
    ftArray, 
    ftUser, 
    ftEvtText, 
    ftIntern=INT_MAX // force the flowtype size to 4bytes
}; 

//-----------------------------------------------------------------------------
// custom bool type to match the 4 bytes DELPHI LongBool type
typedef int LONGBOOL;

// TRUE and FALSE are allready defined in WinDef.h
#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

//-----------------------------------------------------------------------------
// strings conformance between CPP and DELPHI
// used everywhere you need some string
typedef char* PCHAR;

//-----------------------------------------------------------------------------
// Midi code
// used in TEVT structure
struct TCodeMidi
{   
    unsigned char Channel;
    unsigned char Msg;
    unsigned char Data1;
    unsigned char Data2;
}; 

//-----------------------------------------------------------------------------
// used in TEVT structure
struct  TDWORD
{
    DWORD val;
};

//-----------------------------------------------------------------------------
// Usine native color format
typedef DWORD TColorUSINE ;

//-----------------------------------------------------------------------------
// handle to an audio file
typedef void* PAUDIO;

//-----------------------------------------------------------------------------
// set of color used by Usine
struct TUsineColorSet 
{
    TColorUSINE clURed,
    clPanelBack,
    clPanelBorder,

    // patch
    clPatchCaption,
    clPatchBack,
    clpgNameBackground,

    // fader
    clFont,
    clValueBack,
    clMidiLearnIcon,
    clVstParamIcon,
    clQuantizeIcon,
    clNotStoredIcon,
    clDATA,
    clMIDI,
    clPCM,
    ClARRAY,
    ClText,

    clCheckBoxOff,
    clCheckBoxOn,
    clControlBorder,

    clButtonClick,
    clButton,
    clListBoxBtn,

    clSelected,
    clLineNumber,
    clStep,
    clMainBack,
    clPCMFaderBack,
    clScrollBarCursor,

    // sequencer
    clSeqPatchBorder,
    clSeqWaveForm,
    clSeqGrid,
    clSeqGridLite,

    // piano roll
    clPianoRollWhite,
    clPianoRollBlack,
    clPianoRollMidiEvents,
    clPianoRollCCEvents,

    clCursor,
    clLoopSection,
    clOscilloBack,
    clBrowser,

    clInvalidLink,
	clColor,
	clMasterTrack;
};

//-----------------------------------------------------------------------------
// Event associated to the parameter
// used in SetEventAddress function declaration (see end of file)
struct TEVT;

//-----------------------------------------------------------------------------
// VST Time info structure (see Steinberg VST SDK)
// the same VST structure is used in Usine User Modules
// used in TMasterInfo structure (see below)
struct VstTimeInfo
{
    double samplePos;           // current location
    double sampleRate;
    double nanoSeconds;         // system time
    double PpqPos;              // 1 ppq
    double tempo;               // in bpm
    double barStartPos;         // last bar start, in 1 ppq
    double cycleStartPos;       // 1 ppq
    double cycleEndPos;         // 1 ppq
    long   timeSigNumerator;    // time signature
    long   timeSigDenominator;
    long   smpteOffset;
    long   smpteFrameRate;      // 0:24, 1:25, 2:29.97, 3:30, 4:29.97 df, 5:30 df
    long   samplesToNextClock;  // midi clock resolution (24 ppq), can be negative
    long   flags;  
    double cyclePpqPos;         // usine cycle ppq
} ;

//-----------------------------------------------------------------------------
// functions pointers used in TMasterInfo structure (see below)

// forward declarations
struct TMasterInfo;
struct TModuleInfo;

// layouts
typedef void (*TRepaintPanel)           ( TModuleInfo* pTModuleInfo );
typedef void (*TAddLayoutLineSeparator) ( TModuleInfo* pTModuleInfo );
typedef void (*TAddLayoutLineColor)     ( TModuleInfo* pTModuleInfo, TColorUSINE* pVal, PCHAR Caption );
typedef void (*TAddLayoutLineBoolean)   ( TModuleInfo* pTModuleInfo, LONGBOOL* pVal, PCHAR Caption );
typedef void (*TAddLayoutLineInteger)   ( TModuleInfo* pTModuleInfo, int* pVal, PCHAR Caption, int min, int max );

// internal messages
typedef void (*TSendMessage)    ( TModuleInfo* pTModuleInfo, PCHAR Msg );

// events manipuation
typedef bool (*TCompareEvt)     ( TEVT* e1, TEVT* e2 );
typedef void (*TCopyEvt)        ( TEVT* scr, TEVT* dest );
typedef void (*TConcatEvt)      ( TEVT* i1, TEVT* i2, TEVT* o );
typedef void (*TShiftEvt)       ( int n, TEVT* input, TEVT* output );
typedef void (*TSetEvtSize)     ( TEVT* ev, int size );
typedef int  (*TGetEvtSize)     ( TEVT* ev );
typedef void (*TSetEvtMaxSize)  ( TEVT* ev, int size );
typedef void (*TDestroyEvt)     ( TEVT* &ev );
typedef void (*TCreateEvt)      ( TEVT* &ev, int originalsize );
typedef void (*TMoveBLOC32)     ( float* src, float* dest, int size );

// access to evt values
typedef void        (*TSetEvtData)       ( TEVT* ev, float value );
typedef float       (*TGetEvtData)       ( TEVT* ev );
typedef void        (*TSetEvtArrayData)  ( TEVT* ev, int idx, float value );
typedef float       (*TGetEvtArrayData)  ( TEVT* ev, int idx );
typedef void        (*TSetEvtDWORD)      ( TEVT* ev, DWORD value );             // not used in the SDK 3
typedef TDWORD      (*TGetEvtDWORD)      ( TEVT* ev );                          // not used in the SDK 3
typedef void        (*TSetEvtArrayDWORD) ( TEVT* ev, int idx, DWORD value );    // not used in the SDK 3
typedef TDWORD      (*TGetEvtArrayDWORD) ( TEVT* ev, int idx );                 // not used in the SDK 3
typedef void        (*TSetEvtArrayMidi)  ( TEVT* ev, int idx, TCodeMidi midi );
typedef TCodeMidi   (*TGetEvtArrayMidi)  ( TEVT* ev, int idx );
typedef  void       (*TSetEvtPChar)      ( TEVT* ev, PCHAR delphistring );
typedef PCHAR       (*TGetEvtPChar)      ( TEVT* ev );
typedef float*      (*TGetEvtDataAddr)   ( TEVT*ev );

// evt data manipulation
typedef void  (*TAddEvt3)   ( TEVT* in1, TEVT* in2, TEVT* out );
typedef void  (*TAddEvt2)   ( TEVT* in1, TEVT* in2 );
typedef void  (*TSubEvt3)   ( TEVT* in1, TEVT* in2, TEVT* out );
typedef void  (*TSubEvt2)   ( TEVT* in1, TEVT* in2 );
typedef void  (*TDivEvt3)   ( TEVT* in1, TEVT* in2, TEVT* out );
typedef void  (*TMultEvt3)  ( TEVT* in1, TEVT* in2, TEVT* out );
typedef void  (*TMultEvt2)  ( TEVT* in1, TEVT* in2 );
typedef void  (*TMultEvt1)  ( float m, TEVT* out );
typedef void  (*TModEvt3)   ( TEVT* in1, TEVT* in2, TEVT* out );
typedef void  (*TPowerEvt3) ( TEVT* in1, TEVT* in2, TEVT* out );
typedef void  (*TExpEvt1)   ( TEVT* in1 );
typedef void  (*TSqrtEvt1)  ( TEVT* in1 );

typedef float (*TMaxEvt1) ( TEVT* in1 );
typedef float (*TMinEvt1) ( TEVT* in1 );
typedef void  (*TMaxEvt3) ( TEVT* in1, TEVT* in2, TEVT* out );
typedef void  (*TMinEvt3) ( TEVT* in1, TEVT* in2, TEVT* out );

typedef void  (*TThreshEvt2)      ( const TEVT* in1, TEVT* out, float min, float max );
typedef void  (*TThreshEvt1)      ( TEVT* in1, float min, float max );
typedef void  (*TMixMidiEvt3)     ( TEVT* in1, TEVT* in2, TEVT* out );
typedef void  (*TMixMidiEvt2)     ( TEVT* in1, TEVT* in2 );
typedef void  (*TFadeInAudioEvt)  ( TEVT* TPCMin );
typedef void  (*TFadeOutAudioEvt) ( TEVT* TPCMin );
typedef void  (*TClearAudioEvt)   ( TEVT* in1 );

// Audio File manipulation
typedef PAUDIO (*TCreateAudioFile)          ();
typedef void   (*TDestroyAudioFile)         (PAUDIO audiofile);
typedef float  (*TGetSampleAudioFile)       (PAUDIO audiofile, int channel, int pos);
typedef void   (*TGetBlocSampleAudioFile)   (PAUDIO audiofile, int channel, int pos, TEVT* evt);
typedef int    (*TGetSizeAudioFile)         (PAUDIO audiofile);
typedef int    (*TGetChannelAudioFile)      (PAUDIO audiofile);
typedef int    (*TGetSampleRateAudioFile)   (PAUDIO audiofile);
typedef int    (*TGetBitPerSampleAudioFile) (PAUDIO audiofile);
typedef void   (*TLoadInMemoryAudioFile)    (PAUDIO audiofile, PCHAR name);
typedef void   (*TLoadStayOnDiskAudioFile)  (PAUDIO audiofile, PCHAR name);
typedef void   (*TGetPeaksAudioFile)        (PAUDIO audiofile, TEVT* evt);
typedef void   (*TSaveToDiskAudioFile)      (PAUDIO audiofile, PCHAR name, int nbChannels);
typedef void   (*TSetChannelsAudioFile)     (PAUDIO audiofile, int nbChannels);
typedef void   (*TSetSizeAudioFile)         (PAUDIO audiofile, int size);
typedef void   (*TSetSampleAudioFile)       (PAUDIO audiofile, int channel, int pos, float sample);
typedef void   (*TClearAudioFile)           (PAUDIO audiofile);

// math util
typedef float (*TLinearInterpolation) ( float f, float a, float b );
typedef float (*TCubicInterpolation)  ( float fr, float inm1, float inp, float inp1, float inp2 );
typedef float (*TSplineInterpolation) ( float fr, float inm1, float inp, float inp1, float inp2 );

// files manipulation
typedef LONGBOOL (*TProcessOpenDialog) ( PCHAR* filename, PCHAR initialDir, PCHAR filter );
typedef LONGBOOL (*TProcessSaveDialog) ( PCHAR* filename, PCHAR initialDir, PCHAR filter );
typedef PCHAR    (*TFindFile)          ( PCHAR* filename, PCHAR initialDir );
    
// trace functions
typedef void (*TsTrace)          ( PCHAR s );
typedef void (*TiTrace)          ( int i );
typedef void (*TfTrace)          ( float f );
typedef void (*TTraceLogPChar)   ( PCHAR s );
typedef void (*TTraceErrorPChar) ( PCHAR s );

// smooth functions
typedef void (*TSmoothSingle) ( float& oldValue, TEVT* currentEvent, float factor );
typedef void (*TSmoothEvent) ( float& oldValue, TEVT* currentEvent, float target, float factor );
typedef void (*TMultEvt2Audio)  ( TEVT* in1, TEVT* in2 );

//-----------------------------------------------------------------------------
// Usine master informations
// contains few important informations about Usine
// used in InitModule and GetModuleInfo functions declaration (see end of file)
struct TMasterInfo 
{
    VstTimeInfo*  pTimeInfo;    // Vsttimeinfo pointer
    int           BlocSize;     // size of Usine audio blocs (number of samples)
    int           PanelWidth;   // default control panel Width
    TRepaintPanel RepaintPanel; // ask Usine to repaint the module Panel
    HWND          UsineHandle;  // Usine mainform handle (THandle)
    void*         notused1;     // pointer to Usine internal User Module

    // add layout lines in the layout panel
    TAddLayoutLineSeparator AddLayoutLineSeparator;
    TAddLayoutLineColor     AddLayoutLineColor;
    TAddLayoutLineBoolean   AddLayoutLineBoolean;
    TAddLayoutLineInteger   AddLayoutLineInteger;

    // the colorset used by usine 
    TUsineColorSet*         UsineColorSet;
    
    // for memory alignement
    int	notused[4];       
                                                            
    // Internal usine message
    TSendMessage    SendUsineMsg;

    // Events manipuation
    TCopyEvt        CopyEvt;
    TCompareEvt     CompareEvt;
    TConcatEvt      ConcatEvt;
    TShiftEvt       ShiftEvt;
    TSetEvtSize     SetEvtSize;
    TSetEvtMaxSize  SetEvtMaxSize;
    TDestroyEvt     DestroyEvt;
    TCreateEvt      CreateEvt;
    TGetEvtSize     GetEvtSize;

    // Events values access
    TSetEvtData         SetEvtData;
    TGetEvtData         GetEvtData;
    TSetEvtArrayData    SetEvtArrayData;
    TGetEvtArrayData    GetEvtArrayData;
    TSetEvtDWORD        SetEvtDWORD;
    TGetEvtDWORD        GetEvtDWORD;
    TSetEvtArrayDWORD   SetEvtArrayDWORD;
    TGetEvtArrayDWORD   GetEvtArrayDWORD;
    TSetEvtArrayMidi    SetEvtArrayMidi;
    TGetEvtArrayMidi    GetEvtArrayMidi;
    TSetEvtPChar        SetEvtPChar;
    TGetEvtPChar        GetEvtPChar;

    TGetEvtDataAddr     GetEvtDataAddr;
    TMoveBLOC32 		MoveBLOC32;

    // Events data manipulation
    TAddEvt3    AddEvt3;
    TAddEvt2    AddEvt2;
    TSubEvt3    SubEvt3;
    TSubEvt2    SubEvt2;
    TMultEvt3   MultEvt3;
    TMultEvt2   MultEvt2;
    TMultEvt1   MultEvt1;
    TDivEvt3    DivEvt3;
    TModEvt3    ModEvt3;
    TPowerEvt3  PowerEvt3;
    TExpEvt1    ExpEvt1;
    TSqrtEvt1   SqrtEvt1;

    TMaxEvt1    MaxEvt1;
    TMinEvt1    MinEvt1;
    TMaxEvt3    MaxEvt3;
    TMinEvt3    MinEvt3;

    TThreshEvt2      ThreshEvt2;
    TThreshEvt1      ThreshEvt1;
    TMixMidiEvt3     MixMidiEvt3;
    TMixMidiEvt2     MixMidiEvt2;
    TFadeInAudioEvt  FadeInAudioEvt;
    TFadeOutAudioEvt FadeOutAudioEvt;
    TClearAudioEvt   ClearAudioEvt;

    // Audio File manipulation
    TCreateAudioFile            CreateAudioFile;	
    TDestroyAudioFile           DestroyAudioFile;			
    TGetSampleAudioFile         GetSampleAudioFile;			
    TGetBlocSampleAudioFile     GetBlocSampleAudioFile;		
    TGetSizeAudioFile           GetSizeAudioFile;			
    TGetChannelAudioFile        GetChannelAudioFile;		
    TGetSampleRateAudioFile	    GetSampleRateAudioFile;		
    TGetBitPerSampleAudioFile   GetBitPerSampleAudioFile;	
    TLoadInMemoryAudioFile      LoadInMemoryAudioFile;		
    TLoadStayOnDiskAudioFile    LoadStayOnDiskAudioFile;	
    TGetPeaksAudioFile          GetPeaksAudioFile;			
    TSaveToDiskAudioFile        SaveToDiskAudioFile;		
    TSetChannelsAudioFile       SetChannelsAudioFile;		
    TSetSizeAudioFile           SetSizeAudioFile;			
    TSetSampleAudioFile         SetSampleAudioFile;			
    TClearAudioFile             ClearAudioFile;				

    // Math util
    TLinearInterpolation    LinearInterpolation;	
    TCubicInterpolation	    CubicInterpolation;		
    TSplineInterpolation    SplineInterpolation;

    // Files manipulation
    TProcessOpenDialog  ProcessOpenDialog;
    TProcessSaveDialog  ProcessSaveDialog;
    TFindFile           FindFile;
    
    // Usine path
    PCHAR   SoundPath;
    PCHAR   PatchPath;
    PCHAR   Vst1Path;
    PCHAR   Vst2Path;
    PCHAR   RecordPath;

    // Trace functions
    TsTrace             sTrace;
    TiTrace             iTrace;
    TfTrace             fTrace;
    TTraceLogPChar      TraceLogPChar;
    TTraceErrorPChar    TraceErrorPChar;
 
    // smooth functions
    TSmoothSingle       SmoothSingle;
    TSmoothEvent        SmoothEvent;
    TMultEvt2Audio      MultEvt2Audio;

    int                 CheckCode;  // used internaly by Usine
};

//-----------------------------------------------------------------------------
// possible type of callback system 
// ctNormal       : processed in the normal Usine messages handler. 
//                  The latencie is defined by the User in the Usine Setup
// ctImmediate    : each time the parameter change, callback is processed immediatelly;
//                  be careful, the the callBack procedure is called in the audio thread;
// ctAsynchronous : processed in the window message handler : 
//                  use this if the process is very long or if you open a modal window;
//                  slower than the fcNormal
// used in TParamInfo (see below) 
enum TFastCallBackType 
{
    ctNormal,       
    ctImmediate,    
    ctAsynchronous  
};

//-----------------------------------------------------------------------------
// usine parameter description
// used in GetParamInfo function declaration (see end of file)
struct TParamInfo 
{
    TParamType        ParamType;            // parameter type
    PCHAR             Caption;              // displayed caption
    LONGBOOL          IsInput;              // TRUE if the parameter has input port
    LONGBOOL          IsOutput;             // TRUE if the parameter has output port
    TScale            Scale;                // scale, if the param is a fader
    int               Color;                // front color of the fader or on Color of the switch
    int               OffColor;             // off color only if the parameter is a switch
    float             MinValue;             // min param value
    float             MaxValue;             // max param value
    float             DefaultValue;         // default param value, when created
    PCHAR             Symbol;               // displayed symbol
    PCHAR             Format;               // format string of the displayed value
    PCHAR             ListBoxStrings;       // if listbox looks like '"item1","item2"'
    PCHAR             TextValue;            // default text value if param type = ptTextField
    LONGBOOL          ReadOnly;             // TRUE if the user cant modify value
    TFastCallBackType CallBackType;         // see TFastCallBackType definition
    LONGBOOL          DontSave;             // specifies if the parameter need to be save or not
    int               DisplayOrder;         // set the display order of the parameter in the list (optional)
    LONGBOOL          IsSeparator;          // determines if the parameter is followed by a blank line in the list
    LONGBOOL          IsInvisible;          // set the parameter totally invisible
    LONGBOOL          IsStoredInConductor;  // set the parameter stored in the conductor, can be CPU killer with array (default false)
    LONGBOOL          IsStoredInPreset;     // set the param stored in the preset (default true)
};

//-----------------------------------------------------------------------------
// 3 kind of user modules
// mtSimple : no panel and no form
// mtForm   : has a form that appears when the user dblClick on the module
// mtControl : has a control panel that appears on the control panel
// used in TModuleInfo structure (see below) 
typedef enum TModuleType 
{ 
    mtSimple, 
    mtForm, 
    mtControl
};

//-------------------------------------------
// general module information
// used in InitModule and GetModuleInfo functions declaration (see end of file)
struct TModuleInfo 
{
    PCHAR       Name;           // short name displayed in the patch view
    PCHAR       Description;    // long name displayed in the browser
    TModuleType ModuleType;     // module type: simple, form, control
    TColorUSINE ModuleColor;    // module color in the patch panel
    int         NumberOfParams; // number of parameters

    // form and panelwidth
    //   if moduletype = mtForm : form width
    //   if moduletype = mtControl :
    //      width is set to MasterInfo.PanelWidth
    //   if moduletype = mtSimple : ignored
    int         DefaultWidth;   
    
    // form and panel height
    //   if moduletype = mtSimple : ignored
    int         DefaultHeight;

    LONGBOOL    DontProcess;        // TRUE if the module doesn't need any procesing 

    // default usine layout params
    TColorUSINE BackgroundColor;    // color of the background
    TColorUSINE BorderColor;        // color of the border
    LONGBOOL    ShowBorder;         // if TRUE the border must be drawn
    int         BorderWidth;        // width of the panel border

    //option for not show additionnal parameters, false by default  
    LONGBOOL DoNotCreateAddCtrl ;   // if true usine don't create parameters like mousedown, hint, etc...

    // pointer to Usine internal User Module
    void*   UsineUserModule;    

    
    // to verify that the module has been created properly
    int     CheckCode; // Usine internal use

    // pre creation query system
    PCHAR   QueryString;        // string to ask about entries numbers
    PCHAR   QueryListValues;    // coma separated list with possible responce
    int     QueryDefaultIdx;    // pre selected choice index for the coma list (0 to N-1)
    int     QueryIdx;           // this value is set by usine

    // layout infos
    // used only with mtForm and mtControl modules
    PCHAR       BackBitMapFilename; // file name of the background bitmap
    LONGBOOL    Rounded;            // rounded corners style panel
    int         RoundSize;          // round corners size
    TColorUSINE CaptionColor;       // backcolor of the caption area
    TColorUSINE CaptionTextColor;   // caption font color
    int         CaptionTextSize;    // caption font size
    LONGBOOL    ShowCaption;        // show the caption area  
    int         CaptionHeight;      // filled by usine: gives the caption bar height
    PCHAR       Caption;            // caption string (set by the user)
};

//-----------------------------------------------------------------------------
// additionnals type that are not defined by Usine
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// int point for coordinates in pixels
struct TPointF 
{
    float x;
    float y;
};

//-----------------------------------------------------------------------------
// float point for coordinates in coefficiant (from 0 to 1)
struct TPointI 
{
    int x;
    int y;
};

//-----------------------------------------------------------------------------
// RGB color
struct TColorRGB
{
    float a;   
    float r;
    float g;
    float b;
}; 
//-----------------------------------------------------------------------------
// HSL color
struct TColorHSL
{  
    float a;
    float h;
    float s;
    float l;
}; 

//-----------------------------------------------------------------------------
// end of memory alignment with DELPHI
#pragma pack(pop)

//-----------------------------------------------------------------------------
// globals functions exposed by the module to communicate with usine
//-----------------------------------------------------------------------------

#define DllExport extern "C" __declspec( dllexport )

//-----------------------------------------------------------------------------
// create, general info and destroy
// called by usine to get the version of the SDK used
DllExport const int GetSDKVersion(void);
// called by usine to get implementation type (language + compilator) of the module
DllExport TImplementation GetImplementation(void);
// Create Module
DllExport void Create(void* &pModule); 
// Destroy Module
DllExport void Destroy(void* pModule); 
// called by usine to get modules informations
DllExport void GetModuleInfo (void* pModule, TMasterInfo* pMasterInfo, TModuleInfo* pModuleInfo);

//-----------------------------------------------------------------------------
// query system and init
// Get total parameters number of the module
DllExport int GetNumberOfParams( int QIdx);
// called after the query popup
DllExport void AfterQuery (void* pModule, TMasterInfo* pMasterInfo, TModuleInfo* pModuleInfo, int QIdx);
// initialization procedure
DllExport void InitModule (void* pModule, TMasterInfo* pMasterInfo, TModuleInfo* pModuleInfo);

//-----------------------------------------------------------------------------
// parameters and process
// called by usine to get params informations
DllExport void GetParamInfo (void* pModule,int ParamIndex, TParamInfo* pParamInfo);
// tels to the module what are the effectives events address
DllExport void SetEventAddress (void* pModule, int ParamIndex, TEVT* pEvent);
// called by Usine when a parameter value has changed
DllExport void CallBack (void* pModule, TMessage *Message);
// main process procedure
DllExport void Process (void* pModule);

//-----------------------------------------------------------------------------
// chunk system
// returns the chunk string len, needed for master memory allocation
DllExport int GetChunkLen (void* pModule, LONGBOOL Preset);
// get the chunk string used to store modules intern data
// ! : all parameters values are strored automatically by Usine
DllExport void GetChunk (void* pModule, void* chunk, LONGBOOL Preset);
// chunk string send by Usine when loading
DllExport void SetChunk (void* pModule, const void* chunk, LONGBOOL Preset);

//-----------------------------------------------------------------------------
// layout
// create user defined layout options in the 'edit layout' panel
DllExport void CreateLayout(void* pModule);
// Called when someting has changed in the layout
DllExport void LayoutHasChanged(void* pModule);
// resize the panel
DllExport void Resize (void* pModule, int W, int H);
// called by Usine to paint the panel
DllExport void Paint (void* pModule, HDC DC, DWORD ParentColor);

//-----------------------------------------------------------------------------
// mouse and multi touch
// mouse move event
DllExport void MouseMove(void* pModule, TShiftState Shift, int X, int Y);
// mouse down event
DllExport void MouseDown(void* pModule, TMouseButton Button, TShiftState Shift, int X,int Y);
// mouse up event
DllExport void MouseUp (void* pModule,  TMouseButton Button, TShiftState Shift, int X,int Y);
// multi touch move event
DllExport void MouseMoveMultiProc(void* pModule, TShiftState Shift, TEVT* X, TEVT* Y);
// multi touch down event
DllExport void MouseDownMultiProc(void* pModule, TMouseButton Button, TShiftState Shift, TEVT* X, TEVT* Y);
// multi touch up event
DllExport void MouseUpMultiProc (void* pModule, TMouseButton Button, TShiftState Shift,TEVT* X, TEVT* Y);

#endif // _USINEDEFINITIONS_H