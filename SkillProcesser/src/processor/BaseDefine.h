#pragma once

#include <string>

#define MAX_LINE_SIZE 1024
typedef struct
{
	int startLine = 0;
	int endLine = 0;
	std::string id;
	std::string block;

	void init()
	{
		startLine = 0;
		endLine = 0;
		id.clear();
		block.clear();
	}
} SKILL_BLOCK;

typedef struct
{
	std::string heroId;
	std::string skinId;
	std::string roleId;
	std::string skinName;
	std::string skillId;
	std::string skillName;
} MODEL_INFO;

enum class CONTENT_TYPE
{
	NONE = 0,
	SKILL_START = 1,
	SKILLDATA_LUA = SKILL_START,
	SKILL_INI,
	SKILLCONDITION_INI,
	SKILLPRESENTATION_INI,
	PRT_C_XML,
	PRT_P_XML,
	SKIN_START,
	UNITS_XML = SKIN_START,
	ROLES_XML,
	SKIN_PRT_P_XML,
};

enum class EDIT_TYPE
{
	NONE,
	SKIN_MODEL,
	SKILL_MODEL,
};