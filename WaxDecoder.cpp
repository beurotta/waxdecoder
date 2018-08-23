//-----------------------------------------------------------------------------
//
//@file  
//	WaxDecoder.cpp
//
//@date 
//	2012/07/xx
//
//@author
//  naarud
//
//@brief 
//	Implementation of the TWaxDecoder class.
//
//@historic 
//	2012/07/04
//    first release, implement the timecoder used in v1.2 of xwax
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include "WaxDecoder.h"

//----------------------------------------------------------------------------
// create, general info and destroy methodes
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Get the SDK version
const int GetSDKVersion(void) 
{
	return SDK_VERSION;
}

//-----------------------------------------------------------------------------
// Get the module implementation
TImplementation GetImplementation() 
{
  return CPP_GCC_432;
}

//-----------------------------------------------------------------------------
// Create
void Create(void* &pModule) 
{
	pModule = new TWaxDecoder();
}

//-----------------------------------------------------------------------------
// destroy
void Destroy(void* pModule) 
{
	// cast is important to call the good destructor
	delete ((TWaxDecoder*)pModule);
}

//-----------------------------------------------------------------------------
// module description
void GetModuleInfo (void* pModule, TMasterInfo* pMasterInfo, TModuleInfo* pModuleInfo) 
{
	pModuleInfo->Name				= "WaxDecoder";
	pModuleInfo->Description		= "Wax Decoder";
	pModuleInfo->ModuleType         = mtSimple;
	pModuleInfo->ModuleColor        = ColorUSINE(128, 128, 128);
	pModuleInfo->NumberOfParams     = 6;
	pModuleInfo->DontProcess		= false;
}

//-----------------------------------------------------------------------------
// query system and init methodes
//-----------------------------------------------------------------------------

// query system not used

//-----------------------------------------------------------------------------
// initialisation
void InitModule (void* pModule, TMasterInfo* pMasterInfo, TModuleInfo* pModuleInfo) 
{
	// make convenient pointer to the module
	TWaxDecoder* pWaxDecoder = ((TWaxDecoder*)pModule);
	
	// remember, it's up to us to initialize the UserModule
	pWaxDecoder->Init(pMasterInfo, pModuleInfo);
	
	// init timecoder to 'serato_2a' timecode at 33 rpm
	pWaxDecoder->LoadTimecoder(TC_NAMES[0], RPM_SPEED[0]);
}

//----------------------------------------------------------------------------
// parameters and process
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Parameters description
void GetParamInfo (void* pModule,int n, TParamInfo* pParamInfo) 
{	
	switch (n) 
    {
    // Audio in
	case 0:
		pParamInfo->ParamType		= ptAudio;
		pParamInfo->Caption			= "in L";
		pParamInfo->IsInput			= true;
		pParamInfo->IsOutput		= false;
		pParamInfo->ReadOnly		= false;
		break;

    // Audio in
	case 1:
		pParamInfo->ParamType		= ptAudio;
		pParamInfo->Caption			= "in R";
		pParamInfo->IsInput			= true;
		pParamInfo->IsOutput		= false;
		pParamInfo->ReadOnly		= false;
		break;

	// timecode name input
	case 2:
		pParamInfo->ParamType		= ptListBox;
		pParamInfo->Caption			= "timecode";
		pParamInfo->ListBoxStrings	= "\"serato_2a\",\"serato_2b\",\"serato_cd\",\"traktor_a\",\"traktor_b\",\"mixvibes_v2\",\"mixvibes_7inch\"";
		pParamInfo->IsInput			= true;
		pParamInfo->IsOutput		= false;
		pParamInfo->DefaultValue	= 0;
		pParamInfo->ReadOnly		= false;
		pParamInfo->CallBackType	= ctNormal;
		break;

	// rotational speed input
	case 3:
		pParamInfo->ParamType		= ptListBox;
		pParamInfo->Caption			= "rpm";
		pParamInfo->ListBoxStrings	= "\"33\",\"45\"";
		pParamInfo->IsInput			= true;
		pParamInfo->IsOutput		= false;
		pParamInfo->DefaultValue	= 0;
		pParamInfo->ReadOnly		= false;
		pParamInfo->CallBackType	= ctNormal;
		break;

	// position output
	case 4:
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
		break;

	// pitch output
	case 5:
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
		break;

		// default case
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// set the parameters events address
void SetEventAddress (void* pModule, int n, TEVT* pEvent) 
{
	// make convenient pointer to the module
	TWaxDecoder* pWaxDecoder = ((TWaxDecoder*)pModule);
	
	switch (n) 
    {
	
    // audio input
	case 0:
		pWaxDecoder->m_audioInputTab[0] = pEvent;
		break;

    // audio input
	case 1:
		pWaxDecoder->m_audioInputTab[1] = pEvent;
		break;

    // timecode name listbox input
	case 2:
		pWaxDecoder->m_lbxTimecodes = pEvent;
		break;

    // rotational speed listbox input
	case 3:
		pWaxDecoder->m_lbxRpmSpeed = pEvent;
		break;

    // position data field output
	case 4:
		pWaxDecoder->m_dtfPositionOut = pEvent;
		break;

    // pitch data field output
	case 5:
		pWaxDecoder->m_dtfPitchOut = pEvent;
		break;

	// default case
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// Parameters callback
void CallBack (void* pModule, TMessage* Message) 
{
	// make convenient pointer to the module
	TWaxDecoder* pWaxDecoder = ((TWaxDecoder*)pModule);
	
	// param: timecode or rpm list box inputs
	if ((Message->wParam==2) || (Message->wParam==3))
        if(Message->lParam==MSG_CHANGE)
        {
            int TC_i = pWaxDecoder->GetEvtData(pWaxDecoder->m_lbxTimecodes);
            int RPM_i = pWaxDecoder->GetEvtData(pWaxDecoder->m_lbxRpmSpeed);
            
            pWaxDecoder->LoadTimecoder(TC_NAMES[TC_i], RPM_SPEED[RPM_i]);
        }
}

//-----------------------------------------------------------------------------
// Main process
void Process (void* pModule) 
{
	((TWaxDecoder*)pModule)->OutputTCoder();
}

//----------------------------------------------------------------------------
// layout
//----------------------------------------------------------------------------

void CreateLayout(void* pModule){}
void LayoutHasChanged(void* pModule){}
void Resize (void* pModule, int W, int H) { }
void Paint (void* pModule, HDC DC, DWORD ParentColor) {}

//----------------------------------------------------------------------------
// chunk system
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GetChunkLen : No chunk to store --> result := 0;
int GetChunkLen (void* pModule) 
{
	return 0;
}

//-----------------------------------------------------------------------------
// GetChunk : No chunk to store --> nothing to do...
void GetChunk (void* pModule, PCHAR chunk) {}

//-----------------------------------------------------------------------------
// SetChunk No chunk to restore --> nothing to do...
void SetChunk (void* pModule, const PCHAR chunk) {}

