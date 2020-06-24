#include "../CSingleton.h"
#include "../CEnvParams.h"
#include "../tools/CommonFuncs.h"
#include "CSkillDataLuaProcessor.h"

CSkillDataLuaProcessor::CSkillDataLuaProcessor()
	: m_nSkillStartLine(-1)
	, m_nSkillEndLine(-1)
	, m_nDamageStartLine(-1)
	, m_nDamageEndLine(-1)
{

}

void CSkillDataLuaProcessor::SetHeroId(std::string heroId)
{
	std::string path = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + heroId + "_common/data/config/skilldata.lua";
	SetPath(path);

	m_skills.clear();
	m_nSkillStartLine = -1;
	m_nSkillEndLine = -1;
	parseLuaSkillInfo(m_lines, m_skills);
	m_damages.clear();
	m_nDamageStartLine = -1;
	m_nDamageEndLine = -1;
	parseLuaDamageInfo(m_lines, m_damages);
}

SKILL_MAP& CSkillDataLuaProcessor::GetSkills()
{
	return m_skills;
}

void CSkillDataLuaProcessor::GetSkillData(std::string skillId, std::map<std::string, std::string>& outSkills, std::string newSkillId/* = ""*/)
{
	auto iter = m_skills.find(skillId);
	if (iter != m_skills.end())
	{
		for (auto iterSkill = iter->second.begin(); iterSkill != iter->second.end(); iterSkill++)
		{
			auto SubSkillId = iterSkill->second.id;
			auto content = iterSkill->second.block;

			if (!newSkillId.empty())
			{
				SubSkillId = std::to_string(getValueByBase(skillId, iterSkill->second.id, newSkillId));

				replace_str(content, "[" + iterSkill->second.id + "]", "[" + SubSkillId + "]");

				auto index_1 = content.find("AghanimSkill");
				if (std::string::npos != index_1)
				{
					auto index_2 = content.find("=", index_1 + 1);
					auto index_3 = content.find(",", index_2 + 1);
					auto strAghId = content.substr(index_2 + 1, index_3 - index_2 );
					strAghId = SelectNumber(strAghId);
					int nAghId = getValueByBase(skillId, strAghId, newSkillId);
					auto contentSrc = content.substr(index_1 + 1, index_3 - index_1);
					string contentNew = "AghanimSkill = " + std::to_string(nAghId);
					replace_str(content, contentSrc, contentNew);
				}

				index_1 = content.find("damageMergeId");
				if (std::string::npos != index_1)
				{
					auto index_2 = content.find("=", index_1 + 1);
					auto index_3 = index_2 + 1;
					for (auto index = index_3; index < content.size(); index++)
					{
						if (content[index] == ',' || content[index] == '\r' || content[index] == '\n')
						{
							index_3 = index;
							break;
						}
					}
					auto strMergeId = content.substr(index_2 + 1, index_3 - index_2 - 1);
					strMergeId = SelectNumber(strMergeId);
					int nMergeId = getValueByBase(skillId, strMergeId, newSkillId);
					auto contentSrc = content.substr(index_1, index_3 - index_1);
					string contentNew = "damageMergeId = " + std::to_string(nMergeId);
					replace_str(content, contentSrc, contentNew);
				}
			}
			outSkills[SubSkillId] = content;
		}
	}
}

void CSkillDataLuaProcessor::GetDamageData(std::string skillId, std::map<std::string, std::string>& outSkills, std::string newSkillId/* = ""*/)
{
	auto iter = m_damages.find(skillId);
	if (iter != m_damages.end())
	{
		for (auto iterSkill = iter->second.begin(); iterSkill != iter->second.end(); iterSkill++)
		{
			auto SubSkillId = iterSkill->second.id;
			auto content = iterSkill->second.block;

			if (!newSkillId.empty())
			{
				SubSkillId = std::to_string(getValueByBase(skillId, iterSkill->second.id, newSkillId));

				replace_str(content, "[" + iterSkill->second.id + "]", "[" + SubSkillId + "]");

				auto index_1 = content.find("mainid");
				if (std::string::npos != index_1)
				{
					auto index_2 = content.find("=", index_1 + 1);
					auto index_3 = content.find(",", index_2 + 1);
					auto strMainId = content.substr(index_2 + 1, index_3 - index_2);
					strMainId = SelectNumber(strMainId);
					int nNewMainId = getValueByBase(skillId, strMainId, newSkillId);
					auto contentSrc = content.substr(index_1 + 1, index_3 - index_1);
					string contentNew = "mainid = " + std::to_string(nNewMainId);
					replace_str(content, contentSrc, contentNew);
				}
			}
			outSkills[SubSkillId] = content;
		}
	}
}

