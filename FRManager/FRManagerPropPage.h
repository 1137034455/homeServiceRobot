#pragma once

// FRManagerPropPage.h : CFRManagerPropPage 属性页类的声明。


// CFRManagerPropPage : 有关实现的信息，请参阅 FRManagerPropPage.cpp。

class CFRManagerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CFRManagerPropPage)
	DECLARE_OLECREATE_EX(CFRManagerPropPage)

// 构造函数
public:
	CFRManagerPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_FRMANAGER };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

