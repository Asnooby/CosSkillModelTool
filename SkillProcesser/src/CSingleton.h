#pragma onceo
#include "CEnvParams.h"
#include "processor/CUnitProcessor.h"
#include "processor/CSkillIniProcessor.h"
#include "processor/CSkillPrtCProcessor.h"
#include "processor/CSkillPrtPProcessor.h"
#include "processor/CSkillDataLuaProcessor.h"
#include "processor/CSkinSpDescribeProcessor.h"
#include "processor/CSkillConditionIniProcessor.h"
#include "processor/CSkillPresentationIniProcessor.h"

#include "IDllExport.h"

class DLL_API CSingleton
{
	CSingleton();
public:
	static CEnvParams gEnvParams;
	static CUnitProcessor gUnitProcessor;
	static CSkillIniProcessor gSkillIniProcessor;
	static CSkillPrtCProcessor gSkillPrtCProcessor;
	static CSkillPrtPProcessor gSkillPrtPProcessor;
	static CSkillDataLuaProcessor gSkillDataLuaProcesser;
	static CSkinSpDescribeProcessor gSkinSpDescribeProcessor;
	static CSkillConditionIniProcessor gSkillConditionIniProcessor;
	static CSkillPresentationIniProcessor gSkillPresentationIniProcessor;
};
