/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/// FaceBase.cpp: implementation of the CFaceBase class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "UseSelf.h"
#include "FaceBase.h"
#include "direct.h"
#include <math.h>
#include <float.h>
#include <process.h>
#include "CvvImage.h"
#include "CvvImage.cpp"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// globals
static const char* base_signature = "FACE DATABASE\n";
static const char* person_signature = "PERSONAL INFO\n";

void ConvertNameToFolder( const char* name, char* folder )
{
	int i, j = 0;
	int len = strlen(name);

	for( i = 0; i < len; i++ )
	{
		if( name[i] == ' ' )
		{
			if( j == 0 || folder[j-1] != '.' )
			{
				folder[j++] = '.';
			}
		}
		else
		{
			folder[j++] = name[i];
		}
	}

	folder[j] = '\0';
}


/***************************************************************************************\
Face data base structure:

<root_folder>\
<person_folder_1>\
<image_11>
<image_12>
<image_13>
...
info.txt
<person_folder_2>\
<image_21>
<image_22>
<image_23>
...
info.txt
<person_folder_3>\
<image_21>
<image_22>
<image_23>
...
info.txt
...
<index_file>

( names in <> brackets can be arbitrary ).
e.g.

NNBase\
Abrosimov.Dmirty\
ad_1.bmp
ad_near_window.bmp
ad_smiling.bmp
index.txt
Oblomov.Sergey\
serg.bmp
photo_3.bmp
index.txt
NNBaseIndex.txt

Main base index file contains name of the base followed by list of personal folders.
Format is the following:

line            content
--------------------------------
1           FACE DATABASE           (signature)
2           base name/description
3           <empty line>
4           <person_folder1>
5           <person_folder2>
...                ...

In every personal folder there is a file with predefined name: info.txt
It contains person name followed by list of images for that person.

line            content
--------------------------------
1           PERSONAL INFO           (signature)
2           person name/description
3           <empty line>
4           <image_name1>
5           [<roi1>]
6           <image_name2>
7           [<roi2>]
...          ...

Every image name may be followed by four numbers which are represent coordinates of
top-left corner of the face rectangle in the image and width and height
of the rectnagle, if no roi is specified, the entire image is considered as a face.

lines, started with #, are comments, and skipped when file is readed.

\***************************************************************************************/

static char* chomp( char* str )
{
	int l = strlen( str );
	if( l > 0 && str[l-1] == '\n' )
	{
		str[l-1] = '\0';
	}
	return str;
}

const int STR_BUF_SIZE = 1000;

void ExtractDCT( float* src, float* dst, int num_vec, int dst_len )
{
	float* src_ = src+1;
	float* dst_ = dst;

	for( int i = 0; i < num_vec; i++ )
	{
		memcpy( dst_, src_, dst_len * sizeof(float) );
		src_+= dst_len+1;
		dst_+= dst_len;
	}
} 

/****************************************************************************************\
*                                  CFaceBase  class                                      *
\****************************************************************************************/

// CFaceBase
CFaceBase::CFaceBase()
{
	//default parameters
	m_stnum[0] = 5;
	m_stnum[1] = 3;
	m_stnum[2] = 6;
	m_stnum[3] = 6;
	m_stnum[4] = 6;
	m_stnum[5] = 3;
	for( int i = 0; i < 128; i++ )
	{
		m_mixnum[i] = 3;
	}

	m_modified = false;
	m_trained_index = -1;
	m_base_view = 0;
	SetImageSize( CSize(100,120) );

	m_delta = cvSize(4,4);
	m_obsSize = cvSize(3,3);
	m_dctSize = cvSize(12,12);

	m_useWidth = FALSE;
	m_useHeight = FALSE;
	m_scaleWidth = 0;
	m_scaleHeight = 0;
	m_suppress_intensity = FALSE;

}

CFaceBase::~CFaceBase()
{
	Unload();
}


void  CFaceBase::SetFileName( const CString& filename )
{
	m_filename = filename;
	SetModified();
}


void  CFaceBase::SetName( const CString& name )
{
	m_basename = name;
	SetModified();
}


void  CFaceBase::GetRootFolder( char* root_folder, int* root_path_len )
{
	char buffer[STR_BUF_SIZE];

	// construct root folder path
	_splitpath( m_filename, root_folder, buffer, 0, 0 );
	strcat( root_folder, buffer );
	if( root_path_len ) *root_path_len = strlen( root_folder );
}

//获取个人的文件夹
bool  CFaceBase::GetPersonFolder( const char* root_folder, int root_folder_len,
	const char* person_folder, char* folder )
{
	char buffer[STR_BUF_SIZE];
	char drive[STR_BUF_SIZE];
	int  len;

	strcpy( buffer, person_folder );

	len = strlen( buffer );
	if( len == 0 ) return false;

	// add slash (if absent)
	if( buffer[len-1] != '\\')
	{
		buffer[len] = '\\';
		buffer[len+1] = '\0';
	}

	_splitpath( buffer, drive, 0, 0, 0 );
	if( strlen( drive ) > 0 ) return false;

	if( root_folder != folder )
	{
		strcpy( folder, root_folder );
	}

	strcpy( folder + root_folder_len, buffer );
	return true;
}

//获取个人子文件夹
void  CFaceBase::GetPersonSubFolder( const char* folder, char* subfolder )
{
	char buffer[STR_BUF_SIZE];
	char name[STR_BUF_SIZE];
	char ext[STR_BUF_SIZE];

	strcpy( buffer, folder );

	ASSERT( buffer[strlen(buffer)-1] == '\\' );

	// delete slash
	buffer[strlen(buffer)-1] = '\0';
	/*说明
	　　分解路径，把你的完整路径给分割开来，就是一个对字符串进行分割的函数
	  参数表
	  　path，　Full path（完整路径）
	   drive ， Optional drive letter, followed by a colon (:)（磁盘驱动包含：）
	   dir， Optional directory path, including trailing slash. Forward slashes (/ ), backslashes (\ ), or both may be used.（文件路径，无论是以“/”，“\”）
	   fname， Base filename (no extension)（文件名）
	   ext ， Optional filename extension, including leading period (.)（后缀名）
	   */
	_splitpath( buffer, 0, 0, name, ext );

	strcpy( subfolder, name );
	strcat( subfolder, ext );
}


