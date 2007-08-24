/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#ifndef __IARCHIVE_H
#define __IARCHIVE_H

#include "IStream.h"
#include "IProgress.h"
#include "PropID.h"  

#define ARCHIVE_INTERFACE_SUB(i, base,  x) \
DEFINE_GUID(IID_ ## i, \
0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x06, 0x00, x, 0x00, 0x00); \
struct i: public base

#define ARCHIVE_INTERFACE(i, x) ARCHIVE_INTERFACE_SUB(i, IUnknown, x)

namespace NFileTimeType
{
  enum EEnum
  {
    kWindows,
    kUnix,
    kDOS
  };
}

namespace NArchive
{
  enum 
  {
    kName = 0,
    kClassID,
    kExtension,
    kAddExtension,
    kUpdate,
    kKeepName,
    kStartSignature,
    kFinishSignature,
    kAssociate
  };

  namespace NExtract
  {
    namespace NAskMode
    {
      enum 
      {
        kExtract = 0,
        kTest,
        kSkip,
      };
    }
    namespace NOperationResult
    {
      enum 
      {
        kOK = 0,
        kUnSupportedMethod,
        kDataError,
        kCRCError,
      };
    }
  }
  namespace NUpdate
  {
    namespace NOperationResult
    {
      enum 
      {
        kOK = 0,
        kError,
      };
    }
  }
}

ARCHIVE_INTERFACE(IArchiveOpenCallback, 0x10)
{
  STDMETHOD(SetTotal)(const UInt64 *files, const UInt64 *bytes) PURE;
  STDMETHOD(SetCompleted)(const UInt64 *files, const UInt64 *bytes) PURE;
};  

ARCHIVE_INTERFACE_SUB(IArchiveExtractCallback, IProgress, 0x20)
{
  STDMETHOD(GetStream)(UInt32 index, ISequentialOutStream **outStream, 
      Int32 askExtractMode) PURE;
  
  STDMETHOD(PrepareOperation)(Int32 askExtractMode) PURE;
  STDMETHOD(SetOperationResult)(Int32 resultEOperationResult) PURE;
};  

ARCHIVE_INTERFACE(IArchiveOpenVolumeCallback, 0x30)
{
  STDMETHOD(GetProperty)(PROPID propID, PROPVARIANT *value) PURE;
  STDMETHOD(GetStream)(const wchar_t *name, IInStream **inStream) PURE;
};  

ARCHIVE_INTERFACE(IInArchiveGetStream, 0x40)
{
  STDMETHOD(GetStream)(UInt32 index, ISequentialInStream **stream) PURE;  
};  

ARCHIVE_INTERFACE(IArchiveOpenSetSubArchiveName, 0x50)
{
  STDMETHOD(SetSubArchiveName)(const wchar_t *name) PURE;
};  

ARCHIVE_INTERFACE(IInArchive, 0x60)
{
  STDMETHOD(Open)(IInStream *stream, const UInt64 *maxCheckStartPosition,
      IArchiveOpenCallback *openArchiveCallback) PURE;  
  STDMETHOD(Close)() PURE;  
  STDMETHOD(GetNumberOfItems)(UInt32 *numItems) PURE;  
  STDMETHOD(GetProperty)(UInt32 index, PROPID propID, PROPVARIANT *value) PURE;
  STDMETHOD(Extract)(const UInt32* indices, UInt32 numItems, 
      Int32 testMode, IArchiveExtractCallback *extractCallback) PURE;
  
  
  

  STDMETHOD(GetArchiveProperty)(PROPID propID, PROPVARIANT *value) PURE;

  STDMETHOD(GetNumberOfProperties)(UInt32 *numProperties) PURE;  
  STDMETHOD(GetPropertyInfo)(UInt32 index,     
      BSTR *name, PROPID *propID, VARTYPE *varType) PURE;

  STDMETHOD(GetNumberOfArchiveProperties)(UInt32 *numProperties) PURE;  
  STDMETHOD(GetArchivePropertyInfo)(UInt32 index,     
      BSTR *name, PROPID *propID, VARTYPE *varType) PURE;
};  

ARCHIVE_INTERFACE_SUB(IArchiveUpdateCallback, IProgress, 0x80)
{
  STDMETHOD(GetUpdateItemInfo)(UInt32 index, 
      Int32 *newData, 
      Int32 *newProperties, 
      UInt32 *indexInArchive 
      ) PURE;
  STDMETHOD(GetProperty)(UInt32 index, PROPID propID, PROPVARIANT *value) PURE;
  STDMETHOD(GetStream)(UInt32 index, ISequentialInStream **inStream) PURE;
  STDMETHOD(SetOperationResult)(Int32 operationResult) PURE;
};  

ARCHIVE_INTERFACE_SUB(IArchiveUpdateCallback2, IArchiveUpdateCallback, 0x82)
{
  STDMETHOD(GetVolumeSize)(UInt32 index, UInt64 *size) PURE;
  STDMETHOD(GetVolumeStream)(UInt32 index, ISequentialOutStream **volumeStream) PURE;
};  

ARCHIVE_INTERFACE(IOutArchive, 0xA0)
{
  STDMETHOD(UpdateItems)(ISequentialOutStream *outStream, UInt32 numItems,
      IArchiveUpdateCallback *updateCallback) PURE;
  STDMETHOD(GetFileTimeType)(UInt32 *type) PURE;  
};  

ARCHIVE_INTERFACE(ISetProperties, 0x03)
{
  STDMETHOD(SetProperties)(const wchar_t **names, const PROPVARIANT *values, Int32 numProperties) PURE;
};  

#endif
