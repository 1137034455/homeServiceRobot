#pragma once

// FRManagerPropPage.h : CFRManagerPropPage ����ҳ���������


// CFRManagerPropPage : �й�ʵ�ֵ���Ϣ������� FRManagerPropPage.cpp��

class CFRManagerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CFRManagerPropPage)
	DECLARE_OLECREATE_EX(CFRManagerPropPage)

// ���캯��
public:
	CFRManagerPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_FRMANAGER };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

