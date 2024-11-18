// CUnittool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CUnittool.h"
#include "IOManager.h"

// CUnittool 대화 상자

IMPLEMENT_DYNAMIC(CUnittool, CDialog)

CUnittool::CUnittool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CUnittool, pParent)
	, m_strTest(_T(""))
	, m_strResult(_T(""))
	, m_strName(_T(""))
	, baseDamage(0)
	, maxHP(0)
	, m_strFindName(_T(""))
{

}

CUnittool::~CUnittool(void)
{
}

void CUnittool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_JOBNAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_BASEDAMAGE, baseDamage);
	DDX_Text(pDX, IDC_EDIT_MAXHP, maxHP);
	DDX_Text(pDX, IDC_EDIT_SPEED, speed);
	DDX_Text(pDX, IDC_EDIT_DODGE, dodge);
	DDX_Text(pDX, IDC_EDIT_PROTECTION, protection);
	DDX_Text(pDX, IDC_EDIT_ACCURACY, accuracyModifier);
	DDX_Text(pDX, IDC_EDIT_CRITICAL, criticalHitChance);
	DDX_Text(pDX, IDC_EDIT_VIRTUE, virtueChance);


	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}


BEGIN_MESSAGE_MAP(CUnittool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnittool::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnittool::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnittool::OnAdd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON3, &CUnittool::OnDelete)
	ON_EN_CHANGE(IDC_EDIT6, &CUnittool::OnSearch)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnittool::OnSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnittool::OnLoad)
END_MESSAGE_MAP()

void CUnittool::OnBnClickedButton1()
{
	UpdateData(TRUE);			// 다이얼로그 박스로부터 입력된 값들을 얻어옴

	m_strResult = m_strTest;

	UpdateData(FALSE);			// 변수에 저장된 값들을 다이얼로그 박스에 반영
}


void CUnittool::OnListBox()
{
	UpdateData(TRUE);

	CString		strFindName;

	// GetCurSel : 리스트 박스 목록 중 선택된 셀의 인덱스를 반환
	int	iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : 인덱스에 해당하는 셀의 문자열을 리스트 박스로부터 얻어옴
	m_ListBox.GetText(iIndex, strFindName);

	auto	iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	m_strName = iter->second->jobName.c_str();
	maxHP = iter->second->maxHP;
	baseDamage = iter->second->baseDamage;
	speed = iter->second->speed;
	dodge = iter->second->dodge;
	protection = iter->second->protection;
	accuracyModifier = iter->second->accuracyModifier;
	criticalHitChance = iter->second->criticalHitChance;
	virtueChance = iter->second->virtueChance;



	UpdateData(FALSE);
}


void CUnittool::OnAdd()
{
	UpdateData(TRUE);

	if (m_mapUnitData.find(m_strName) != m_mapUnitData.end())
	{
		AfxMessageBox(L"Duplicate name! Add failed.");
		return;
	}

	UNITDATA* pUnit = new UNITDATA();

	pUnit->jobName = std::wstring(m_strName.GetString());
	pUnit->maxHP = maxHP;
	pUnit->baseDamage = baseDamage;
	pUnit->speed = speed;
	pUnit->dodge = dodge;
	pUnit->protection = protection;
	pUnit->accuracyModifier = accuracyModifier;
	pUnit->criticalHitChance = criticalHitChance;
	pUnit->virtueChance = virtueChance;

	m_mapUnitData.insert({ m_strName, pUnit });
	m_ListBox.AddString(m_strName);

	UpdateData(FALSE);
}

void CUnittool::OnSearch()
{
	UpdateData(TRUE);

	auto& iter = m_mapUnitData.find(m_strFindName);

	if (iter == m_mapUnitData.end())
		return;

	// FindString : 0번 인덱스로 부터 해당 문자열의 목록을 찾아 해당 셀 인덱스를 반환
	int iIndex = m_ListBox.FindString(0, m_strFindName);

	if (LB_ERR == iIndex)
		return;

	m_ListBox.SetCurSel(iIndex);

	m_strName = iter->second->jobName.c_str();
	maxHP = iter->second->maxHP;
	baseDamage = iter->second->baseDamage;
	speed = iter->second->speed;
	dodge = iter->second->dodge;
	protection = iter->second->protection;
	accuracyModifier = iter->second->accuracyModifier;
	criticalHitChance = iter->second->criticalHitChance;
	virtueChance = iter->second->virtueChance;

	UpdateData(FALSE);

}

void CUnittool::OnDelete()
{
	UpdateData(TRUE);

	CString	strFindName = L"";

	int		iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	m_ListBox.GetText(iIndex, strFindName);

	auto& iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	Safe_Delete(iter->second);
	m_mapUnitData.erase(strFindName);

	//DeleteString : 해당 목록의 문자열을 삭제
	m_ListBox.DeleteString(iIndex);


	UpdateData(FALSE);
}

void CUnittool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), [](auto& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	});

	m_mapUnitData.clear();
}


BOOL CUnittool::OnInitDialog()
{
	CDialog::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}





void CUnittool::OnSave()
{
	IOManager::save(IOManager::selectSaveFilePath(), [&](HANDLE handle)->void
		{
			IOManager::serialize(handle, (int)m_mapUnitData.size());

			for (auto& unit : m_mapUnitData)
			{
				IOManager::serialize(handle, std::wstring(unit.first));
				IOManager::serialize(handle, unit.second->accuracyModifier);
				IOManager::serialize(handle, unit.second->baseDamage);
				IOManager::serialize(handle, unit.second->criticalHitChance);
				IOManager::serialize(handle, unit.second->dodge);
				IOManager::serialize(handle, unit.second->jobName);
				IOManager::serialize(handle, unit.second->maxHP);

				IOManager::serialize(handle, unit.second->jobName);

				IOManager::serialize(handle, unit.second->protection);
				IOManager::serialize(handle, unit.second->speed);
				IOManager::serialize(handle, unit.second->virtueChance);
			}
		});

}


void CUnittool::OnLoad()
{
	UpdateData(TRUE);

	m_ListBox.ResetContent();

	for (auto& item : m_mapUnitData)
	{
		delete item.second;
	}
	m_mapUnitData.clear();

	IOManager::load(IOManager::selectLoadFilePath(), [&](HANDLE handle)->void
		{
			int unitCount = 0;
			IOManager::deserialize(handle, unitCount);

			for (int i = 0; i < unitCount; i++)
			{
				std::wstring key;
				IOManager::deserialize(handle, key);
				UNITDATA* loadedUnit = new UNITDATA();
				ZeroMemory(loadedUnit, sizeof(UNITDATA));
				IOManager::deserialize(handle, loadedUnit->accuracyModifier);
				IOManager::deserialize(handle, loadedUnit->baseDamage);
				IOManager::deserialize(handle, loadedUnit->criticalHitChance);
				IOManager::deserialize(handle, loadedUnit->dodge);
				IOManager::deserialize(handle, loadedUnit->jobName);
				IOManager::deserialize(handle, loadedUnit->maxHP);

				IOManager::deserialize(handle, loadedUnit->jobName);

				IOManager::deserialize(handle, loadedUnit->protection);
				IOManager::deserialize(handle, loadedUnit->speed);
				IOManager::deserialize(handle, loadedUnit->virtueChance);
				m_mapUnitData[key.c_str()] = loadedUnit;
			}
		});


	for (auto& item : m_mapUnitData)
	{
		m_ListBox.AddString(item.first);
	}


	UpdateData(FALSE);
}
