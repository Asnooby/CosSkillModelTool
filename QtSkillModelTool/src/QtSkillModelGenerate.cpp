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
		m_SkillDataLuaProcesser.GetSkillIds(m_info.skillId, skillIds);
		std::map<std::string, std::string> prtSkillNames;
		m_SkillPresentationIniProcessor.GetSkillPrtName(m_info.skinId, skillIds, prtSkillNames);

		auto newSkillId = ui.edit_dst_skillid->text().toStdString();
		auto skinId = ui.edit_dst_skinid->text().toStdString();
		auto heroId = skinId.substr(0, skinId.length() - 3);

		switch (type)
		{
		case CONTENT_TYPE::SKILLDATA_LUA:
		{
			if (heroId.empty() || 0 == heroId.compare(m_info.heroId))
			{
				return m_SkillDataLuaProcesser.GetSkillTotalContent(m_info.skillId);
			}
			else
			{
				std::map<std::string, std::string> skillData;
				CSingleton::gSkillDataLuaProcesser.GetSkillData(m_info.skillId, skillData, newSkillId);
				std::map<std::string, std::string> damageData;
				CSingleton::gSkillDataLuaProcesser.GetDamageData(m_info.skillId, damageData, newSkillId);
				return m_SkillDataLuaProcesser.GenerateTotalContent(heroId, skillData, damageData);
			}
		}
		case CONTENT_TYPE::SKILL_INI:
		{
			return m_SkillIniProcessor.GetSkillTotalContent(skillIds);
		}
		case CONTENT_TYPE::SKILLCONDITION_INI:
		{
			return m_SkillConditionIniProcessor.GetSkillTotalContent(skillIds);
		}
		case CONTENT_TYPE::SKILLPRESENTATION_INI:
		{
			return m_SkillPresentationIniProcessor.GetSkillTotalContent(skillIds);
		}
		case CONTENT_TYPE::PRT_C_INI:
		{
			return m_SkillPrtCProcessor.GetSkillTotalContent(prtSkillNames);
		}
		case CONTENT_TYPE::PRT_P_INI:
		{
			std::set<std::string> setPrtNames;
			m_SkillPrtCProcessor.GetPrtNames(prtSkillNames, setPrtNames);
			return m_SkillPrtPProcessor.GetSkillTotalContent(setPrtNames);
		}
		}
		return std::string("");
	});
}

void QtSkillModelGenerate::onBtnGenerateClicked()
{
	auto skinId = ui.edit_dst_skinid->text().toStdString();
	auto heroId = skinId.substr(0, skinId.length() - 3);
	m_SkillDataLuaProcesser.SetHeroId(heroId);

	m_pPreview->RefreshContent();
}
void QtSkillModelGenerate::onBtnExportClicked()
{

}

void QtSkillModelGenerate::SetModelInfo(MODEL_INFO info)
{
    m_info = info;
    ui.edit_src_skinid->setText(QString::fromUtf8(info.skinId.c_str()));
    ui.edit_src_skinname->setText(QString::fromUtf8(info.skinName.c_str()));
    ui.edit_src_skillid->setText(QString::fromUtf8(info.skillId.c_str()));
    ui.edit_src_skillname->setText(QString::fromUtf8(info.skillId.c_str()));

	m_SkillIniProcessor.SetHeroId(info.heroId);
	m_SkillDataLuaProcesser.SetHeroId(info.heroId);
	m_SkillConditionIniProcessor.SetHeroId(info.heroId);
	m_SkillPresentationIniProcessor.SetHeroId(info.heroId);
	m_SkillPresentationIniProcessor.SetSkinId(info.skinId);

	auto skinPath = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + info.skinId + "/presentations/";
	auto pathPrtC = DeepFindInDir(skinPath, "_c.prt");
	auto prtNameC = pathPrtC;
	if (!prtNameC.empty())
	{
		std::replace(prtNameC.begin(), prtNameC.end(), '\\', '/');
		auto index = prtNameC.rfind("/");
		prtNameC = prtNameC.substr(index + 1, prtNameC.length() - index - 1);
	}
	auto pathPrtP = DeepFindInDir(skinPath, "_p.prt");
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