bool  CFaceBase::Load()
{
	FILE* f = 0;
	char buffer[STR_BUF_SIZE];
	char root_folder[STR_BUF_SIZE];
	int  root_path_len;
	bool error = false;

	if( m_filename.GetLength() == 0 ) return false;

	f = fopen( m_filename, "rt" );
	if( !f ) return false;

	m_base.RemoveAll();

	if( !fgets( buffer, STR_BUF_SIZE, f ) || strcmp( buffer, base_signature ))
		return false;

	// read header
	if( !fgets( buffer, STR_BUF_SIZE, f )) return false;

	m_basename = chomp( buffer );

	// construct root folder path
	GetRootFolder( root_folder, &root_path_len );

	// skip one line after the base name
	fgets( buffer, STR_BUF_SIZE, f );

	// load all the people data
	for(;;)
	{
		CPerson* person;
		if( !fgets( buffer,STR_BUF_SIZE, f )) break;

		if( strlen(buffer) == 0 || buffer[0] == '#' ) continue;
		chomp( buffer );

		if( !GetPersonFolder( root_folder, root_path_len, buffer, root_folder ))
			continue;

		person = new CPerson( this );

		person->SetFolder( root_folder );
		if( !person->Load() )
		{
			delete person;
			error = true;
			continue;
		}
		m_base.AddTail( person );
	}    

	fclose(f);
	SetModified( error );
	return true;
}


void  CFaceBase::Unload()
{
	Save();
	while( !m_base.IsEmpty() )
	{
		CPerson* person = m_base.RemoveHead();
		delete person;
	}
}

bool  CFaceBase::Save()
{
	if( m_filename.GetLength() > 0 )
	{
		POSITION pos = m_base.GetHeadPosition();
		while( pos )
		{
			CPerson* person = m_base.GetNext( pos );
			person->Save();
		}

		if( IsModified() )
		{
			FILE* f = fopen( m_filename, "wt" );
			char  subfolder[STR_BUF_SIZE];
			if( !f ) return false;

			fputs( base_signature, f );
			fputs( m_basename, f );
			fputs( "\n\n", f );

			pos = m_base.GetHeadPosition();

			while( pos )
			{
				CPerson* person = m_base.GetNext( pos );
				const CString& str = person->GetFolder();
				GetPersonSubFolder( str, subfolder );
				fprintf( f, "%s\n", subfolder );
			}
			fclose(f);
		}
		SetModified(false);
	}
	return true;
}


CPerson*  CFaceBase::AddPerson( const char* name, const char* folder, bool import_data )
{
	char temp_folder[STR_BUF_SIZE];
	char root_folder[STR_BUF_SIZE];
	CPerson* person = 0;
	int len;

	if( !import_data )
	{
		ASSERT( strlen( name ) > 0 );
		if( !folder )
		{
			ConvertNameToFolder( name, temp_folder );
			folder = temp_folder;
		}
	}
	else
	{
		ASSERT( strlen( folder ) > 0 && name == 0 );
	}

	GetRootFolder( root_folder, &len );
	GetPersonFolder( root_folder, len, folder, root_folder );

	person = new CPerson( this );
	person->SetFolder( root_folder );
	if( import_data )
	{
		if( !person->Load())
		{
			delete person;
			person = 0;
		}
	}
	else
	{
		_mkdir( root_folder );// 建立一个目录
		person->SetName( name );
		person->SetModified();
	}

	if( person )
	{
		m_base.AddTail( person );
		SetModified();
	}
	Save();
	return person;
}


void  CFaceBase::RemovePerson( POSITION pos )
{
	CPerson* person = m_base.GetAt( pos );
	if( person )
	{
		person->Unload();
		m_base.RemoveAt( pos );
		delete person;
		SetModified();
	}
}


void  CFaceBase::SetImageSize( CSize size )
{
	m_baseImgSize = size;
	SetModified();
}


CPerson*  CFaceBase::GetPerson( int index )
{
	if( index < 0 ) return NULL;

	POSITION pos = m_base.FindIndex( index );
	return pos ? m_base.GetAt( pos ) : 0;
}


CPerson*  CFaceBase::FindPersonByName( const CString& name )
{
	POSITION pos = m_base.GetHeadPosition();

	while( pos )
	{
		CPerson* person = m_base.GetNext(pos);
		if( person && name.CompareNoCase( person->GetName()) == 0 ) return person;
	}
	return 0;
}


int  CFaceBase::GetPersonIndex( CPerson* person )
{
	POSITION pos = m_base.GetHeadPosition();
	int i;

	for( i = 0; pos != 0; i++ )
	{
		CPerson* p = m_base.GetNext(pos);
		if( p == person ) return i;
	}

	return -1;
}


void  CFaceBase::Draw( int index, CImage2& img, SIZE win_size, int y_pos,
	SIZE base_size, SIZE delta )
{
	ASSERT( delta.cx > base_size.cx && delta.cy > base_size.cy );

	int  nx = MAX((win_size.cx + delta.cx - base_size.cx)/delta.cx,1);
	int  row = y_pos/delta.cy;
	int  idx0 = row*nx;
	int  x, y;
	POSITION pos;
	CPersonImgList* list = 0;
	int  count;

	if( index >= 0 )
	{
		CPerson* person = GetPerson( index );
		if( !person ) return;

		list = &person->GetImgList();
		count = list->GetCount();
	}
	else
	{
		count = m_base.GetCount();
	}

	if( !img.GetImage() || idx0 >= count ) return;

	if( list )
	{
		pos = list->FindIndex( idx0 );
	}
	else
	{
		pos = m_base.FindIndex( idx0 );
	}

	for( y = row*delta.cy - y_pos + (delta.cy - base_size.cy)/2;
		y < win_size.cy ; y += delta.cy )
	{
		for( x = (delta.cx - base_size.cx)/2; ; )
		{
			CPersonImage* src_img = 0;
			if( list )
			{
				src_img = list->GetNext( pos );
			}
			else
			{
				CPerson* person = m_base.GetNext( pos );
				if( person )
				{
					CPersonImgList& l = person->GetImgList();
					if( !l.IsEmpty())
					{
						src_img = l.GetHead();
					}
				}
			}

			if( src_img )
			{
				src_img->Draw( img, win_size, CPoint( x, y ), base_size );
			}
			if( !pos ) return;

			x += delta.cx;
			if( x + base_size.cx > win_size.cx ) break;
		}
	}
}


