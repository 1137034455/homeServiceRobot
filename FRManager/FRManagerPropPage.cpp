// FRManagerPropPage.cpp : CFRManagerPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "FRManager.h"
#include "FRManagerPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CFRManagerPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CFRManagerPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CFRManagerPropPage, "FRMANAGER.FRManagerPropPage.1",
	0xc359bbe, 0x8646, 0x4685, 0xb7, 0xd5, 0xc9, 0xa2, 0xac, 0xe8, 0x21, 0x9)



// CFRManagerPropPage::CFRManagerPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CFRManagerPropPage ��ϵͳע�����

BOOL CFRManagerPropPage::CFRManagerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_FRMANAGER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CFRManagerPropPage::CFRManagerPropPage - ���캯��

CFRManagerPropPage::CFRManagerPropPage() :
	COlePropertyPage(IDD, IDS_FRMANAGER_PPG_CAPTION)
{
}



// CFRManagerPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CFRManagerPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CFRManagerPropPage ��Ϣ�������
