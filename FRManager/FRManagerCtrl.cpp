// FRManagerCtrl.cpp : CFRManagerCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "FRManager.h"
#include "FRManagerCtrl.h"
#include "FRManagerPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define TONGYI_DAXIAO 300

IMPLEMENT_DYNCREATE(CFRManagerCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CFRManagerCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SHOWIMAGE,OnHuatu)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CFRManagerCtrl, COleControl)
	DISP_FUNCTION_ID(CFRManagerCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	//DISP_FUNCTION_ID(CFRManagerCtrl, "OpenCamera", dispidOpenCamera, OpenCamera, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFRManagerCtrl, "TrainPerson", dispidTrainPerson, TrainPerson, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CFRManagerCtrl, "AddPerson", dispidAddPerson, AddPerson, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CFRManagerCtrl, "Recognize", dispidRecognize, Recognize, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CFRManagerCtrl, "OpenCamera", dispidOpenCamera, OpenCamera, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CFRManagerCtrl, "InitFRManager", dispidInitFRManager, InitFRManager, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CFRManagerCtrl, "InitFRManager2", dispidInitFRManager2, InitFRManager2, VT_EMPTY, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CFRManagerCtrl, COleControl)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CFRManagerCtrl, 1)
	PROPPAGEID(CFRManagerPropPage::guid)
END_PROPPAGEIDS(CFRManagerCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CFRManagerCtrl, "FRMANAGER.FRManagerCtrl.1",
	0x5d2bc97d, 0x4de, 0x453b, 0x95, 0xcb, 0xe5, 0xda, 0x90, 0x28, 0x87, 0x54)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CFRManagerCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID IID_DFRManager = { 0xB986E496, 0xA112, 0x4057, { 0x82, 0xAD, 0xE6, 0xB8, 0x38, 0x88, 0xBF, 0x5 } };
const IID IID_DFRManagerEvents = { 0x398C975D, 0x5DAA, 0x4AFA, { 0x9B, 0x46, 0x43, 0xAD, 0xDA, 0x5C, 0x30, 0x31 } };


// �ؼ�������Ϣ

static const DWORD _dwFRManagerOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CFRManagerCtrl, IDS_FRMANAGER, _dwFRManagerOleMisc)



// CFRManagerCtrl::CFRManagerCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CFRManagerCtrl ��ϵͳע�����

BOOL CFRManagerCtrl::CFRManagerCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_FRMANAGER,
			IDB_FRMANAGER,
			afxRegApartmentThreading,
			_dwFRManagerOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CFRManagerCtrl::CFRManagerCtrl - ���캯��

CFRManagerCtrl::CFRManagerCtrl()
	: m_recordPath(_T(""))
{
	InitializeIIDs(&IID_DFRManager, &IID_DFRManagerEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
	flag_OpenCamera=false;
	m_pPicCtlHdc=NULL;
	m_dc=NULL;
}



// CFRManagerCtrl::~CFRManagerCtrl - ��������

CFRManagerCtrl::~CFRManagerCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}



// CFRManagerCtrl::OnDraw - ��ͼ����

void CFRManagerCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
}



// CFRManagerCtrl::DoPropExchange - �־���֧��

void CFRManagerCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CFRManagerCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CFRManagerCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CFRManagerCtrl::AboutBox - ���û���ʾ�����ڡ���

void CFRManagerCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_FRMANAGER);
	dlgAbout.DoModal();
}



// CFRManagerCtrl ��Ϣ�������


// void CFRManagerCtrl::OpenCamera(void)
// {
// 	AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 
// 	// TODO: �ڴ���ӵ��ȴ���������
// 	CString video_str;
// 
// 	if(!(capture=cvCaptureFromCAM(0)))
// 	{
// 		MessageBox("�ðɣ�δ������������ͷ%>_<%������");
// 		return ;
// 	}
// 	if(!faceDetect.Init("C:\\Program Files\\OpenCV\\opencv\\data\\haarcascades\\haarcascade_frontalface_alt2.xml"))
// 	{
// 		MessageBox("haarcascade_profileface.xml ����ʧ�ܣ�");
// 		return ;	
// 	}
// 	flag_OpenCamera=TRUE;
// 	SetTimer(1,40,NULL);
// }


void CFRManagerCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (capture)
	{
		{
			if( !cvGrabFrame( capture ))
				return;

			frame = cvRetrieveFrame(capture,-1);

			if( frame )
			{
				flag_OpenCamera=true;
				SendMessage(WM_SHOWIMAGE);
			}
			else
				return;
		}
	}
	COleControl::OnTimer(nIDEvent);
}

LRESULT CFRManagerCtrl::OnHuatu(WPARAM wParam,LPARAM lParam)
{
	//CClientDC pDC(this);
	if (m_dc==NULL)
	{
		m_dc=(CClientDC*)GetDC();
	}
	if (m_pPicCtlHdc==NULL)
	{
		m_pPicCtlHdc=m_dc->GetSafeHdc();
	}
	
	CRect rect;

	GetWindowRect(rect);
	ScreenToClient(rect);

	CRect ROI=CRect(0,0,0,0);	

	if (flag_OpenCamera)
	{
		draw_frame=cvCloneImage(frame);
		faceDetect.DetectAndDrawFaces(frame,&ROI,draw_frame);

// 		GetCenterRect(rect,draw_frame,&rect);
// 		ShowImageOnClient(pDC,draw_frame,rect,ROI);
		m_cvvImage.CopyOf(draw_frame);
		m_cvvImage.DrawToHDC(m_pPicCtlHdc,m_pPicCtlRect);
		cvReleaseImage(&draw_frame);
	}
	return -1;
}

void CFRManagerCtrl::GetCenterRect(CRect rt,IplImage* image,CRect* rect)
{
	float a=MIN((float)rt.Width()/image->width,(float)rt.Height()/image->height);
	float width,height;
	width=image->width*a;
	height=image->height*a;
	rect->left=(rt.Width()-width)/2.;
	rect->top=(rt.Height()-height)/2.;
	rect->bottom=rect->top+height;
	rect->right=rect->left+width;
}


void CFRManagerCtrl::ShowImageOnClient(HDC pDC, IplImage* image, CRect rect,CRect ROI,int flags)
{
	char info[100];
	IplImage* image_show=cvCloneImage(image);
	if (ROI.Width()!=0&&ROI.Height()!=0)
	{
		CvRect rc;
		rc.x=ROI.left;
		rc.y=ROI.top;
		rc.height=ROI.Height();
		rc.width=ROI.Width();
		cvSetImageROI(image,rc);

		cvNamedWindow("����ǰ");
		cvShowImage("����ǰ",image);

		//ת��ɫ�ʿռ�
		cvCvtColor(image,image,CV_RGB2HSV);
		//����ͨ��
		IplImage* imgChannel[3] = { 0, 0, 0 };  
		
		for (int i=0;i<image->nChannels;i++)
		{
			imgChannel[i] = cvCreateImage( cvGetSize( image ), IPL_DEPTH_8U, 1 );  //Ҫ��ͨ��ͼ�����ֱ��ͼ���⻯  
		}

		cvSplit( image, imgChannel[0], imgChannel[1], imgChannel[2],0);//HSVA  

		CvFont font;
		cvInitFont( &font,CV_FONT_HERSHEY_PLAIN,1, 1, 0, 1, 8);
		for (int i=0;i<image->nChannels;i++)
		{
			CvScalar avg=cvAvg(imgChannel[i]);	
			memset(info,'\0',100);
			sprintf(info,"%f",avg.val[0]);
			
			cvPutText(image_show, info , cvPoint(0,20*(i+1)), &font, CV_RGB(255,0,0));
		}		
		

		CvScalar avg=cvAvg(imgChannel[2]);
		cvCvtScale(imgChannel[2],imgChannel[2],1.0,IlluminationThreshold-avg.val[0]);
		cvMerge( imgChannel[0], imgChannel[1], imgChannel[2], 0, image );  
		for (int i=0;i<image->nChannels;i++)
		{
			CvScalar avg=cvAvg(imgChannel[i]);	
			memset(info,'\0',100);
			sprintf(info,"%f",avg.val[0]);

			cvPutText(image_show, info , cvPoint(0,20*(i+4)), &font, CV_RGB(255,0,0));
		}	

		cvCvtColor(image,image,CV_HSV2RGB);
		cvNamedWindow("�����");
		cvShowImage("�����",image);

		for (int i=0;i<image->nChannels;i++)
		{
			cvReleaseImage(&imgChannel[i] ); 
		}
// 		avg=cvAvg(image);
// 		memset(info,'\0',100);
// 		sprintf(info,"%f",avg.val[0]);
// 		cvPutText(image_show, info , cvPoint(0,40), &font, CV_RGB(255,0,0));
	}
	
	
	char* imagedata=image_show->imageData;
	LPBITMAPINFO lpbitm;
	lpbitm=CtreateMapInfo(image_show,flags);
	StretchDIBits(pDC,
		rect.left,rect.top,rect.Width(),rect.Height(),
		0,0,image_show->width,image_show->height,
		imagedata,lpbitm,DIB_RGB_COLORS,SRCCOPY);
	cvReleaseImage(&image_show);
	//pDC.Rectangle(rect);
}
/*********************************************************************************************************/

