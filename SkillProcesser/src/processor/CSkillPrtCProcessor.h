#pragma once

#include "../IDllExport.h"
#include <string>
#include <map>
#include <set>
#include "BaseDefine.h"

class DLL_API CSkillPrtCProcessor
{
public:
	CSkillPrtCProcessor();

public:
	void SetPath(std::string path);
	void GetPrtNames(std::map<std::string, std::string>& skillPrtNames, std::set<std::string>& outCfg);
	std::string GetSkillTotalContent(std::map<std::string, std::string>& skillIds);

private:
	std::map<std::string, SKILL_BLOCK> m_skills;
};