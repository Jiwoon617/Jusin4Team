#pragma once

#include "Include.h"
#include "CTextureMgr.h"

class CDevice;
class CMainGame
{
public:
	HRESULT		Initialize(void);
	void		Update(void);
	void		Late_Update(void);
	void		Render(void);
	void		Release(void);

private:
	CDevice*			m_pGraphicDev = nullptr;

public:
	CMainGame();
	~CMainGame();
};

// 깊이 우선 탐색과 너비 우선 탐색이 무엇인지 알아와라