#include "stdafx.h"
#include "homographicfilter.h"
//�ɸ���Ҷ�任ϵ���ϳ�OPENCV�ķ��任����
CvMat* cvmGetMatFromComplex(CvComplex comp,int flags)
{
	CvMat* result=0;
	if (comp.IsEmpty())
		return 0;
	int i,j,M,N;
	comp.GetSize(M,N);
	result=cvCreateMat(M,N,CV_64FC1);
	cvZero(result);
	//���任
	if (flags==CV_DXT_INVERSE)
	{
		//��һ��
		for(i=0;i<M;i++)
		{
			if (i==0)
				((double*)result->data.ptr)[0]=comp.GetElemRe(0,0);
			else
				if (i%2==1)
					((double*)(result->data.ptr+i*result->step))[0]=comp.GetElemRe((i+1)/2,0);
				else
					((double*)(result->data.ptr+i*result->step))[0]=comp.GetElemIm(i/2,0);
		}
		//�м��
		for (i=0;i<M-1;i++)
		{
			for (j=1;j<N-1;j++)
			{
				if (j%2==1)
					((double*)(result->data.ptr+i*result->step))[j]=comp.GetElemRe(i,(j+1)/2);
				else
					((double*)(result->data.ptr+i*result->step))[j]=comp.GetElemIm(i,j/2);
			}
		}
		//M-1��
		if (M%2==0)
		{
			((double*)(result->data.ptr+(M-1)*result->step))[0]=comp.GetElemRe(M/2,0);
			for (j=1;j<N-1;j++)
			{
				if (j%2==1)
					((double*)(result->data.ptr+(M-1)*result->step))[j]=comp.GetElemRe(M-1,(j+1)/2);
				else
					((double*)(result->data.ptr+(M-1)*result->step))[j]=comp.GetElemIm(M-1,j/2);
			}
		}
		//N/2+1��
		if (N%2==0)
		{
			((double*)(result->data.ptr+0*result->step))[N-1]=comp.GetElemRe(0,N/2);
			for (i=1;i<M-1;i++)
			{
				if (i%2==1)
					((double*)(result->data.ptr+i*result->step))[N-1]=comp.GetElemRe((i+1)/2,N/2);
				else
					((double*)(result->data.ptr+i*result->step))[N-1]=comp.GetElemIm(i/2,N/2);
			}
		}
		if (M%2==0 && N%2==0)
		{
			((double*)(result->data.ptr+(M-1)*result->step))[N-1]=comp.GetElemIm(M-1,N/2);
		}
	}
	else
	//��ͨ����(�Ǳ任)
	{
		for (i=0;i<M;i++)
		{
			for (j=0;j<N;j++)//����...
				((double*)(result->data.ptr+i*result->step))[j]=comp.GetAbs(i,j);
		}
	}
	return result;
}
void cvmExp(CvComplex& comp)
{
	if (comp.IsEmpty())
		return;
	int h,w;
	comp.GetSize(h,w);
	for (int i=0;i<h;i++)
	{
		for (int j=0;j<w;j++)
		{
			comp.SetElemRe(i,j,exp(comp.GetElemRe(i,j))*cos(comp.GetElemIm(i,j)));
			comp.SetElemIm(i,j,exp(comp.GetElemRe(i,j))*sin(comp.GetElemIm(i,j)));
		}
	}
}
void cvmMatDotComplex(CvMat* src,CvComplex& comp)
{
	assert(src && CV_IS_MAT(src) && !comp.IsEmpty());
	CvSize size=cvGetSize(src);
	for (int i=0;i<size.height;i++)
	{
		for (int j=0;j<size.width;j++)
		{
			comp.SetElemRe(i,j,comp.GetElemRe(i,j)*cvmGet(src,i,j));
			comp.SetElemIm(i,j,comp.GetElemIm(i,j)*cvmGet(src,i,j));
		}
	}

}
void cvmExpi(CvMat* src,CvComplex& result)
{
	assert(src && CV_IS_MAT(src));
	CvSize size=cvGetSize(src);
	result.Init(size.height,size.width);
	for (int i=0;i<size.height;i++)
	{
		for (int j=0;j<size.width;j++)
		{
			result.SetElemRe(i,j,cos(cvmGet(src,i,j)));
			result.SetElemIm(i,j,sin(cvmGet(src,i,j)));
		}
	}
	return ;
}

