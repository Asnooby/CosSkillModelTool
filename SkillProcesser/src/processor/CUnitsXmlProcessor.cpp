#include "CUnitsXmlProcessor.h"
#include "../CSingleton.h"
#include "../CEnvParams.h"
#include "../tools/CommonFuncs.h"

CUnitsXmlProcessor::CUnitsXmlProcessor()
	:m_setBasePrtNames({ "standby", "move", "enter_dead", "dead" })
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

void CUnitsXmlProcessor::GetUnitData(std::string unitId, std::pair<std::string, std::string>& outCfg
	, std::string newUnitId/* = ""*/, std::string newRoleId/* = ""*/, std::string skinName/* = ""*/, std::string newSkinName/* = ""*/) 
{
	if (m_units.end() != m_units.find(unitId))
	{
		outCfg.first = unitId;
		auto block = m_units[unitId].block;
		if (unitId != newUnitId)
		{
			outCfg.first = newUnitId;
			auto index_1 = block.find("<Unit");
			if (std::string::npos != index_1)
			{
				auto index_2 = block.find('"', index_1 + 1);
				auto index_3 = block.find('"', index_2 + 1);
				auto contentSrc = block.substr(index_1, index_3 - index_1);
				std::string contentDst = "<Unit id=\"" + newUnitId + "\"";
				replace_str(block, contentSrc, contentDst);
			}
		}
		if (!newRoleId.empty())
		{
			auto index_1 = block.find("<RoleID");
			if (std::string::npos != index_1)
			{
				auto index_2 = block.find('"', index_1 + 1);
				auto index_3 = block.find('"', index_2 + 1);
				auto contentSrc = block.substr(index_1, index_3 - index_1);
				std::string contentDst = "<RoleID value=\"" + newRoleId + "\"";
				replace_str(block, contentSrc, contentDst);
			}
		}
		if (skinName != newSkinName)
		{
			for (auto& key : m_setBasePrtNames)
			{
				auto index_1 = block.find("<" + key + " ");
				if (std::string::npos != index_1)
				{
					auto index_2 = block.find('"', index_1 + 1);
					auto index_3 = block.find('"', index_2 + 1);
					auto contentSrc = block.substr(index_1, index_3 - index_1);
					auto contentDst = contentSrc;
					replace_str(contentDst, skinName, newSkinName);
					replace_str(block, contentSrc, contentDst);
				}
			}
		}
		outCfg.second = std::move(block);
	}
}

std::string CUnitsXmlProcessor::GenerateTotalContent(std::string unitId, std::string block)
{
	vector<string> lines = m_lines;

	auto iterEnd = lines.end();
	if (m_units.empty())
	{
		lines.push_back("<Units>\r\n");
		lines.push_back("</Units>");
		iterEnd = lines.end() - 1;
	}
	else
	{
		for ( auto iter = lines.rbegin(); iter != lines.rend(); iter++)
		{
			if (std::string::npos != iter->find("/Units"))
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

const std::set<std::string>& CUnitsXmlProcessor::GetBasePrtNames()
{
	return m_setBasePrtNames;
}

void CUnitsXmlProcessor::GetBasePresentations(std::string unitId, std::set<std::string>& outCfg)
{
	if (m_units.end() != m_units.find(unitId))
	{
		auto block = m_units[unitId];
		for (auto& key : m_setBasePrtNames)
		{
			const auto block = m_units[unitId].block;
			auto index_1 = block.find("<" + key + " ");
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

void CUnitsXmlProcessor::ExportGeneratedTotalContent(std::string roleId, std::string block)
{
	SetPath(m_path, true);
	auto content = GenerateTotalContent(roleId, block);
	WriteTotalContent(content);
}