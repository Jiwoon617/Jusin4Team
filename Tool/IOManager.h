
class IOManager
{
private:
	IOManager() = default;
	~IOManager() = default;
public:
	static std::wstring selectSaveFilePath();
	static std::wstring selectLoadFilePath();
	static std::wstring selectFolderPath();

	static HRESULT load(const std::wstring& path, std::function<void(HANDLE)> function);
	static HRESULT save(const std::wstring& path, std::function<void(HANDLE)> function);


	template <typename T>
	static HRESULT serialize(HANDLE hFile, const T& value)
	{
		static_assert(!std::is_same<T, char*>::value, "문자열은 사용할 수 없습니다.");
		if (!WriteFile(hFile, &value, sizeof(T), NULL, NULL))
			return E_FAIL;

		return S_OK;
	}
	template <>
	static HRESULT serialize(HANDLE hFile, const CString& value)
	{
		int length = value.GetLength();
		if (!WriteFile(hFile, &length, sizeof(int), NULL, NULL))
			return E_FAIL;
		if (!WriteFile(hFile, value.GetString(), length * sizeof(TCHAR), NULL, NULL))
			return E_FAIL;

		return S_OK;
	}
	template <>
	static HRESULT serialize(HANDLE hFile, const std::string& value)
	{
		int length = value.size();
		if (!WriteFile(hFile, &length, sizeof(int), NULL, NULL))
			return E_FAIL;
		if (!WriteFile(hFile, value.c_str(), length, NULL, NULL))
			return E_FAIL;

		return S_OK;
	}
	template <>
	static HRESULT serialize(HANDLE hFile, const std::wstring& value)
	{
		int length = value.size();
		if (!WriteFile(hFile, &length, sizeof(int), NULL, NULL))
			return E_FAIL;
		if (!WriteFile(hFile, value.c_str(), length * sizeof(wchar_t), NULL, NULL))
			return E_FAIL;

		return S_OK;
	}

	template <typename T>
	static HRESULT deserialize(HANDLE hFile, T& value)
	{
		static_assert(!std::is_same<T, char*>::value, "문자열은 사용할 수 없습니다.");
		DWORD readAmount = 0;
		if (!ReadFile(hFile, &value, sizeof(T), &readAmount, NULL))
			return E_FAIL;
		if (readAmount == 0) return E_FAIL;

		return S_OK;
	}

	template <>
	static HRESULT deserialize(HANDLE hFile, CString& value)
	{
		int length;
		if (!ReadFile(hFile, &length, sizeof(int), NULL, NULL))
			return E_FAIL;

		if (length < 0) return E_FAIL;
		if (length == 0) return S_OK;

		if (!ReadFile(hFile, value.GetBuffer(length), length * sizeof(TCHAR), NULL, NULL))
			return E_FAIL;
		value.ReleaseBuffer(length);

		return S_OK;
	}
	template <>
	static HRESULT deserialize(HANDLE hFile, std::string& value)
	{
		int length;
		if (!ReadFile(hFile, &length, sizeof(int), NULL, NULL))
			return E_FAIL;

		if (length < 0) return E_FAIL;
		if (length == 0) return S_OK;

		value.resize(length);
		if (!ReadFile(hFile, &value[0], length, NULL, NULL))
			return E_FAIL;

		return S_OK;
	}
	template <>
	static HRESULT deserialize(HANDLE hFile, std::wstring& value)
	{
		int length;
		if (!ReadFile(hFile, &length, sizeof(int), NULL, NULL))
			return E_FAIL;
		if (length < 0) return E_FAIL;
		if (length == 0) return S_OK;

		value.resize(length);
		if (!ReadFile(hFile, &value[0], length * sizeof(wchar_t), NULL, NULL))
			return E_FAIL;

		return S_OK;

		//#include <tchar.h> // tchar로 쓰는 방법
		//	TCHAR* buffer = new TCHAR[length + 1];  // 널 문자 공간을 추가로 확보
		//	if (ReadFile(hFile, buffer, length * sizeof(TCHAR), NULL, NULL))
		//	{
		//		buffer[length] = _T('\0');
		//		value = std::to_string(*buffer); => std::string용
		// 	    CString value = buffer; => cstring용
		//	}
		//	delete[] buffer;
	}

private:
};
