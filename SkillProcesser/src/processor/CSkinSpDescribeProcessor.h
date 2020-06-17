#pragma once

#include "CFileBase.h"
#include "../IDllExport.h"
#include <string>
#include <map>
#include <vector>

class DLL_API CSkinSpDescribeProcessor : public CFileBase
{
public:
	CSkinSpDescribeProcessor();

public:
	bool init(std::string path);
	bool isSpecialSkillId(std::string skillId);
	std::string GetSkinSpSkillId(std::string skinId, std::string skillId);
	std::string GetSkinSpDamageId(std::string skinId, std::string skillId);

private:
	std::map<std::string, std::map< std::string, std::string> > m_skills;
	std::map<std::string, std::map< std::string, std::string> > m_damages;
};