/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#include "windows.h"

void fsIFOnMemoryError ()
{
	if (IDCANCEL == MessageBox (NULL, "Out of memory! Please close some applications and press Retry or Cancel to exit without saving any data", "Out of memory", MB_ICONEXCLAMATION|MB_RETRYCANCEL))
		ExitProcess (0);
}