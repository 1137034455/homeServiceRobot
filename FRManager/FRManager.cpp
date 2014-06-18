// FRManager.cpp : CFRManagerApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "FRManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CFRManagerApp theApp;

const GUID CDECL _tlid = { 0x427C99FE, 0xB836, 0x4964, { 0xB1, 0x61, 0x94, 0x90, 0xDF, 0xAB, 0x75, 0xC7 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CFRManagerApp::InitInstance - DLL 初始化

BOOL CFRManagerApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// CFRManagerApp::ExitInstance - DLL 终止

int CFRManagerApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
