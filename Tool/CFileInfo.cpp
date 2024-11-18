#include "pch.h"
#include "CFileInfo.h"

CFileInfo::CFileInfo()
{
}

CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRealtivePath(CString strFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// PathRelativePathTo : szCurDirPath에서 strFullPath로 가는 상대 경로를 구해서
	// 	szRelativePath에 저장하는 함수

	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

int CFileInfo::DirFileCount(const wstring& wstrPath)
{
	wstring		strFilePath = wstrPath + L"\\*.*";

	CFileFind			Find;
	BOOL	bContinue = Find.FindFile(strFilePath.c_str());

	int iFileCnt(0);

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iFileCnt;
	}

	return iFileCnt;
}

void CFileInfo::DirInfoExraction(const wstring& wstrPath, list<IMGPATH*>& rInfoList)
{
	wstring		strFilePath = wstrPath + L"\\*.*";

	CFileFind			Find;
	BOOL	bContinue = Find.FindFile(strFilePath.c_str());

	int iFileCnt(0);

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			DirInfoExraction(wstring(Find.GetFilePath()), rInfoList);
		}

		else
		{
			if (Find.IsSystem())
				continue;

			IMGPATH* pImgPath = new IMGPATH;
			TCHAR	szPathBuf[MAX_STR] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());

			PathRemoveFileSpec(szPathBuf);

			pImgPath->iCount = DirFileCount(szPathBuf);

			// GetFileTitle : 확장자 명을 제외한 온전한 파일의 이름만 얻어오는 함수
			wstring	wstrTextureName = Find.GetFileTitle().GetString();

			// substr(시작, 끝) : 시작에서 끝에 해당하는 문자열을 잘라내어 얻어옴
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

			PathCombine(szPathBuf, szPathBuf, wstrTextureName.c_str());

			pImgPath->wstrPath = ConvertRealtivePath(szPathBuf);

		//..\Frame150\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack%d.png

			PathRemoveFileSpec(szPathBuf);

			//..\Frame150\Texture\Stage\Effect\BossMultiAttack\
			// PathFindFileName : 파일명을 찾거나 맨 마지막 폴더명을 찾아냄
			pImgPath->wstrStateKey = PathFindFileName(szPathBuf);

			PathRemoveFileSpec(szPathBuf);
			pImgPath->wstrObjKey = PathFindFileName(szPathBuf);

			rInfoList.push_back(pImgPath);
			bContinue = 0;
		}	

	}

}