LPBITMAPINFO CFRManagerCtrl::CtreateMapInfo(IplImage* workImg,int flag)
{   
	//  ����λͼ��Ϣ
	RGBQUAD VgaDefPal[256] = {
		{0x00,0x00,0x00,0x00},{0xa8,0x00,0x00,0x00},{0x00,0xa8,0x00,0x00},{0xa8,0xa8,0x00,0x00},
		{0x00,0x00,0xa8,0x00},{0xa8,0x00,0xa8,0x00},{0x00,0x54,0xa8,0x00},{0xa8,0xa8,0xa8,0x00},
		{0x54,0x54,0x54,0x00},{0xfc,0x54,0x54,0x00},{0x54,0xfc,0x54,0x00},{0xfc,0xfc,0x54,0x00},
		{0x54,0x54,0xfc,0x00},{0xfc,0x54,0xfc,0x00},{0x54,0xfc,0xfc,0x00},{0xfc,0xfc,0xfc,0x00},
		{0x00,0x00,0x00,0x00},{0x14,0x14,0x14,0x00},{0x20,0x20,0x20,0x00},{0x2c,0x2c,0x2c,0x00},
		{0x38,0x38,0x38,0x00},{0x44,0x44,0x44,0x00},{0x50,0x50,0x50,0x00},{0x60,0x60,0x60,0x00},
		{0x70,0x70,0x70,0x00},{0x80,0x80,0x80,0x00},{0x90,0x90,0x90,0x00},{0xa0,0xa0,0xa0,0x00},
		{0xb4,0xb4,0xb4,0x00},{0xc8,0xc8,0xc8,0x00},{0xe0,0xe0,0xe0,0x00},{0xfc,0xfc,0xfc,0x00},
		{0xfc,0x00,0x00,0x00},{0xfc,0x00,0x40,0x00},{0xfc,0x00,0x7c,0x00},{0xfc,0x00,0xbc,0x00},
		{0xfc,0x00,0xfc,0x00},{0xbc,0x00,0xfc,0x00},{0x7c,0x00,0xfc,0x00},{0x40,0x00,0xfc,0x00},
		{0x00,0x00,0xfc,0x00},{0x00,0x40,0xfc,0x00},{0x00,0x7c,0xfc,0x00},{0x00,0xbc,0xfc,0x00},
		{0x00,0xfc,0xfc,0x00},{0x00,0xfc,0xbc,0x00},{0x00,0xfc,0x7c,0x00},{0x00,0xfc,0x40,0x00},
		{0x00,0xfc,0x00,0x00},{0x40,0xfc,0x00,0x00},{0x7c,0xfc,0x00,0x00},{0xbc,0xfc,0x00,0x00},
		{0xfc,0xfc,0x00,0x00},{0xfc,0xbc,0x00,0x00},{0xfc,0x7c,0x00,0x00},{0xfc,0x40,0x00,0x00},
		{0xfc,0x7c,0x7c,0x00},{0xfc,0x7c,0x9c,0x00},{0xfc,0x7c,0xbc,0x00},{0xfc,0x7c,0xdc,0x00},
		{0xfc,0x7c,0xfc,0x00},{0xdc,0x7c,0xfc,0x00},{0xbc,0x7c,0xfc,0x00},{0x9c,0x7c,0xfc,0x00},
		{0x7c,0x7c,0xfc,0x00},{0x7c,0x9c,0xfc,0x00},{0x7c,0xbc,0xfc,0x00},{0x7c,0xdc,0xfc,0x00},
		{0x7c,0xfc,0xfc,0x00},{0x7c,0xfc,0xdc,0x00},{0x7c,0xfc,0xbc,0x00},{0x7c,0xfc,0x9c,0x00},
		{0x7c,0xfc,0x7c,0x00},{0x9c,0xfc,0x7c,0x00},{0xbc,0xfc,0x7c,0x00},{0xdc,0xfc,0x7c,0x00},
		{0xfc,0xfc,0x7c,0x00},{0xfc,0xdc,0x7c,0x00},{0xfc,0xbc,0x7c,0x00},{0xfc,0x9c,0x7c,0x00},
		{0xfc,0xb4,0xb4,0x00},{0xfc,0xb4,0xc4,0x00},{0xfc,0xb4,0xd8,0x00},{0xfc,0xb4,0xe8,0x00},
		{0xfc,0xb4,0xfc,0x00},{0xe8,0xb4,0xfc,0x00},{0xd8,0xb4,0xfc,0x00},{0xc4,0xb4,0xfc,0x00},
		{0xb4,0xb4,0xfc,0x00},{0xb4,0xc4,0xfc,0x00},{0xb4,0xd8,0xfc,0x00},{0xb4,0xe8,0xfc,0x00},
		{0xb4,0xfc,0xfc,0x00},{0xb4,0xfc,0xe8,0x00},{0xb4,0xfc,0xd8,0x00},{0xb4,0xfc,0xc4,0x00},
		{0xb4,0xfc,0xb4,0x00},{0xc4,0xfc,0xb4,0x00},{0xd8,0xfc,0xb4,0x00},{0xe8,0xfc,0xb4,0x00},
		{0xfc,0xfc,0xb4,0x00},{0xfc,0xe8,0xb4,0x00},{0xfc,0xd8,0xb4,0x00},{0xfc,0xc4,0xb4,0x00},
		{0x70,0x00,0x00,0x00},{0x70,0x00,0x1c,0x00},{0x70,0x00,0x38,0x00},{0x70,0x00,0x54,0x00},
		{0x70,0x00,0x70,0x00},{0x54,0x00,0x70,0x00},{0x38,0x00,0x70,0x00},{0x1c,0x00,0x70,0x00},
		{0x00,0x00,0x70,0x00},{0x00,0x1c,0x70,0x00},{0x00,0x38,0x70,0x00},{0x00,0x54,0x70,0x00},
		{0x00,0x70,0x70,0x00},{0x00,0x70,0x54,0x00},{0x00,0x70,0x38,0x00},{0x00,0x70,0x1c,0x00},
		{0x00,0x70,0x00,0x00},{0x1c,0x70,0x00,0x00},{0x38,0x70,0x00,0x00},{0x54,0x70,0x00,0x00},
		{0x70,0x70,0x00,0x00},{0x70,0x54,0x00,0x00},{0x70,0x38,0x00,0x00},{0x70,0x1c,0x00,0x00},
		{0x70,0x38,0x38,0x00},{0x70,0x38,0x44,0x00},{0x70,0x38,0x54,0x00},{0x70,0x38,0x60,0x00},
		{0x70,0x38,0x70,0x00},{0x60,0x38,0x70,0x00},{0x54,0x38,0x70,0x00},{0x44,0x38,0x70,0x00},
		{0x38,0x38,0x70,0x00},{0x38,0x44,0x70,0x00},{0x38,0x54,0x70,0x00},{0x38,0x60,0x70,0x00},
		{0x38,0x70,0x70,0x00},{0x38,0x70,0x60,0x00},{0x38,0x70,0x54,0x00},{0x38,0x70,0x44,0x00},
		{0x38,0x70,0x38,0x00},{0x44,0x70,0x38,0x00},{0x54,0x70,0x38,0x00},{0x60,0x70,0x38,0x00},
		{0x70,0x70,0x38,0x00},{0x70,0x60,0x38,0x00},{0x70,0x54,0x38,0x00},{0x70,0x44,0x38,0x00},
		{0x70,0x50,0x50,0x00},{0x70,0x50,0x58,0x00},{0x70,0x50,0x60,0x00},{0x70,0x50,0x68,0x00},
		{0x70,0x50,0x70,0x00},{0x68,0x50,0x70,0x00},{0x60,0x50,0x70,0x00},{0x58,0x50,0x70,0x00},
		{0x50,0x50,0x70,0x00},{0x50,0x58,0x70,0x00},{0x50,0x60,0x70,0x00},{0x50,0x68,0x70,0x00},
		{0x50,0x70,0x70,0x00},{0x50,0x70,0x68,0x00},{0x50,0x70,0x60,0x00},{0x50,0x70,0x58,0x00},
		{0x50,0x70,0x50,0x00},{0x58,0x70,0x50,0x00},{0x60,0x70,0x50,0x00},{0x68,0x70,0x50,0x00},
		{0x70,0x70,0x50,0x00},{0x70,0x68,0x50,0x00},{0x70,0x60,0x50,0x00},{0x70,0x58,0x50,0x00},
		{0x40,0x00,0x00,0x00},{0x40,0x00,0x10,0x00},{0x40,0x00,0x20,0x00},{0x40,0x00,0x30,0x00},
		{0x40,0x00,0x40,0x00},{0x30,0x00,0x40,0x00},{0x20,0x00,0x40,0x00},{0x10,0x00,0x40,0x00},
		{0x00,0x00,0x40,0x00},{0x00,0x10,0x40,0x00},{0x00,0x20,0x40,0x00},{0x00,0x30,0x40,0x00},
		{0x00,0x40,0x40,0x00},{0x00,0x40,0x30,0x00},{0x00,0x40,0x20,0x00},{0x00,0x40,0x10,0x00},
		{0x00,0x40,0x00,0x00},{0x10,0x40,0x00,0x00},{0x20,0x40,0x00,0x00},{0x30,0x40,0x00,0x00},
		{0x40,0x40,0x00,0x00},{0x40,0x30,0x00,0x00},{0x40,0x20,0x00,0x00},{0x40,0x10,0x00,0x00},
		{0x40,0x20,0x20,0x00},{0x40,0x20,0x28,0x00},{0x40,0x20,0x30,0x00},{0x40,0x20,0x38,0x00},
		{0x40,0x20,0x40,0x00},{0x38,0x20,0x40,0x00},{0x30,0x20,0x40,0x00},{0x28,0x20,0x40,0x00},
		{0x20,0x20,0x40,0x00},{0x20,0x28,0x40,0x00},{0x20,0x30,0x40,0x00},{0x20,0x38,0x40,0x00},
		{0x20,0x40,0x40,0x00},{0x20,0x40,0x38,0x00},{0x20,0x40,0x30,0x00},{0x20,0x40,0x28,0x00},
		{0x20,0x40,0x20,0x00},{0x28,0x40,0x20,0x00},{0x30,0x40,0x20,0x00},{0x38,0x40,0x20,0x00},
		{0x40,0x40,0x20,0x00},{0x40,0x38,0x20,0x00},{0x40,0x30,0x20,0x00},{0x40,0x28,0x20,0x00},
		{0x40,0x2c,0x2c,0x00},{0x40,0x2c,0x30,0x00},{0x40,0x2c,0x34,0x00},{0x40,0x2c,0x3c,0x00},
		{0x40,0x2c,0x40,0x00},{0x3c,0x2c,0x40,0x00},{0x34,0x2c,0x40,0x00},{0x30,0x2c,0x40,0x00},
		{0x2c,0x2c,0x40,0x00},{0x2c,0x30,0x40,0x00},{0x2c,0x34,0x40,0x00},{0x2c,0x3c,0x40,0x00},
		{0x2c,0x40,0x40,0x00},{0x2c,0x40,0x3c,0x00},{0x2c,0x40,0x34,0x00},{0x2c,0x40,0x30,0x00},
		{0x2c,0x40,0x2c,0x00},{0x30,0x40,0x2c,0x00},{0x34,0x40,0x2c,0x00},{0x3c,0x40,0x2c,0x00},
		{0x40,0x40,0x2c,0x00},{0x40,0x3c,0x2c,0x00},{0x40,0x34,0x2c,0x00},{0x40,0x30,0x2c,0x00},
		{0xa8,0xa8,0xa8,0x00},{0x54,0xfc,0xfc,0x00},{0xfc,0x54,0xfc,0x00},{0xfc,0xfc,0x54,0x00},
		{0xfc,0x54,0x54,0x00},{0x54,0xfc,0x54,0x00},{0x54,0x54,0xfc,0x00},{0xfc,0xfc,0xfc,0x00}  
	};
	BITMAPINFOHEADER BIH={40,1,1,1,8,0,0,0,0,0,0};
	LPBITMAPINFO lpBmi;
	int      wid,hei,bits,colors,i;

	wid =workImg->width;
	hei =workImg->height;
	bits=workImg->depth*workImg->nChannels;

	if (bits>8) 
		colors=0;
	else 
		colors=1<<bits;

	lpBmi=(LPBITMAPINFO) malloc(40+4*colors);
	BIH.biWidth   =wid;
	BIH.biHeight  =-hei;						//Ϊ��ֵʱͼ���ԭ�������½ǣ���ֵʱ�����Ͻ�
	BIH.biBitCount=(BYTE) bits;
	memcpy(lpBmi,&BIH,40);                  //  ����λͼ��Ϣͷ

	if (bits==8) {                          //  256 ɫλͼ
		if (flag==1) {                      //  ���ûҽ׵�ɫ��
			for (i=0;i<256;i++) 
			{
				VgaColorTab[i].rgbRed=VgaColorTab[i].rgbGreen=
					VgaColorTab[i].rgbBlue=(BYTE) i;
			}
			memcpy(lpBmi->bmiColors,VgaColorTab,1024);
		}
		else if (flag==2) 
		{                 //  ����Ĭ�ϵ�ɫ��
			memcpy(lpBmi->bmiColors,VgaDefPal,1024);
		}
		else if (flag==3) {                 //  �����Զ����ɫ��
			memcpy(lpBmi->bmiColors,VgaColorTab,1024);
		}
	}
	return(lpBmi);
}

