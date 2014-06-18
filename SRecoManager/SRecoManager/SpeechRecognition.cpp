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



//�ͷź���Destroy����������������ã��ͷ����������õ����нӿڣ�

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

////����Start��Stop�������ƿ�ʼ��ֹͣ���ܼ�ʶ������������ͨ����������ӿڵ�SetRecoState������ʵ�֣�/////

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


//����ʶ���漰���������룬ͨ���û�Ͳ��������������������ʶ��ǰ��
//��Ҫ�жϻ�Ͳ��λ�ú������Ƿ�����Ա�֤����ʶ�������ܻ����Ч����������
//����MicrophoneSetup��������ʶ������ӿڵ�DisplayUI��������ʾһ�����û�Ͳ���򵼣�

HRESULT CTTS::MicrophoneSetup(HWND hWndParent)
{
	return m_cpRecoEngine->DisplayUI(hWndParent, NULL, SPDUI_MicTraining, NULL, 0);
}


// ����ѵ��������ʶ�����Ҫ������Ϊ�˻��������ʶ��Ч����
// �����������ѵ������������ʶ��������Ϥ˵���ߵĿ�����
// ����VoiceTraining��������ʶ������ӿڵ�DisplayUI��������ʾһ������ѵ����,������ʾ��

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
