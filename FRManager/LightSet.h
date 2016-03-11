#pragma once
#include "StdAfx.h"
#include "cv.h"
#include "math.h"
#include "cxcore.h"
#include "highgui.h"


/// <summary>
/// ������Ҫ�ṩ�˼������չ�һ��������㷨��ʹ�����߲�ͬǿ�ȹ����µ�ʶ����
/// </summary>
class CLightSet
{
public:
	CLightSet(void);
	~CLightSet(void);

	//��ͼƬ����Ԥ����
	void RunLightPrep(IplImage* src,IplImage* dst);
	//��ͼƬ���м򵥵Ĺ��չ�һ������
	void LightNormalization(IplImage* src, IplImage* dst, int threshold);
private:
	void GetPath(char* path,char* argv,char* add,int n=100);
	void lpfilter(CvMat *matD,CvMat *matH,float D0,float rH,float rL,float c);
	void fft2(IplImage *src,CvMat *dst);
	void cvShiftDFT(CvArr * src_arr, CvArr * dst_arr );
	void CDM(int M,int N,CvMat *mat);
};

