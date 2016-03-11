#pragma once
#define IlluminationThreshold 80
#include "cv.h"
#include "highgui.h"
#include "ContEHMM.h"  
#include "CvvImage.h"
#include "LightSet.h"
#include "FaceBase.h"
#include "PersonImage.h"
#include "BaseSet.h"

#define CImage2 CvvImage

class CPerson;
class CFaceBase;

class CPerson  
{
public:
	void ClearHMM();
	CContEHMM& GetHMM();
	CPerson( CFaceBase* parent );
	virtual ~CPerson();

	int      GetActiveFace() { return m_nActiveFace; };

	void     SetName( const CString& name );
	const    CString& GetName() { return m_name; }
	void     SetFolder( const CString& folder );
	void     GenerateFileName( const char* base, char* filename );
	const    CString& GetFolder() { return m_folder; }
	int      AddImage( const char* filename, CImage2* import_image, CRect roi );
	void     RemoveImage( POSITION pos );
	bool     Load();
	void     Unload();
	bool     Save();
	void     LoadRest();
	void     UnloadRest();
	CPersonImgList& GetImgList() { return m_imgs; }
	void     SetModified( bool modified = true )
	{
		m_modified = modified;
		if( modified )
			m_trained = false;
	}
	bool     IsModified() { return m_modified; }
	CFaceBase*  GetParentBase() { return m_parent; }
	void     TrainHMM();
	bool     IsTrained() { return m_trained; };
	void     DeleteHMMInfo();

protected:
	int      m_nActiveFace;
	CString  m_name;
	CString  m_folder;
	CPersonImgList  m_imgs;
	CContEHMM m_hmm;
	CFaceBase* m_parent;
	CLightSet m_light;
	bool     m_trained;
	bool     m_modified;
	bool     GetPersonFullImageName( const char* root, int root_len,
		const char* image, char* full_image_name );
	void     ExtractPersonImageName( const char* full_image_name, char* image );
	void Hist(IplImage* src,IplImage* dst);
};

