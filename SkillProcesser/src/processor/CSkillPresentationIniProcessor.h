#pragma once

#include "CFileBase.h"
#include "../IDllExport.h"
#include <string>
#include <map>
#include <vector>
#include "BaseDefine.h"

class DLL_API CSkillPresentationIniProcessor : public CFileBase
{
public:
	CSkillPresentationIniProcessor();

public:
	void SetHeroId(std::string heroId);
	void SetSkinId(std::string skinId);
	std::string GetSkillTotalContent(std::vector<std::string>& skillIds);
	std::string GenerateTotalContent(std::string skinId, std::map<std::string, std::string>& skillDatas);
	void ExportGeneratedTotalContent(std::string skinId, std::map<std::string, std::string>& skillDatas);
	void GetSkillPrtName(std::string skinId, std::vector<std::string>& SkillIds, std::map<std::string, std::string>& outCfg, std::string skillId = "", std::string newSkillId = "");
	void SetSkinName(std::map<std::string, std::string>& skillData, std::string skinName, std::string newSkinName);

private:
	std::map<std::string, SKILL_BLOCK> m_skills;
};