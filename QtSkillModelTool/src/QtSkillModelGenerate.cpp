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
			if (m_infoGenerate.skinId.empty() || 0 == m_infoGenerate.heroId.compare(m_info.skinId))
			{
				return CSingleton::gSkillPresentationIniProcessor.GetSkillTotalContent(skillIds);
			}
			else
			{
				m_SkillPresentationIniProcessor.SetSkinName(prtSkillNames, m_info.skinName, m_infoGenerate.skinName);
				return m_SkillPresentationIniProcessor.GenerateTotalContent(m_infoGenerate.skinId, prtSkillNames);
			}
		}
		case CONTENT_TYPE::PRT_C_INI:
		{
			std::map<std::string, std::string> skillData;
			CSingleton::gSkillPrtCProcessor.GetSkillPrtData(prtSkillNames, skillData, m_info.skinName, m_infoGenerate.skinName);
			return m_SkillPrtCProcessor.GenerateTotalContent(skillData);
		}
		case CONTENT_TYPE::PRT_P_INI:
		{
			std::set<std::string> setPrtNames;
			CSingleton::gSkillPrtCProcessor.GetPrtNames(prtSkillNames, setPrtNames);
			std::map<std::string, std::string> prtData;
			CSingleton::gSkillPrtPProcessor.GetPrtData(setPrtNames, prtData, m_info.skinName, m_infoGenerate.skinName);
			return m_SkillPrtPProcessor.GenerateTotalContent(prtData);
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

	setModelInfo(m_infoGenerate.heroId, m_infoGenerate.skinId, m_infoGenerate.skinName);

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

void QtSkillModelGenerate::SetModelInfo(MODEL_INFO info)
{
    m_info = info;
	m_infoGenerate = info;
    ui.edit_src_skinid->setText(QString::fromUtf8(info.skinId.c_str()));
    ui.edit_src_skinname->setText(QString::fromUtf8(info.skinName.c_str()));
    ui.edit_src_skillid->setText(QString::fromUtf8(info.skillId.c_str()));

	setModelInfo(info.heroId, info.skinId, info.skinName);
}

void QtSkillModelGenerate::setModelInfo(std::string heroId, std::string skinId, std::string skinName)
{
	m_SkillIniProcessor.SetHeroId(heroId);
	m_SkillDataLuaProcesser.SetHeroId(heroId);
	m_SkillConditionIniProcessor.SetHeroId(heroId);
	m_SkillPresentationIniProcessor.SetHeroId(heroId);
	m_SkillPresentationIniProcessor.SetSkinId(skinId);

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