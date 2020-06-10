#pragma once

#include "IDllExport.h"
#include <string>
#include <map>
#include <vector>

#include "IDllExport.h"

typedef struct
{
	int startLine = 0;
	int endLine = 0;
	std::string id;
	std::string block;

	void init()
	{
		startLine = 0;
		endLine = 0;
		id.clear();
		block.clear();
	}
} SKILL_BLOCK;

typedef std::map<std::string, std::map<std::string, SKILL_BLOCK> > SKILL_MAP;
class DLL_API CSkillDataLuaProcesser
{
public:
	CSkillDataLuaProcesser();

public:
	void SetHeroId(std::string heroId);
	SKILL_MAP& GetSkills();
	void GetSkillData(std::string skillId, std::map<std::string, std::string>& outSkills, bool bWithAddition);
	void GetDamageData(std::string skillId, std::map<std::string, std::string>& outSkills);
	std::string GetSkillTotalContent(std::string skillId);

protected:
	void parseLuaSkillInfo(std::string path, SKILL_MAP& outCfg);
	void parseLuaDamageInfo(std::string path, SKILL_MAP& outCfg);

private:
	SKILL_MAP m_skills;
	SKILL_MAP m_damages;
};