// SRecoManagerPropPage.cpp : CSRecoManagerPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "SRecoManager.h"
#include "SRecoManagerPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CSRecoManagerPropPage, COlePropertyPage)

// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CSRecoManagerPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CSRecoManagerPropPage, "SRECOMANAGER.SRecoManagerPropPage.1",
	0xa72b880b, 0x3359, 0x4c3b, 0x94, 0x9, 0x99, 0x54, 0xcd, 0xb8, 0x53, 0x8a)



// CSRecoManagerPropPage::CSRecoManagerPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CSRecoManagerPropPage ��ϵͳע�����

BOOL CSRecoManagerPropPage::CSRecoManagerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SRECOMANAGER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CSRecoManagerPropPage::CSRecoManagerPropPage - ���캯��

CSRecoManagerPropPage::CSRecoManagerPropPage() :
	COlePropertyPage(IDD, IDS_SRECOMANAGER_PPG_CAPTION)
{
}



// CSRecoManagerPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CSRecoManagerPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CSRecoManagerPropPage ��Ϣ�������
