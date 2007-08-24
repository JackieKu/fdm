/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/          

#if !defined(AFX_VMSDOWNLOADSGROUPSMGR_H__C90C07E2_3147_4BB8_A890_781C75428830__INCLUDED_)
#define AFX_VMSDOWNLOADSGROUPSMGR_H__C90C07E2_3147_4BB8_A890_781C75428830__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "vmsObjectSmartPtr.h"
#include "tree.h"  

#define GRP_OTHER_ID		((UINT)0)

struct vmsDownloadsGroup : public vmsObject
{
	fsString strName;		
	fsString strOutFolder;	
	fsString strExts;		

	
	
	size_t cDownloads;
	
	bool bAboutToBeDeleted;

	
	UINT nId;
};

typedef vmsObjectSmartPtr <vmsDownloadsGroup> vmsDownloadsGroupSmartPtr;

typedef fs::ListTree <vmsDownloadsGroupSmartPtr>* PDLDS_GROUPS_TREE;

#define DLDSGRPSFILE_CURRENT_VERSION		((WORD)1)
#define DLDSGRPSFILE_SIG					"FDM Groups"
struct vmsDownloadsGroupsFileHdr
{
	char szSig [sizeof (DLDSGRPSFILE_SIG) + 1];
	WORD wVer;

	vmsDownloadsGroupsFileHdr ()
	{
		strcpy (szSig, DLDSGRPSFILE_SIG);
		wVer = DLDSGRPSFILE_CURRENT_VERSION;
	}
};  

class vmsDownloadsGroupsMgr  
{
public:
	static LPCSTR GetAudioExts();
	static LPCSTR GetVideoExts ();
	
	
	
	
	
	
	PDLDS_GROUPS_TREE Add (vmsDownloadsGroupSmartPtr pGroup, vmsDownloadsGroupSmartPtr pParentGroup, BOOL bKeepIdAsIs = FALSE);
	PDLDS_GROUPS_TREE Add (vmsDownloadsGroupSmartPtr grp, PDLDS_GROUPS_TREE pParentGroup, BOOL bKeepIdAsIs = FALSE);
	
	void DeleteGroup (vmsDownloadsGroupSmartPtr pGroup);
	
	size_t GetTotalCount();
	
	vmsDownloadsGroupSmartPtr GetGroup (size_t nIndex);
	
	PDLDS_GROUPS_TREE GetGroupsTree();

	
	BOOL SaveToDisk();
	BOOL LoadFromDisk();

	
	vmsDownloadsGroupSmartPtr FindGroup (UINT nId);
	vmsDownloadsGroupSmartPtr FindGroupByName (LPCSTR pszName);
	vmsDownloadsGroupSmartPtr FindGroupByExt (LPCSTR pszExt);
	fsString GetGroupFullName (UINT nId);
	
	PDLDS_GROUPS_TREE FindGroupInTree (vmsDownloadsGroupSmartPtr pGroup);
	
	
	
	
	void SetGroupsRootOutFolder (LPCSTR psz);
	fsString GetGroupsRootOutFolder();
	
	void GetGroupWithSubgroups (vmsDownloadsGroupSmartPtr pGroup, std::vector <vmsDownloadsGroupSmartPtr> &v);

	vmsDownloadsGroupsMgr();
	virtual ~vmsDownloadsGroupsMgr();

protected:
	void RebuildGroupsList (PDLDS_GROUPS_TREE pRoot, std::vector <PDLDS_GROUPS_TREE> &v);
	void RebuildGroupsList();
	BOOL SaveGroupToFile (HANDLE hFile, vmsDownloadsGroupSmartPtr pGroup);
	BOOL SaveGroupsTreeToFile(HANDLE hFile, PDLDS_GROUPS_TREE pRoot);
	vmsDownloadsGroupSmartPtr FindGroupByName (LPCSTR pszName, PDLDS_GROUPS_TREE pRoot);
	BOOL LoadGroupFromFile (HANDLE hFile, vmsDownloadsGroupSmartPtr pGroup);
	BOOL LoadGroupsTreeFromFile (HANDLE hFile, PDLDS_GROUPS_TREE pRoot);
	void SetGroupsRootOutFolder (PDLDS_GROUPS_TREE pRoot, LPCSTR pszFolder);
	void GetSubgroups (PDLDS_GROUPS_TREE pGroup, std::vector <vmsDownloadsGroupSmartPtr> &v);
	UINT m_nGrpNextId; 
	
	
	void CreateDefaultGroups();
	
	fs::ListTree <vmsDownloadsGroupSmartPtr> m_tGroups;
	
	std::vector <PDLDS_GROUPS_TREE> m_vGroups; 
};

#endif 
