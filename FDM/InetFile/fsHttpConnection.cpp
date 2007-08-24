/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "fsHttpConnection.h"
#include "fsInternetSession.h"        

fsHttpConnection::fsHttpConnection()
{

}

fsHttpConnection::~fsHttpConnection()
{

}

fsInternetResult fsHttpConnection::Connect(LPCSTR pszServer, LPCSTR pszUser, LPCSTR pszPassword, INTERNET_PORT nPort)
{
	if (!m_pSession)
		return IR_NOTINITIALIZED;

	HINTERNET hSession = m_pSession->GetHandle ();

	if (!hSession)
		return IR_NOTINITIALIZED;

	if (m_hServer)
		InternetCloseHandle (m_hServer);

	m_hServer = InternetConnect (hSession, pszServer, nPort,
					pszUser, pszPassword, INTERNET_SERVICE_HTTP, 0, NULL);

	if (!m_hServer)
		return fsWinInetErrorToIR ();

	m_strServer = pszServer;
	m_uPort = nPort;

	return IR_SUCCESS;
}
