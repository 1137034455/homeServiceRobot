

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Mar 11 14:34:50 2016
 */
/* Compiler settings for SRecoManager.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_SRecoManagerLib,0xC67F3774,0x376F,0x46D8,0xA6,0xD7,0x3F,0x62,0x75,0xD5,0x68,0x37);


MIDL_DEFINE_GUID(IID, DIID__DSRecoManager,0xE06F5A13,0xE5B4,0x47BB,0xA6,0xAA,0x2C,0x34,0xC4,0xF5,0x20,0x91);


MIDL_DEFINE_GUID(IID, DIID__DSRecoManagerEvents,0x489CD01C,0x3062,0x4B02,0x9C,0x30,0x7E,0x13,0x95,0x40,0x7B,0x7F);


MIDL_DEFINE_GUID(CLSID, CLSID_SRecoManager,0xE6A83528,0x143B,0x4C33,0xB7,0xED,0x53,0xC6,0x39,0x11,0xCC,0xCF);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



