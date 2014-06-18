// SpeechRecognition.cpp: implementation of the CTTSclass.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpeechRecognition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CMDGRAMMER 11111

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTTS::CTTS()
{

}

CTTS::~CTTS()
{
	//Destroy();
}


BOOL CTTS::Initialize(HWND hWnd, BOOL bIsShared)
{
	if (FAILED(CoInitialize(NULL))) 
	{
		m_sError=_T("Error intialization COM");
		return FALSE;
	}

	HRESULT hr = S_OK;

	if (bIsShared)
	{
		// Shared reco engine.
		// For a shared reco engine, the audio gets setup automatically
		hr = m_cpRecoEngine.CoCreateInstance( CLSID_SpSharedRecognizer );
	}

	else
	{
		hr = m_cpRecoEngine.CoCreateInstance(CLSID_SpInprocRecognizer);
	}

	if (SUCCEEDED(hr))
	{

		hr = m_cpVoice.CoCreateInstance( CLSID_SpVoice );
	}
	return (hr == S_OK);

}



//释放函数Destroy被类的析构函数调用，释放了类所引用的所有接口：

void CTTS::Destroy()
{
	if (m_cpDictationGrammar) 
		m_cpDictationGrammar.Release();
	if (m_cpRecoCtxt) 
		m_cpRecoCtxt.Release();
	if (m_cpRecoEngine) 
		m_cpRecoEngine.Release();
	CoUninitialize();
}

////函数Start和Stop用来控制开始和停止接受及识别语音，它们通过调用引擎接口的SetRecoState方法来实现：/////

BOOL CTTS::Start()
{
	if (m_bOnDictation)
		return TRUE;
	HRESULT hr = m_cpRecoEngine->SetRecoState( SPRST_ACTIVE );
	if (FAILED(hr))
		return FALSE;
	m_bOnDictation = TRUE;
	return TRUE;
}


BOOL CTTS::Stop()
{
	if (! m_bOnDictation)
		return TRUE;

	HRESULT hr = m_cpRecoEngine->SetRecoState( SPRST_INACTIVE );
	if (FAILED(hr))
		return FALSE;
	m_bOnDictation = FALSE;
	return TRUE;
}


CString CTTS::GetText(ULONG ulStart, ULONG nlCount)
{
	USES_CONVERSION;
	CSpEvent event;
	CSpDynamicString dstrText;
	// Process all of the recognition events
	while (event.GetFrom(m_cpRecoCtxt) == S_OK)
	{
		switch (event.eEventId)
		{
			case SPEI_RECOGNITION:
			// There may be multiple recognition results, so get all of them
			{
				HRESULT hr = S_OK;
				if (nlCount == -1)
					event.RecoResult()->GetText(SP_GETWHOLEPHRASE, 
					SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL);
				else
				{
					ASSERT(nlCount > 0);
					event.RecoResult()->GetText(ulStart, nlCount, FALSE, 
						&dstrText, NULL);
				}
			}
			break;
		}
	}
	CString str=(CString)dstrText;
	return str;
}


HRESULT CTTS::InitTokenList(HWND hWnd, BOOL bIsComboBox)
{
	if (bIsComboBox)
		return SpInitTokenComboBox(hWnd, SPCAT_RECOGNIZERS);
	else
		return SpInitTokenListBox(hWnd, SPCAT_RECOGNIZERS);
}


//语音识别涉及语音的输入，通常用话筒来输入语音。进行语音识别前，
//需要判断话筒的位置和设置是否合理，以保证语音识别引擎能获得有效的语音输入
//函数MicrophoneSetup调用语音识别引擎接口的DisplayUI方法来显示一个设置话筒的向导：

HRESULT CTTS::MicrophoneSetup(HWND hWndParent)
{
	return m_cpRecoEngine->DisplayUI(hWndParent, NULL, SPDUI_MicTraining, NULL, 0);
}


// 语音训练是语音识别的重要基础，为了获得期望的识别效果，
// 必须进行语音训练，以让语音识别引擎熟悉说话者的口音。
// 函数VoiceTraining调用语音识别引擎接口的DisplayUI方法来显示一个语音训练向导,如下所示：

HRESULT CTTS::VoiceTraining(HWND hWndParent)
{
	return m_cpRecoEngine->DisplayUI(hWndParent, NULL, SPDUI_UserTraining, NULL, 0);
}


void CTTS::Speak(CString str)
{
	USES_CONVERSION;
	m_strText=str;
	LPWSTR lpwstr=new wchar_t[m_strText.GetLength()+1];
	wcscpy(lpwstr,T2W((LPCTSTR)m_strText));
	m_cpVoice->Speak(lpwstr, SPF_ASYNC, NULL);
}
