#pragma once

#define IlluminationThreshold 80
#include "cv.h"
#include "highgui.h"
#include "ContEHMM.h"
#include "CvvImage.h"
#include "Person.h"
#include "BaseSet.h"
#include "LightSet.h"
#define CImage2 CvvImage

class CPersonImage;

class CPersonImage
{
public:

	CPersonImage();
	virtual ~CPersonImage();
	bool     Save();
	bool     Load();
	void     Unload();
	void     SetFileName( const CString& filename );
	void     SetRoiInFile( CRect r );
	CRect    GetRoiInFile() { return m_roi_in_file; }
	const CString& GetFileName() { return m_filename; }
	void     SetModified( bool modified = true ) { m_modified = modified; }
	bool     IsModified() { return m_modified; }
	void     CalcRect( SIZE win_size, POINT pos, SIZE base_size,
		CRect& src_rect, CRect& dst_rect );
	void     Draw( CImage2& img, SIZE win_size, POINT pos, SIZE pic_size );
	CImage2&  GetImage() { return m_img; }

protected:
	CString  m_filename;
	CImage2   m_img;
	CRect    m_roi_in_file;
	bool     m_modified;
public:
	CImage2 m_saveImg;
};

void ConvertNameToFolder( const char* name, char* folder );


