// FRManagerPropPage.cpp : CFRManagerPropPage 属性页类的实现。

#include "stdafx.h"
#include "FRManager.h"
#include "FRManagerPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CFRManagerPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CFRManagerPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CFRManagerPropPage, "FRMANAGER.FRManagerPropPage.1",
	0xc359bbe, 0x8646, 0x4685, 0xb7, 0xd5, 0xc9, 0xa2, 0xac, 0xe8, 0x21, 0x9)



// CFRManagerPropPage::CFRManagerPropPageFactory::UpdateRegistry -
// 添加或移除 CFRManagerPropPage 的系统注册表项

BOOL CFRManagerPropPage::CFRManagerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_FRMANAGER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CFRManagerPropPage::CFRManagerPropPage - 构造函数

CFRManagerPropPage::CFRManagerPropPage() :
	COlePropertyPage(IDD, IDS_FRMANAGER_PPG_CAPTION)
{
}



// CFRManagerPropPage::DoDataExchange - 在页和属性间移动数据

void CFRManagerPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CFRManagerPropPage 消息处理程序
