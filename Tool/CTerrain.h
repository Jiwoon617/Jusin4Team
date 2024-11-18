#pragma once

#include "Include.h"

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	vector<TILE*>& Get_VecTile() { return m_vecTile; }

public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

public:
	void			Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID);
	int				Get_TileIndex(const D3DXVECTOR3& vPos);
	bool			Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool			Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

public:
	void	Initialize();
	void	Update();
	void	Mini_Render();
	void	Render();
	void	Release();

private:
	vector<TILE*>		m_vecTile;
	CToolView*			m_pMainView;

};

// Invalidate(FALSE) : ȭ���� �����ϴ� �Լ�

// ������ �������� �̿��Ͽ� ���콺�� Ŭ���� Ÿ���� �̹����� �����ض�.

// y = ax + b

// a : ���� (y ������ / x ������)
// b : y ���� (x���� 0�� ����� y ��)

// 2. �̴ϸ��� ����϶�(���콺 ��ŷ ����)
// - ��ü Ÿ���� �̴ϸ� ȭ�鿡 ��� �� ��

// CMiniView* pMiniView->Invalidate(FALSE);