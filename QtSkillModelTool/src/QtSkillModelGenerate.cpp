#include "QtSkillModelGenerate.h"
#include "src/CSingleton.h"
#include "src/tools/CommonFuncs.h"

QtSkillModelGenerate::QtSkillModelGenerate(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    initUI();
    bindSignalEvent();
}

void QtSkillModelGenerate::showEvent(QShowEvent* event)
{
	ui.group_skillid->setVisible(CSingleton::gEditType == EDIT_TYPE::SKILL_MODEL);
	ui.group_roleid->setVisible(CSingleton::gEditType == EDIT_TYPE::SKIN_MODEL);
}

void QtSkillModelGenerate::bindSignalEvent()
{
    connect(ui.btn_generate, SIGNAL(clicked()), this, SLOT(onBtnGenerateClicked()));
    connect(ui.btn_export, SIGNAL(clicked()), this, SLOT(onBtnExportClicked()));
}

void QtSkillModelGenerate::initUI()
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setModal(true);

	m_pPreview = new QtConfigPreview(this);
	ui.gLayoutPreview->addWidget(m_pPreview);
	m_pPreview->show();

	m_pPreview->RegisterFuncGetContent([&](CONTENT_TYPE type)->std::string {
		std::vector<std::string> skillIds;
		CSingleton::gSkillDataLuaProcesser.GetSkillIds(m_info.skillId, skillIds);
		std::map<std::string, std::string> prtSkillNames;
		CSingleton::gSkillPresentationIniProcessor.GetSkillPrtName(m_info.skinId, skillIds, prtSkillNames, m_info.skillId, m_infoGenerate.skillId);

		switch (type)
		{
		case CONTENT_TYPE::SKILLDATA_LUA:
		{
			if (m_infoGenerate.heroId.empty() || 0 == m_infoGenerate.heroId.compare(m_info.heroId))
			{
				return CSingleton::gSkillDataLuaProcesser.GetSkillTotalContent(m_info.skillId);
			}
			else
			{
				std::map<std::string, std::string> skillData;
				CSingleton::gSkillDataLuaProcesser.GetSkillData(m_info.skillId, skillData, m_infoGenerate.skillId);
				std::map<std::string, std::string> damageData;
				CSingleton::gSkillDataLuaProcesser.GetDamageData(m_info.skillId, damageData, m_infoGenerate.skillId);
				return m_SkillDataLuaProcesser.GenerateTotalContent(skillData, damageData);
			}
		}
		case CONTENT_TYPE::SKILL_INI:
		{
			if (m_infoGenerate.heroId.empty() || 0 == m_infoGenerate.heroId.compare(m_info.heroId))
			{
				return CSingleton::gSkillIniProcessor.GetSkillTotalContent(skillIds);
			}
			else
			{
				std::map<std::string, std::string> skillData;
				CSingleton::gSkillIniProcessor.GetSkillData(skillIds, m_info.skillId, m_infoGenerate.skillId, skillData);
				return m_SkillIniProcessor.GenerateTotalContent(skillData);
			}
		}
		case CONTENT_TYPE::SKILLCONDITION_INI:
		{
			if (m_infoGenerate.heroId.empty() || 0 == m_infoGenerate.heroId.compare(m_info.heroId))
			{
				return CSingleton::gSkillConditionIniProcessor.GetSkillTotalContent(skillIds);
			}
			else
			{
				std::map<std::string, std::string> skillData;
				CSingleton::gSkillConditionIniProcessor.GetSkillData(skillIds, m_info.skillId, m_infoGenerate.skillId, skillData);
				return m_SkillConditionIniProcessor.GenerateTotalContent(skillData);
			}
		}
		case CONTENT_TYPE::SKILLPRESENTATION_INI:
		{
			if (m_infoGenerate.skinId.empty() || 0 == m_infoGenerate.skinId.compare(m_info.skinId))
			{
				return CSingleton::gSkillPresentationIniProcessor.GetSkillTotalContent(skillIds);
			}
			else
			{
				m_SkillPresentationIniProcessor.SetSkinName(prtSkillNames, m_info.skinName, m_infoGenerate.skinName);
				return m_SkillPresentationIniProcessor.GenerateTotalContent(m_infoGenerate.skinId, prtSkillNames);
			}
		}
		case CONTENT_TYPE::PRT_C_XML:
		{
			if (m_infoGenerate.skinId.empty() || 0 == m_infoGenerate.skinId.compare(m_info.skinId))
			{
				return CSingleton::gSkillPrtCProcessor.GetSkillTotalContent(prtSkillNames);
			}
			else
			{
				std::map<std::string, std::string> skillData;
				CSingleton::gSkillPrtCProcessor.GetSkillPrtData(prtSkillNames, skillData, m_info.skinName, m_infoGenerate.skinName);
				return m_SkillPrtCProcessor.GenerateTotalContent(skillData);
			}
		}
		case CONTENT_TYPE::PRT_P_XML:
		{
			std::set<std::string> setPrtNames;
			CSingleton::gSkillPrtCProcessor.GetPrtNames(prtSkillNames, setPrtNames);
			if (m_infoGenerate.skinId.empty() || 0 == m_infoGenerate.skinId.compare(m_info.skinId))
			{
				return CSingleton::gSkillPrtPProcessor.GetPrtTotalContent(setPrtNames);
			}
			else
			{
				std::map<std::string, std::string> prtData;
				CSingleton::gSkillPrtPProcessor.GetPrtData(setPrtNames, prtData, m_info.skinName, m_infoGenerate.skinName);
				return m_SkillPrtPProcessor.GenerateTotalContent(prtData);
			}
		}
		case CONTENT_TYPE::UNITS_XML:
		{
			if (m_infoGenerate.skinId.empty() || 0 == m_infoGenerate.skinId.compare(m_info.skinId))
			{
				return CSingleton::gUnitsXmlProcessor.GetTotalContent(m_info.skinId);
			}
			else
			{
				std::pair<std::string, std::string> cfg;
				CSingleton::gUnitsXmlProcessor.GetUnitData(m_info.skinId, cfg, m_infoGenerate.skinId, m_infoGenerate.roleId, m_info.skinName, m_infoGenerate.skinName);
				return m_UnitsXmlProcessor.GenerateTotalContent(cfg.first, cfg.second);
			}
		}
		case CONTENT_TYPE::ROLES_XML:
		{
			if (m_infoGenerate.skinId.empty() || 0 == m_infoGenerate.skinId.compare(m_info.skinId))
			{
				return CSingleton::gRolesXmlProcessor.GetTotalContent(m_info.roleId);
			}
			else
			{
				std::pair<std::string, std::string> cfg;
				CSingleton::gRolesXmlProcessor.GetRoleData(m_info.roleId, cfg, m_infoGenerate.roleId, m_info.skinName, m_infoGenerate.skinName);
				return m_RolesXmlProcessor.GenerateTotalContent(cfg.first, cfg.second);
			}
		}
		case CONTENT_TYPE::SKIN_PRT_P_XML:
		{
			std::set<std::string> setPrtNames;
			CSingleton::gUnitsXmlProcessor.GetBasePresentations(m_info.skinId, setPrtNames);
			if (m_infoGenerate.skinId.empty() || 0 == m_infoGenerate.skinId.compare(m_info.skinId))
			{
				return CSingleton::gSkillPrtPProcessor.GetPrtTotalContent(setPrtNames);
			}
			else
			{
				std::map<std::string, std::string> prtData;
				CSingleton::gSkillPrtPProcessor.GetPrtData(setPrtNames, prtData, m_info.skinName, m_infoGenerate.skinName);
				return m_SkillPrtPProcessor.GenerateTotalContent(prtData);
			}
		}
		}
		return std::string("");
	});
}

