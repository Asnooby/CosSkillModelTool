#pragma once

#include "CFileBase.h"
#include "../IDllExport.h"
#include <string>
#include <map>
#include <vector>

class DLL_API CRolesXmlProcessor : public CFileBase
{
public:
	CRolesXmlProcessor();
	void SetSkinId(std::string skinId);
	std::string GetPrtPPath(std::string roleId);
	std::string GetPrtCPath(std::string roleId);
	std::string GetTotalContent(std::string roleId);
	void GetRoleData(std::string roleId, std::pair<std::string, std::string>& outCfg, std::string newRoleId = "", std::string skinName = "", std::string newSkinName = "");
	std::string GenerateTotalContent(std::string roleId, std::string block);
	void ExportGeneratedTotalContent(std::string roleId, std::string block);

public:

private:
	std::map<std::string, SKILL_BLOCK> m_roles;
};