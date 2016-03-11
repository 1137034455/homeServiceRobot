#pragma once
#ifndef CVVIMAGE_CLASS_DEF
#define CVVIMAGE_CLASS_DEF
#include "opencv.hpp"
class  CvvImage
{
public:
   CvvImage();
   virtual ~CvvImage();
   
   virtual bool  Create( int width, int height, int bits_per_pixel, int image_origin = 0 );
   
   virtual bool  Load( const char* filename, int desired_color = 1 );
   
   virtual bool  LoadRect( const char* filename,
      int desired_color, CvRect r );
#if defined WIN32 || defined _WIN32
   virtual bool  LoadRect( const char* filename,
      int desired_color, RECT r )
   {
      return LoadRect( filename, desired_color,
         cvRect( r.left, r.top, r.right - r.left, r.bottom - r.top ));
   }
#endif
   
   virtual bool  Save( const char* filename );
   
   virtual void  CopyOf( CvvImage& image, int desired_color = -1 );
   virtual void  CopyOf( IplImage* img, int desired_color = -1 );
   IplImage* GetImage() { return m_img; };
   virtual void  Destroy(void);
   
   int Width() { return !m_img ? 0 : !m_img->roi ? m_img->width : m_img->roi->width; };
   int Height() { return !m_img ? 0 : !m_img->roi ? m_img->height : m_img->roi->height;};
   int Bpp() { return m_img ? (m_img->depth & 255)*m_img->nChannels : 0; };
   virtual void  Fill( int color );
   
   virtual void  Show( const char* window );

#if defined WIN32 || defined _WIN32
   
   virtual void  Show( HDC dc, int x, int y, int width, int height,
      int from_x = 0, int from_y = 0 );
   
   virtual void  DrawToHDC( HDC hDCDst, RECT* pDstRect );
#endif
protected:
   IplImage*  m_img;

};

CV_INLINE RECT NormalizeRect( RECT r )
{
	int t;
	if( r.left > r.right )
	{
		t = r.left;
		r.left = r.right;
		r.right = t;
	}
	if( r.top > r.bottom )
	{
		t = r.top;
		r.top = r.bottom;
		r.bottom = t;
	}

	return r;
}

CV_INLINE CvRect RectToCvRect( RECT sr )
{
	sr = NormalizeRect( sr );
	return cvRect( sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top );
}

CV_INLINE RECT CvRectToRect( CvRect sr )
{
	RECT dr;
	dr.left = sr.x;
	dr.top = sr.y;
	dr.right = sr.x + sr.width;
	dr.bottom = sr.y + sr.height;

	return dr;
}

CV_INLINE IplROI RectToROI( RECT r )
{
	IplROI roi;
	r = NormalizeRect( r );
	roi.xOffset = r.left;
	roi.yOffset = r.top;
	roi.width = r.right - r.left;
	roi.height = r.bottom - r.top;
	roi.coi = 0;

	return roi;
}
CV_INLINE void FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin )
{
	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset( bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if( bpp == 8 )
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for( i = 0; i < 256; i++ )
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

typedef CvvImage CImage2;
#endif