BOOL cvmDFT(CvMat *src,CvComplex& dfft,int flags)
{
	assert(src);
	int i,j;
	int M,N;
	CvSize size=cvGetSize(src);
	if (size.height%2!=0 || size.width%2!=0)
		return FALSE;
	cvDFT(src,src,flags);
	size=cvGetSize(src);
	M=size.height;	N=size.width;
	CvComplex64f comp;
	int u0=M/2,v0=N/2;//ԭ������
	dfft.Init(M,N);
	switch(flags)
	{
	case CV_DXT_FORWARD:
		//��һ��
		for(i=0;i<M;i++)
		{
			if (i==0)
				dfft.SetElemRe(0,0,((double*)src->data.ptr)[0]);
			else
				if (i%2==1)
					dfft.SetElemRe((i+1)/2,0,((double*)(src->data.ptr+i*src->step))[0]);
				else
					dfft.SetElemIm(i/2,0,((double*)(src->data.ptr+i*src->step))[0]);
		}
		//�м��
		for (i=0;i<M-1;i++)
		{
			for (j=1;j<N-1;j++)
			{
				if (j%2==1)
					dfft.SetElemRe(i,(j+1)/2,((double*)(src->data.ptr+i*src->step))[j]);
				else
					dfft.SetElemIm(i,j/2,((double*)(src->data.ptr+i*src->step))[j]);

			}
		}
		//M-1��
		//if (M%2==0)
		{
			dfft.SetElemRe(M/2,0,((double*)(src->data.ptr+(M-1)*src->step))[0]);
			//dfft.SetElemIm(M/2,0,0.0);
			for (j=1;j<N-1;j++)
			{
				if (j%2==1)
					dfft.SetElemRe((M-1),(j+1)/2,((double*)(src->data.ptr+(M-1)*src->step))[j]);
				else
					dfft.SetElemIm((M-1),j/2,((double*)(src->data.ptr+(M-1)*src->step))[j]);
			}
		}
		//N/2+1��
		//if (N%2==0)
		{
			dfft.SetElemRe(0,N/2,((double*)(src->data.ptr+0*src->step))[N-1]);
			//dfft.SetElemIm(0,N/2,0.0);
			for (i=1;i<M-1;i++)
			{
				if (i%2==1)
					dfft.SetElemRe((i+1)/2,N/2,((double*)(src->data.ptr+i*src->step))[N-1]);
				else
					dfft.SetElemIm(i/2,N/2,((double*)(src->data.ptr+i*src->step))[N-1]);
			}
		}
		//if (M%2==0 && N%2==0)
		{
			//dfft.SetElemRe((M-1),N/2,0.0);
			dfft.SetElemIm((M-1),N/2,((double*)(src->data.ptr+(M-1)*src->step))[N-1]);
		}
		//���ݸ���Ҷ�任�ĶԳ��Խ��й���ԭ��ľ�����
		//��һ���°벿��
		for(i=u0+1;i<M;i++)
		{
			dfft.GetElem(2*u0-i,0,comp);
			dfft.SetElem(i,0,comp);
		}
		//��һ���Ұ벿��
		for(j=v0+1;j<N;j++)
		{
			dfft.GetElem(0,2*v0-j,comp);
			dfft.SetElem(0,j,comp);
		}
		//���ྵ��
		for(i=1;i<M;i++)
		{
			for (j=N/2+1;j<N;j++)
			{
				dfft.GetElem(2*u0-i,2*v0-j,comp);
				dfft.SetElem(i,j,comp);
			}
		}
		break;
	case CV_DXT_INV_SCALE:
		for (i=0;i<M;i++)
		{
			for (j=0;j<N;j++)
				dfft.SetElemRe(i,j,((double*)(src->data.ptr+i*src->step))[j]);
		}
		break;
	case CV_DXT_INVERSE:
		for (i=0;i<M;i++)
		{
			for (j=0;j<N;j++)
				dfft.SetElemRe(i,j,((double*)(src->data.ptr+i*src->step))[j]);
		}
		break;
	}
	return TRUE;
}