void  CFaceBase::UpdateTrainedImage()
{
	if( m_base_view )
	{
		m_base_view->InvalidateRect(0);
		m_base_view->UpdateWindow();
	}
}


void  CFaceBase::DeleteHMMInfo()
{
	POSITION pos = m_base.GetHeadPosition();

	while( pos )
	{
		CPerson* person = m_base.GetNext(pos);
		if( person )
		{
			person->DeleteHMMInfo();
		}
	}
}

int CFaceBase::RecognizeOtherBase(CFaceBase* other)
{   
	CWaitCursor wait;

	int total = 0;
	int right = 0;
	int wrong = 0;

	//iterate through all persons and all images
	for( int i = 0; i < other->GetPersonList().GetCount(); i++ )
	{
		CPerson* other_person = other->GetPerson( i );

		CPerson* this_person = FindPersonByName( other_person->GetName() );

		//match person with original base
		if ( this_person )
		{
			other_person->LoadRest();
			//iterate images
			CPersonImgList& list = other_person->GetImgList();

			POSITION pos = list.GetHeadPosition();
			while ( pos )
			{   
				//int indices[2048]; //2 kilomen

				float fLike = -1;
				int nIndex = -1;
				CPersonImage* img = list.GetNext(pos);
				RecognizePerson( img->GetImage(), CRect(0,0,0,0), fLike, nIndex);

				if ( GetPerson( nIndex ) == this_person )
				{
					right++;
				}
				else
				{
					wrong++;
				}
				total++;

				//write number of already processed images to status bar
				//CString RecogResult;

				char RecogResult[64];

			}

			other_person->UnloadRest();
		}
	}

	//message
	CString message;
	ASSERT( total == (right + wrong) );
	message.Format("Total images processed %d\nRight recognized %d\nReco rate is %.1f%% ",
		total, right, 100*(float)right/(float)total );


	MessageBox( 0, message, "Batch recognition results", MB_OK );


	return 1;
}

int CFaceBase::SetParams(   //sampling params
	CvSize dctSize, 
	CvSize obsSize, 
	CvSize delta,
	//HMM params
	int* states, int mix,
	//image scaling params
	BOOL use_width, int width,
	BOOL use_height, int height,
	BOOL suppress_intens, BOOL leave_hmm_alive)
{
	BOOL clearHMM = FALSE;

	//sampling params
	if ( (dctSize.height != m_dctSize.height) || (dctSize.width != m_dctSize.width) ||
		(obsSize.height != m_obsSize.height) || (obsSize.width != m_obsSize.width) ||
		(delta.height != m_delta.height) || (delta.width != m_delta.width) ||
		(m_suppress_intensity != suppress_intens ) )
	{
		clearHMM  = TRUE;
	}

	m_dctSize =  dctSize;
	m_obsSize =  obsSize;
	m_delta   =  delta;
	m_suppress_intensity = suppress_intens;

	//hmm params
	BOOL dochange = FALSE;

	for( int i = 0; i <= states[0]; i++ )
	{
		if ( m_stnum[i] != (states[i]) )
		{ 
			dochange = TRUE; 
			break;
		}
	}
	if ( mix != m_mixnum[0] ) 
		dochange = TRUE;

	if ( dochange )
	{
		//change hmm params 
		int counter = 0;
		for( int i = 0; i <= states[0]; i++ )
		{
			m_stnum[i] = (states[i]);
			for( int j  = 0; j < states[i]; j++, counter++ )
			{
				m_mixnum[counter]  = mix;
			}
		}
		clearHMM |= dochange;
	} 

	//image scaling params
	if ( (m_useWidth != use_width) ||
		(m_useHeight != use_height) )
	{ 
		clearHMM = TRUE;
	}
	else if ( ( m_useWidth &&  (width != m_scaleWidth) ) || 
		( m_useHeight && (height != m_scaleHeight) ) )
	{
		clearHMM = TRUE;
	}

	m_useWidth = use_width;
	m_useHeight = use_height;
	m_scaleWidth = width;
	m_scaleHeight = height;

	if( clearHMM && (!leave_hmm_alive) )
	{
		for ( int i = 0; i < m_base.GetCount(); i++ )
		{
			CPerson* person = GetPerson( i );
			person->ClearHMM();
			person->SetModified(true);
		}
	}
	return clearHMM;
}    

bool CFaceBase::TrainPerson(int index, bool loaded)
{
	CPerson* person = GetPerson( index );

	if (!person) return false;

	if (person->IsTrained() ) 
		return false;

	CPersonImgList& imgs = person->GetImgList();

	int num_img = imgs.GetCount();

	//if( num_img <= 0 ) return false;

	if ( !loaded )
	{
		person->LoadRest();
		person->TrainHMM();
		person->UnloadRest();         
	}
	else
		person->TrainHMM(); 

	return true;
}

void CFaceBase::TrainAll(int flag)
{
	for( POSITION pos = m_base.GetHeadPosition(); pos ; )
	{
		CPerson* person = m_base.GetNext(pos);
		if ((flag == TRAIN_UNTRAINED) && person->IsTrained() ) continue;

		person->LoadRest();
		person->TrainHMM();
		person->UnloadRest();
	}
	Save();
}

