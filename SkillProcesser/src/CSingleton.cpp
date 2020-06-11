#pragma once
#include "CSingleton.h"

CEnvParams CSingleton::gEnvParams;
CUnitProcessor CSingleton::gUnitProcessor;
CSkillIniProcessor CSingleton::gSkillIniProcessor;
CSkillPrtCProcessor CSingleton::gSkillPrtCProcessor;
CSkillPrtPProcessor CSingleton::gSkillPrtPProcessor;
CSkillDataLuaProcessor CSingleton::gSkillDataLuaProcesser;
CSkinSpDescribeProcessor CSingleton::gSkinSpDescribeProcessor;
CSkillConditionIniProcessor CSingleton::gSkillConditionIniProcessor;
CSkillPresentationIniProcessor CSingleton::gSkillPresentationIniProcessor;

CSingleton::CSingleton()
{

}