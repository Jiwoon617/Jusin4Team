#include "pch.h"
#include "IOManager.h"


std::wstring IOManager::selectSaveFilePath(void)
{
	OPENFILENAME ofn;
	wchar_t szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
	ofn.lpstrFilter = L"All File(*.*)|*.*||";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;


	wchar_t initialDirectory[260] = { 0 };
	GetCurrentDirectory(MAX_PATH, initialDirectory);
	//PathRemoveFileSpec(initialDirectory);
	//lstrcat(initialDirectory, L"\\Data");
	ofn.lpstrInitialDir = initialDirectory;

	if (GetSaveFileName(&ofn))
	{
		return std::wstring(ofn.lpstrFile);
	}
	return std::wstring();
}

std::wstring IOManager::selectLoadFilePath(void)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	wchar_t szFile[260] = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
	ofn.lpstrFilter = L"All File(*.*)|*.*||";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	wchar_t initialDirectory[260] = { 0 };
	GetCurrentDirectory(MAX_PATH, initialDirectory);
	//PathRemoveFileSpec(initialDirectory);
	//lstrcat(initialDirectory, L"\\Data");
	//PathRemoveExtension(initialDirectory); => 확장자명 제거(shlwapi.h)
	PathRemoveExtension(initialDirectory);
	ofn.lpstrInitialDir = initialDirectory;

	if (GetOpenFileName(&ofn))
	{
		return std::wstring(ofn.lpstrFile);
	}
	return std::wstring();
}

std::wstring IOManager::selectFolderPath(void)
{
	BROWSEINFO bi = { 0 };
	bi.lpfn = NULL;
	bi.lpszTitle = L"Folder Selection";

	wchar_t initialDirectory[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, initialDirectory);
	//PathRemoveFileSpec(initialDirectory);
	//lstrcat(initialDirectory, L"\\Data");
	ITEMIDLIST* pidlRoot = nullptr;
	if (SUCCEEDED(SHParseDisplayName(initialDirectory, NULL, &pidlRoot, 0, NULL)))
		bi.pidlRoot = pidlRoot; // 초기 경로 설정
	else
		bi.pidlRoot = NULL;

	LPITEMIDLIST itemIDList = SHBrowseForFolder(&bi);

	if (itemIDList != 0)
	{
		wchar_t path[MAX_PATH];
		if (!SHGetPathFromIDList(itemIDList, path))
		{
			return std::wstring();
		}

		CoTaskMemFree(itemIDList);
		return std::wstring(path);
	}
	return std::wstring();
}

//쓰는 방법(람다함수)
//HRESULT res = IOManager::save(IOManager::selectSaveFilePath(), [](HANDLE handle)->void{
//		serialize(handle, value); => 클래스에 맞게 쓰면 됨
//		serialize(handle, value); });
HRESULT IOManager::load(const std::wstring& path, std::function<void(HANDLE)> function)
{
	if (path == L"")
		return E_FAIL;


	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return E_FAIL;
	}

	function(hFile);

	CloseHandle(hFile);
	return S_OK;
}

//쓰는 방법(람다함수)
//HRESULT res = IOManager::load(IOManager::selectLoadFilePath(), [](HANDLE handle)->void{
//		deserialize(handle, value);
//		deserialize(handle, value); });
HRESULT IOManager::save(const std::wstring& path, std::function<void(HANDLE)> function)
{
	if (path == L"")
		return E_FAIL;

	HANDLE hFile = CreateFile(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return E_FAIL;
	}

	function(hFile);

	CloseHandle(hFile);
	return S_OK;
}


std::list<std::pair<std::wstring, std::wstring>> IOManager::findAllFiles(const std::wstring& folderPath)
{
	std::list<std::pair< std::wstring, std::wstring>> result = {};
	std::list<std::wstring> folderQueue = { folderPath };
	WIN32_FIND_DATA findData;

	while (!folderQueue.empty())
	{
		std::wstring path = folderQueue.front();
		folderQueue.pop_front();

		HANDLE hFind = FindFirstFile((path + L"\\*.*").c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE)
			continue;

		do
		{
			if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0)
				continue;


			wchar_t fullPath[MAX_PATH];
			PathCombine(fullPath, path.c_str(), findData.cFileName);


			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				folderQueue.push_back(fullPath);
			}
			else
			{
				result.push_back({ findData.cFileName, fullPath });
			}

		}
		while (FindNextFile(hFind, &findData) != 0);

		FindClose(hFind);
	}

	return result;
}