void QtSkillModelGenerate::onBtnGenerateClicked()
{
	m_infoGenerate.skinId = ui.edit_dst_skinid->text().toStdString();
	if (m_infoGenerate.skinId.empty())
	{
		m_infoGenerate.skinId = m_info.skinId;
	}
	m_infoGenerate.skinName = ui.edit_dst_skinname->text().toStdString();
	if (m_infoGenerate.skinName.empty())
	{
		m_infoGenerate.skinName = m_info.skinName;
	}
	m_infoGenerate.heroId = m_infoGenerate.skinId.substr(0, m_infoGenerate.skinId.length() - 3);
	if (m_infoGenerate.heroId.empty())
	{
		m_infoGenerate.heroId = m_info.heroId;
	}
	m_infoGenerate.skillId = ui.edit_dst_skillid->text().toStdString();
	if (m_infoGenerate.skillId.empty())
	{
		m_infoGenerate.skillId = m_info.skillId;
	}
	m_infoGenerate.roleId = ui.edit_dst_roleid->text().toStdString();
	if (m_infoGenerate.roleId.empty())
	{
		m_infoGenerate.roleId = m_info.roleId;
	}

	setModelInfo(m_infoGenerate.heroId, m_infoGenerate.skinId, m_infoGenerate.skinName);
}
void QtSkillModelGenerate::onBtnExportClicked()
{
	m_infoGenerate.skinId = ui.edit_dst_skinid->text().toStdString();
	if (m_infoGenerate.skinId.empty())
	{
		m_infoGenerate.skinId = m_info.skinId;
	}
	m_infoGenerate.skinName = ui.edit_dst_skinname->text().toStdString();
	if (m_infoGenerate.skinName.empty())
	{
		m_infoGenerate.skinName = m_info.skinName;
	}
	m_infoGenerate.heroId = m_infoGenerate.skinId.substr(0, m_infoGenerate.skinId.length() - 3);
	m_infoGenerate.skillId = ui.edit_dst_skillid->text().toStdString();
	if (m_infoGenerate.skillId.empty())
	{
		m_infoGenerate.skillId = m_info.skillId;
	}
	m_infoGenerate.roleId = ui.edit_dst_roleid->text().toStdString();
	if (m_infoGenerate.roleId.empty())
	{
		m_infoGenerate.roleId = m_info.roleId;
	}

	setModelInfo(m_infoGenerate.heroId, m_infoGenerate.skinId, m_infoGenerate.skinName);

	switch (CSingleton::gEditType)
	{
	case EDIT_TYPE::SKILL_MODEL:
	{
		exportSkillConfig();
	}break;
	case EDIT_TYPE::SKIN_MODEL:
	{
		exportSkinConfig();
	}break;
	}
	completeSkinModelFiles();
}

