// SRecoManagerPropPage.cpp : CSRecoManagerPropPage 属性页类的实现。

#include "stdafx.h"
#include "SRecoManager.h"
#include "SRecoManagerPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CSRecoManagerPropPage, COlePropertyPage)

// 消息映射

BEGIN_MESSAGE_MAP(CSRecoManagerPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CSRecoManagerPropPage, "SRECOMANAGER.SRecoManagerPropPage.1",
	0xa72b880b, 0x3359, 0x4c3b, 0x94, 0x9, 0x99, 0x54, 0xcd, 0xb8, 0x53, 0x8a)



// CSRecoManagerPropPage::CSRecoManagerPropPageFactory::UpdateRegistry -
// 添加或移除 CSRecoManagerPropPage 的系统注册表项

BOOL CSRecoManagerPropPage::CSRecoManagerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SRECOMANAGER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CSRecoManagerPropPage::CSRecoManagerPropPage - 构造函数

CSRecoManagerPropPage::CSRecoManagerPropPage() :
	COlePropertyPage(IDD, IDS_SRECOMANAGER_PPG_CAPTION)
{
}



// CSRecoManagerPropPage::DoDataExchange - 在页和属性间移动数据

void CSRecoManagerPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CSRecoManagerPropPage 消息处理程序
