#pragma once

#include "CFileBase.h"
#include "../IDllExport.h"
#include <string>
#include <map>
#include <vector>
#include <set>
#include "BaseDefine.h"

typedef std::map<std::string, std::map<std::string, SKILL_BLOCK> > SKILL_MAP;
class DLL_API CSkillDataLuaProcessor : public CFileBase
{
public:
	CSkillDataLuaProcessor();

public:
	void SetHeroId(std::string heroId);
	SKILL_MAP& GetSkills();
	void GetSkillData(std::string skillId, std::map<std::string, std::string>& outSkills, std::string newSkillId = "");
	void GetDamageData(std::string skillId, std::map<std::string, std::string>& outSkills, std::string newSkillId = "");
	void GetSkillIds(std::string skillId, std::vector<std::string>& outSkills);
	std::string GetSkillTotalContent(std::string skillId);

	std::string GenerateTotalContent(const std::map<std::string, std::string>& skillsInject, const std::map<std::string, std::string>& damagesInject);
	void ExportGeneratedTotalContent(const std::map<std::string, std::string>& skillsInject, const std::map<std::string, std::string>& damagesInject);

protected:
	void parseLuaSkillInfo(const std::vector<std::string>& lines, SKILL_MAP& outCfg);
	void parseLuaDamageInfo(const std::vector<std::string>& lines, SKILL_MAP& outCfg);

private:
	std::map<std::string, std::set<std::string> > m_useSkills;
	SKILL_MAP m_skills;
	SKILL_MAP m_damages;
	int m_nSkillStartLine;
	int m_nSkillEndLine;
	int m_nDamageStartLine;
	int m_nDamageEndLine;
};