int CFaceBase::RecognizeBatch(CStringList* image_list)
{   
	int images_processed = 0;
	int right_recognized = 0;
	int wrong_recognized = 0;

	POSITION pos = image_list->GetHeadPosition();
	while( pos )
	{
		CString path = image_list->GetNext(pos);

		//check if person exists in base
		char drive[1024], dir[1024], fname[256], ext[32];
		_splitpath( path, drive, dir, fname, ext );

		CString input_name = fname;
		input_name.Delete( input_name.GetLength() - 4,  4 );
		//find person with such name
		if ( !FindPersonByName( input_name ) ) continue;

		CImage2 img;
		img.Load( path, 0 );

		//do recognition
		float fLike = -1;
		int nIndex = -1;
		int num = RecognizePerson( img, CRect(0,0,0,0), fLike, nIndex);

		ASSERT( num );

		CPerson* person = GetPerson( nIndex );
		ASSERT( person );
		CString person_name = person->GetName();

		//compare recognized person name and input file name
		if( person_name == input_name )
		{
			right_recognized++;
		}
		else
		{
			wrong_recognized++;
		}
		images_processed++;
	}

	//output results
	CString message;
	message.Format("Total images processed %d\nRight recognized %d\nReco rate is %.1f%% ",
		images_processed, right_recognized, 100*(float)right_recognized/(float)images_processed );

	MessageBox( 0, message, "Batch recognition results", MB_OK );

	return 1;

}

int CFaceBase::RecognizePerson(CImage2& image, CRect rect, float& fLike,int& nIndex)
{       
	if (m_base.GetCount()<=0)
	{
		return NO_PERSON;
	}
	if ( rect == CRect( 0,0,0,0 ) )
	{
		rect.bottom = image.Height();
		rect.right = image.Width();
	}

	float like_array[1000]; ASSERT( m_base.GetCount() < 1000 );

	CImage2 gray_img;
	gray_img.CopyOf( image );

	/************************************************************/
	//光线归一化处理
	//光照归一化处理
	/************************************************************************/
	IplImage* src = gray_img.GetImage();
	cvSetImageROI( src, RectToCvRect( rect ));

	//转换色彩空间
	cvCvtColor(src,src,CV_RGB2HSV);
	//分离通道
	IplImage* imgChannel[3] = { 0, 0, 0 };  

	for (int i=0;i<src->nChannels;i++)
	{
		imgChannel[i] = cvCreateImage( cvGetSize( src ), IPL_DEPTH_8U, 1 );  //要求单通道图像才能直方图均衡化  
	}

	cvSplit( src, imgChannel[0], imgChannel[1], imgChannel[2],0);//HSVA  

	CvScalar avg=cvAvg(imgChannel[2]);
	cvCvtScale(imgChannel[2],imgChannel[2],1.0,YUZHI-avg.val[0]);
	cvMerge( imgChannel[0], imgChannel[1], imgChannel[2], 0, src );  		

	cvCvtColor(src,src,CV_HSV2RGB);

	for (int i=0;i<src->nChannels;i++)
	{
		cvReleaseImage(&imgChannel[i] ); 
	}
	IplImage* ipl=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);

	cvCvtColor(src,ipl,CV_RGB2GRAY);

// 	cvNamedWindow("beforeReco");
// 	cvShowImage("beforeReco",ipl);
// 	CvScalar avg=cvAvg(src);
// 	double shift=100-avg.val[0];
// 	cvCvtScale(src,src,1.0,shift);
// 	IplImage* ipl=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
// 	cvCvtScale(src,ipl,1.0);

// 	cvNamedWindow("DF");
// 	cvShowImage("DF",ipl);

// 	IplImage* ipl32 = cvCreateImage(cvGetSize(src),IPL_DEPTH_32F,1);
// 	cvSetImageROI( ipl32, RectToCvRect( rect ));
// 	cvSetImageROI( src, RectToCvRect( rect ));
// 	CLightSet light;
// 	light.RunLightPrep(src,ipl32);
// 	IplImage* ipl=cvCreateImage(cvGetSize(ipl32),IPL_DEPTH_8U,1);
// 	cvCvtScale(ipl32,ipl,255);
// 	cvNamedWindow("src");
// 	cvShowImage("src",src);
// 
// 	cvNamedWindow("dst");
// 	cvShowImage("dst",ipl);
// 
// 	cvWaitKey(-1);
	/************************************************************/

	int code = 1;

	bool doRescale = false;

	int new_height = 0;
	int new_width = 0; 

	if ( m_useWidth )
	{
		doRescale = true;
		new_width = m_scaleWidth;
	}
	if ( m_useHeight )
	{
		doRescale = true;
		new_height = m_scaleHeight;
	}
	//recompute width or height if any is absent
	IplImage* ipl_scaled;
	if ( doRescale )
	{
		if ( !new_width )
		{
			new_width  = new_height * ipl->roi->width / ipl->roi->height;
		}
		else if ( !new_height ) 
		{
			new_height  = new_width * ipl->roi->height / ipl->roi->width;
		}

		//rescale
		ipl_scaled = cvCreateImage( cvSize( new_width, new_height ), IPL_DEPTH_8U, 1 );

		cvResize(ipl, ipl_scaled, /*ipl_scaled->width, ipl->width, 
								  ipl_scaled->height, ipl->height,*/ CV_INTER_NN);

	}
	else
		ipl_scaled = ipl;


	CvSize cvroi = cvSize( ipl_scaled->roi ? ipl_scaled->roi->width : ipl_scaled->width,
		ipl_scaled->roi ? ipl_scaled->roi->height : ipl_scaled->height);

	CvSize num_obs;
	CvImgObsInfo* info;

	CV_COUNT_OBS( &cvroi, &m_dctSize, &m_delta, &num_obs ); 

	int vect_len = m_obsSize.height*m_obsSize.width;

	if( m_suppress_intensity )
	{
		vect_len--;
	}

	info = cvCreateObsInfo( num_obs, vect_len );

	CString str;
	if( m_suppress_intensity )
	{
		float* observations = (float*)malloc( num_obs.height * num_obs.width * (vect_len+1) * sizeof(float) );
		cvImgToObs_DCT( /*normalized_image*/ipl_scaled, observations, m_dctSize, m_obsSize, m_delta );
		ExtractDCT( observations, info->obs, num_obs.height * num_obs.width, vect_len );
		free( observations);
	}
	else
	{
		cvImgToObs_DCT( /*normalized_image*/ipl_scaled, info->obs, m_dctSize, m_obsSize, m_delta );		
	}

	if ( doRescale )
	{
		cvReleaseImage( &ipl_scaled );
	}

	float max_like = -100000000; 

	int count=m_base.GetCount();
	for( int i = 0 ; i < m_base.GetCount(); i++ )
	{
		CPerson* person = m_base.GetAt( m_base.FindIndex(i) );
		CvEHMM* hmm = 0;

		if( !person->IsTrained() )
		{
			code = 0;
			break;
		}

		hmm = person->GetHMM().GetIppiEHMM();

		if (!hmm) //person not trained
		{
			code = 0;
			break;
		}

		cvEstimateObsProb( info, hmm );
		like_array[i] = cvEViterbi( info, hmm );
	}

	cvReleaseObsInfo( &info ); 
	gray_img.Destroy();

	if( !code )					//一旦某个人未经训练变参加
		return SOMEONE_NOT_TRAINED;

	float maxl = -FLT_MAX;
	int maxind = -1;

	for( int j = 0; j < m_base.GetCount(); j++ )
	{
		if (like_array[j] > maxl) { maxl = like_array[j]; maxind = j; }
	}

	fLike = maxl;
	nIndex = maxind;