void QtSkillModelGenerate::exportSkillConfig()
{
	std::vector<std::string> skillIds;
	CSingleton::gSkillDataLuaProcesser.GetSkillIds(m_info.skillId, skillIds);

	if (m_infoGenerate.heroId.compare(m_info.heroId) || 0 == m_infoGenerate.skillId.compare(m_info.skillId))
	{
		// skilldata.lua
		std::map<std::string, std::string> skillData;
		CSingleton::gSkillDataLuaProcesser.GetSkillData(m_info.skillId, skillData, m_infoGenerate.skillId);
		std::map<std::string, std::string> damageData;
		CSingleton::gSkillDataLuaProcesser.GetDamageData(m_info.skillId, damageData, m_infoGenerate.skillId);
		m_SkillDataLuaProcesser.ExportGeneratedTotalContent(skillData, damageData);

		// skill.ini
		skillData.clear();
		CSingleton::gSkillIniProcessor.GetSkillData(skillIds, m_info.skillId, m_infoGenerate.skillId, skillData);
		m_SkillIniProcessor.ExportGeneratedTotalContent(skillData);

		//skillcondition.ini
		skillData.clear();
		CSingleton::gSkillConditionIniProcessor.GetSkillData(skillIds, m_info.skillId, m_infoGenerate.skillId, skillData);
		m_SkillConditionIniProcessor.ExportGeneratedTotalContent(skillData);
	}
	if (m_infoGenerate.skinId.compare(m_info.skinId) || 0 == m_infoGenerate.skillId.compare(m_info.skillId))
	{
		std::map<std::string, std::string> prtSkillNames;
		CSingleton::gSkillPresentationIniProcessor.GetSkillPrtName(m_info.skinId, skillIds, prtSkillNames, m_info.skillId, m_infoGenerate.skillId);
		std::map<std::string, std::string> skillData;
		CSingleton::gSkillPrtCProcessor.GetSkillPrtData(prtSkillNames, skillData, m_info.skinName, m_infoGenerate.skinName);
		std::set<std::string> setPrtNames;
		CSingleton::gSkillPrtCProcessor.GetPrtNames(prtSkillNames, setPrtNames);

		//skillpresentation.ini
		m_SkillPresentationIniProcessor.SetSkinName(prtSkillNames, m_info.skinName, m_infoGenerate.skinName);
		m_SkillPresentationIniProcessor.ExportGeneratedTotalContent(m_infoGenerate.skinId, prtSkillNames);

		//_c.prt
		m_SkillPrtCProcessor.ExportGeneratedTotalContent(skillData);

		//_p.prt
		std::map<std::string, std::string> prtData;
		CSingleton::gSkillPrtPProcessor.GetPrtData(setPrtNames, prtData, m_info.skinName, m_infoGenerate.skinName);
		m_SkillPrtPProcessor.ExportGeneratedTotalContent(prtData);
	}
}

void QtSkillModelGenerate::exportSkinConfig()
{
	if (m_infoGenerate.skinId.compare(m_info.skinId))
	{
		std::pair<std::string, std::string> cfgUnits;
		CSingleton::gUnitsXmlProcessor.GetUnitData(m_info.skinId, cfgUnits, m_infoGenerate.skinId, m_infoGenerate.roleId, m_info.skinName, m_infoGenerate.skinName);
		m_UnitsXmlProcessor.ExportGeneratedTotalContent(cfgUnits.first, cfgUnits.second);

		std::pair<std::string, std::string> cfgRoles;
		CSingleton::gRolesXmlProcessor.GetRoleData(m_info.roleId, cfgRoles, m_infoGenerate.roleId, m_info.skinName, m_infoGenerate.skinName);
		m_RolesXmlProcessor.ExportGeneratedTotalContent(cfgRoles.first, cfgRoles.second);

		std::set<std::string> setPrtNames;
		CSingleton::gUnitsXmlProcessor.GetBasePresentations(m_info.skinId, setPrtNames);
		std::map<std::string, std::string> prtData;
		CSingleton::gSkillPrtPProcessor.GetPrtData(setPrtNames, prtData, m_info.skinName, m_infoGenerate.skinName);
		m_SkillPrtPProcessor.ExportGeneratedTotalContent(prtData);
	}
}

