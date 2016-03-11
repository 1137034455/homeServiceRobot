#include "stdafx.h"


inline void FileRead(CFile& file,CString& str)
{
	HLOCAL hMem;
	char* pbyte;
	LONG len;
	len=file.GetLength();
	hMem=LocalAlloc(LHND,len+1);
	if(hMem==NULL)
	{
		AfxMessageBox("内存分配失败！");
		return ;
	}
	pbyte=(char*)LocalLock(hMem);
	memset(pbyte,0,len+1);
	file.Read(pbyte,len);
	str=pbyte;
	LocalUnlock(pbyte);
	LocalFree(hMem);
}

inline void WriteXML(CString path,CString goods)
{
	CFile file;
	if(!file.Open(path,CFile::modeRead))
	{
		AfxMessageBox("文件打开失败！");
		return;
	}
	CString str;

	FileRead(file,str);
	if (str.Find(goods)!=-1)
	{
		return;
	}
	int end;
	int len=str.GetLength();
	end=str.Find("</DEFINE>",0);
	CString str_left=str.Left(end);

	CString str_right=str.Right(len-end);

	CString ID1="   <ID NAME=\"CMD_";
	CString ID2="";
	CString ID3="\" VAL=\"";
	CString ID4="";
	CString ID5="\" />\r\n";

	int sel=-3;
	int e;
	while(sel!=-1)
	{
		e=sel;
		sel=str.Find("<ID",sel=sel+3);
	}

	CString last_str=str.Mid(e,end-e);
	//MessageBox(last_str);
	int i;
	CString int1="";
	CString int2="";
	int k=last_str.Find("VAL",0);
	for(i=0;i<k;i++)
	{
		CString s=last_str.Mid(i,1);
		//char ss1[1]=s.GetBuffer(1);
		if (s>='0'&&s<='9')
		{
			int1+=s;
		}
	}
	for(i=k;i<last_str.GetLength();i++)
	{
		CString s=last_str.Mid(i,1);
		if (s>='0'&&s<='9')
		{
			int2+=s;
		}
	}
	int a1=atoi(int1.GetBuffer(int1.GetLength()));
	int a2=atoi(int2.GetBuffer(int2.GetLength()));
	a1++;
	a2++;
	ID2.Format("%d",a1);
	ID4.Format("%d",a2);
	//CString addID="   <ID NAME=\"CMD_10004\" VAL=\"10004\" />\r\n";
	CString addID;
	addID=ID1+ID2+ID3+ID4+ID5;

	str=str_left+addID+str_right;

	int end2;
	int len2=str.GetLength();
	end2=str.Find("</GRAMMAR>",0);
	CString str_left2=str.Left(end2);

	CString str_right2=str.Right(len2-end2);

	//CString addOrder="<RULE ID=\"CMD_10005\" TOPLEVEL=\"ACTIVE\">\r\n<PHRASE>李智博</PHRASE>\r\n</RULE>\r\n";
	CString order1="<RULE ID=\"CMD_";
	CString order2=ID2;
	CString order3="\" TOPLEVEL=\"ACTIVE\">\r\n<PHRASE>";
	CString order4=goods;
	CString order5="</PHRASE>\r\n</RULE>\r\n";

	CString addOrder=order1+order2+order3+order4+order5;

	str=str_left2+addOrder+str_right2;

	file.Close();

	if(!file.Open(path,CFile::modeWrite))
	{
		AfxMessageBox("写入文件失败！");
	}
	file.Write(str,str.GetLength());
}