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
	std::string skinName;
	std::string skillId;
	std::string skillName;
} MODEL_INFO;