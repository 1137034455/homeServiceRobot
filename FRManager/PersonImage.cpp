/****************************************************************************************\
*                              CPersonImage class                                        *
\****************************************************************************************/
#include "stdafx.h"

#include "PersonImage.h"
#include "CvvImage.cpp"

CPersonImage::CPersonImage()
{
	m_modified = false;
	m_roi_in_file = CRect(0,0,0,0);
}


CPersonImage::~CPersonImage()
{
	Unload();    
}


bool  CPersonImage::Load()
{
	bool res = m_img.LoadRect( m_filename, CV_LOAD_IMAGE_UNCHANGED, m_roi_in_file ); // load as a grayscale image
	SetModified( false );
	return res;
}


void  CPersonImage::Unload()
{
	Save(); 
	m_img.Destroy();
}


bool  CPersonImage::Save()
{
	bool res = true;
	if( IsModified() )
	{
		res = m_saveImg.Save( m_filename );
		SetModified( false );
	}
	return res;
}


void  CPersonImage::SetFileName( const CString& filename )
{
	m_filename = filename;
	SetModified( m_img.Width() != 0 );
}


void  CPersonImage::SetRoiInFile( CRect r )
{
	m_roi_in_file = r;
	SetModified( m_img.Width() != 0 );
}


void  CPersonImage::CalcRect( SIZE win_size, POINT pos, SIZE base_size,
	CRect& src_rect, CRect& dst_rect )
{
	IplImage* src = m_img.GetImage();
	int  w, h;
	int  mul_k, div_k;
	CRect sr, dr;

	src_rect = dst_rect = CRect(0,0,0,0);

	w = src->width;
	h = src->height;

	sr.left = sr.top = 0;
	sr.right = w;
	sr.bottom = h;

	// calc scaling coefficients
	if( w*base_size.cy > h*base_size.cx )
	{
		mul_k = base_size.cx;
		div_k = w;
	}
	else
	{
		mul_k = base_size.cy;
		div_k = h;
	}

	// calc resultant width & height
	dr.right = w * mul_k/div_k;
	dr.bottom = h * mul_k/div_k;

	// calculate top-left coordinates
	dr.left = pos.x + (base_size.cx - dr.right)/2;
	dr.top = pos.y + (base_size.cy - dr.bottom)/2;

	// calculate bottom-right coordinates
	dr.right += dr.left;
	dr.bottom += dr.top;

	// rough clipping
	if( !(dr.right < 0 || dr.bottom < 0 || dr.left > win_size.cx || dr.top > win_size.cy))
	{
		// fine clipping
		if( dr.left < 0 )
		{
			sr.left = -dr.left * div_k/mul_k;
			dr.left = 0;
		}

		if( dr.top < 0 )
		{
			sr.top = -dr.top * div_k/mul_k;
			dr.top = 0;
		}

		if( dr.right > win_size.cx )
		{
			sr.right -= (dr.right - win_size.cx) * div_k/mul_k;
			dr.right = win_size.cx;
		}

		if( dr.bottom > win_size.cy )
		{
			sr.bottom -= (dr.bottom - win_size.cy) * div_k/mul_k;
			dr.bottom = win_size.cy;
		}

		if( !(sr.Width() <= 0 || sr.Height() <= 0 ||
			dr.Width() <= 0 || dr.Height() <= 0))
		{
			src_rect = sr;
			dst_rect = dr;
		}
	}
}


void  CPersonImage::Draw( CImage2& img, SIZE win_size, POINT pos, SIZE base_size )
{
	IplImage* dst = img.GetImage();
	IplImage* src = m_img.GetImage();
	IplROI *src_roi_tmp, *dst_roi_tmp;
	IplROI  src_roi, dst_roi;
	ASSERT( dst != 0 );
	CRect sr, dr;

	if( !src ) return;

	src_roi_tmp = src->roi;
	dst_roi_tmp = dst->roi;

	src->roi = &src_roi;
	dst->roi = &dst_roi;

	CalcRect( win_size, pos, base_size, sr, dr );

	if( !sr.IsRectEmpty() && !dr.IsRectEmpty())
	{
		// set ROIs
		src_roi = RectToROI( sr );
		dst_roi = RectToROI( dr );

		cvResize( src, dst, CV_INTER_LINEAR );
	}

	src->roi = src_roi_tmp;
	dst->roi = dst_roi_tmp;
}