/*	cvReleaseImage(&ipl32);*/
	cvReleaseImage(&ipl);

	return ALL_TRAINED;
}


/****************************************************************************************\
*                                  CPerson class                                         *
\****************************************************************************************/

CPerson::CPerson( CFaceBase* parent )
{
	m_modified = false;
	m_trained  = false;
	m_parent = parent;

	m_nActiveFace = -1;
	ASSERT( parent != 0 );
}

CPerson::~CPerson()
{
	Unload();
}


void   CPerson::SetName( const CString& name )
{
	m_name = name;
	SetModified();
}


void  CPerson::SetFolder( const CString& folder )
{
	m_folder = folder;
	SetModified();
}


bool  CPerson::GetPersonFullImageName( const char* root, int root_len,
	const char* image, char* full_image_name )
{
	char buffer[STR_BUF_SIZE];
	char drive[STR_BUF_SIZE];
	int  len;

	strcpy( buffer, image );

	len = strlen( buffer );
	if( len == 0 ) return false;

	_splitpath( buffer, drive, 0, 0, 0 );
	if( strlen( drive ) > 0 ) return false;

	if( root != full_image_name )
	{
		strcpy( full_image_name, root );
	}

	strcpy( full_image_name + root_len, buffer );
	return true;
}


void  CPerson::ExtractPersonImageName( const char* full_image_name, char* image )
{
	char buffer[STR_BUF_SIZE];
	char name[STR_BUF_SIZE];
	char ext[STR_BUF_SIZE];

	strcpy( buffer, full_image_name );

	_splitpath( buffer, 0, 0, name, ext );

	strcpy( image, name );
	strcat( image, ext );
}

bool  CPerson::Load()
{
	FILE* f = 0;
	char buffer[STR_BUF_SIZE];
	char root[STR_BUF_SIZE];
	int  root_len;
	bool error = false;
	CPersonImage* image = 0;
	RECT roi;
	bool already_read = false;

	strcpy( root, m_folder );
	root_len = m_folder.GetLength();
	strcpy( root + root_len, "info.txt" );

	f = fopen( root, "rt" );
	if( !f ) return false;

	m_imgs.RemoveAll();

	// read header
	if( !fgets( buffer, STR_BUF_SIZE, f ) || strcmp(buffer,person_signature))
		return false;

	if( !fgets( buffer, STR_BUF_SIZE, f ))
		return false;

	m_name = chomp( buffer );

	// skip one line after the base name
	fgets( buffer, STR_BUF_SIZE, f );

	// create image list for the person and load the first image
	for(;;)
	{
		if( !already_read && !fgets( buffer,STR_BUF_SIZE, f )) break;

		already_read = false;
		if( strlen(buffer) == 0 || buffer[0] == '#' ) continue;
		chomp( buffer );

		if( !GetPersonFullImageName( root, root_len, buffer, root ))
			continue;

		image = new CPersonImage;
		image->SetFileName( root );

		/* read ROI coordinates */
		while( fgets( buffer,STR_BUF_SIZE, f ))
		{
			if( strlen(buffer) > 0 && buffer[0] == '#' ) continue;
			already_read = true;
			if( sscanf( buffer, "%u%u%u%u", &roi.left, &roi.top,
				&roi.right, &roi.bottom ) == 4 )
			{
				roi.right += roi.left;
				roi.bottom += roi.top;
				image->SetRoiInFile( roi );
				already_read = false;
			}
			break;
		}
		bool flag_load=image->Load();
		if( m_imgs.IsEmpty() && !flag_load)
		{
			delete image;
			error = true;
			continue;
		}
		m_imgs.AddTail( image );
	}

	fclose(f);

	//load hmm if present
	strcpy( root + root_len, "hmm.txt" );
	f = fopen(root,"rt");
	if( !f ) m_trained = false;
	else
	{
		fclose(f);
		SetModified( error );
		m_trained = m_hmm.Load( root );
	}

	return true;
}


void  CPerson::Unload()
{
	Save();
	while( !m_imgs.IsEmpty() )
	{
		CPersonImage* image = m_imgs.RemoveHead();
		delete image;
	}
}