/*********************************************************************************************************/

void CFRManagerCtrl::TrainPerson(LPCTSTR personName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
// 	CClientDC pDC(this);
// 	CRect rect;
// 
// 	GetWindowRect(rect);
// 	ScreenToClient(rect);
	if (m_dc==NULL)
	{
		m_dc=(CClientDC*)GetDC();
	}
	if (m_pPicCtlHdc==NULL)
	{
		m_pPicCtlHdc=m_dc->GetSafeHdc();
	}
	CRect ROI;
	if (flag_OpenCamera)
	{
		KillTimer(1);
		draw_frame=cvCloneImage(frame);
		faceDetect.DetectAndDrawFaces(frame,&ROI,draw_frame);

// 		GetCenterRect(rect,draw_frame,&rect);
// 		ShowImageOnClient(pDC,draw_frame,rect);
		m_cvvImage.CopyOf(draw_frame);
		m_cvvImage.DrawToHDC(m_pPicCtlHdc,m_pPicCtlRect);
	}
	else
	{
		MessageBox("�������ͷ��");
		return;
	}

	if (ROI.IsRectEmpty())
	{
		MessageBox("�����������");
		SetTimer(1,40,NULL);
		return;
	}
	if (faceDetect.num_face!=1)
	{
		MessageBox("ѵ����Ч����֤������ֻ��һ����ɫ�ķ���");
		SetTimer(1,40,NULL);
		return;
	}

	int index;
	CImage2 image;
	int count=facebase.GetPersonList().GetCount();
	int i=0;
	for (i=0;i<count;i++)
	{
		CPerson* p=facebase.GetPerson(i);
		if (personName==p->GetName())
		{
			break;
		}
	}
	//ʹ�ñ�ѵ����ͼ���ROI�Ĵ�С��Ϊ  TONGYI_DAXIAO*TONGYI_DAXIAO
	index=i;
	double scale;
	scale=(double)(ROI.Width())/TONGYI_DAXIAO;
	CvSize dst_size;
	dst_size.width=(double)(frame->width)/scale;
	dst_size.height=(double)(frame->height)/scale;
	IplImage* dst_image;
	dst_image=cvCreateImage(dst_size,frame->depth,frame->nChannels);
	cvResize(frame,dst_image);

	CPerson* person=facebase.GetPerson(index);
	image.CopyOf(dst_image);

	int a=person->GetImgList().GetCount();
	CString name=person->GetName();
	CString name_path;
	name_path.Format("%s%d.bmp",name,a);

	ROI.left=(double)(ROI.left)/scale;
	ROI.top=(double)(ROI.top)/scale;
	ROI.right=ROI.left+300;
	ROI.bottom=ROI.top+300;

	person->AddImage(name_path,&image,ROI);

	facebase.TrainPerson(index, true );

	SetTimer(1,40,NULL);
	HWND HSRobot_hWnd=::FindWindow(NULL,"��ͥ���������");
	::PostMessageA(HSRobot_hWnd,WM_CHAR,NULL,NULL);
}


