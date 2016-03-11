// FaceDetect.cpp: implementation of the CFaceDetect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaceDetect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CFaceDetect::CFaceDetect()
{
	m_pStorage = cvCreateMemStorage();
	m_pCascade = 0;
}

CFaceDetect::~CFaceDetect()
{
	if(m_pCascade)
	{
		End();
	}
}

BOOL CFaceDetect::Init(const char* szClassifierCascadePath) 
{
//     CvHaarClassifierCascade* m_pCascade = cvLoadHaarClassifierCascade(
//         szClassifierCascadePath,
//         cvSize( ORIG_WIN_SIZE, ORIG_WIN_SIZE ));
	m_pCascade = (CvHaarClassifierCascade*)cvLoad( szClassifierCascadePath, 0, 0, 0 );
    if( !m_pCascade )
        return 0;
    //m_pCascade = cvCreateHidHaarClassifierCascade( cascade );
    //cvReleaseHaarClassifierCascade( &m_pCascade );

	return (m_pCascade!=NULL);
}

CArrayRect* CFaceDetect::DetectAndDrawFaces( IplImage* img,CRect* rc_ROI,IplImage* draw_img )
{
	CRect  rc(0,0,0,0);
	CRect max;
	CArrayRect* arr_RectFaces=new CArrayRect;
    if( m_pCascade && img )
    {
        int scale = 2;
        CvSize img_size = cvGetSize( img );
        IplImage* temp = cvCreateImage( cvSize(img_size.width/2,img_size.height/2), 8, 3 );
        int i;
		
		//cvCopy(img,draw_img);
        cvPyrDown( img, temp );
        cvClearMemStorage( m_pStorage );

		int thickness=2;
		int max_rect=0;
		
        if( m_pCascade )
        {
            CvSeq* faces = cvHaarDetectObjects( temp, m_pCascade, m_pStorage, 1.2, 2, 1 );
            for( i = 0; i < (faces ? faces->total : 0); i++ )
            {
                CvRect r = *(CvRect*)cvGetSeqElem( faces, i );
				cvRectangle( draw_img,
                             cvPoint(r.x*scale,/*img->height - */r.y*scale),
                             cvPoint((r.x+r.width)*scale,/*img->height - */(r.y+r.height)*scale),
                             CV_RGB(255,0,0), thickness );
				CRect rect;
				rect.left=rc.left = r.x*scale;
				rect.top=rc.top = r.y*scale;
				rect.right=rc.right = (r.x+r.width)*scale;
				rect.bottom=rc.bottom = (r.y+r.height)*scale;

				arr_RectFaces->Add(rect);
				if (rc.Width()*rc.Height()>max_rect)
				{
					max=rc;
					max_rect=rc.Width()*rc.Height();
				}
            }
			num_face=faces->total;
        }
		//rc_ROI=&max;
		rc_ROI->left=max.left;
		rc_ROI->top=max.top;
		rc_ROI->right=max.right;
		rc_ROI->bottom=max.bottom;
        cvReleaseImage( &temp );
    }

	return arr_RectFaces;
}

void CFaceDetect::End()
{
	if(m_pCascade)
	{
		cvReleaseHaarClassifierCascade(&m_pCascade);
		m_pCascade = NULL;
	}
}