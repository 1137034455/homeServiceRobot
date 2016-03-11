// SRecoManagerCtrl.cpp : CSRecoManagerCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "SRecoManager.h"
#include "SRecoManagerCtrl.h"
#include "SRecoManagerPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define MAXLEN 1000

IMPLEMENT_DYNCREATE(CSRecoManagerCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CSRecoManagerCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_MESSAGE(WM_TINGDATA,OnTingData)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CSRecoManagerCtrl, COleControl)
	DISP_FUNCTION_ID(CSRecoManagerCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSRecoManagerCtrl, "GetSRecoResult", dispidGetSRecoResult, GetSRecoResult, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CSRecoManagerCtrl, "Speak", dispidSpeak, Speak, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CSRecoManagerCtrl, "WriteOrderToXML", dispidWriteOrderToXML, WriteOrderToXML, VT_EMPTY, VTS_BSTR VTS_BSTR)
	//DISP_FUNCTION_ID(CSRecoManagerCtrl, "InitSRecoManager", dispidInitSRecoManager, InitSRecoManager, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSRecoManagerCtrl, "InitSRManager", dispidInitSRManager, InitSRManager, VT_I4, VTS_BSTR)
	DISP_FUNCTION_ID(CSRecoManagerCtrl, "LoadSRGrammar", dispidLoadSRGrammar, LoadSRGrammar, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CSRecoManagerCtrl, "Test", dispidTest, Test, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CSRecoManagerCtrl, COleControl)
	//EVENT_CUSTOM_ID("SRecoEvent", eventidSRecoEvent, SRecoEvent, VTS_NONE)
	EVENT_CUSTOM_ID("SRecognizeEvent", eventidSRecognizeEvent, SRecognizeEvent, VTS_NONE)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CSRecoManagerCtrl, 1)
	PROPPAGEID(CSRecoManagerPropPage::guid)
END_PROPPAGEIDS(CSRecoManagerCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CSRecoManagerCtrl, "SRECOMANAGER.SRecoManagerCtrl.1",
	0xe6a83528, 0x143b, 0x4c33, 0xb7, 0xed, 0x53, 0xc6, 0x39, 0x11, 0xcc, 0xcf)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CSRecoManagerCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID IID_DSRecoManager = { 0xE06F5A13, 0xE5B4, 0x47BB, { 0xA6, 0xAA, 0x2C, 0x34, 0xC4, 0xF5, 0x20, 0x91 } };
const IID IID_DSRecoManagerEvents = { 0x489CD01C, 0x3062, 0x4B02, { 0x9C, 0x30, 0x7E, 0x13, 0x95, 0x40, 0x7B, 0x7F } };


// �ؼ�������Ϣ

static const DWORD _dwSRecoManagerOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSRecoManagerCtrl, IDS_SRECOMANAGER, _dwSRecoManagerOleMisc)



// CSRecoManagerCtrl::CSRecoManagerCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CSRecoManagerCtrl ��ϵͳע�����

BOOL CSRecoManagerCtrl::CSRecoManagerCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_SRECOMANAGER,
			IDB_SRECOMANAGER,
			afxRegApartmentThreading,
			_dwSRecoManagerOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CSRecoManagerCtrl::CSRecoManagerCtrl - ���캯��

CSRecoManagerCtrl::CSRecoManagerCtrl()
{
	InitializeIIDs(&IID_DSRecoManager, &IID_DSRecoManagerEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
}



// CSRecoManagerCtrl::~CSRecoManagerCtrl - ��������

CSRecoManagerCtrl::~CSRecoManagerCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}



// CSRecoManagerCtrl::OnDraw - ��ͼ����

void CSRecoManagerCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));
}



// CSRecoManagerCtrl::DoPropExchange - �־���֧��

void CSRecoManagerCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CSRecoManagerCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CSRecoManagerCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CSRecoManagerCtrl::AboutBox - ���û���ʾ�����ڡ���

void CSRecoManagerCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_SRECOMANAGER);
	dlgAbout.DoModal();
}



// CSRecoManagerCtrl ��Ϣ�������


int CSRecoManagerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	
	return 0;
}

LRESULT CSRecoManagerCtrl::OnTingData(WPARAM wParam,LPARAM lParam)
{
	m_TingData=*(CString*)wParam;
	SRecognizeEvent();
	return -1;
}


BSTR CSRecoManagerCtrl::GetSRecoResult(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: �ڴ���ӵ��ȴ���������
	strResult=m_TingData;
	return strResult.AllocSysString();
}




void CSRecoManagerCtrl::OnDestroy()
{
	COleControl::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_TTS.Destroy();
}

void CSRecoManagerCtrl::Speak(LPCTSTR strSpeak)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	CString str_Speak=strSpeak;
	m_TTS.Speak(str_Speak);
}


void CSRecoManagerCtrl::WriteOrderToXML(LPCTSTR path_Yufaku, LPCTSTR str_Order)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	WriteXML(path_Yufaku,str_Order);
}

CString CSRecoManagerCtrl::getModulePath(){
		CString module_path;
		char module[MAXLEN];
		::GetModuleFileName(AfxGetInstanceHandle(),module,MAXLEN);
		module_path=module;
		char a='\\';
		module_path=module_path.Left(module_path.ReverseFind(a));
		return module_path;
}

LONG CSRecoManagerCtrl::InitSRManager(LPCTSTR path_Yufaku)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	if (AmbientUserMode())
	{
		if (!m_TTS.Initialize())
		{
			return 0;
		}

		//��ȡ��ǰ��·��
// 		CString module_path=getModulePath();
// 		m_pathYufaku=module_path+"\\action.xml";

		m_pathYufaku=path_Yufaku;
		info=new MONITOR_STRUCT;
		info->hWnd=m_hWnd;
		info->yufaku=m_pathYufaku;
		info->Msg=WM_TINGDATA;
		CreateSRThread(h_Jianting,info);
	}
	return 0;
}


void CSRecoManagerCtrl::LoadSRGrammar(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	TerminateThread(h_Jianting, 0);
	CloseHandle(h_Jianting);

	info->hWnd=m_hWnd;
	info->yufaku=m_pathYufaku;
	info->Msg=WM_TINGDATA;
	CreateSRThread(h_Jianting,info);
}


void CSRecoManagerCtrl::Test(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	MessageBox("��ֻ�Ǹ����ԣ�");
}
