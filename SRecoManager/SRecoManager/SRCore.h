#ifndef SELFUSE
#define SELFUSE 654321

#ifdef SpeechSDK 
#include"sapi.h"
#include "sphelper.h"
#define ID_DICTIONARY                   101

/// <summary>
/// ���ݸ������̵߳����ݽṹ
/// </summary>
struct MONITOR_STRUCT
{
	HWND hWnd;			//�����������ݵ�ʱ��Ҫ���͵Ĵ��ھ��
	CString yufaku;		//Ҫ���ص��﷨��
	UINT Msg;			//�����������ݵ�ʱ���͵�msg
};

inline HRESULT BlockForResult(ISpRecoContext * pRecoCtxt , ISpRecoResult ** ppResult)//ʶ�������İ����������ݴ���ʶ������ppResult����
{
	HRESULT hr = S_OK;
	CSpEvent event;
	
	while (SUCCEEDED(hr) &&
		SUCCEEDED(hr = event.GetFrom(pRecoCtxt)) &&
		hr == S_FALSE)
	{
		hr = pRecoCtxt->WaitForNotifyEvent(INFINITE);
	}
	
	*ppResult = event.RecoResult();
	if (*ppResult)
	{
		(*ppResult)->AddRef();
	}
	return hr;
}

/// <summary>
/// �����Ƿ�˵�����߳�
/// <param name="monitor_sturct">���ݽ������ڼ��������ݽṹ</param>
/// </summary>
inline DWORD WINAPI SRMonitor(LPVOID monitor_sturct)
{
	HWND hWnd=((MONITOR_STRUCT*)monitor_sturct)->hWnd;
	CString yufaku=((MONITOR_STRUCT*)monitor_sturct)->yufaku;
	UINT Msg=((MONITOR_STRUCT*)monitor_sturct)->Msg;
	HRESULT hr = E_FAIL;

	CString xml=yufaku.Right(3);
	bool flag_yufa;
	xml.MakeLower();
	if(xml!="xml")
	{
		flag_yufa=false;
	}
	else
	{
		flag_yufa=true;
	}


	if (SUCCEEDED(hr = ::CoInitialize(NULL)))														//��ʼ��COM��
	{
		{
			CComPtr<ISpRecoContext> cpRecoCtxt;
			CComPtr<ISpRecoGrammar> cpGrammar;
			/*CComPtr<ISpVoice> cpVoice;*/
			hr = cpRecoCtxt.CoCreateInstance(CLSID_SpSharedRecoContext);

			//�﷨�����ǰ׼��
			WCHAR wszXMLFile[100]=L"";
			MultiByteToWideChar(CP_ACP, 0, (LPCSTR)yufaku, -1, wszXMLFile, 256);					//ANSIתUNINCODE

			if (cpRecoCtxt /*&& cpVoice*/ &&
				SUCCEEDED(hr = cpRecoCtxt->SetNotifyWin32Event()) &&
                SUCCEEDED(hr = cpRecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION))) &&//���ø���Ȥ�¼�
                SUCCEEDED(hr = cpRecoCtxt->SetAudioOptions(SPAO_RETAIN_AUDIO, NULL, NULL)))
			{
				
				if (flag_yufa)
				{
					
					if(	FAILED(hr = cpRecoCtxt->CreateGrammar(ID_DICTIONARY, &cpGrammar)) ||	//�����﷨����Ϊʹ�������﷨��
						FAILED(hr = cpGrammar->LoadCmdFromFile(wszXMLFile,SPLO_DYNAMIC)))		//�����﷨��
					{
						::CoUninitialize();
						AfxMessageBox("���������﷨��ʧ�ܣ�");
						return 0;
					}
				}
				/*	const WCHAR * const pchStop = StopWord();*/
				else
				{
					if(	FAILED(hr = cpRecoCtxt->CreateGrammar(0, &cpGrammar)) ||				//�����﷨����Ϊʹ�������﷨��
						FAILED(hr = cpGrammar->LoadDictation(NULL, SPLO_STATIC)) )				//�����﷨��
					{
						::CoUninitialize();
						AfxMessageBox("����ͨ���﷨��ʧ�ܣ�");
						return 0;
					}
				}

				if(	FAILED(hr = cpGrammar->SetDictationState(SPRS_ACTIVE))||					//�����﷨��
					FAILED(hr = cpGrammar->SetRuleState( NULL,NULL,SPRS_ACTIVE )))				
				{
					::CoUninitialize();
					return 0;
				}
				USES_CONVERSION;																//���ڱ���ת��������CString��LPCWSTR��
				CComPtr<ISpRecoResult> cpResult;
				
				//��ʼ����
				while (SUCCEEDED(hr = BlockForResult(cpRecoCtxt, &cpResult)))
				{
					cpGrammar->SetDictationState(SPRS_INACTIVE);								//���������ݺ�ʹ�﷨��ʧЧ
					
					CSpDynamicString dstrText;
					
					//�����������ݴ���dstrText
					if (SUCCEEDED(cpResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, 
						TRUE, &dstrText, NULL)))
					{
						
						CString tingdata=(CString)dstrText;

						//�����������ݺ󣬰Ѽ����������ݷ��͵�ָ���Ĵ���
						::SendMessage(hWnd,Msg,(WPARAM)&tingdata,NULL);							

						cpResult.Release();
					}
					cpGrammar->SetDictationState(SPRS_ACTIVE);									//���¼����﷨��
				} 
			}
		}
		::CoUninitialize();
	}
	return 0;
}

inline void CreateSRThread(HANDLE& xiancheng,LPVOID info,LPTHREAD_START_ROUTINE thread=SRMonitor)
{
	xiancheng=CreateThread(NULL,0,thread,(LPVOID)info,0,NULL);
	CloseHandle(xiancheng);
}


#endif//SpeakSDK

#endif
