// CPathFind.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CPathFind.h"
#include "CFileInfo.h"


// CPathFind 대화 상자

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CPathFind, pParent)
{

}

CPathFind::~CPathFind()
{
}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFind::OnListBox)
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFind::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON7, &CPathFind::OnLoadData)
END_MESSAGE_MAP()

void CPathFind::OnListBox()
{
	
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR	szFilePath[MAX_PATH] = L"";

	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR		szFileName[MAX_STR] = L"";

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		CFileInfo::DirInfoExraction(szFilePath, m_PathInfoList);
	}

	m_ListBox.ResetContent();

	wstring	wstrCombined = L"";
	TCHAR	szBuf[MAX_STR] = L"";

	for (auto& pImgPath : m_PathInfoList)
	{
		// _itow_s : 정수를 문자열로 변환, 10진수로 변환시키겠다는 의미
		_itow_s(pImgPath->iCount, szBuf, 10);

		wstrCombined = pImgPath->wstrObjKey + L"|" + pImgPath->wstrStateKey + L"|" + szBuf + L"|" + pImgPath->wstrPath;

		m_ListBox.AddString(wstrCombined.c_str());
	}
		
	UpdateData(FALSE);
}

void CPathFind::OnDestroy()
{
	CDialog::OnDestroy();	

	for_each(m_PathInfoList.begin(), m_PathInfoList.end(), CDeleteObj());
	m_PathInfoList.clear();

}





void CPathFind::OnSaveData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CPathFind::OnLoadData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
