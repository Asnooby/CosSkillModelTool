#pragma once
#include "CSingleton.h"

EDIT_TYPE CSingleton::gEditType = EDIT_TYPE::NONE;
CEnvParams CSingleton::gEnvParams;
CSkinProcessor CSingleton::gUnitProcessor;
CSkillIniProcessor CSingleton::gSkillIniProcessor;
CSkillPrtCProcessor CSingleton::gSkillPrtCProcessor;
CSkillPrtPProcessor CSingleton::gSkillPrtPProcessor;
CSkillDataLuaProcessor CSingleton::gSkillDataLuaProcesser;
CSkinSpDescribeProcessor CSingleton::gSkinSpDescribeProcessor;
CSkillConditionIniProcessor CSingleton::gSkillConditionIniProcessor;
CSkillPresentationIniProcessor CSingleton::gSkillPresentationIniProcessor;
CUnitsXmlProcessor CSingleton::gUnitsXmlProcessor;
CRolesXmlProcessor CSingleton::gRolesXmlProcessor;

CSingleton::CSingleton()
{

}