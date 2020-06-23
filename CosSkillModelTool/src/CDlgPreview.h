#pragma once
#include <string>
#include "afxdialogex.h"
#include <functional>
#include "src/processor/BaseDefine.h"

typedef std::function<std::string(CONTENT_TYPE)> SignalGetContentByType;

class CDlgPreview : public CDialog
{
public:
	CDlgPreview(CWnd* pParent = nullptr);
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTcnSelchangeTabPreview(NMHDR* pNMHDR, LRESULT* pResult);

public:
	void SetPrtName(std::string prtP, std::string prtC);
	void RegisterFuncGetContent(SignalGetContentByType func);

public:
	CTabCtrl m_ctrlTabPreview;
	CRichEditCtrl m_ctrlEditPreview;
	SignalGetContentByType m_pFuncGetContent;
};
