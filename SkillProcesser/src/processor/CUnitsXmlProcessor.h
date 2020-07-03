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
	std::string GetRoleId(std::string unitId);
	const std::set<std::string>& GetBasePrtNames();
	void GetBasePresentations(std::string unitId, std::set<std::string>& outCfg);
	std::string GetTotalContent(std::string unitId);
	void GetUnitData(std::string unitId, std::pair<std::string, std::string>& outCfg, std::string newUnitId = "", std::string newRoleId = "", std::string skinName = "", std::string newSkinName = "");
	std::string GenerateTotalContent(std::string unitId, std::string block);
	void ExportGeneratedTotalContent(std::string unitId, std::string block);
	
public:

private:
	std::map<std::string, SKILL_BLOCK> m_units;
	std::set<std::string> m_setBasePrtNames;
};