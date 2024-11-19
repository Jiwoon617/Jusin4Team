// RoomTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "RoomTool.h"
#include "IOManager.h"
#include "Include.h"


// RoomTool 대화 상자

IMPLEMENT_DYNAMIC(RoomTool, CDialog)

RoomTool::RoomTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_RoomTool, pParent)
	, effectTime(_T(""))
	, activeCond(_T(""))
	, condAmount(0)
	, effectTarget(_T(""))
	, effect(_T(""))
	, effectAmount(0)
	, effectLength(0)
	, effectName(_T(""))
	, enemy1(_T(""))
	, enemy2(_T(""))
	, enemy3(_T(""))
	, enemy4(_T(""))
{

}

RoomTool::~RoomTool()
{
}

void RoomTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EFFTIME, effectTimeCombo);
	DDX_Control(pDX, IDC_COMBO_ACTIVECOND, activeCondCombo	);
	DDX_Control(pDX, IDC_COMBO_EFFTARGET, effectTargetCombo);
	DDX_Control(pDX, IDC_COMBO_EFFECT, effectCombo);
	DDX_CBString(pDX, IDC_COMBO_EFFTIME, effectTime);
	DDX_CBString(pDX, IDC_COMBO_ACTIVECOND, activeCond);
	DDX_Text(pDX, IDC_EDIT_CONDAMOUNT, condAmount);
	DDX_CBString(pDX, IDC_COMBO_EFFTARGET, effectTarget);
	DDX_CBString(pDX, IDC_COMBO_EFFECT, effect);
	DDX_Text(pDX, IDC_EDIT_EFFAMOUNT, effectAmount);
	DDX_Text(pDX, IDC_EDIT_EFFLENGTH, effectLength);
	DDX_Text(pDX, IDC_EDIT_EFFNAME, effectName);
	DDX_Text(pDX, IDC_EDIT_ENEMY1, enemy1);
	DDX_Text(pDX, IDC_EDIT_ENEMY2, enemy2);
	DDX_Text(pDX, IDC_EDIT_ENEMY3, enemy3);
	DDX_Text(pDX, IDC_EDIT_ENEMY4, enemy4);
	DDX_Control(pDX, IDC_LIST_EFFECT, effectList);
	DDX_Control(pDX, IDC_LIST_UNIT, unitList);
	//DDX_Control(pDX, IDLOAD_BACKIMAGE, imagePreview);
	DDX_Control(pDX, IDC_PICTURE_PREVIEW, imagePreview);
	DDX_Control(pDX, IDC_STATIC_ROOMIMAGEFILENAME, roomImageName);
}


BEGIN_MESSAGE_MAP(RoomTool, CDialog)
	ON_BN_CLICKED(IDBUTTON_ROOMADD, &RoomTool::OnBnClickedEffectAdd)
	ON_LBN_SELCHANGE(IDC_LIST_EFFECT, &RoomTool::OnLbnSelchangeListEffect)
	ON_BN_CLICKED(IDSAVE_ROOM, &RoomTool::OnBnClickedRoomSave)
	ON_BN_CLICKED(IDLOAD_ROOM, &RoomTool::OnBnClickedRoomLoad)
	ON_BN_CLICKED(IDLOAD_BACKIMAGE, &RoomTool::OnBnClickedBackimage)
	ON_BN_CLICKED(IDLOAD_UNIT, &RoomTool::OnBnClickedUnitLoad)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// RoomTool 메시지 처리기


void RoomTool::OnBnClickedEffectAdd()
{
	UpdateData(TRUE);
	if (effects.find(effectName) != effects.end())
	{
		AfxMessageBox(L"Duplicated name!");
		return;
	}


	RoomEffect* newEffect = new RoomEffect();

	newEffect->effectAmount = effectAmount;
	newEffect->effectLength = effectLength;
	newEffect->condAmount = condAmount;
	newEffect->effectTime = effectTime;
	newEffect->activeCond = activeCond;
	newEffect->effectTarget = effectTarget;
	newEffect->effect = effect;
	newEffect->effectName = effectName;

	effectList.AddString(effectName);
	effects[effectName] = newEffect;

	UpdateData(FALSE);
}


void RoomTool::OnLbnSelchangeListEffect()
{
	UpdateData(TRUE);

	CString		strFindName;

	int	iIndex = effectList.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	effectList.GetText(iIndex, strFindName);

	auto	iter = effects.find(strFindName);

	if (iter == effects.end())
		return;

	effectAmount = iter->second->effectAmount;
	effectLength = iter->second->effectLength;
	condAmount = iter->second->condAmount;
	effectTime = iter->second->effectTime.c_str();
	activeCond = iter->second->activeCond.c_str();
	effectTarget = iter->second->effectTarget.c_str();
	effect = iter->second->effect.c_str();
	effectName = iter->second->effectName.c_str();

	UpdateData(FALSE);
}


