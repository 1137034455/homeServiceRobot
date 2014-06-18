

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Mar 29 09:58:45 2013
 */
/* Compiler settings for FRManager.idl:
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


#ifndef __FRManageridl_h__
#define __FRManageridl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DFRManager_FWD_DEFINED__
#define ___DFRManager_FWD_DEFINED__
typedef interface _DFRManager _DFRManager;
#endif 	/* ___DFRManager_FWD_DEFINED__ */


#ifndef ___DFRManagerEvents_FWD_DEFINED__
#define ___DFRManagerEvents_FWD_DEFINED__
typedef interface _DFRManagerEvents _DFRManagerEvents;
#endif 	/* ___DFRManagerEvents_FWD_DEFINED__ */


#ifndef __FRManager_FWD_DEFINED__
#define __FRManager_FWD_DEFINED__

#ifdef __cplusplus
typedef class FRManager FRManager;
#else
typedef struct FRManager FRManager;
#endif /* __cplusplus */

#endif 	/* __FRManager_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __FRManagerLib_LIBRARY_DEFINED__
#define __FRManagerLib_LIBRARY_DEFINED__

/* library FRManagerLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_FRManagerLib;

#ifndef ___DFRManager_DISPINTERFACE_DEFINED__
#define ___DFRManager_DISPINTERFACE_DEFINED__

/* dispinterface _DFRManager */
/* [uuid] */ 


EXTERN_C const IID DIID__DFRManager;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B986E496-A112-4057-82AD-E6B83888BF05")
    _DFRManager : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DFRManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DFRManager * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DFRManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DFRManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DFRManager * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DFRManager * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DFRManager * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DFRManager * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DFRManagerVtbl;

    interface _DFRManager
    {
        CONST_VTBL struct _DFRManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DFRManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DFRManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DFRManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DFRManager_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DFRManager_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DFRManager_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DFRManager_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DFRManager_DISPINTERFACE_DEFINED__ */


#ifndef ___DFRManagerEvents_DISPINTERFACE_DEFINED__
#define ___DFRManagerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DFRManagerEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DFRManagerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("398C975D-5DAA-4AFA-9B46-43ADDA5C3031")
    _DFRManagerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DFRManagerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DFRManagerEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DFRManagerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DFRManagerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DFRManagerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DFRManagerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DFRManagerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DFRManagerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DFRManagerEventsVtbl;

    interface _DFRManagerEvents
    {
        CONST_VTBL struct _DFRManagerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DFRManagerEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DFRManagerEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DFRManagerEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DFRManagerEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DFRManagerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DFRManagerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DFRManagerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DFRManagerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_FRManager;

#ifdef __cplusplus

class DECLSPEC_UUID("5D2BC97D-04DE-453B-95CB-E5DA90288754")
FRManager;
#endif
#endif /* __FRManagerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


