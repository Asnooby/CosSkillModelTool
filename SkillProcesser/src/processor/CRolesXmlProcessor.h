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
	std::string GetTotalContent(std::string roleId);
	std::string GetPrtPPath(std::string roleId);
	std::string GetPrtCPath(std::string roleId);

public:

private:
	std::map<std::string, SKILL_BLOCK> m_roles;
};