#pragma once

#include "CFileBase.h"
#include "../IDllExport.h"
#include <string>
#include <map>
#include <set>
#include "BaseDefine.h"

class DLL_API CSkillPrtPProcessor : public CFileBase
{
public:
	CSkillPrtPProcessor();

public:
	void SetPath(const std::string path, bool createIfNotExist = false);
	void GetPrtData(std::set<std::string>& prtNames, std::map<std::string, std::string>& outCfg, std::string skinName, std::string newSkinName);
	std::string GetSkillTotalContent(std::set<std::string>& prtNames);
	std::string GenerateTotalContent(std::map<std::string, std::string>& prtData);
	void ExportGeneratedTotalContent(std::map<std::string, std::string>& prtData);

private:
	std::map<std::string, SKILL_BLOCK> m_skills;
};