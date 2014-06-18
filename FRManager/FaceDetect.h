// FaceDetect.h: interface for the CFaceDetect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACEDETECT_H__E92122F4_116A_4D5A_AFEA_13EEF65F9C4D__INCLUDED_)
#define AFX_FACEDETECT_H__E92122F4_116A_4D5A_AFEA_13EEF65F9C4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cvaux.h"
#include "cv.h"
#include "highgui.h"
#include <limits.h>
#include <ctype.h>
// struct Node 
// 
// {
// 
// 	int index;
// 
// 	char name[16];
// 
// };
// typedef CArray<Node*,Node*> CArrayRect;
typedef CArray<CRect,CRect&> CArrayRect;

class CFaceDetect  
{
	enum { ORIG_WIN_SIZE = 24 };

	CvMemStorage* m_pStorage;
	CvHaarClassifierCascade* m_pCascade;

public:
	CFaceDetect();
	virtual ~CFaceDetect();
	
	BOOL Init(const char* szClassifierCascadePath = NULL);
	CArrayRect* DetectAndDrawFaces( IplImage* img,CRect* rc_ROI,IplImage* draw_img=NULL );
	void End();
	int num_face;
	//CArrayRect arr_Rect;
};

#endif // !defined(AFX_FACEDETECT_H__E92122F4_116A_4D5A_AFEA_13EEF65F9C4D__INCLUDED_)