bool  CPerson::Save()
{
	if( IsModified() )
	{
		char buffer[STR_BUF_SIZE];

		POSITION pos = m_imgs.GetHeadPosition();
		while( pos )
		{
			CPersonImage* image = m_imgs.GetNext( pos );
			image->Save();
		}

		strcpy( buffer, m_folder );
		strcat( buffer, "info.txt" );

		FILE* f = fopen( buffer, "wt" );
		if( !f ) return false;

		fputs( person_signature, f );
		fputs( m_name, f );
		fputs( "\n\n", f );

		pos = m_imgs.GetHeadPosition();

		// write image names and ROI coordinates
		while( pos )
		{
			CPersonImage* image = m_imgs.GetNext( pos );
			const CString& str = image->GetFileName();
			CRect r = image->GetRoiInFile();
			ExtractPersonImageName( str, buffer );
			fprintf( f, "%s\n", buffer );
			if( !r.IsRectEmpty() )
			{
				fprintf(f, "%5u%5u%5u%5u\n", r.left, r.top, r.Width(), r.Height() );
			}
		}
		fclose(f);
	}

	if (IsTrained())
	{
		char buffer[STR_BUF_SIZE];

		//save hmm         
		strcpy( buffer, m_folder );
		strcat( buffer, "hmm.txt" );

		m_hmm.Save( buffer );
	}
	else
	{
		char buffer[STR_BUF_SIZE];
		strcpy( buffer, m_folder );
		strcat( buffer, "hmm.txt" );        
		remove( buffer );
	}

	SetModified(false);

	return true;
}


void  CPerson::LoadRest()
{
	// load all the face images starting from second (first is always loaded)
	POSITION pos = m_imgs.FindIndex(1);

	while( pos )
	{
		POSITION tmp_pos = pos;
		CPersonImage* image = m_imgs.GetNext( pos );
		if( !image->Load())
		{
			m_imgs.RemoveAt(tmp_pos);
			SetModified();
			delete image;
		}
	}
}

void  CPerson::UnloadRest()
{
	// load all the face images starting from second (first is always loaded)
	POSITION pos = m_imgs.FindIndex(1);

	while( pos )
	{
		CPersonImage* image = m_imgs.GetNext( pos );
		image->Unload();
	}
}


void  CPerson::GenerateFileName( const char* base, char* filename )
{
	char path[STR_BUF_SIZE];
	int base_len = base ? strlen(base) : 0;
	int i = 0;

	if( base_len > 0 )
	{
		strcpy( filename, base );
	}
	else
	{
		char ext[STR_BUF_SIZE];
		strcpy( path, m_folder );
		// remove slash
		path[m_folder.GetLength()-1] = '\0';
		// use folder name as a base
		_splitpath( path, 0, 0, filename, ext );
		strcat( filename, ext );
		base_len = strlen( filename );
	}

	for(;;)
	{
		FILE* f = 0;
		for( ; i < 10000; i++ )
		{
			GetPersonFullImageName( m_folder, m_folder.GetLength(), filename, path );
			sprintf( path + strlen(path), "%04d.bmp", i );
			f = fopen( path, "rb" );
			if( !f ) break;
			fclose(f);
		}
		if( i == 10000 )
		{
			ASSERT(0);
			return;
		}

		// try to open for writing. If success, output name
		f = fopen( path, "wb" );
		if( !f ) continue;

		fclose(f);
		remove( path );
		strcpy( filename, path );
		break;
	}
}

int  CPerson::AddImage( const char* filename, CImage2* import_image, CRect rect )
{
	char root[STR_BUF_SIZE];
	CPersonImage* image;
	int root_len;

	strcpy( root, m_folder );
	root_len = m_folder.GetLength();

	if( !filename )
	{
		ASSERT( import_image != 0 );
		GenerateFileName( 0, root );
	}
	else
	{
		GetPersonFullImageName( root, root_len, filename, root );
	}

	image = new CPersonImage;
	image->SetFileName( root );

	if( import_image )
	{
		CImage2& dst_img = image->GetImage();
		IplImage* src_img = import_image->GetImage();
		IplROI* temp_roi;
		IplROI  roi;
		ASSERT( src_img != 0 );

		temp_roi = src_img->roi;

		if( rect.IsRectEmpty() )
		{
			src_img->roi = 0;
		}
		else
		{
			src_img->roi = &roi;
			roi = RectToROI( rect );
		}

		CImage2& saveImage=image->m_saveImg;
		saveImage.CopyOf(*import_image,-1);

		dst_img.CopyOf( *import_image, -1 );
		src_img->roi = temp_roi;
		image->SetRoiInFile( CRect(0,0,0,0));
		image->SetModified();

		// to reserve file name
		image->Save();
	}
	else
	{
		image->SetRoiInFile( rect );
		image->Load();
	}

	m_imgs.AddTail( image );

	SetModified();

	return m_imgs.GetCount() - 1;
}

void  CPerson::RemoveImage( POSITION pos )
{
	CPersonImage* image = m_imgs.GetAt( pos );
	if( image )
	{
		m_imgs.RemoveAt( pos );
		image->Unload();
		remove( image->GetFileName() );
		delete image;
		SetModified();
	}
}

void CPerson::DeleteHMMInfo()
{
	m_trained = false;
	CString str = GetFolder() + "hmm.txt";
	remove( str );
}

CContEHMM& CPerson::GetHMM()
{
	return m_hmm;
}

void CPerson::ClearHMM()
{
	m_hmm.Release();
	m_trained = false;
}

