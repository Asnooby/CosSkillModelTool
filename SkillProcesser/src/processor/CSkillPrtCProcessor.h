#pragma once

#include "CFileBase.h"
#include "../IDllExport.h"
#include <string>
#include <map>
#include <set>
#include "BaseDefine.h"

class DLL_API CSkillPrtCProcessor : public CFileBase
{
public:
	CSkillPrtCProcessor();

public:
	void SetPath(const std::string path, bool createIfNotExist = false);
	void GetPrtNames(std::map<std::string, std::string>& skillPrtNames, std::set<std::string>& outCfg);
	void GetSkillPrtData(std::map<std::string, std::string>& skillPrtNames, std::map<std::string, std::string>& outCfg, std::string skinName = "", std::string newSkinName = "");
	std::string GetSkillTotalContent(std::map<std::string, std::string>& skillIds);
	std::string GenerateTotalContent(std::map<std::string, std::string>& skillData);
	void ExportGeneratedTotalContent(std::map<std::string, std::string>& skillData);

private:
	std::map<std::string, SKILL_BLOCK> m_skills;
};