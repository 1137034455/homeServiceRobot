#ifndef _HOMOGRAPHICFILTER_H
#define _HOMOGRAPHICFILTER_H

//代码完成同态滤波(图像增强),主调函数: cvmHomographicFilter()
//入口参数:IplImage *src
//返    回:滤波后的双精度图像矩阵 CvMat* ...

//如果对您有用,就请放心使用,无版权问题!
//代码已经在Vs.NET 2003 + OpenCV 5.0 beta 测试通过，测试图像为light.bmp
//如果遇到编译错误,请自行解决...哈哈...
//有什么好的建议,就请QQ联系: 6429353 , 或 机器视觉在线,OPENCV讨论群(1#,2#,3#,4#,5#)
//谢谢...

#include "cv.h"
#include "cvcam.h"
#include "highgui.h"
#include <fstream>
#include <iostream>

using namespace std;
//封装的复数操作类
typedef struct CvComplex
{
private:
	CvComplex64f *complex;
	int rows;
	int cols;
public:
	void Save(char* filename)
	{
		if (IsEmpty()) return;
		ofstream of;
		of.open(filename,ofstream::out|ofstream::trunc);
		if (of.is_open())
		{
			of << rows << "×" << cols << endl;
			for(int i=0;i<rows;i++)
			{
				for (int j=0;j<cols;j++)
				{
					if (GetElemIm(i,j)>=0)
						of << GetElemRe(i,j) << "+" << GetElemIm(i,j) << "i" ;
					else
						of << GetElemRe(i,j) << "" << GetElemIm(i,j) << "i" ;
					if (j==(cols-1))
						of << endl << endl;
					else
						of <<  ",";
				}
			}
			of.close();
		}
	}
	double GetAngle(int rs,int cls)
	{
		if (rs>rows || cls>cols || rs<0 || cls<0)
			return 0.0;
		return atan2(complex[rs*cols+cls].im,complex[rs*cols+cls].re);
	}
	double GetMaxAbs()
	{
		double max=0.0,tmp;
		if (IsEmpty()) return -1;
		for(int i=0;i<rows;i++)
		{
			for (int j=0;j<cols;j++)
			{
				tmp=abs(complex[i*cols+j]);
				if (tmp>max)
					max=tmp;
			}
		}
		return max;
	}
	double GetAbs(int rs,int cls)
	{
		if (rs>rows || cls>cols || rs<0 || cls<0)
			return 0.0;
		return abs(complex[rs*cols+cls]);
	}
	void GetSize(int& h,int& w)
	{
		h=rows;w=cols;
	}
	void Zero()
	{
		if (IsEmpty()) return;
		CvComplex64f comp;
		comp.re=0.0;
		comp.im=0.0;
		for (int i=0;i<rows;i++)
		{
			for (int j=0;j<cols;j++)
				SetElem(i,j,comp);
		}
	}
	void Init(int rs,int cls)
	{
		if (!IsEmpty())
			Destroy();
		rows=rs;cols=cls;
		complex=(CvComplex64f*)new CvComplex64f[rs*cls];
		Zero();
	}
	void SetElemRe(int rs,int cls,double v)
	{
		if (rs>rows || cls>cols || rs<0 || cls<0)
			return;
		complex[rs*cols+cls].re=v;
	}
	void SetElemIm(int rs,int cls,double v)
	{
		if (rs>rows || cls>cols || rs<0 || cls<0)
			return;
		complex[rs*cols+cls].im=v;
	}
	void SetElem(int rs,int cls,CvComplex64f comp)
	{
		if (rs>rows || cls>cols || rs<0 || cls<0)
			return;
		complex[rs*cols+cls].re=comp.re;
		complex[rs*cols+cls].im=comp.im;
	}
	double GetElemIm(int rs,int cls)
	{
		if (rs>rows || cls>cols || rs<0 || cls<0)
			return -1;
		return complex[rs*cols+cls].im;
	}
	double GetElemRe(int rs,int cls)
	{
		if (rs>rows || cls>cols || rs<0 || cls<0)
			return -1;
		return complex[rs*cols+cls].re;
	}
	void GetElem(int rs,int cls,CvComplex64f& comp)
	{
		if (rs>rows || cls>cols || rs<0 || cls<0)
			return;
		comp.re=complex[rs*cols+cls].re;
		comp.im=complex[rs*cols+cls].im;
	}
	CvComplex()
	{
		rows=0;cols=0;complex=0;
	}
	BOOL IsEmpty()
	{
		return (rows==0 && cols==0 && complex==0);
	}
	void Destroy()
	{
		if (IsEmpty())
			return;
		if (complex) 
			delete [] complex;
		complex=0;
		rows=0;cols=0;
	}
}
CvComplex;
CvMat* cvmHomographicFilter(IplImage* src);
BOOL cvmDFT(CvMat *src,CvComplex& dfft,int flags);
void cvmExpi(CvMat* src,CvComplex& result);
void cvmExp(CvComplex &comp);
void cvmMatDotComplex(CvMat* src,CvComplex& comp);
CvMat* cvmGetMatFromComplex(CvComplex comp,int flags=CV_DXT_INVERSE);
#endif