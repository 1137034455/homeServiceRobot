// ContEHMM.h: interface for the CContEHMM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTEHMM_H__61B039BC_12D3_4DC9_8130_21625B086E92__INCLUDED_)
#define AFX_CONTEHMM_H__61B039BC_12D3_4DC9_8130_21625B086E92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CV.h"
#include "legacy.hpp "
#include "CvvImage.h"

class CContEHMM  
{
public:
	BOOL Release();
	CContEHMM();
	virtual ~CContEHMM(); 

    bool CreateHMM( int* num_states, int* num_mix, int vect_size ); 
    int GetVectSize() { return m_vectSize; };
    
    //IppiEHMM* GetIppiEHMM() { return m_hmm; };
    CvEHMM* GetIppiEHMM() { return m_hmm; };
    

    bool Save( const char* filename );
    bool Load( const char* filename );

protected:
    
    //IppiEHMM* m_hmm;
    CvEHMM* m_hmm;    
    int m_vectSize;
    
};

#endif // !defined(AFX_CONTEHMM_H__61B039BC_12D3_4DC9_8130_21625B086E92__INCLUDED_)
