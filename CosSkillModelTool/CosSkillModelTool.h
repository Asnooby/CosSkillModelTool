
// CosSkillModelTool.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号

// CCosSkillModelToolApp:
// 有关此类的实现，请参阅 CosSkillModelTool.cpp
//

class CCosSkillModelToolApp : public CWinApp
{
public:
	CCosSkillModelToolApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

private:
	HMODULE m_hProcessor;
};

extern CCosSkillModelToolApp theApp;