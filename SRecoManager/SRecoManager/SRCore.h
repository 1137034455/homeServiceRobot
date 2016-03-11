#ifndef SELFUSE
#define SELFUSE 654321

#ifdef SpeechSDK 
#include"sapi.h"
#include "sphelper.h"
#define ID_DICTIONARY                   101

/// <summary>
/// 传递给监听线程的数据结构
/// </summary>
struct MONITOR_STRUCT
{
	HWND hWnd;			//当监听到数据的时候要发送的窗口句柄
	CString yufaku;		//要加载的语法库
	UINT Msg;			//当监听到数据的时候发送的msg
};

inline HRESULT BlockForResult(ISpRecoContext * pRecoCtxt , ISpRecoResult ** ppResult)//识别上下文把听到的数据存于识别结果（ppResult）中
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
/// 监听是否说话的线程
/// <param name="monitor_sturct">传递进来用于监听的数据结构</param>
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


	if (SUCCEEDED(hr = ::CoInitialize(NULL)))														//初始化COM口
	{
		{
			CComPtr<ISpRecoContext> cpRecoCtxt;
			CComPtr<ISpRecoGrammar> cpGrammar;
			/*CComPtr<ISpVoice> cpVoice;*/
			hr = cpRecoCtxt.CoCreateInstance(CLSID_SpSharedRecoContext);

			//语法库加载前准备
			WCHAR wszXMLFile[100]=L"";
			MultiByteToWideChar(CP_ACP, 0, (LPCSTR)yufaku, -1, wszXMLFile, 256);					//ANSI转UNINCODE

			if (cpRecoCtxt /*&& cpVoice*/ &&
				SUCCEEDED(hr = cpRecoCtxt->SetNotifyWin32Event()) &&
                SUCCEEDED(hr = cpRecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION))) &&//设置感兴趣事件
                SUCCEEDED(hr = cpRecoCtxt->SetAudioOptions(SPAO_RETAIN_AUDIO, NULL, NULL)))
			{
				
				if (flag_yufa)
				{
					
					if(	FAILED(hr = cpRecoCtxt->CreateGrammar(ID_DICTIONARY, &cpGrammar)) ||	//创建语法规则为使用命令语法库
						FAILED(hr = cpGrammar->LoadCmdFromFile(wszXMLFile,SPLO_DYNAMIC)))		//加载语法库
					{
						::CoUninitialize();
						AfxMessageBox("加载命令语法库失败！");
						return 0;
					}
				}
				/*	const WCHAR * const pchStop = StopWord();*/
				else
				{
					if(	FAILED(hr = cpRecoCtxt->CreateGrammar(0, &cpGrammar)) ||				//创建语法规则为使用命令语法库
						FAILED(hr = cpGrammar->LoadDictation(NULL, SPLO_STATIC)) )				//加载语法库
					{
						::CoUninitialize();
						AfxMessageBox("加载通用语法库失败！");
						return 0;
					}
				}

				if(	FAILED(hr = cpGrammar->SetDictationState(SPRS_ACTIVE))||					//激活语法库
					FAILED(hr = cpGrammar->SetRuleState( NULL,NULL,SPRS_ACTIVE )))				
				{
					::CoUninitialize();
					return 0;
				}
				USES_CONVERSION;																//用于编码转换（比如CString到LPCWSTR）
				CComPtr<ISpRecoResult> cpResult;
				
				//开始聆听
				while (SUCCEEDED(hr = BlockForResult(cpRecoCtxt, &cpResult)))
				{
					cpGrammar->SetDictationState(SPRS_INACTIVE);								//聆听到数据后使语法库失效
					
					CSpDynamicString dstrText;
					
					//把听到的数据存入dstrText
					if (SUCCEEDED(cpResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, 
						TRUE, &dstrText, NULL)))
					{
						
						CString tingdata=(CString)dstrText;

						//当监听到数据后，把监听到的数据发送到指定的窗口
						::SendMessage(hWnd,Msg,(WPARAM)&tingdata,NULL);							

						cpResult.Release();
					}
					cpGrammar->SetDictationState(SPRS_ACTIVE);									//重新激活语法库
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
