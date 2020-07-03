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

void CRolesXmlProcessor::GetRoleData(std::string roleId, std::pair<std::string, std::string>& outCfg
	, std::string newRoleId/* = ""*/, std::string skinName/* = ""*/, std::string newSkinName/* = ""*/)
{
	if (m_roles.end() != m_roles.find(roleId))
	{
		outCfg.first = roleId;
		auto block = m_roles[roleId].block;
		if (roleId != newRoleId)
		{
			outCfg.first = newRoleId;
			auto index_1 = block.find("<Role ");
			if (std::string::npos != index_1)
			{
				auto index_2 = block.find('"', index_1 + 1);
				auto index_3 = block.find('"', index_2 + 1);
				auto contentSrc = block.substr(index_1, index_3 - index_1);
				std::string contentDst = "<Role id=\"" + newRoleId + "\"";
				replace_str(block, contentSrc, contentDst);
			}
		}
		if (skinName != newSkinName)
		{
			replace_str(block, skinName, newSkinName);
		}
		outCfg.second = std::move(block);
	}
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

std::string CRolesXmlProcessor::GenerateTotalContent(std::string roleId, std::string block)
{
	vector<string> lines = m_lines;

	auto iterEnd = lines.end();
	if (m_roles.empty())
	{
		lines.push_back("<Roles>\r\n");
		lines.push_back("</Roles>");
		iterEnd = lines.end() - 1;
	}
	else
	{
		for (auto iter = lines.rbegin(); iter != lines.rend(); iter++)
		{
			if (std::string::npos != iter->find("/Roles"))
			{
				iterEnd = iter.base() - 1;
				break;
			}
		}
	}

	lines.insert(iterEnd, block);

	std::string content;
	for (const auto& line : lines)
	{
		content += line;
	}
	return content;
}

void CRolesXmlProcessor::ExportGeneratedTotalContent(std::string roleId, std::string block)
{
	SetPath(m_path, true);
	auto content = GenerateTotalContent(roleId, block);
	WriteTotalContent(content);
}