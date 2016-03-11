
#pragma once
#include "Person.h"
#include "PersonImage.h"

class CPerson;
class CPersonImage;

typedef  CList<CPerson*,CPerson*> CPersonList;
typedef  CList<CPersonImage*,CPersonImage*> CPersonImgList;
// globals
static const char* base_signature = "FACE DATABASE\n";
static const char* person_signature = "PERSONAL INFO\n";

CV_INLINE void ConvertNameToFolder( const char* name, char* folder )
{
	int i, j = 0;
	int len = strlen(name);

	for( i = 0; i < len; i++ )
	{
		if( name[i] == ' ' )
		{
			if( j == 0 || folder[j-1] != '.' )
			{
				folder[j++] = '.';
			}
		}
		else
		{
			folder[j++] = name[i];
		}
	}

	folder[j] = '\0';
}


/***************************************************************************************\
Face data base structure:

<root_folder>\
<person_folder_1>\
<image_11>
<image_12>
<image_13>
...
info.txt
<person_folder_2>\
<image_21>
<image_22>
<image_23>
...
info.txt
<person_folder_3>\
<image_21>
<image_22>
<image_23>
...
info.txt
...
<index_file>

( names in <> brackets can be arbitrary ).
e.g.

NNBase\
Abrosimov.Dmirty\
ad_1.bmp
ad_near_window.bmp
ad_smiling.bmp
index.txt
Oblomov.Sergey\
serg.bmp
photo_3.bmp
index.txt
NNBaseIndex.txt

Main base index file contains name of the base followed by list of personal folders.
Format is the following:

line            content
--------------------------------
1           FACE DATABASE           (signature)
2           base name/description
3           <empty line>
4           <person_folder1>
5           <person_folder2>
...                ...

In every personal folder there is a file with predefined name: info.txt
It contains person name followed by list of images for that person.

line            content
--------------------------------
1           PERSONAL INFO           (signature)
2           person name/description
3           <empty line>
4           <image_name1>
5           [<roi1>]
6           <image_name2>
7           [<roi2>]
...          ...

Every image name may be followed by four numbers which are represent coordinates of
top-left corner of the face rectangle in the image and width and height
of the rectnagle, if no roi is specified, the entire image is considered as a face.

lines, started with #, are comments, and skipped when file is readed.

\***************************************************************************************/

CV_INLINE static char* chomp( char* str )
{
	int l = strlen( str );
	if( l > 0 && str[l-1] == '\n' )
	{
		str[l-1] = '\0';
	}
	return str;
}

const int STR_BUF_SIZE = 1000;

CV_INLINE void ExtractDCT( float* src, float* dst, int num_vec, int dst_len )
{
	float* src_ = src+1;
	float* dst_ = dst;

	for( int i = 0; i < num_vec; i++ )
	{
		memcpy( dst_, src_, dst_len * sizeof(float) );
		src_+= dst_len+1;
		dst_+= dst_len;
	}
} 