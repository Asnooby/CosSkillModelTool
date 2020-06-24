#pragma once

#include "CFileBase.h"
#include "../IDllExport.h"
#include <string>
#include <map>
#include <set>

class DLL_API CUnitsXmlProcessor : public CFileBase
{
public:
	CUnitsXmlProcessor();
	void SetSkinId(std::string skinId);
	std::string GetTotalContent(std::string unitId);
	std::string GetRoleId(std::string unitId);
	void GetBasePresentations(std::string unitId, std::set<std::string>& outCfg);

public:

private:
	std::map<std::string, SKILL_BLOCK> m_units;
};