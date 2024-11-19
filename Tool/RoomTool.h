#pragma once
#include "afxdialogex.h"

struct RoomEffect
{
	int effectAmount = 0;
	int effectLength = 0;
	int condAmount = 0;
	std::wstring effectTime;
	std::wstring activeCond;
	std::wstring effectTarget;
	std::wstring effect;
	std::wstring effectName;
};

struct Room
{
	std::wstring roomName;
	std::wstring backImageName;

	std::vector <std::wstring> enemy;
	std::vector<RoomEffect*> roomEffect;
};


// RoomTool 대화 상자

class RoomTool : public CDialog
{
	DECLARE_DYNAMIC(RoomTool)

public:
	RoomTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~RoomTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RoomTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	std::map<CString, RoomEffect*> effects;

	int effectAmount;
	int effectLength;
	int condAmount;
	CString effectTime;
	CString activeCond;
	CString effectTarget;
	CString effect;
	CString effectName;

	CString backImage;

	CString enemy1;
	CString enemy2;
	CString enemy3;
	CString enemy4;

	CImage image;
	CStatic imagePreview;

	CComboBox effectTimeCombo;
	CComboBox activeCondCombo;
	CComboBox effectTargetCombo;
	CComboBox effectCombo;

	CListBox effectList;
	CListBox unitList;
public:
	afx_msg void OnBnClickedEffectAdd();
	afx_msg void OnLbnSelchangeListEffect();
	afx_msg void OnBnClickedRoomSave();
	afx_msg void OnBnClickedRoomLoad();
	afx_msg void OnBnClickedBackimage();
	afx_msg void OnBnClickedUnitLoad();
	CStatic roomImageName;
	afx_msg void OnDestroy();
};
