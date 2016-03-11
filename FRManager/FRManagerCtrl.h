#pragma once

// FRManagerCtrl.h : CFRManagerCtrl ActiveX �ؼ����������


// CFRManagerCtrl : �й�ʵ�ֵ���Ϣ������� FRManagerCtrl.cpp��

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

// ���캯��
public:
	CFRManagerCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CFRManagerCtrl();

	DECLARE_OLECREATE_EX(CFRManagerCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CFRManagerCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CFRManagerCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CFRManagerCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
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
	CvCapture * capture;				//ץȡ��Ƶ
	IplImage* frame;					//ÿһ֡ͼ��ı���
	IplImage* draw_frame;
	IplImage* hist_frame;
	CFaceBase facebase;					//��������
	CvvImage m_cvvImage;
	HDC m_pPicCtlHdc;
	CRect m_pPicCtlRect;
	CClientDC* m_dc;
	bool flag_OpenCamera;				//������ͷ�ı�ʾ

	RGBQUAD  VgaColorTab[256];

	CFaceDetect faceDetect;				//�����������

	CvMemStorage* m_pStorage;
	CvHaarClassifierCascade* m_pCascade;//����������

	CString m_trainperson;
	CString m_classifierPath;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	LRESULT OnHuatu(WPARAM,LPARAM);		//�ڿؼ����м���ʾ��Ƶ��Ϣ
private:
	//һЩ�ڲ�ʹ�õĺ���
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
	void InitFRManager(LPCTSTR classifierPath);//��ȡ����ʶ��������ĵ�ַ����ʼ��FRManager
private:
	void Init(void);
protected:
	void InitFRManager2(LPCTSTR classifierPath, LPCTSTR RecordPath);
private:
	CString m_recordPath;
};