void CPerson::Hist(IplImage* src,IplImage* dst)
{
	//MessageBox(NULL,"sdf12","SDF12",MB_OKCANCEL);
// 	IplImage* redImage=cvCreateImage(cvGetSize(src),src->depth,1);	//用于存储输入图像的红色分量图像 
// 	IplImage* greenImage=cvCreateImage(cvGetSize(src),src->depth,1);//用于存储输入图像的绿色分量图像 
// 	IplImage* blueImage=cvCreateImage(cvGetSize(src),src->depth,1);	//用于存储输入图像的蓝色分量图像  
// 	cvSplit(src,blueImage,greenImage,redImage,NULL);				//将彩色图像image的蓝色分量图像blueImage﹑ //绿色分量图像greenImage和红色分量图像redImage提取出来   
// 	cvEqualizeHist(redImage,redImage);								//对红色分量图像进行直方图均衡化处理 
// 	cvEqualizeHist(greenImage,greenImage);							//对绿色分量图像进行直方图均衡化处理 
// 	cvEqualizeHist(blueImage,blueImage);							//对蓝色分量图像进行直方图均衡化处理  
// 	cvMerge(blueImage,greenImage,redImage,NULL,dst);
// 	cvReleaseImage(&redImage);
// 	cvReleaseImage(&greenImage); 
// 	cvReleaseImage(&blueImage);	
	cvEqualizeHist(src,dst);
}

