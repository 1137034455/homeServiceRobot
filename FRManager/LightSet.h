#pragma once
#include "StdAfx.h"
#include "cv.h"
#include "math.h"
#include "cxcore.h"
#include "highgui.h"


/// <summary>
/// 此类主要提供了几个光照归一化处理的算法，使得增高不同强度光照下的识别率
/// </summary>
class CLightSet
{
public:
	CLightSet(void);
	~CLightSet(void);

	//对图片进行预处理
	void RunLightPrep(IplImage* src,IplImage* dst);
	//对图片进行简单的光照归一化处理
	void LightNormalization(IplImage* src, IplImage* dst, int threshold);
private:
	void GetPath(char* path,char* argv,char* add,int n=100);
	void lpfilter(CvMat *matD,CvMat *matH,float D0,float rH,float rL,float c);
	void fft2(IplImage *src,CvMat *dst);
	void cvShiftDFT(CvArr * src_arr, CvArr * dst_arr );
	void CDM(int M,int N,CvMat *mat);
};

