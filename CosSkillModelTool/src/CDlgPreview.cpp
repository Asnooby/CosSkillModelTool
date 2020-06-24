#pragma once
#include "pch.h"
#include "CDlgPreview.h"
#include "../resource.h"
#include <algorithm>

CDlgPreview::CDlgPreview(CWnd* pParent/* = nullptr*/)
	: CDialog(IDD_FRAME_PREVIEW, pParent)
{
	Create(IDD_FRAME_PREVIEW, pParent);
}

void CDlgPreview::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_TAB_PREVIEW, m_ctrlTabPreview);
	DDX_Control(pDX, IDC_EDIT_PREVIEW, m_ctrlEditPreview);
}

BEGIN_MESSAGE_MAP(CDlgPreview, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PREVIEW, &CDlgPreview::OnTcnSelchangeTabPreview)
END_MESSAGE_MAP()

BOOL CDlgPreview::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctrlTabPreview.InsertItem((int)CONTENT_TYPE::SKILLDATA_LUA, _T("skilldata.lua"));
	m_ctrlTabPreview.InsertItem((int)CONTENT_TYPE::SKILL_INI, _T("skill.ini"));
	m_ctrlTabPreview.InsertItem((int)CONTENT_TYPE::SKILLCONDITION_INI, _T("skillcondition.ini"));
	m_ctrlTabPreview.InsertItem((int)CONTENT_TYPE::SKILLPRESENTATION_INI, _T("skillpresentation.ini"));
	m_ctrlTabPreview.InsertItem((int)CONTENT_TYPE::PRT_C_XML, _T("_c.prt"));
	m_ctrlTabPreview.InsertItem((int)CONTENT_TYPE::PRT_P_XML, _T("_p.prt"));

	return TRUE;
}

void CDlgPreview::OnTcnSelchangeTabPreview(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_pFuncGetContent)
	{
		auto content = m_pFuncGetContent((CONTENT_TYPE)TabCtrl_GetCurSel(pNMHDR->hwndFrom));
		std::replace(content.begin(), content.end(), '\t', ' ');

		m_ctrlEditPreview.Clear();



		CHARFORMAT cf;
		ZeroMemory(&cf, sizeof(CHARFORMAT));
		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
			CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		cf.dwEffects = 0;
		cf.yHeight = 14 * 14;//文字高度
		cf.crTextColor = RGB(0, 0, 0); //文字颜色
		StrCpyW(cf.szFaceName, _T("隶书"));
		m_ctrlEditPreview.SetSel(0, -1); //设置处理区域
		m_ctrlEditPreview.SetSelectionCharFormat(cf);
		m_ctrlEditPreview.SetSel(0, -1);
		m_ctrlEditPreview.ReplaceSel(CString(content.c_str()));
		m_ctrlEditPreview.PostMessage(WM_VSCROLL, SB_TOP, 0);

		m_ctrlEditPreview.Invalidate();
	}
	*pResult = 0;
}

void CDlgPreview::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	RECT rect;
	m_ctrlEditPreview.GetWindowRect(&rect);
	rect.left = 0;
	rect.top = 28;
	rect.right = lpRect->right - lpRect->left;
	rect.bottom = lpRect->bottom - lpRect->top;
	m_ctrlEditPreview.MoveWindow(&rect, bRepaint);

	m_ctrlTabPreview.GetWindowRect(&rect);
	rect.left = 56;
	rect.top = 8;
	rect.right = lpRect->right - lpRect->left;
	rect.bottom = lpRect->bottom - lpRect->top;
	m_ctrlTabPreview.MoveWindow(&rect, bRepaint);

	CDialog::MoveWindow(lpRect, bRepaint);
}

void CDlgPreview::SetPrtName(std::string prtP, std::string prtC)
{
	CString str(prtC.c_str());
	m_ctrlTabPreview.DeleteItem(4);
	m_ctrlTabPreview.InsertItem(4, str);
	m_ctrlTabPreview.DeleteItem(5);
	m_ctrlTabPreview.InsertItem(5, str);
	m_ctrlTabPreview.SetCurSel(0);
}

void CDlgPreview::RegisterFuncGetContent(SignalGetContentByType func)
{
	m_pFuncGetContent = func;
}