void QtSkillModelGenerate::SetModelInfo(MODEL_INFO info)
{
	m_pPreview->RefreshTab();
    m_info = info;
	m_infoGenerate = info;
    ui.edit_src_skinid->setText(QString::fromUtf8(info.skinId.c_str()));
    ui.edit_src_skinname->setText(QString::fromUtf8(info.skinName.c_str()));
    ui.edit_src_skillid->setText(QString::fromUtf8(info.skillId.c_str()));
	ui.edit_src_roleid->setText(QString::fromUtf8(info.roleId.c_str()));

	setModelInfo(info.heroId, info.skinId, info.skinName);
}

void QtSkillModelGenerate::setModelInfo(std::string heroId, std::string skinId, std::string skinName)
{
	m_SkillIniProcessor.SetHeroId(heroId);
	m_SkillDataLuaProcesser.SetHeroId(heroId);
	m_SkillConditionIniProcessor.SetHeroId(heroId);
	m_SkillPresentationIniProcessor.SetHeroId(heroId);
	m_SkillPresentationIniProcessor.SetSkinId(skinId);
	m_UnitsXmlProcessor.SetSkinId(skinId);
	m_RolesXmlProcessor.SetSkinId(skinId);

	auto skinPath = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + skinId + "/presentations/";
	std::string pathPrtC = DeepFindInDir(skinPath, "_c.prt");
	if (pathPrtC.empty())
	{
		pathPrtC = skinPath + "player/" + skinName + "/" + skinName + "_c.prt";
	}
	auto prtNameC = pathPrtC;
	if (!prtNameC.empty())
	{
		std::replace(prtNameC.begin(), prtNameC.end(), '\\', '/');
		auto index = prtNameC.rfind("/");
		prtNameC = prtNameC.substr(index + 1, prtNameC.length() - index - 1);
	}
	std::string pathPrtP = DeepFindInDir(skinPath, "_p.prt");
	if (pathPrtP.empty())
	{
		pathPrtP = skinPath + "player/" + skinName + "/" + skinName + "_p.prt";
	}
	auto prtNameP = pathPrtP;
	if (!prtNameP.empty())
	{
		std::replace(prtNameP.begin(), prtNameP.end(), '\\', '/');
		auto index = prtNameP.rfind("/");
		prtNameP = prtNameP.substr(index + 1, prtNameP.length() - index - 1);
	}
	m_SkillPrtCProcessor.SetPath(pathPrtC);
	m_SkillPrtPProcessor.SetPath(pathPrtP);

	m_pPreview->SetPrtName(prtNameP, prtNameC);
}

void QtSkillModelGenerate::completeSkinModelFiles()
{
	std::string HeroFolder = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + m_infoGenerate.heroId + "_common/";
	std::string SkinFolder = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + m_infoGenerate.skinId + "/";
	std::set<std::string> FileComplement(
		{
			HeroFolder + "data/config/" + "creatureinfo.ini",
			HeroFolder + "data/config/" + "p2pskill.ini",
			HeroFolder + "data/config/" + "serverstate.ini",
			HeroFolder + "data/config/" + "skill.ini",
			HeroFolder + "data/config/" + "skillcondition.ini",
			HeroFolder + "data/config/" + "skilldata.lua",
			HeroFolder + "data/config/" + "skillpresentation.ini",
			HeroFolder + "data/config/" + "sounddata.lua",
			HeroFolder + "data/config/" + "summonbiont.ini",
			HeroFolder + "data/config/" + "units.xml",
			HeroFolder + "data/config/" + "unitvoice.ini",
			HeroFolder + "presentations/" + "roles.xml",

			SkinFolder + "data/config/" + "creatureinfo.ini",
			SkinFolder + "data/config/" + "p2pskill.ini",
			SkinFolder + "data/config/" + "serverstate.ini",
			SkinFolder + "data/config/" + "skill.ini",
			SkinFolder + "data/config/" + "skillcondition.ini",
			SkinFolder + "data/config/" + "skillpresentation.ini",
			SkinFolder + "data/config/" + "sounddata.lua",
			SkinFolder + "data/config/" + "summonbiont.ini",
			SkinFolder + "data/config/" + "units.xml",
			SkinFolder + "data/config/" + "unitvoice.ini",
			SkinFolder + "presentations/" + "roles.xml",
			m_SkillPrtPProcessor.GetPath(),
			m_SkillPrtCProcessor.GetPath()
		}
	);

	for (const auto& path : FileComplement)
	{
		createFile(path);
	}
}