void RoomTool::OnBnClickedRoomSave()
{
	UpdateData(TRUE);

	Room* newRoom = new Room();
	newRoom->backImageName = backImage;

	newRoom->enemy.push_back(enemy1.GetString());
	newRoom->enemy.push_back(enemy2.GetString());
	newRoom->enemy.push_back(enemy3.GetString());
	newRoom->enemy.push_back(enemy4.GetString());

	for (auto& item : effects)
	{
		newRoom->roomEffect.push_back(item.second);
	}


	IOManager::save(IOManager::selectSaveFilePath(), [&](HANDLE handle)->void
		{
			IOManager::serialize(handle, newRoom->roomName);
			IOManager::serialize(handle, newRoom->backImageName);


			IOManager::serialize(handle, (int)newRoom->enemy.size());
			for (auto& item : newRoom->enemy)
			{
				IOManager::serialize(handle, item);
			}

			IOManager::serialize(handle, (int)newRoom->roomEffect.size());
			for (auto& item : newRoom->roomEffect)
			{
				IOManager::serialize(handle, item->effectAmount);
				IOManager::serialize(handle, item->effectLength);
				IOManager::serialize(handle, item->condAmount);
				IOManager::serialize(handle, item->effectTime);
				IOManager::serialize(handle, item->activeCond);
				IOManager::serialize(handle, item->effectTarget);
				IOManager::serialize(handle, item->effect);
				IOManager::serialize(handle, item->effectName);
			}
		});




	UpdateData(FALSE);
}


void RoomTool::OnBnClickedRoomLoad()
{
	UpdateData(TRUE);
	Room* newRoom = new Room();
	
	HRESULT result = IOManager::load(IOManager::selectLoadFilePath(), [&](HANDLE handle)->void
		{
			IOManager::deserialize(handle, newRoom->roomName);
			IOManager::deserialize(handle, newRoom->backImageName);

			int size = 0;
			IOManager::deserialize(handle, size);
			newRoom->enemy.reserve(size);
			for (int i = 0; i < size; i++)
			{
				std::wstring enemyName;
				IOManager::deserialize(handle, enemyName);
				newRoom->enemy.push_back(enemyName);
			}

			IOManager::deserialize(handle, size);
			RoomEffect* loadedEffect = nullptr;
			for (int i = 0; i < size; i++)
			{
				loadedEffect = new RoomEffect();
				IOManager::deserialize(handle, loadedEffect->effectAmount);
				IOManager::deserialize(handle, loadedEffect->effectLength);
				IOManager::deserialize(handle, loadedEffect->condAmount);
				IOManager::deserialize(handle, loadedEffect->effectTime);
				IOManager::deserialize(handle, loadedEffect->activeCond);
				IOManager::deserialize(handle, loadedEffect->effectTarget);
				IOManager::deserialize(handle, loadedEffect->effect);
				IOManager::deserialize(handle, loadedEffect->effectName);
				newRoom->roomEffect.push_back(loadedEffect);
			}
		});

	if (FAILED(result))
	{
		UpdateData(FALSE);
		return;
	}

	imagePreview.SetBitmap(NULL);
	image.Destroy();
	CString path = backImage;
	if (newRoom->backImageName != L"")
	{
		backImage = newRoom->backImageName.c_str();
		image.Load(backImage);
		imagePreview.SetBitmap(image);

		PathStripPath(path.GetBuffer());
		path.ReleaseBuffer();
	}

	roomImageName.SetWindowTextW(path.GetString());

	enemy1 = newRoom->enemy[0].c_str();
	enemy2 = newRoom->enemy[1].c_str();
	enemy3 = newRoom->enemy[2].c_str();
	enemy4 = newRoom->enemy[3].c_str();

	for (auto& item : newRoom->roomEffect)
	{
		effects[item->effectName.c_str()] = item;
		effectList.AddString(item->effectName.c_str());
	}

	delete newRoom;

	UpdateData(FALSE);
}


void RoomTool::OnBnClickedBackimage()
{
	backImage = IOManager::selectLoadFilePath().c_str();
	imagePreview.SetBitmap(NULL);
	image.Destroy();
	image.Load(backImage);
	imagePreview.SetBitmap(image);

	CString path = backImage;
	PathStripPath(path.GetBuffer());
	path.ReleaseBuffer();

	roomImageName.SetWindowTextW(path.GetString());

}


void RoomTool::OnBnClickedUnitLoad()
{
	UpdateData(TRUE);

	unitList.ResetContent();

	std::vector<UNITDATA*> units;


	HRESULT result = IOManager::load(IOManager::selectLoadFilePath(), [&](HANDLE handle)->void
		{
			int unitCount = 0;
			IOManager::deserialize(handle, unitCount);

			for (int i = 0; i < unitCount; i++)
			{
				CString key;
				IOManager::deserialize(handle, key);
				UNITDATA* loadedUnit = new UNITDATA();
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

				units.push_back(loadedUnit);
			}
		});

	if (FAILED(result))
	{
		UpdateData(FALSE);
		return;
	}


	for (auto& item : units)
	{
		unitList.AddString(item->jobName.c_str());
		delete item;
	}
	units.clear();

	UpdateData(FALSE);
}


void RoomTool::OnDestroy()
{
	CDialog::OnDestroy();

	for (auto& effect : effects)
	{
		delete effect.second;
	}
	effects.clear();

}