void CFRManagerCtrl::AddPerson(LPCTSTR personName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	if (!flag_OpenCamera)
	{
		MessageBox("���ȴ�����ͷ��");
		return;
	}
	KillTimer(1);
	CString add_name;
	
	add_name=personName;
	CPerson* person=facebase.AddPerson( add_name, 0, false );
	
	SetTimer(1,40,NULL);
}

BSTR CFRManagerCtrl::Recognize(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: �ڴ���ӵ��ȴ���������

	if (m_dc==NULL)
	{
		m_dc=(CClientDC*)GetDC();
	}
	if (m_pPicCtlHdc==NULL)
	{
		m_pPicCtlHdc=m_dc->GetSafeHdc();
	}

	CRect ROI;
	if (!flag_OpenCamera)
	{
		MessageBox("�������ͷ��");
		return NULL;
	}

	//����ʱ�ر�Timer��������������
	KillTimer(1);
	draw_frame=cvCloneImage(frame);
	faceDetect.DetectAndDrawFaces(frame,&ROI,draw_frame);

	m_cvvImage.CopyOf(draw_frame);
	m_cvvImage.DrawToHDC(m_pPicCtlHdc,m_pPicCtlRect);
	if (ROI.IsRectEmpty())
	{
		MessageBox("�����������");
		SetTimer(1,40,NULL);
		return NULL;
	}
	//���������г���һ���ĺ�ɫ����ʱ��ʾ���ܼ�⵽���Ż��߶�������
	// 	if (faceDetect.num_face!=1)
	// 	{
	// 		MessageBox("ʶ����Ч����֤������ֻ��һ����ɫ�ķ���");
	// 		SetTimer(1,40,NULL);
	// 		return;
	// 	}

	int index;
	CImage2 image;
	index=facebase.GetPersonList().GetCount()-1;
	if (index<0)
	{
		MessageBox("������Ӻ��Ѳ�����һ����ѵ����");
		SetTimer(1,40,NULL);
		return NULL;
	}
	CPerson* person=facebase.GetPerson(index);
	image.CopyOf(frame);

	float  fLike = -1;
	int nIndex = -1;
	int result = facebase.RecognizePerson( image, ROI, fLike, nIndex);
	if( result == NO_PERSON ) 
	{
		MessageBox("�����˻�����ʶ�κκ��ѣ����ȶԻ����˽���ѵ����");
	}
	else if (result==SOMEONE_NOT_TRAINED)
	{
		MessageBox("�Բ����к���δ��ѵ��������û��ѵ�����ĺ��ѣ�");
	}
	else if (result==ALL_TRAINED)
	{   
		CString message = "";

		if( (nIndex >= 0) )
		{		
			CPerson* person = facebase.GetPerson( nIndex );
			strResult=person->GetName();
// 			CString str_Result;
// 			str_Result.Format("���ѣ�����%s",strResult);
// 			MessageBox(str_Result);
		}
	}
	SetTimer(1,40,NULL);
	return strResult.AllocSysString();
}


int CFRManagerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}


void CFRManagerCtrl::OnDestroy()
{
	COleControl::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if(flag_OpenCamera)
	{
		cvReleaseCapture(&capture);
	}
}


LONG CFRManagerCtrl::OpenCamera(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	CString video_str;

	if(!(capture=cvCaptureFromCAM(0)))
	{
		MessageBox("�ðɣ�δ������������ͷ%>_<%������");
		return -1;
	}
	if(!faceDetect.Init(m_classifierPath))
	{
		MessageBox("haarcascade_profileface.xml ����ʧ�ܣ�");
		return -1;	
	}
	flag_OpenCamera=TRUE;
	SetTimer(1,40,NULL);
	return 0;
}


void CFRManagerCtrl::InitFRManager(LPCTSTR classifierPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	m_classifierPath=classifierPath;
	Init();
}


void CFRManagerCtrl::Init(void)
{
	if (AmbientUserMode())
	{	
		//CClientDC dc(this);
		//m_pPicCtlHdc=dc.GetSafeHdc();
		//m_pPicCtlHdc=GetDlgItem(IDC_PICCTL)->GetDC()->GetSafeHdc();
		GetClientRect(&m_pPicCtlRect);
		facebase.SetFileName(m_recordPath);
		if (!facebase.Load())
		{
			MessageBox("������Ϣ����ʧ�ܣ�");
		}
	}
}


void CFRManagerCtrl::InitFRManager2(LPCTSTR classifierPath, LPCTSTR RecordPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	m_classifierPath=classifierPath;
	m_recordPath=RecordPath;
	Init();
}
