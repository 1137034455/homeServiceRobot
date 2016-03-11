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
//M*/// FaceBase.h: interface for the CFaceBase class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "cv.h"
#include "highgui.h"
#include "ContEHMM.h"  
#include "CvvImage.h"
#include "Person.h"
#include "PersonImage.h"
#include "BaseSet.h"
#include "LightSet.h"

#define CImage2 CvvImage

#define TRAIN_ALL 1
#define TRAIN_UNTRAINED 2

#define NO_PERSON 0
#define SOMEONE_NOT_TRAINED -1
#define ALL_TRAINED 1

#define IlluminationThreshold 80		//光照强度归一化的值

class CPerson;
class CPersonImage;

class CFaceBase  
{
public:
	int RecognizeOtherBase(CFaceBase* other );
	int RecognizePerson( CImage2& image, CRect rect, float& fLike,int& nIndex );
    int RecognizeBatch(CStringList* image_list);

	void TrainAll( int flag );
	bool TrainPerson( int index, bool loaded );
	CFaceBase();
	virtual ~CFaceBase();

    void     SetFileName( const CString& filename );
    const    CString&  GetFileName() { return m_filename; }
    void     SetName( const CString& name );
    const    CString&  GetName() { return m_basename; }
    bool     Load();
    void     Unload();
    bool     Save();
    CPerson* AddPerson( const char* name, const char* folder, bool import_data );
    void     RemovePerson( POSITION pos );
    CPersonList& GetPersonList() { return m_base; };
    void     SetImageSize( CSize size );
    CSize    GetImageSize() { return m_baseImgSize; };
    void     SetModified( bool modified = true ) { m_modified = modified; }
    bool     IsModified() { return m_modified; }
    void     Draw( int index, CImage2& img, SIZE win_size, int y_pos,
                   SIZE pic_size, SIZE delta );
    CPerson* GetPerson( int index );
    CPerson* FindPersonByName( const CString& name );
    int      GetPersonIndex( CPerson* person );
    CImage2&  GetTrainedImage() { return m_trained_image; }
    int      GetTrainedIndex() { return m_trained_index; }
    void     SetTrainedIndex( int index ) { m_trained_index = index; }
    void     UpdateTrainedImage();
    void     SetBaseView( CWnd* view ) { m_base_view = view; }
    CWnd*    GetBaseView() { return m_base_view; }
    void     DeleteHMMInfo();

    int  SetParams( //sampling params
                    CvSize dctSize, 
                    CvSize obsSize, 
                    CvSize delta,
                    //HMM params
                    int* states, int mix,
                    //image scaling params
                    BOOL use_width, int width,
                    BOOL use_height, int height,
                    BOOL suppress_intens,
                    BOOL leave_hmm_alive = FALSE);


    //hmm params 
    int m_stnum[32];
    int m_mixnum[128];
    
    CvSize m_delta;
    CvSize m_obsSize;
    CvSize m_dctSize;

    BOOL m_useWidth; 
    BOOL m_useHeight; 
    int m_scaleWidth; 
    int m_scaleHeight;

    BOOL m_suppress_intensity;

protected:
    CSize    m_baseImgSize;
    CString  m_filename;
    CString  m_basename;
    CPersonList m_base;
    bool     m_modified;
    int      m_trained_index;
    CImage2   m_trained_image;
    CWnd*    m_base_view;
	CLightSet m_light;
    bool     GetPersonFolder( const char* root_folder, int root_folder_len,
                              const char* person_folder, char* folder );
    void     GetPersonSubFolder( const char* folder, char* subfolder );
    void     GetRootFolder( char* root_folder, int* root_path_len );
};