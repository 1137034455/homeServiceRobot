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
#include "FaceBase.h"
#include "Person.h"
#include "PersonImage.h"
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

			while(pos)
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

	IplImage* src = gray_img.GetImage();
	cvSetImageROI( src, RectToCvRect( rect ));

	//光照归一化处理
	m_light.LightNormalization(src, src, IlluminationThreshold);

	IplImage* ipl=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);

	cvCvtColor(src,ipl,CV_RGB2GRAY);

	int code = 1;

	bool doRescale = false;

	int new_height = 0;
	int new_width = 0; 

	if(m_useWidth)
	{
		doRescale = true;
		new_width = m_scaleWidth;
	}
	if(m_useHeight)
	{
		doRescale = true;
		new_height = m_scaleHeight;
	}
	//recompute width or height if any is absent
	IplImage* ipl_scaled;
	if (doRescale)
	{
		if (!new_width)
		{
			new_width  = new_height * ipl->roi->width / ipl->roi->height;
		}
		else if(!new_height) 
		{
			new_height  = new_width * ipl->roi->height / ipl->roi->width;
		}

		//rescale
		ipl_scaled = cvCreateImage( cvSize( new_width, new_height ), IPL_DEPTH_8U, 1 );

		cvResize(ipl, ipl_scaled, /*ipl_scaled->width, ipl->width, 
								  ipl_scaled->height, ipl->height,*/ CV_INTER_NN);
	}
	else
	{
		ipl_scaled = ipl;
	}

	CvSize cvroi = cvSize( ipl_scaled->roi ? ipl_scaled->roi->width : ipl_scaled->width,
		ipl_scaled->roi ? ipl_scaled->roi->height : ipl_scaled->height);

	CvSize num_obs;
	CvImgObsInfo* info;

	CV_COUNT_OBS(&cvroi, &m_dctSize, &m_delta, &num_obs ); 

	int vect_len = m_obsSize.height*m_obsSize.width;

	if(m_suppress_intensity)
	{
		vect_len--;
	}

	info = cvCreateObsInfo(num_obs, vect_len);

	CString str;
	//是否抑制直流分量
	if(m_suppress_intensity)
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

	if (doRescale)
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