void CSkillDataLuaProcessor::GetSkillIds(std::string skillId, std::vector<std::string>& outSkills)
{
	auto iter = m_skills.find(skillId);
	if (iter != m_skills.end())
	{
		for (auto iterSkill = iter->second.begin(); iterSkill != iter->second.end(); iterSkill++)
		{
			outSkills.push_back(iterSkill->second.id);
		}
	}
	auto iterDamage = m_damages.find(skillId);
	if (iterDamage != m_damages.end())
	{
		for (auto iterSkill = iterDamage->second.begin(); iterSkill != iterDamage->second.end(); iterSkill++)
		{
			outSkills.push_back(iterSkill->second.id);
		}
	}
	auto iterUse = m_useSkills.find(skillId);
	if (iterUse != m_useSkills.end())
	{
		outSkills.insert(outSkills.end(), iterUse->second.begin(), iterUse->second.end());
	}
}

std::string CSkillDataLuaProcessor::GetSkillTotalContent(std::string skillId)
{
	std::map<std::string, std::string> skillData;
	GetSkillData(skillId, skillData);
	std::map<std::string, std::string> damageData;
	GetDamageData(skillId, damageData);

	std::string content;
	content += "local skillInfo = {\r\n";
	if (!skillData.empty())
	{
		for (auto iter = skillData.begin(); iter != skillData.end(); iter++)
		{
			content += iter->second;
		}
	}
	else
	{
		content += "\r\n";
	}
	content += "}\r\n";
	content += "\r\n";
	content += "local damageInfo = {\r\n";
	if (!damageData.empty())
	{
		for (auto iter = damageData.begin(); iter != damageData.end(); iter++)
		{
			content += iter->second;
		}
	}
	else
	{
		content += "\r\n";
	}
	content += "}\r\n";
	content += "\r\n";
	content += "return { skillInfo, damageInfo }";

	return content;
}

std::string CSkillDataLuaProcessor::GenerateTotalContent(const std::map<std::string, std::string>& skillsInject, const std::map<std::string, std::string>& damagesInject)
{
	vector<string> lines = m_lines;

	auto nSkillStartLine = m_nSkillStartLine;
	auto nSkillEndLine = m_nSkillEndLine;
	auto nDamageStartLine = m_nDamageStartLine;
	auto nDamageEndLine = m_nDamageEndLine;
	if (-1 == nSkillEndLine)
	{
		lines.push_back("local skillInfo = {\r\n");
		lines.push_back("\r\n");
		lines.push_back("}\r\n");
		nSkillStartLine = 0;
		nSkillEndLine = 2;
	}
	if (-1 == nDamageEndLine)
	{
		lines.push_back("\r\n");
		lines.push_back("local damageInfo = {\r\n");
		lines.push_back("\r\n");
		lines.push_back("}\r\n");
		lines.push_back("\r\n");
		lines.push_back("return { skillInfo, damageInfo }");
		nDamageStartLine = nSkillEndLine + 2;
		nDamageEndLine = nSkillEndLine + 4;
	}

	for (auto iter = damagesInject.begin(); iter != damagesInject.end(); iter++)
	{
		if (m_damages.find(iter->first) == m_damages.end())
		{
			lines.insert(lines.begin() + nDamageEndLine++, iter->second);
		}
	}

	for (auto iter = skillsInject.begin(); iter != skillsInject.end(); iter++)
	{
		//if (m_skills.find(iter->first) == m_skills.end())
		//{
			lines.insert(lines.begin() + nSkillEndLine++, iter->second);
		//}
	}

	std::string content;
	for (auto& line : lines)
	{
		content += line;
	}

	return content;
}

void CSkillDataLuaProcessor::ExportGeneratedTotalContent(const std::map<std::string, std::string>& skillsInject, const std::map<std::string, std::string>& damagesInject)
{
	SetPath(m_path, true);
	auto content = GenerateTotalContent(skillsInject, damagesInject);
	WriteTotalContent(content);
}

