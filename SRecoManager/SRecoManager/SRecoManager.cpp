// SRecoManager.cpp : CSRecoManagerApp �� DLL ע���ʵ�֡�

#include "stdafx.h"
#include "SRecoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSRecoManagerApp theApp;

const GUID CDECL _tlid = { 0xC67F3774, 0x376F, 0x46D8, { 0xA6, 0xD7, 0x3F, 0x62, 0x75, 0xD5, 0x68, 0x37 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CSRecoManagerApp::InitInstance - DLL ��ʼ��

BOOL CSRecoManagerApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣
	}

	return bInit;
}



// CSRecoManagerApp::ExitInstance - DLL ��ֹ

int CSRecoManagerApp::ExitInstance()
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
