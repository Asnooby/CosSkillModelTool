#include "CSkinSpDescribeProcessor.h"
#include "../tools/CommonFuncs.h"
#include "../third/cjson/cJSON.h"
#include "BaseDefine.h"

CSkinSpDescribeProcessor::CSkinSpDescribeProcessor()
{

}

bool CSkinSpDescribeProcessor::init(std::string path)
{
	FILE* file;
	fopen_s(&file, path.c_str(), "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		auto nSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		char* pBuffer = new char[nSize];
		memset(pBuffer, 0, nSize);
		fread(pBuffer, 1, nSize, file);
		fclose(file);

		auto pJson = cJSON_Parse(pBuffer);
		if (pJson)
		{
			auto pItem = cJSON_GetObjectItem(pJson, "skill");
			if (pItem)
			{
				auto pItemSkin = pItem->child;
				while (pItemSkin)
				{
					auto pItemSkill = pItemSkin->child;
					while (pItemSkill)
					{
						m_skills[pItemSkin->string][pItemSkill->string] = std::to_string(pItemSkill->valueint);
						pItemSkill = pItemSkill->next;
					}
					pItemSkin = pItemSkin->next;
				}
			}
			pItem = cJSON_GetObjectItem(pJson, "damage");
			if (pItem)
			{
				auto pItemSkin = pItem->child;
				while (pItemSkin)
				{
					auto pItemSkill = pItemSkin->child;
					while (pItemSkill)
					{
						m_damages[pItemSkin->string][pItemSkill->string] = std::to_string(pItemSkill->valueint);
						pItemSkill = pItemSkill->next;
					}
					pItemSkin = pItemSkin->next;
				}
			}
			return true;
		}

		delete[] pBuffer;
		pBuffer = nullptr;
	}

	return false;
}

bool CSkinSpDescribeProcessor::isSpecialSkillId(std::string skillId)
{
	for (auto iter = m_skills.begin(); iter != m_skills.end(); iter++)
	{
		for (auto iterSkill = iter->second.begin(); iterSkill != iter->second.end(); iterSkill++)
		{
			if (0 == skillId.compare(iterSkill->second))
			{
				return true;
			}
		}
	}
	for (auto iter = m_damages.begin(); iter != m_damages.end(); iter++)
	{
		for (auto iterSkill = iter->second.begin(); iterSkill != iter->second.end(); iterSkill++)
		{
			if (0 == skillId.compare(iterSkill->second))
			{
				return true;
			}
		}
	}
	return false;
}

std::string CSkinSpDescribeProcessor::GetSkinSpSkillId(std::string skinId, std::string skillId)
{
	auto iterSkin = m_skills.find(skinId);
	if (iterSkin != m_skills.end())
	{
		for (auto iterSkill = iterSkin->second.begin(); iterSkill != iterSkin->second.end(); iterSkill++)
		{
			if (0 == skillId.compare(iterSkill->second))
			{
				return iterSkill->first;
			}
		}
	}

	return skillId;
}

std::string CSkinSpDescribeProcessor::GetSkinSpDamageId(std::string skinId, std::string skillId)
{
	auto iterSkin = m_damages.find(skinId);
	if (iterSkin != m_damages.end())
	{
		for (auto iterSkill = iterSkin->second.begin(); iterSkill != iterSkin->second.end(); iterSkill++)
		{
			if (0 == skillId.compare(iterSkill->second))
			{
				return iterSkill->first;
			}
		}
	}

	return skillId;
}