// CMyForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CMyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_CMyForm)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnUnittool)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnMapTool)
	ON_BN_CLICKED(IDC_BUTTON8, &CMyForm::OnPathFind)
	ON_BN_CLICKED(IDC_BUTTON9, &CMyForm::OnBnClickedRoomTool)
END_MESSAGE_MAP()


// CMyForm 진단

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_Font.CreatePointFont(180, L"궁서");

	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON8)->SetFont(&m_Font);
}

void CMyForm::OnUnittool()
{
	// GetSafeHwnd : 현재 다이얼로그 윈도우 핸들을 반환

	if(nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_CUnittool);

	m_UnitTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnMapTool()
{
	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_CMapTool);

	m_MapTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnPathFind()
{
	if (nullptr == m_PathFind.GetSafeHwnd())
		m_PathFind.Create(IDD_CPathFind);

	m_PathFind.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedRoomTool()
{
	if (nullptr == m_RoomTool.GetSafeHwnd())
		m_RoomTool.Create(IDD_RoomTool);

	m_RoomTool.ShowWindow(SW_SHOW);
}