void CSkillDataLuaProcessor::parseLuaSkillInfo(const std::vector<std::string>& lines, SKILL_MAP& outCfg)
{
	if (!lines.empty())
	{
		std::map< std::string, std::string > mapMainSkill;
		SKILL_BLOCK curNode;
		int index = 0;
		int nBrakets = 0;
		int lineIndex = 0;
		for (auto& line : lines)
		{
			if (std::string::npos != line.find("local skillInfo = {"))
			{
				m_nSkillStartLine = lineIndex;
				nBrakets = std::count(line.begin(), line.end(), '{');
			}
			else
			{
				if (0 < nBrakets)
				{
					nBrakets = nBrakets + std::count(line.begin(), line.end(), '{') - std::count(line.begin(), line.end(), '}');
					if (0 == nBrakets)
					{
						m_nSkillEndLine = lineIndex;
						break;
					}
				}
			}

			if (line.npos != line.find("[[{"))
			{
				curNode.id = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1);
				curNode.block = "";
				curNode.endLine = -1;
				curNode.startLine = lineIndex;

				if (mapMainSkill.end() == mapMainSkill.find(curNode.id))
				{
					mapMainSkill[curNode.id] = curNode.id;
				}
			}

			if (!curNode.id.empty())
			{
				curNode.block = curNode.block + line;

				if (line.npos != line.find("damageMergeId"))
				{
					int index_1 = line.find("damageMergeId");
					index_1 = line.find("=", index_1);
					auto str = line.substr(index_1, line.size() - index_1 - 1);
					str = SelectNumber(str.c_str());
					if (0 != curNode.id.compare(str))
					{
						mapMainSkill[curNode.id] = str;
					}
				}
				else if (line.npos != line.find("AghanimSkill"))
				{
					int index_1 = line.find("AghanimSkill");
					index_1 = line.find("=", index_1);
					auto str = line.substr(index_1 + 1, line.size() - index_1 - 1);
					str = SelectNumber(str.c_str());
					mapMainSkill[str] = curNode.id;
				}
				else if (line.npos != line.find("skillSelectIDInfo"))
				{
					int index_1 = line.find("skillSelectIDInfo");
					index_1 = line.find("=", index_1);
					auto str = line.substr(index_1 + 1, line.size() - index_1 - 1);
					auto vStr = split(str, ",");
					for (auto& id : vStr)
					{
						id = SelectNumber(id);
						if (!id.empty())
						{
							mapMainSkill[id] = curNode.id;
						}
					}
				}
				else if (line.npos != line.find("useskill"))
				{
					int index_1 = line.find("useskill");
					index_1 = line.find("=", index_1);
					auto str = line.substr(index_1 + 1, line.size() - index_1 - 1);
					str = SelectNumber(str);
					m_useSkills[curNode.id].insert(m_useSkills[curNode.id].end(), str);
				}

				if (line.npos != line.find("}]]"))
				{
					curNode.endLine = lineIndex;

					if (!CSingleton::gSkinSpDescribeProcessor.isSpecialSkillId(curNode.id))
					{
						outCfg[curNode.id][curNode.id] = curNode;
					}
					curNode.init();
				}
			}

			lineIndex++;
		}

		for (auto iter = outCfg.begin(); iter != outCfg.end();)
		{
			std::string strMainSkill = iter->first;
			while (!mapMainSkill[strMainSkill].empty() && 0 != mapMainSkill[strMainSkill].compare(strMainSkill))
			{
				strMainSkill = mapMainSkill[strMainSkill];
			}
			if (0 != iter->first.compare(strMainSkill))
			{
				outCfg[strMainSkill][iter->first] = iter->second[iter->first];
				iter = outCfg.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
}

void CSkillDataLuaProcessor::parseLuaDamageInfo(const std::vector<std::string>& lines, SKILL_MAP& outCfg)
{
	if (!lines.empty())
	{
		std::map< std::string, std::string > mapMainSkill;
		SKILL_BLOCK curNode;
		int index = 0;
		int nBrakets = 0;
		int lineIndex = 0;
		for (auto& line : lines)
		{
			if (std::string::npos != line.find("local damageInfo = {"))
			{
				m_nDamageStartLine = lineIndex;
				nBrakets = std::count(line.begin(), line.end(), '{');
			}
			else
			{
				if (0 < nBrakets)
				{
					nBrakets = nBrakets + std::count(line.begin(), line.end(), '{') - std::count(line.begin(), line.end(), '}');
					if (0 == nBrakets)
					{
						m_nDamageEndLine = lineIndex;
						break;
					}
				}
			}

			if (line.npos != line.find("mainid"))
			{
				curNode.id = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1);
				curNode.block = line;
				curNode.endLine = -1;
				curNode.startLine = lineIndex;

				auto index_1 = line.find("mainid");
				index_1 = line.find("=", index_1 + 1);
				auto index_2= line.find(",", index_1 + 1);
				auto mainid = line.substr(index_1, index_2 - index_1 + 1);
				mainid = SelectNumber(mainid.c_str());

				outCfg[mainid][curNode.id] = curNode;
			}

			lineIndex++;
		}
	}
}