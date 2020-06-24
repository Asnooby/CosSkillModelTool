
// CosSkillModelToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CosSkillModelTool.h"
#include "CosSkillModelToolDlg.h"
#include "afxdialogex.h"
#include "src/CSingleton.h"
#include "src/processor/CSkinProcessor.h"
#include "src/processor/CSkillDataLuaProcessor.h"
#include "src/tools/CommonFuncs.h"
#include <algorithm>
#include <functional>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "src/CEnvParams.h"


// CCosSkillModelToolDlg 对话框



CCosSkillModelToolDlg::CCosSkillModelToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COSSKILLMODELTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCosSkillModelToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROJECT, m_ctrlEditProject);
	DDX_Control(pDX, IDC_LIST_SRC_HEROID, m_ctrlListHeros);
	DDX_Control(pDX, IDC_LIST_SRC_SKINID, m_ctrlListSkins);
	DDX_Control(pDX, IDC_LIST_SRC_SKILLID, m_ctrlListSkills);
}

BEGIN_MESSAGE_MAP(CCosSkillModelToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_PROJECT, &CCosSkillModelToolDlg::OnEnChangeEditProject)
	ON_BN_CLICKED(IDC_BUTTON1, &CCosSkillModelToolDlg::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST_SRC_HEROID, &CCosSkillModelToolDlg::OnLbnSelchangeListSrcLookid)
	ON_LBN_SELCHANGE(IDC_LIST_SRC_SKINID, &CCosSkillModelToolDlg::OnLbnSelchangeListSrcSkinId)
	ON_LBN_SELCHANGE(IDC_LIST_SRC_SKILLID, &CCosSkillModelToolDlg::OnLbnSelchangeListSrcSkillId)
END_MESSAGE_MAP()


// CCosSkillModelToolDlg 消息处理程序

BOOL CCosSkillModelToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_ctrlEditProject.SetWindowText(_T("I:/git/moba2.0"));

	CRect rect;
	GetDlgItem(IDC_GROUP_PREVIEW)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_pDlgPreview = new CDlgPreview(this);
	m_pDlgPreview->MoveWindow(rect, true);
	m_pDlgPreview->ShowWindow(SW_SHOW);

	m_pDlgPreview->RegisterFuncGetContent([&](CONTENT_TYPE type)->std::string {
		CString str;
		m_ctrlListSkills.GetText(m_ctrlListSkills.GetCurSel(), str);
		USES_CONVERSION;
		std::string skillId = W2A(str);
		auto index = skillId.find("|");
		if (skillId.npos != index)
		{
			skillId = skillId.substr(0, index - 1);
			skillId = SelectNumber(skillId);
		}

		switch (type)
		{
		case CONTENT_TYPE::SKILLDATA_LUA:
		{
			return CSingleton::gSkillDataLuaProcesser.GetSkillTotalContent(skillId);
		}
		case CONTENT_TYPE::SKILL_INI:
		{
			return std::string("SKILL_INI");
		}
		case CONTENT_TYPE::SKILLCONDITION_INI:
		{
			return std::string("SKILLCONDITION_INI");
		}
		case CONTENT_TYPE::SKILLPRESENTATION_INI:
		{
			return std::string("SKILLPRESENTATION_INI");
		}
		case CONTENT_TYPE::PRT_C_XML:
		{
			return std::string("PRT_C_XML");
		}
		case CONTENT_TYPE::PRT_P_XML:
		{
			return std::string("PRT_P_XML");
		}
		}
		return std::string("");
	});

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCosSkillModelToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCosSkillModelToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCosSkillModelToolDlg::OnEnChangeEditProject()
{
	CString str;
	m_ctrlEditProject.GetWindowText(str);
	USES_CONVERSION;
	CSingleton::gEnvParams.strProjectPath = W2A(str);
}

