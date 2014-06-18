// FRManager.cpp : CFRManagerApp �� DLL ע���ʵ�֡�

#include "stdafx.h"
#include "FRManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CFRManagerApp theApp;

const GUID CDECL _tlid = { 0x427C99FE, 0xB836, 0x4964, { 0xB1, 0x61, 0x94, 0x90, 0xDF, 0xAB, 0x75, 0xC7 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CFRManagerApp::InitInstance - DLL ��ʼ��

BOOL CFRManagerApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣
	}

	return bInit;
}



// CFRManagerApp::ExitInstance - DLL ��ֹ

int CFRManagerApp::ExitInstance()
{
	// TODO: �ڴ�������Լ���ģ����ֹ���롣

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - ������ӵ�ϵͳע���

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - �����ϵͳע������Ƴ�

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