void CPerson::TrainHMM()
{

	int color[24] = { RGB(255,128,128), RGB(255,255,128), RGB(128,255,128), RGB(128,255,255), RGB(0,128,255),
		RGB(255,128,255), RGB(255,0,0), RGB(255,128,0), RGB(0,128,0), RGB(0,0,0), RGB(255,255,128),
		RGB(255,0,128), RGB(255,128,128), RGB(255,255,128),RGB(128,255,128), RGB(128,255,255),
		RGB(0,128,255),RGB(255,128,255),RGB(255,0,0),RGB(255,128,0),RGB(0,128,0), 
		RGB(0,0,0),RGB(255,255,128), RGB(255,0,128)  };

	//training loop can be not converged
	const int max_iterations = 80;    

	int num_img = m_imgs.GetCount();
	//num_img=1;
	if( num_img <= 0 )
	{	
		return;
	}	

	CFaceBase* parent = GetParentBase();
	//CImage2& segmentation = parent->GetTrainedImage();
	//segmentation.Create( 320, 320, 24 );

	int vect_len = parent->m_obsSize.height*parent->m_obsSize.width;

	//suppress first DCT coefficient
	if( parent->m_suppress_intensity)
	{
		vect_len--;
	}

	CvEHMM* hmm = m_hmm.GetIppiEHMM();
	if (!hmm) m_hmm.CreateHMM( parent->m_stnum, parent->m_mixnum, vect_len );

	hmm = m_hmm.GetIppiEHMM();

	//Create observation info 
	CArray< CvImgObsInfo* , CvImgObsInfo* > obs_info;   
	obs_info.SetSize( num_img );

	CvImgObsInfo** obs_info_array = obs_info.GetData();

	char name[100];
	CLightSet light;
	for( int i = 0; i < num_img; i++ )
	{
		POSITION pos = m_imgs.FindIndex(i);
		/*IplImage* ipl = m_imgs.GetAt(pos)->GetImage().GetImage();*/

		IplImage* src = m_imgs.GetAt(pos)->GetImage().GetImage();
		ASSERT(src->nChannels==3);
		//Hist(ipl,ipl);				//对图像进行直方图均衡化处理
		//光照归一化处理
		/************************************************************************/
		//转换色彩空间
		cvCvtColor(src,src,CV_RGB2HSV);
		//分离通道
		IplImage* imgChannel[3] = { 0, 0, 0 };  

		for (int i=0;i<src->nChannels;i++)
		{
			imgChannel[i] = cvCreateImage( cvGetSize( src ), IPL_DEPTH_8U, 1 );  //要求单通道图像才能直方图均衡化  
		}

		cvSplit( src, imgChannel[0], imgChannel[1], imgChannel[2],0);//HSVA  
		
		CvScalar avg=cvAvg(imgChannel[2]);
		cvCvtScale(imgChannel[2],imgChannel[2],1.0,YUZHI-avg.val[0]);
		cvMerge( imgChannel[0], imgChannel[1], imgChannel[2], 0, src );  		

		cvCvtColor(src,src,CV_HSV2RGB);

		for (int i=0;i<src->nChannels;i++)
		{
			cvReleaseImage(&imgChannel[i] ); 
		}
		IplImage* ipl=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);

		cvCvtColor(src,ipl,CV_RGB2GRAY);
// 		cvNamedWindow("beforeTrain");
// 		cvShowImage("beforeTrain",ipl);
// 		CvScalar avg=cvAvg(src);
// 		double shift=100-avg.val[0];
// 		cvCvtScale(src,src,1.0,shift);
// 		IplImage* ipl=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
// 		cvCvtScale(src,ipl,1.0);

// 		cvNamedWindow("DF");
// 		cvShowImage("DF",ipl);
// 		IplImage* ipl32 = cvCreateImage(cvGetSize(src),IPL_DEPTH_32F,1);
// 		light.RunLightPrep(src,ipl32);
// 		cvNormalize(ipl32,ipl32);
// 		IplImage* ipl=cvCreateImage(cvGetSize(ipl32),IPL_DEPTH_8U,1);
// 		cvCvtScale(ipl32,ipl,255);
// 
// 		memset(name,'\0',100);
// 
// 		sprintf(name,"src%d",i);
// 		cvNamedWindow(name);
// 		cvShowImage(name,src);
// 
// 		memset(name,'\0',100);
// 		sprintf(name,"dst%d",i);
// 		cvNamedWindow(name);
// 		cvShowImage(name,ipl);
// 
// 		cvWaitKey(-1);
		/************************************************************************/
		bool doRescale = false;

		int new_height = 0;
		int new_width = 0;

		if ( parent->m_useWidth )
		{
			doRescale = true;
			new_width = parent->m_scaleWidth;
		}
		if ( parent->m_useHeight )
		{
			doRescale = true;
			new_height = parent->m_scaleHeight;
		}
		//recompute width or height if any is absent
		IplImage* ipl_scaled;

		CvSize image_roi = cvSize( ipl->roi ? ipl->roi->width : ipl->width, 
			ipl->roi ? ipl->roi->height : ipl->height );

		if ( doRescale )
		{
			if ( !new_width )
			{
				new_width  = new_height * image_roi.width / image_roi.height;
			}
			else if ( !new_height ) 
			{
				new_height  = new_width * image_roi.height / image_roi.width;
			}

			//rescale
			ipl_scaled = cvCreateImage( cvSize(new_width, new_height), IPL_DEPTH_8U, 1 );

			cvResize(ipl, ipl_scaled, /*ipl_scaled->width, ipl->width, 
									  ipl_scaled->height, ipl->height,*/ CV_INTER_NN);

		}
		else
			ipl_scaled = ipl;

		CvSize roi = cvSize( ipl_scaled->roi ? ipl_scaled->roi->width : ipl_scaled->width,
			ipl_scaled->roi ? ipl_scaled->roi->height : ipl_scaled->height);


		CvSize num_obs;

// 		CV_COUNT_OBS( &roi, &(m_dctSize), &(m_delta), &num_obs );得到DCT观察值的块信息。
// 
// 		Roi：图像中感兴趣的区域，如果图像已经剪切好，则次之可以设置为图像的宽度与高度。
// 
// 		m_dctSize：DCT采样块的大小。如：12*12。
// 
// 		m_delta：DCT采样的步长，如4*4。
// 
// 		num_obs：输出参数，结果为DCT在该图像上横向，竖向的采样个数，如20*26，即为在横向上有20个DCT的观测向量，竖向有26个DCT的观测向量。

		CV_COUNT_OBS( &roi, &(parent->m_dctSize), &(parent->m_delta), &num_obs ); 


		obs_info_array[i] = cvCreateObsInfo( num_obs, vect_len );

		CvImgObsInfo* info = obs_info_array[i];

		//IplImage* normalized_image = cvCreateImage( roi, IPL_DEPTH_8U, 1 );
		//NormalizeImageForHMM( ipl_scaled, normalized_image );

		//cvImgToObs_DCT:从图像中提取的观察向量即DCT(离散余弦变换)系数
		
		if( parent->m_suppress_intensity )
		{
			float* observations = (float*)malloc( num_obs.height * num_obs.width * (vect_len+1) * sizeof(float) );
			cvImgToObs_DCT( /*normalized_image*/ipl_scaled, observations, parent->m_dctSize, parent->m_obsSize, parent->m_delta );
			ExtractDCT( observations, info->obs, num_obs.height * num_obs.width, vect_len );
			free( observations);
		}
		else
		{
			cvImgToObs_DCT( /*normalized_image*/ipl_scaled, info->obs, parent->m_dctSize, parent->m_obsSize, parent->m_delta );
		}

		if ( doRescale )
		{
			cvReleaseImage( &ipl_scaled );
		}

		//cvReleaseImage( &normalized_image );

		cvUniformImgSegm( info, hmm );

	}                                              

	cvInitMixSegm( obs_info_array, num_img, hmm );

	bool trained = false;
	float old_likelihood = 0;


	int counter = 0;

	while( (!trained) && (counter < max_iterations) )
	{ 
		counter++;

		int j;
#if 0

		//segment images
		for( j = 0; j < 1; j++ )
		{       
			IplImage* ipl_segm = segmentation.GetImage();
			CvImgObsInfo* obs = obs_info_array[j];

			int counter=0;
			for(int k = 0; k < obs->obs_y; k++ )
			{
				for(int m = 0; m < obs->obs_x; m++,counter++ )
				{                       
					cvCircle( ipl_segm, cvPoint( (parent->m_dctSize.width>>1) + 
						(parent->m_delta.width)* m ,
						(parent->m_dctSize.height>>1) + 
						(parent->m_delta.height)* k ), 3,
						color[obs->state[counter*2+1]], 1 );
				}
			} 

			parent->SetTrainedIndex(j);
			parent->UpdateTrainedImage();

		}        

#endif            
		//计算各个状态下混高斯的均值、方差和混高斯的权重。
		cvEstimateHMMStateParams( obs_info_array, num_img, hmm);
		//计算超态与超态之间，相邻子态之间的转移矩阵
		cvEstimateTransProb( obs_info_array, num_img, hmm); 

		float likelihood = 0;     
		for( j = 0; j < num_img; j++ )
		{   
			//cvEstimateObsProb:计算在每个内嵌HMM状态下的每个观测值的高斯概率。得到ehmm->obsProb。
			//对应于每一行（obs_x）所有观测向量，在所有超态下的所有子态发生的概率
			//（这个概率是每个子态下混高斯的加权），

			cvEstimateObsProb( obs_info_array[j], hmm );

// 			1,先对每一行的各个观测向量在所有超态的子态下进行viterbi算法，计算
// 
// 			最能解释当前行观测向量的子态序列，及其最大概率。
// 
// 			2, 把得到的概率最为当前行发生在各个超态下的概率，再在超态下计算viterbi算法，
// 
// 			计算最能解释当前行观测向量的超态序列，并给出最大概率likelihood。
// 
// 			3, 最能解释各个超态的子态序列和各个超态序列被放回到obsInfoVec[j]->obs_info->state
// 
// 			4,结果，影响的是obsInfoVec[j]中超态在行上的分布，每个超态在每一行中超态的子态中的分布。

			likelihood += cvEViterbi( obs_info_array[j], hmm );
		}
		likelihood /= num_img*obs_info_array[0]->obs_size;

		cvMixSegmL2( obs_info_array, num_img, hmm);

		trained = ( fabs(likelihood - old_likelihood) < 0.01 ); 
		old_likelihood = likelihood;                   
	}

	for(int i = 0; i < num_img; i++ )
	{
		cvReleaseObsInfo( &(obs_info_array[i]) );
	}

	obs_info.RemoveAll();  
	m_trained = true;
	Save();

}

/****************************************************************************************\
*                              CPersonImage class                                        *
\****************************************************************************************/

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



