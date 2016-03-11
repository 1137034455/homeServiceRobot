#pragma once

// FRManagerCtrl.h : CFRManagerCtrl ActiveX 控件类的声明。


// CFRManagerCtrl : 有关实现的信息，请参阅 FRManagerCtrl.cpp。

#define WM_SHOWIMAGE WM_USER+1005
#include "FaceBase.h"
#include "vector"
#include "FaceDetect.h "
#define SpeechSDK
// #include "CSelfUse.h"


// #include <mmsystem.h>
// #pragma comment(lib, "WINMM.LIB")
// SND_ASYNC


using namespace std;

class CFRManagerCtrl : public COleControl
{
	DECLARE_DYNCREATE(CFRManagerCtrl)

// 构造函数
public:
	CFRManagerCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CFRManagerCtrl();

	DECLARE_OLECREATE_EX(CFRManagerCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CFRManagerCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CFRManagerCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CFRManagerCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		dispidInitFRManager2 = 7L,
		dispidInitFRManager = 6L,
		dispidOpenCamera = 5L,
		dispidRecognize = 4L,
		dispidAddPerson = 3L,
		dispidTrainPerson = 2L,
		//dispidOpenCamera = 1L
	};
protected:
	//void OpenCamera(void);
protected:
	CvCapture * capture;				//抓取视频
	IplImage* frame;					//每一帧图像的保存
	IplImage* draw_frame;
	IplImage* hist_frame;
	CFaceBase facebase;					//人脸管理
	CvvImage m_cvvImage;
	HDC m_pPicCtlHdc;
	CRect m_pPicCtlRect;
	CClientDC* m_dc;
	bool flag_OpenCamera;				//打开摄像头的标示

	RGBQUAD  VgaColorTab[256];

	CFaceDetect faceDetect;				//检测人脸区域

	CvMemStorage* m_pStorage;
	CvHaarClassifierCascade* m_pCascade;//海尔分类器

	CString m_trainperson;
	CString m_classifierPath;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	LRESULT OnHuatu(WPARAM,LPARAM);		//在控件的中间显示视频信息
private:
	//一些内部使用的函数
	void GetCenterRect(CRect rt,IplImage* image,CRect* rect);
	void ShowImageOnClient(HDC pDC, IplImage* image, CRect rect,CRect ROI=CRect(0,0,0,0),int flags=1);
	LPBITMAPINFO CtreateMapInfo(IplImage* workImg,int flag);
/*	void Hist(IplImage* src,IplImage* dst);*/
protected:
	void TrainPerson(LPCTSTR personName);
	void AddPerson(LPCTSTR personName);
	BSTR Recognize(void);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
protected:
	LONG OpenCamera(void);
	void InitFRManager(LPCTSTR classifierPath);//获取人脸识别分类器的地址并初始化FRManager
private:
	void Init(void);
protected:
	void InitFRManager2(LPCTSTR classifierPath, LPCTSTR RecordPath);
private:
	CString m_recordPath;
};

