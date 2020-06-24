#include "CRolesXmlProcessor.h"
#include "../CSingleton.h"
#include "../tools/CommonFuncs.h"

CRolesXmlProcessor::CRolesXmlProcessor()
{

}

void CRolesXmlProcessor::SetSkinId(std::string skinId)
{
	std::string path = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + skinId + "/presentations/roles.xml";
	SetPath(path);
	m_roles.clear();
	parseXmlCfg(m_lines, "Role", "id", m_roles);
}

std::string CRolesXmlProcessor::GetTotalContent(std::string roleId)
{
	std::string content;
	content += "<Roles>\r\n";
	if (m_roles.end() != m_roles.find(roleId))
	{
		content += m_roles[roleId].block;
	}
	content += "</Roles>";

	return content;
}

std::string CRolesXmlProcessor::GetPrtPPath(std::string roleId)
{
	std::string path;
	if (m_roles.end() != m_roles.find(roleId))
	{
		auto content = m_roles[roleId].block;
		auto index_1 = content.find("<Skas");
		if (index_1 != content.npos)
		{
			index_1 = content.find('"', index_1 + 1);
			auto index_2 = content.find('"', index_1 + 1);
			path = content.substr(index_1 + 1, index_2 - index_1 - 1);
		}
	}

	return path;
}

std::string CRolesXmlProcessor::GetPrtCPath(std::string roleId)
{
	std::string path;
	if (m_roles.end() != m_roles.find(roleId))
	{
		auto content = m_roles[roleId].block;
		auto index_1 = content.find("usdPathc");
		if (index_1 != content.npos)
		{
			index_1 = content.find('"', index_1 + 1);
			auto index_2 = content.find('"', index_1 + 1);
			path = content.substr(index_1 + 1, index_2 - index_1 - 1);
		}
	}

	return path;
}