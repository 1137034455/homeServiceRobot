#include "stdafx.h"
#include "FaceRecognise.h"
inline CFaceRecogniseApp* GetApp()
{
	return (CFaceRecogniseApp*)::AfxGetApp();
}

inline int  ShowErrorMessage(CString strErr,BOOL bWarning = FALSE)
{
	LONG flag = MB_OKCANCEL;
	if(bWarning)
	{
		return AfxMessageBox(strErr,MB_OKCANCEL | MB_ICONQUESTION);
	}
	else
	{
		return AfxMessageBox(strErr,MB_OK | MB_ICONSTOP);
	}
}