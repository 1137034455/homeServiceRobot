#pragma once

// SRecoManagerPropPage.h : CSRecoManagerPropPage ����ҳ���������


// CSRecoManagerPropPage : �й�ʵ�ֵ���Ϣ������� SRecoManagerPropPage.cpp��

class CSRecoManagerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSRecoManagerPropPage)
	DECLARE_OLECREATE_EX(CSRecoManagerPropPage)

// ���캯��
public:
	CSRecoManagerPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_SRECOMANAGER };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

