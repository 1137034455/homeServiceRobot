// SRecoManagerCtrl.cpp : CSRecoManagerCtrl ActiveX 控件类的实现。

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



// 消息映射

BEGIN_MESSAGE_MAP(CSRecoManagerCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_MESSAGE(WM_TINGDATA,OnTingData)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// 调度映射

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



// 事件映射

BEGIN_EVENT_MAP(CSRecoManagerCtrl, COleControl)
	//EVENT_CUSTOM_ID("SRecoEvent", eventidSRecoEvent, SRecoEvent, VTS_NONE)
	EVENT_CUSTOM_ID("SRecognizeEvent", eventidSRecognizeEvent, SRecognizeEvent, VTS_NONE)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CSRecoManagerCtrl, 1)
	PROPPAGEID(CSRecoManagerPropPage::guid)
END_PROPPAGEIDS(CSRecoManagerCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CSRecoManagerCtrl, "SRECOMANAGER.SRecoManagerCtrl.1",
	0xe6a83528, 0x143b, 0x4c33, 0xb7, 0xed, 0x53, 0xc6, 0x39, 0x11, 0xcc, 0xcf)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CSRecoManagerCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID IID_DSRecoManager = { 0xE06F5A13, 0xE5B4, 0x47BB, { 0xA6, 0xAA, 0x2C, 0x34, 0xC4, 0xF5, 0x20, 0x91 } };
const IID IID_DSRecoManagerEvents = { 0x489CD01C, 0x3062, 0x4B02, { 0x9C, 0x30, 0x7E, 0x13, 0x95, 0x40, 0x7B, 0x7F } };


// 控件类型信息

static const DWORD _dwSRecoManagerOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSRecoManagerCtrl, IDS_SRECOMANAGER, _dwSRecoManagerOleMisc)



// CSRecoManagerCtrl::CSRecoManagerCtrlFactory::UpdateRegistry -
// 添加或移除 CSRecoManagerCtrl 的系统注册表项

BOOL CSRecoManagerCtrl::CSRecoManagerCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

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



// CSRecoManagerCtrl::CSRecoManagerCtrl - 构造函数

CSRecoManagerCtrl::CSRecoManagerCtrl()
{
	InitializeIIDs(&IID_DSRecoManager, &IID_DSRecoManagerEvents);
	// TODO: 在此初始化控件的实例数据。
}



// CSRecoManagerCtrl::~CSRecoManagerCtrl - 析构函数

CSRecoManagerCtrl::~CSRecoManagerCtrl()
{
	// TODO: 在此清理控件的实例数据。
}



// CSRecoManagerCtrl::OnDraw - 绘图函数

void CSRecoManagerCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(GRAY_BRUSH)));
}



// CSRecoManagerCtrl::DoPropExchange - 持久性支持

void CSRecoManagerCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CSRecoManagerCtrl::OnResetState - 将控件重置为默认状态

void CSRecoManagerCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CSRecoManagerCtrl::AboutBox - 向用户显示“关于”框

void CSRecoManagerCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_SRECOMANAGER);
	dlgAbout.DoModal();
}



// CSRecoManagerCtrl 消息处理程序


int CSRecoManagerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	
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

	// TODO: 在此添加调度处理程序代码
	strResult=m_TingData;
	return strResult.AllocSysString();
}




void CSRecoManagerCtrl::OnDestroy()
{
	COleControl::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_TTS.Destroy();
}

void CSRecoManagerCtrl::Speak(LPCTSTR strSpeak)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str_Speak=strSpeak;
	m_TTS.Speak(str_Speak);
}


void CSRecoManagerCtrl::WriteOrderToXML(LPCTSTR path_Yufaku, LPCTSTR str_Order)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
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

	// TODO: 在此添加调度处理程序代码
	if (AmbientUserMode())
	{
		if (!m_TTS.Initialize())
		{
			return 0;
		}

		//获取当前的路径
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

	// TODO: 在此添加调度处理程序代码
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

	// TODO: 在此添加调度处理程序代码
	MessageBox("这只是个测试！");
}