void CCosSkillModelToolDlg::OnBnClickedButton1()
{
	CSingleton::gUnitProcessor.SetHeroPackageRoot(CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/");
	refreshHeroList();
}

void CCosSkillModelToolDlg::setSelectHero(std::string idAndName)
{
	auto heroId = idAndName;
	auto index = idAndName.find("|");
	if (idAndName.npos != index)
	{
		heroId = idAndName.substr(0, index - 1);
		heroId = SelectNumber(heroId);
	}

	refreshSkinList(heroId);
	refreshSkillList(heroId);
}

void CCosSkillModelToolDlg::setSelectSkin(std::string idAndName)
{
	auto skinId = idAndName;
	auto index = idAndName.find("|");
	if (idAndName.npos != index)
	{
		skinId = idAndName.substr(0, index - 1);
		skinId = SelectNumber(skinId);
	}

	auto skinPath = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + skinId + "/presentations/";
	std::string prtNameC = DeepFindInDir(skinPath, "_c.prt");
	if (!prtNameC.empty())
	{
		std::replace(prtNameC.begin(), prtNameC.end(), '\\', '/');
		auto index = prtNameC.rfind("/");
		prtNameC = prtNameC.substr(index + 1, prtNameC.length() - index - 1);
	}
	std::string prtNameP = DeepFindInDir(skinPath, "_p.prt");
	if (!prtNameP.empty())
	{
		std::replace(prtNameP.begin(), prtNameP.end(), '\\', '/');
		auto index = prtNameP.rfind("/");
		prtNameP = prtNameP.substr(index + 1, prtNameP.length() - index - 1);
	}

	m_pDlgPreview->SetPrtName(prtNameP, prtNameC);
}

void CCosSkillModelToolDlg::refreshHeroList()
{
	for (int i = m_ctrlListHeros.GetCount() - 1; i >= 0; i--)
	{
		m_ctrlListHeros.DeleteString(i);
	}

	//std::string filter;
	auto heros = CSingleton::gUnitProcessor.getHeros();
	for (auto iter = heros.begin(); iter != heros.end(); iter++)
	{
		auto skins = CSingleton::gUnitProcessor.getSkins(iter->first);
		std::string protoSkin;
		for (auto iterSkin = skins.begin(); iterSkin != skins.end(); iterSkin++)
		{
			if (0 == iterSkin->compare(iter->first + "000"))
			{
				protoSkin = *iterSkin;
				break;
			}
		}
		if (protoSkin.empty())
		{
			protoSkin = skins[0];
		}
		auto protoSkinName = CSingleton::gUnitProcessor.getSkinName(protoSkin);

		m_ctrlListHeros.AddString(CString((iter->first + " | " + protoSkinName).c_str()));
		/*
		CSingleton::gSkillDataLuaProcesser.SetHeroId(iter->first);
		auto skills = CSingleton::gSkillDataLuaProcesser.GetSkills();
		if (5 < skills.size())
		{
			filter = filter + " | " + iter->first;
		}
		*/
	}

	if (-1 == m_ctrlListHeros.GetCurSel())
	{
		m_ctrlListHeros.SetCurSel(0);
	}

	::SendMessage(GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(m_ctrlListHeros.GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)m_ctrlListHeros.GetSafeHwnd());
}

void CCosSkillModelToolDlg::refreshSkinList(std::string heroId)
{
	for (int i = m_ctrlListSkins.GetCount() - 1; i >= 0; i--)
	{
		m_ctrlListSkins.DeleteString(i);
	}

	auto skins = CSingleton::gUnitProcessor.getSkins(heroId);
	for (auto iter = skins.begin(); iter != skins.end(); iter++)
	{
		auto protoSkinName = CSingleton::gUnitProcessor.getSkinName(iter->c_str());
		protoSkinName = *iter + " | " + protoSkinName;
		m_ctrlListSkins.AddString(CString(protoSkinName.c_str()));
	}

	if (-1 == m_ctrlListSkins.GetCurSel())
	{
		m_ctrlListSkins.SetCurSel(0);
	}

	::SendMessage(GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(m_ctrlListSkins.GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)m_ctrlListSkins.GetSafeHwnd());
}

void CCosSkillModelToolDlg::refreshSkillList(std::string heroId)
{
	for (int i = m_ctrlListSkills.GetCount() - 1; i >= 0; i--)
	{
		m_ctrlListSkills.DeleteString(i);
	}

	CSingleton::gSkillDataLuaProcesser.SetHeroId(heroId);
	auto skills = CSingleton::gSkillDataLuaProcesser.GetSkills();
	for (auto iter = skills.begin(); iter != skills.end(); iter++)
	{
		m_ctrlListSkills.AddString(CString(iter->first.c_str()));
	}

	if (-1 == m_ctrlListSkills.GetCurSel())
	{
		m_ctrlListSkills.SetCurSel(0);
	}

	::SendMessage(GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(m_ctrlListSkills.GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)m_ctrlListSkills.GetSafeHwnd());
}

void CCosSkillModelToolDlg::OnLbnSelchangeListSrcLookid()
{
	CString str;
	m_ctrlListHeros.GetText(m_ctrlListHeros.GetCurSel(), str);
	USES_CONVERSION;
	setSelectHero(W2A(str));
}

void CCosSkillModelToolDlg::OnLbnSelchangeListSrcSkinId()
{
	CString str;
	m_ctrlListSkins.GetText(m_ctrlListSkins.GetCurSel(), str);
	USES_CONVERSION;
	setSelectSkin(W2A(str));
}

void CCosSkillModelToolDlg::OnLbnSelchangeListSrcSkillId()
{
	NMHDR nmhdr;
	nmhdr.code = TCN_SELCHANGE;
	nmhdr.hwndFrom = m_pDlgPreview->m_ctrlTabPreview.GetSafeHwnd();
	nmhdr.idFrom = m_pDlgPreview->m_ctrlTabPreview.GetDlgCtrlID();
	::SendMessage(m_pDlgPreview->GetSafeHwnd(), WM_NOTIFY, MAKELONG(TCN_SELCHANGE, 0), (LPARAM)(&nmhdr));
}
