#pragma once
#include "CEnvParams.h"
#include "CUnitProcesser.h"
#include "CSkillDataLuaProcesser.h"

#include "IDllExport.h"

class DLL_API CSingleton
{
	CSingleton();
public:
	static CEnvParams gEnvParams;
	static CUnitProcesser gUnitProcesser;
	static CSkillDataLuaProcesser gSkillDataLuaProcesser;
};
