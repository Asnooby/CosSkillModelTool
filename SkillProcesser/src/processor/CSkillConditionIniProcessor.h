#pragma once

#include "CFileBase.h"
#include "../IDllExport.h"
#include <string>
#include <map>
#include <vector>
#include "BaseDefine.h"

class DLL_API CSkillConditionIniProcessor : public CFileBase
{
public:
	CSkillConditionIniProcessor();

public:
	void SetHeroId(std::string heroId);
	std::string GetSkillTotalContent(const std::vector<std::string>& skillIds);
	void GetSkillData(const std::vector<std::string>& skillIds, std::string oldSkillId, std::string newSkillId, std::map<std::string, std::string>& outSkills);
	std::string GenerateTotalContent(const std::map<std::string, std::string>& skillDatas);
	void ExportGeneratedTotalContent(const std::map<std::string, std::string>& skillDatas);

private:
	std::map<std::string, SKILL_BLOCK> m_skills;
};