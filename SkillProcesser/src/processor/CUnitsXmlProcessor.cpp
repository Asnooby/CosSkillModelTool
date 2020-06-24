#include "CUnitsXmlProcessor.h"
#include "../CSingleton.h"
#include "../CEnvParams.h"
#include "../tools/CommonFuncs.h"

CUnitsXmlProcessor::CUnitsXmlProcessor()
{

}

void CUnitsXmlProcessor::SetSkinId(std::string skinId)
{
	std::string path = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + skinId + "/data/config/units.xml";
	SetPath(path);
	m_units.clear();
	parseXmlCfg(m_lines, "Unit", "id", m_units);
}

std::string CUnitsXmlProcessor::GetTotalContent(std::string unitId)
{
	std::string content;
	content += "<Units>\r\n";
	if (m_units.end() != m_units.find(unitId))
	{
		content += m_units[unitId].block;
	}
	content += "</Units>";
	return content;
}

std::string CUnitsXmlProcessor::GetRoleId(std::string unitId)
{
	std::string roleId;
	if (m_units.end() != m_units.find(unitId))
	{
		const auto block = m_units[unitId].block;
		auto index_1 = block.find("RoleID");
		if (std::string::npos != index_1)
		{
			index_1 = block.find('"', index_1 + 1);
			auto index_2 = block.find('"', index_1 + 1);
			roleId = block.substr(index_1 + 1, index_2 - index_1 - 1);
		}
	}
	return roleId;
}

void CUnitsXmlProcessor::GetBasePresentations(std::string unitId, std::set<std::string>& outCfg)
{
	if (m_units.end() != m_units.find(unitId))
	{
		auto block = m_units[unitId];
		const auto keys = { "standby", "move", "fastmove", "enter_dead" , "dead" };
		for (auto& key : keys)
		{
			const auto block = m_units[unitId].block;
			auto index_1 = block.find(key);
			if (std::string::npos != index_1)
			{
				index_1 = block.find('"', index_1 + 1);
				auto index_2 = block.find('"', index_1 + 1);
				auto value = block.substr(index_1 + 1, index_2 - index_1 - 1);
				outCfg.emplace(value);
			}
		}
	}
}