#include "pch.h"
#include "CTextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}

CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO* CTextureMgr::Get_Texture(const TCHAR* pObjKey, const TCHAR* pStateKey, const int& iCount)
{
	auto	iter = find_if(m_mapTexture.begin(), m_mapTexture.end(), [&](auto& MyPair)->bool
		{
			if (pObjKey == MyPair.first)
				return true;

			return false;
		});

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second->Get_Texture(pStateKey, iCount);
}

HRESULT CTextureMgr::Insert_Texture(const TCHAR* pFilePath, TEXTYPE eType, const TCHAR* pObjKey, const TCHAR* pStateKey, const int& iCount)
{
	auto	iter = find_if(m_mapTexture.begin(), m_mapTexture.end(),
		[&](auto& MyPair)->bool
		{
			if (pObjKey == MyPair.first)
				return true;

			return false;
		});

	if (iter == m_mapTexture.end())
	{
		CTexture* pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->Insert_Texture(pFilePath, pStateKey, iCount)))
		{
			ERR_MSG(pFilePath);
			return E_FAIL;
		}

		m_mapTexture.insert({ pObjKey, pTexture });
	}

	return S_OK;
}

void CTextureMgr::Release()
{
	for_each(m_mapTexture.begin(), m_mapTexture.end(), [](auto& MyPair)
		{
			if (MyPair.second)
			{
				delete MyPair.second;
				MyPair.second = nullptr;
			}
		});

	m_mapTexture.clear();
}
