#include "CSingleton.h"
#include "CSkillDataLuaProcesser.h"
#include "CEnvParams.h"
#include "tools/CommonFuncs.h"

#define MAX_LINE_SIZE 1024

CSkillDataLuaProcesser::CSkillDataLuaProcesser()
{

}

void CSkillDataLuaProcesser::SetHeroId(std::string heroId)
{
	std::string path = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + heroId + "_common/data/config/skilldata.lua";
	m_skills.clear();
	parseLuaSkillInfo(path, m_skills);
	m_damages.clear();
	parseLuaDamageInfo(path, m_damages);
}

SKILL_MAP& CSkillDataLuaProcesser::GetSkills()
{
	return m_skills;
}

void CSkillDataLuaProcesser::GetSkillData(std::string skillId, std::map<std::string, std::string>& outSkills, bool bWithAddition)
{
	auto iter = m_skills.find(skillId);
	if (iter != m_skills.end())
	{
		if (bWithAddition)
		{
			for (auto iterSkill = iter->second.begin(); iterSkill != iter->second.end(); iterSkill++)
			{
				outSkills[iterSkill->second.id] = iterSkill->second.block;
			}
		}
		else
		{
			if (iter->second.find(skillId) != iter->second.end())
			{
				outSkills[skillId] = iter->second[skillId].block;
			}
		}
	}
}

void CSkillDataLuaProcesser::GetDamageData(std::string skillId, std::map<std::string, std::string>& outSkills)
{
	auto iter = m_damages.find(skillId);
	if (iter != m_damages.end())
	{
		for (auto iterSkill = iter->second.begin(); iterSkill != iter->second.end(); iterSkill++)
		{
			outSkills[iterSkill->second.id] = iterSkill->second.block;
		}
	}
}

std::string CSkillDataLuaProcesser::GetSkillTotalContent(std::string skillId)
{
	std::map<std::string, std::string> skillData;
	GetSkillData(skillId, skillData, true);
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

void CSkillDataLuaProcesser::parseLuaSkillInfo(std::string path, SKILL_MAP& outCfg)
{
	FILE* file;
	fopen_s(&file, path.c_str(), "rb");
	if (file)
	{
		std::map< std::string, std::string > mapMainSkill;
		int startLine = -1;
		int endLine = -1;
		SKILL_BLOCK curNode;
		char pBuffer[MAX_LINE_SIZE];
		memset(pBuffer, MAX_LINE_SIZE, 0);
		int index = 0;
		int nBrakets = 0;
		int lineIndex = 0;
		while (fgets(pBuffer, MAX_LINE_SIZE, file))
		{
			std::string line = pBuffer;
			if (std::string::npos != line.find("local skillInfo = {"))
			{
				startLine = lineIndex;
				nBrakets = std::count(line.begin(), line.end(), '{');
			}
			else
			{
				if (0 < nBrakets)
				{
					nBrakets = nBrakets + std::count(line.begin(), line.end(), '{') - std::count(line.begin(), line.end(), '}');
					if (0 == nBrakets)
					{
						endLine = lineIndex;
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

				if (line.npos != line.find("}]]"))
				{
					curNode.endLine = lineIndex;

					outCfg[curNode.id][curNode.id] = curNode;
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

		fclose(file);
		file = NULL;
	}
}

void CSkillDataLuaProcesser::parseLuaDamageInfo(std::string path, SKILL_MAP& outCfg)
{
	FILE* file;
	fopen_s(&file, path.c_str(), "rb");
	if (file)
	{
		std::map< std::string, std::string > mapMainSkill;
		int startLine = -1;
		int endLine = -1;
		SKILL_BLOCK curNode;
		char pBuffer[MAX_LINE_SIZE];
		memset(pBuffer, MAX_LINE_SIZE, 0);
		int index = 0;
		int nBrakets = 0;
		int lineIndex = 0;
		while (fgets(pBuffer, MAX_LINE_SIZE, file))
		{
			std::string line = pBuffer;
			if (std::string::npos != line.find("local damageInfo = {"))
			{
				startLine = lineIndex;
				nBrakets = std::count(line.begin(), line.end(), '{');
			}
			else
			{
				if (0 < nBrakets)
				{
					nBrakets = nBrakets + std::count(line.begin(), line.end(), '{') - std::count(line.begin(), line.end(), '}');
					if (0 == nBrakets)
					{
						endLine = lineIndex;
						break;
					}
				}
			}

			if (line.npos != line.find("mainid"))
			{
				curNode.id = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1);
				curNode.block = "";
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

		fclose(file);
		file = NULL;
	}
}