CvMat* cvmHomographicFilter(IplImage* src)
{
	assert(src);
	CvSize imgsize=cvGetSize(src);
	int i,j;
	//����4���������Ե���,����H�˲������˲�����
	int n=3;                      
	double D0=0.9;
	double r1=0.4;
	double r2=1.2;
	//log-DFT-H(U,V)-IDFT-EXP
	CvMat srcmat;
	CvMat *srcmatabs=0,*srcmatlog=0,*srcmatangle=0;
	CvMat *imgfft=0;
	CvMat *bfidft=0;
	CvComplex complex;

	//��һ��:ת���ɻҶ�ͼ��
	//ת���ɻҶ�ͼ��
	IplImage *grayimg=cvCreateImage(imgsize,IPL_DEPTH_8U,1);
	cvCvtColor(src,grayimg,CV_RGB2GRAY);
	
	//�ڶ���:ת����˫���ȸ���ͼ��
	IplImage* doubleimg=cvCreateImage(imgsize,IPL_DEPTH_64F,1);	
	cvConvertScale(grayimg,doubleimg,1.0,0);	
	cvReleaseImage(&grayimg);
	
	//������:����ͼ�����
	cvGetMat(doubleimg,&srcmat);

	//���Ĳ�:��������
	srcmatlog=cvCreateMat(imgsize.height,imgsize.width,CV_64FC1);
	cvZero(srcmatlog);
	cvLog(&srcmat,srcmatlog);
	cvReleaseImage(&doubleimg);

	//���岽:��ɢ����Ҷ�任,�����complex��(�Ը�����ʽ�洢)
	if (!cvmDFT(srcmatlog,complex,CV_DXT_FORWARD))
	{
		cvReleaseMat(&srcmatlog);
		return 0;
	}
	//������:��ģ�����
	srcmatabs=cvCreateMat(imgsize.height,imgsize.width,CV_64FC1);
	srcmatangle=cvCreateMat(imgsize.height,imgsize.width,CV_64FC1);
	for (i=0;i<imgsize.height;i++)
	{
		for (j=0;j<imgsize.width;j++)
		{
			cvmSet(srcmatabs,i,j,complex.GetAbs(i,j));
			cvmSet(srcmatangle,i,j,complex.GetAngle(i,j));
		}
	}
	complex.Destroy();
	CvMat* H=cvCreateMat(imgsize.height,imgsize.width,CV_64FC1);
	CvMat* AF=cvCreateMat(imgsize.height,imgsize.width,CV_64FC1);
	cvZero(H);
	cvZero(AF);
	
	//�����˲���,��ѹ����Ƶ����,��ǿ��Ƶ����
	double tmp;
	for (int u=1;u<=imgsize.height;u++)
	{
		for (int v=1;v<=imgsize.width;v++)
		{
			tmp=sqrt((double)(u*u+v*v));
			cvmSet(H,u-1,v-1,r1+r2/(1+0.414*pow(D0/tmp,2*n)));//�˴�H�˲������Ը����Լ���Ҫ����
		}
	}
	//���߲�:����H(u,v)�˲�
	cvMul(srcmatangle,H,AF);
	cvReleaseMat(&srcmatangle);
	cvReleaseMat(&H);
	cvmExpi(AF,complex);
	cvmMatDotComplex(srcmatabs,complex);
	cvReleaseMat(&srcmatabs);
	
	bfidft=cvCreateMat(imgsize.height,imgsize.width,CV_64FC1);
	CvMat* x=cvCreateMat(imgsize.height,imgsize.width,CV_64FC1);
	//������Ҷͼ��װ�����,׼�����任
	bfidft=cvmGetMatFromComplex(complex);
	complex.Destroy();
	
	//���任
	cvmDFT(bfidft,complex,CV_DXT_INV_SCALE);
	cvmExp(complex);
	cvReleaseMat(&bfidft);
	double max=complex.GetMaxAbs();
	for (i=0;i<imgsize.height;i++)
	{
		for (j=0;j<imgsize.width;j++)
		{
			complex.SetElemRe(i,j,complex.GetElemRe(i,j)/max);
			complex.SetElemIm(i,j,complex.GetElemIm(i,j)/max);
		}
	}
	bfidft=cvmGetMatFromComplex(complex,!CV_DXT_INVERSE);
	complex.Destroy();
	return bfidft;
}
