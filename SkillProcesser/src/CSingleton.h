#pragma once

#include "CEnvParams.h"
#include "processor/CSkinProcessor.h"
#include "processor/CSkillIniProcessor.h"
#include "processor/CSkillPrtCProcessor.h"
#include "processor/CSkillPrtPProcessor.h"
#include "processor/CSkillDataLuaProcessor.h"
#include "processor/CSkinSpDescribeProcessor.h"
#include "processor/CSkillConditionIniProcessor.h"
#include "processor/CSkillPresentationIniProcessor.h"
#include "processor/CUnitsXmlProcessor.h"
#include "processor/CRolesXmlProcessor.h"

#include "IDllExport.h"

class DLL_API CSingleton
{
	CSingleton();
public:
	static EDIT_TYPE gEditType;
	static CEnvParams gEnvParams;
	static CSkinProcessor gUnitProcessor;
	static CSkillIniProcessor gSkillIniProcessor;
	static CSkillPrtCProcessor gSkillPrtCProcessor;
	static CSkillPrtPProcessor gSkillPrtPProcessor;
	static CSkillDataLuaProcessor gSkillDataLuaProcesser;
	static CSkinSpDescribeProcessor gSkinSpDescribeProcessor;
	static CSkillConditionIniProcessor gSkillConditionIniProcessor;
	static CSkillPresentationIniProcessor gSkillPresentationIniProcessor;
	static CUnitsXmlProcessor gUnitsXmlProcessor;
	static CRolesXmlProcessor gRolesXmlProcessor;
};
