

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Mar 11 15:46:01 2016
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __SRecoManageridl_h__
#define __SRecoManageridl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DSRecoManager_FWD_DEFINED__
#define ___DSRecoManager_FWD_DEFINED__
typedef interface _DSRecoManager _DSRecoManager;
#endif 	/* ___DSRecoManager_FWD_DEFINED__ */


#ifndef ___DSRecoManagerEvents_FWD_DEFINED__
#define ___DSRecoManagerEvents_FWD_DEFINED__
typedef interface _DSRecoManagerEvents _DSRecoManagerEvents;
#endif 	/* ___DSRecoManagerEvents_FWD_DEFINED__ */


#ifndef __SRecoManager_FWD_DEFINED__
#define __SRecoManager_FWD_DEFINED__

#ifdef __cplusplus
typedef class SRecoManager SRecoManager;
#else
typedef struct SRecoManager SRecoManager;
#endif /* __cplusplus */

#endif 	/* __SRecoManager_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __SRecoManagerLib_LIBRARY_DEFINED__
#define __SRecoManagerLib_LIBRARY_DEFINED__

/* library SRecoManagerLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_SRecoManagerLib;

#ifndef ___DSRecoManager_DISPINTERFACE_DEFINED__
#define ___DSRecoManager_DISPINTERFACE_DEFINED__

/* dispinterface _DSRecoManager */
/* [uuid] */ 


EXTERN_C const IID DIID__DSRecoManager;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E06F5A13-E5B4-47BB-A6AA-2C34C4F52091")
    _DSRecoManager : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSRecoManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSRecoManager * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSRecoManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSRecoManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSRecoManager * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSRecoManager * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSRecoManager * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSRecoManager * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DSRecoManagerVtbl;

    interface _DSRecoManager
    {
        CONST_VTBL struct _DSRecoManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSRecoManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSRecoManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSRecoManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSRecoManager_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSRecoManager_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSRecoManager_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSRecoManager_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSRecoManager_DISPINTERFACE_DEFINED__ */


#ifndef ___DSRecoManagerEvents_DISPINTERFACE_DEFINED__
#define ___DSRecoManagerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DSRecoManagerEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DSRecoManagerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("489CD01C-3062-4B02-9C30-7E1395407B7F")
    _DSRecoManagerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSRecoManagerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSRecoManagerEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSRecoManagerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSRecoManagerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSRecoManagerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSRecoManagerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSRecoManagerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSRecoManagerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DSRecoManagerEventsVtbl;

    interface _DSRecoManagerEvents
    {
        CONST_VTBL struct _DSRecoManagerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSRecoManagerEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSRecoManagerEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSRecoManagerEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSRecoManagerEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSRecoManagerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSRecoManagerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSRecoManagerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSRecoManagerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SRecoManager;

#ifdef __cplusplus

class DECLSPEC_UUID("E6A83528-143B-4C33-B7ED-53C63911CCCF")
SRecoManager;
#endif
#endif /* __SRecoManagerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


