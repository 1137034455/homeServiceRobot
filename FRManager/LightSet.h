#pragma once
#include "StdAfx.h"
#include "cv.h"
#include "math.h"
#include "cxcore.h"
#include "highgui.h"

class CLightSet
{
public:
	CLightSet(void);
	~CLightSet(void);
	void RunLightPrep(IplImage* src,IplImage* dst);
private:
	void GetPath(char* path,char* argv,char* add,int n=100);
	void lpfilter(CvMat *matD,CvMat *matH,float D0,float rH,float rL,float c);
	void fft2(IplImage *src,CvMat *dst);
	void cvShiftDFT(CvArr * src_arr, CvArr * dst_arr );
	void CDM(int M,int N,CvMat *mat);
};

