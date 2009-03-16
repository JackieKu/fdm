/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __MFC_HELP_H_
#define __MFC_HELP_H_  

extern void ConvertBmp32WithAlphaToBmp32WithoutAlpha (CBitmap& bmp, COLORREF clrBk);
extern LPCTSTR fsGetAppDataFolder ();

extern CString fsGetDataFilePath (LPCTSTR pszFile);    

extern CString vmsGetAppFolder (); 

extern void mfcSetForegroundWindow (CWnd *pwnd);

extern LPCTSTR fsGetFumProgramFilesFolder ();

#endif