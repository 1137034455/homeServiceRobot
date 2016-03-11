/****************************************************************************************\
*                                  CPerson class                                         *
\****************************************************************************************/
#include "stdafx.h"
#include "Person.h"
#include "CvvImage.cpp"

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
	
	for( int i = 0; i < num_img; i++ )
	{
		POSITION pos = m_imgs.FindIndex(i);
		IplImage* src = m_imgs.GetAt(pos)->GetImage().GetImage();

		//光照归一化处理
		m_light.LightNormalization(src, src, IlluminationThreshold);

		IplImage* ipl=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);

		cvCvtColor(src,ipl,CV_RGB2GRAY);

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