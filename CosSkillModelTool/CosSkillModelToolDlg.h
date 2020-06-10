
// CosSkillModelToolDlg.h: 头文件
//

#pragma once

#include <string>
#include "src/CDlgPreview.h"


// CCosSkillModelToolDlg 对话框
class CCosSkillModelToolDlg : public CDialogEx
{
// 构造
public:
	CCosSkillModelToolDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COSSKILLMODELTOOL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void setSelectHero(std::string idAndName);
	void setSelectSkin(std::string idAndName);
	void refreshHeroList();
	void refreshSkinList(std::string heroId);
	void refreshSkillList(std::string heroId);

public:
	CEdit m_ctrlEditProject;
	afx_msg void OnEnChangeEditProject();
	afx_msg void OnBnClickedButton1();
	CListBox m_ctrlListHeros;
	CListBox m_ctrlListSkins;
	CListBox m_ctrlListSkills;
	afx_msg void OnLbnSelchangeListSrcLookid();
	afx_msg void OnLbnSelchangeListSrcSkinId();
	afx_msg void OnLbnSelchangeListSrcSkillId();

private:
	CDlgPreview* m_pDlgPreview;
};
