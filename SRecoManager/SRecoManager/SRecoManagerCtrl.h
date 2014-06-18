#pragma once

// SRecoManagerCtrl.h : CSRecoManagerCtrl ActiveX 控件类的声明。


// CSRecoManagerCtrl : 有关实现的信息，请参阅 SRecoManagerCtrl.cpp。
#define WM_TINGDATA WM_USER+1265
#define SpeechSDK
#include "CSelfUse.h"
#include "SpeechRecognition.h"

class CSRecoManagerCtrl : public COleControl
{
	DECLARE_DYNCREATE(CSRecoManagerCtrl)

// 构造函数
public:
	CSRecoManagerCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CSRecoManagerCtrl();

	DECLARE_OLECREATE_EX(CSRecoManagerCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CSRecoManagerCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSRecoManagerCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CSRecoManagerCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnTingData(WPARAM wParam,LPARAM lParam);
// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {		
		dispidTest = 6L,
		dispidLoadSRGrammar = 5L,
		dispidInitSRManager = 4L,
		dispidWriteOrderToXML = 3L,
		dispidSpeak = 2L,
		eventidSRecognizeEvent = 2L,
		dispidGetSRecoResult = 1L
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	HANDLE h_Jianting;
	CString m_pathYufaku;
	CString m_TingData;
	CTTS m_TTS;
	JIANTING* info;
protected:
	BSTR GetSRecoResult(void);	

	void SRecognizeEvent(void)
	{
		FireEvent(eventidSRecognizeEvent, EVENT_PARAM(VTS_NONE));
	}
public:
	afx_msg void OnDestroy();
protected:
	
	void Speak(LPCTSTR strSpeak);
	void WriteOrderToXML(LPCTSTR path_Yufaku, LPCTSTR str_Order);
	//void InitSRecoManager(void);
	LONG InitSRManager(LPCTSTR path_Yufaku);
	void LoadSRGrammar(void);
	void Test(void);
};

