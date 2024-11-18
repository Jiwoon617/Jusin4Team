#pragma once
#include "Include.h"

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString	ConvertRealtivePath(CString strFullPath);

	static int		DirFileCount(const wstring& wstrPath);
	static void		DirInfoExraction(const wstring& wstrPath, list<IMGPATH*>& rInfoList);

};

