#pragma once

#include "../IDllExport.h"
#include <string>
#include <map>
#include <vector>
#include <set>
#include "BaseDefine.h"

typedef std::map<std::string, std::map<std::string, SKILL_BLOCK> > SKILL_MAP;
class DLL_API CSkillDataLuaProcessor
{
public:
	CSkillDataLuaProcessor();

public:
	void SetHeroId(std::string heroId);
	SKILL_MAP& GetSkills();
	void GetSkillData(std::string skillId, std::map<std::string, std::string>& outSkills);
	void GetDamageData(std::string skillId, std::map<std::string, std::string>& outSkills);
	void GetSkillIds(std::string skillId, std::vector<std::string>& outSkills);
	std::string GetSkillTotalContent(std::string skillId);

protected:
	void parseLuaSkillInfo(std::string path, SKILL_MAP& outCfg);
	void parseLuaDamageInfo(std::string path, SKILL_MAP& outCfg);

private:
	std::map<std::string, std::set<std::string> > m_useSkills;
	SKILL_MAP m_skills;
	SKILL_MAP m_damages;
};