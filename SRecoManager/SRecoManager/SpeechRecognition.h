// SpeechRecognition.h: interface for the CTTSclass.
//
//////////////////////////////////////////////////////////////////////

#include <atlbase.h>
#include "afxwin.h"
extern CComModule _Module;
#include <atlcom.h>
#include <sapi.h>
#include "sphelper.h"
#include <spuihelp.h>


#define WM_SREVENT   WM_USER+102


#if !defined(AFX_SPEECHRECOGNITION_H__72AB0E74_B869_45F1_B354_C20F8D1EEE72__INCLUDED_)
#define AFX_SPEECHRECOGNITION_H__72AB0E74_B869_45F1_B354_C20F8D1EEE72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTTS 
{
public:
	CString m_strText;
	void Speak(CString str);
	CTTS();
	virtual ~CTTS();

	BOOL IsDictationOn()
	{
		return m_bOnDictation;
	}
	BOOL Stop();
	BOOL Start();
	void Destroy();
	BOOL Initialize(HWND hWnd = NULL, BOOL bIsShared = TRUE);

	CString GetText( ULONG ulStart = 0, ULONG nlCount = -1);

	HRESULT VoiceTraining(HWND hWndParent);
	
	
	
	// microphone setup
	
	HRESULT MicrophoneSetup(HWND hWndParent);
	
	
	
	// token list
	
	HRESULT InitTokenList(HWND hWnd, BOOL bIsComboBox = FALSE);
	
	
	
	// error string
	
	CString GetErrorString()
		
	{
		
		return m_sError;
		
	}
	
	
	
	// interface
	
	CComPtr<ISpRecognizer> m_cpRecoEngine;  // SR engine
	
	CComPtr<ISpRecoContext> m_cpRecoCtxt;   //Recognition contextfor dictation
	
	CComPtr<ISpRecoGrammar> m_cpDictationGrammar;  // Dictation grammar 

	CComPtr<ISpVoice>m_cpVoice;
	
	
	
private:
	
	CString m_sError;
    BOOL    m_bOnDictation;



};

#endif // !defined(AFX_SPEECHRECOGNITION_H__72AB0E74_B869_45F1_B354_C20F8D1EEE72__INCLUDED_)
