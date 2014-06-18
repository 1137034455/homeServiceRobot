#pragma once

// SRecoManagerPropPage.h : CSRecoManagerPropPage 属性页类的声明。


// CSRecoManagerPropPage : 有关实现的信息，请参阅 SRecoManagerPropPage.cpp。

class CSRecoManagerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSRecoManagerPropPage)
	DECLARE_OLECREATE_EX(CSRecoManagerPropPage)

// 构造函数
public:
	CSRecoManagerPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_SRECOMANAGER };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

