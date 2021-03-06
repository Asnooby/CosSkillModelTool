#include "QtSkillModelTool.h"
#include "src/CSingleton.h"
#include "src/processor/CSkinProcessor.h"
#include "src/processor/CSkillDataLuaProcessor.h"
#include "src/tools/CommonFuncs.h"
#include "qstringlist.h"
#include "qstringlistmodel.h"

QtSkillModelTool::QtSkillModelTool(QWidget *parent)
    : QMainWindow(parent)
	, m_pPreview(nullptr)
	, m_pDlgGenerator(nullptr)
{
    ui.setupUi(this);

	initUI();
    bindSignalEvent();
}

void QtSkillModelTool::bindSignalEvent()
{
    connect(ui.btnConfirm, SIGNAL(clicked()), this, SLOT(onClickButtonBtnConfirm()));
	connect(ui.btn_generate, SIGNAL(clicked()), this, SLOT(onClickButtonBtnGenerate()));
	connect(ui.list_view_heros, SIGNAL(clicked(QModelIndex)), this, SLOT(onListViewHerosIndexMoved(QModelIndex)));
	connect(ui.list_view_skins, SIGNAL(clicked(QModelIndex)), this, SLOT(onListViewSkinsIndexMoved(QModelIndex)));
	connect(ui.list_view_skills, SIGNAL(clicked(QModelIndex)), this, SLOT(onListViewSkillsIndexMoved(QModelIndex)));
	connect(ui.edit_search_heroid, SIGNAL(textChanged(const QString&)), this, SLOT(onEditChange(const QString&)));
	connect(ui.radio_skin, SIGNAL(toggled(bool)), this, SLOT(onRadioEditTypeToggled(bool)));
	connect(ui.radio_skill, SIGNAL(toggled(bool)), this, SLOT(onRadioEditTypeToggled(bool)));
}

void QtSkillModelTool::initUI()
{
	ui.editPath->setText(QString::fromUtf8("I:/git/moba2.0"));
	ui.edit_search_heroid->setValidator(new QRegExpValidator(QRegExp("[0-9]+$"), ui.edit_search_heroid));
	ui.list_view_heros->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.list_view_skins->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.list_view_skills->setEditTriggers(QAbstractItemView::NoEditTriggers);

	m_pPreview = new QtConfigPreview(ui.frame_preview);
	m_pPreview->show();

	m_pPreview->RegisterFuncGetContent([&](CONTENT_TYPE type)->std::string {
		QStringListModel* pSlm = (QStringListModel*)ui.list_view_skills->model();
		if (nullptr == pSlm)
		{
			return "";
		}

		QVariant variant = pSlm->data(ui.list_view_skills->currentIndex(), Qt::DisplayRole);
		if (!variant.isValid())
		{
			return "";
		}
		std::string skillId = variant.toString().toStdString();
		auto index = skillId.find("|");
		if (skillId.npos != index)
		{
			skillId = skillId.substr(0, index - 1);
			skillId = SelectNumber(skillId);
		}

		pSlm = (QStringListModel*)ui.list_view_skins->model();
		variant = pSlm->data(ui.list_view_skins->currentIndex(), Qt::DisplayRole);
		if (!variant.isValid())
		{
			return "";
		}
		std::string skinId = variant.toString().toStdString();
		index = skinId.find("|");
		if (skinId.npos != index)
		{
			skinId = skinId.substr(0, index - 1);
			skinId = SelectNumber(skinId);
		}

		std::vector<std::string> skillIds;
		CSingleton::gSkillDataLuaProcesser.GetSkillIds(skillId, skillIds);
		std::map<std::string, std::string> prtSkillNames;
		CSingleton::gSkillPresentationIniProcessor.GetSkillPrtName(skinId, skillIds, prtSkillNames);
		switch (type)
		{
			case CONTENT_TYPE::SKILLDATA_LUA:
			{
				return CSingleton::gSkillDataLuaProcesser.GetSkillTotalContent(skillId);
			}
			case CONTENT_TYPE::SKILL_INI:
			{
				return CSingleton::gSkillIniProcessor.GetSkillTotalContent(skillIds);
			}
			case CONTENT_TYPE::SKILLCONDITION_INI:
			{
				return CSingleton::gSkillConditionIniProcessor.GetSkillTotalContent(skillIds);
			}
			case CONTENT_TYPE::SKILLPRESENTATION_INI:
			{
				return CSingleton::gSkillPresentationIniProcessor.GetSkillTotalContent(skillIds);
			}
			case CONTENT_TYPE::PRT_C_XML:
			{
				return CSingleton::gSkillPrtCProcessor.GetSkillTotalContent(prtSkillNames);
			}
			case CONTENT_TYPE::PRT_P_XML:
			{
				std::set<std::string> setPrtNames;
				CSingleton::gSkillPrtCProcessor.GetPrtNames(prtSkillNames, setPrtNames);
				return CSingleton::gSkillPrtPProcessor.GetPrtTotalContent(setPrtNames);
			}
			case CONTENT_TYPE::UNITS_XML:
			{
				return CSingleton::gUnitsXmlProcessor.GetTotalContent(skinId);
			}
			case CONTENT_TYPE::ROLES_XML:
			{
				return CSingleton::gRolesXmlProcessor.GetTotalContent(CSingleton::gUnitsXmlProcessor.GetRoleId(skinId));
			}
			case CONTENT_TYPE::SKIN_PRT_P_XML:
			{
				std::set<std::string> setPrtNames;
				CSingleton::gUnitsXmlProcessor.GetBasePresentations(skinId, setPrtNames);
				return CSingleton::gSkillPrtPProcessor.GetPrtTotalContent(setPrtNames);
			}
		}
		return std::string("");
	});

	onRadioEditTypeToggled(true);
}

void QtSkillModelTool::onClickButtonBtnConfirm()
{
    auto tt = ui.editPath->text();
    CSingleton::gEnvParams.strProjectPath = tt.toUtf8();
	CSingleton::gSkinSpDescribeProcessor.init(CSingleton::gEnvParams.strProjectPath + "/Debug/data/config/skinspdescribe.json");
    CSingleton::gUnitProcessor.SetHeroPackageRoot(CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/");

	refreshHeroList(ui.edit_search_heroid->text().toStdString());
}

void QtSkillModelTool::onClickButtonBtnGenerate()
{
	if (!m_pDlgGenerator)
	{
		m_pDlgGenerator = new QtSkillModelGenerate();
	}
	m_pDlgGenerator->setModal(true);
	m_pDlgGenerator->show();

	MODEL_INFO info;

	QStringListModel* pSlm = (QStringListModel*)ui.list_view_heros->model();
	if (pSlm)
	{
		QVariant variant = pSlm->data(ui.list_view_heros->currentIndex(), Qt::DisplayRole);
		if (variant.isValid())
		{
			auto skinIdAndName = variant.toString().toStdString();
			auto index = skinIdAndName.find("|");
			if (skinIdAndName.npos != index)
			{
				info.heroId = skinIdAndName.substr(0, index - 1);
			}
		}
	}

	pSlm = (QStringListModel*)ui.list_view_skins->model();
	if (pSlm)
	{
		QVariant variant = pSlm->data(ui.list_view_skins->currentIndex(), Qt::DisplayRole);
		if (variant.isValid())
		{
			auto skinIdAndName = variant.toString().toStdString();
			auto index = skinIdAndName.find("|");
			if (skinIdAndName.npos != index)
			{
				info.skinId = skinIdAndName.substr(0, index - 1);
				info.roleId = CSingleton::gUnitsXmlProcessor.GetRoleId(info.skinId);
				info.skinName = skinIdAndName.substr(index + 1, skinIdAndName.length() - index);
				info.skinName.erase(std::remove_if(info.skinName.begin(), info.skinName.end(), [&](char c) { return c == ' '; }), info.skinName.end());
			}
		}
	}

	pSlm = (QStringListModel*)ui.list_view_skills->model();
	if (pSlm)
	{
		QVariant variant = pSlm->data(ui.list_view_skills->currentIndex(), Qt::DisplayRole);
		if (variant.isValid())
		{
			info.skillId = variant.toString().toStdString();
		}
	}
	pSlm = (QStringListModel*)ui.list_view_skills->model();
	if (pSlm)
	{
		QVariant variant = pSlm->data(ui.list_view_skills->currentIndex(), Qt::DisplayRole);
		if (variant.isValid())
		{
			info.skillId = variant.toString().toStdString();
		}
	}
	m_pDlgGenerator->SetModelInfo(info);
}

void QtSkillModelTool::onListViewHerosIndexMoved(QModelIndex index)
{
	QStringListModel* pSlm = (QStringListModel*)ui.list_view_heros->model();
	QVariant variant = pSlm->data(index, Qt::DisplayRole);
	if (variant.isValid())
	{
		setSelectHero(variant.toString().toStdString());
	}
}
void QtSkillModelTool::onListViewSkinsIndexMoved(QModelIndex index)
{
	QStringListModel* pSlm = (QStringListModel*)ui.list_view_skins->model();
	QVariant variant = pSlm->data(index, Qt::DisplayRole);
	if (variant.isValid())
	{
		setSelectSkin(variant.toString().toStdString());
	}
}
void QtSkillModelTool::onListViewSkillsIndexMoved(QModelIndex index)
{
	QStringListModel* pSlm = (QStringListModel*)ui.list_view_skills->model();
	QVariant variant = pSlm->data(index, Qt::DisplayRole);
	if (variant.isValid())
	{
		m_pPreview->RefreshContent();
	}
}

void QtSkillModelTool::onListViewHerosIndexesMoved(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{

}

void QtSkillModelTool::onEditChange(const QString& qStr)
{
	refreshHeroList(qStr.toStdString());
}

void QtSkillModelTool::onRadioEditTypeToggled(bool checked)
{
	if (checked)
	{
		setEditType(ui.radio_skin->isChecked() ? EDIT_TYPE::SKIN_MODEL : (ui.radio_skill->isChecked() ? EDIT_TYPE::SKILL_MODEL : EDIT_TYPE::NONE));
	}
}

void QtSkillModelTool::setSelectHero(std::string idAndName)
{
	auto heroId = idAndName;
	auto index = idAndName.find("|");
	if (idAndName.npos != index)
	{
		heroId = idAndName.substr(0, index - 1);
		heroId = SelectNumber(heroId);
	}

	CSingleton::gSkillDataLuaProcesser.SetHeroId(heroId);
	CSingleton::gSkillIniProcessor.SetHeroId(heroId);
	CSingleton::gSkillConditionIniProcessor.SetHeroId(heroId);
	CSingleton::gSkillPresentationIniProcessor.SetHeroId(heroId);

	refreshSkinList(heroId);
	refreshSkillList(heroId);
}

void QtSkillModelTool::setSelectSkin(std::string idAndName)
{
	auto skinId = idAndName;
	auto index = idAndName.find("|");
	if (idAndName.npos != index)
	{
		skinId = idAndName.substr(0, index - 1);
		skinId = SelectNumber(skinId);
	}

	CSingleton::gSkillPresentationIniProcessor.SetSkinId(skinId);
	CSingleton::gUnitsXmlProcessor.SetSkinId(skinId);
	CSingleton::gRolesXmlProcessor.SetSkinId(skinId);

	auto skinPath = CSingleton::gEnvParams.strProjectPath + "/Debug/singlepackage/heropackage/" + skinId + "/";
	auto roleId = CSingleton::gUnitsXmlProcessor.GetRoleId(skinId);
	auto pathPrtC = CSingleton::gRolesXmlProcessor.GetPrtCPath(roleId);
	auto pathPrtP = CSingleton::gRolesXmlProcessor.GetPrtPPath(roleId);

	auto prtNameC = pathPrtC;
	if (!prtNameC.empty())
	{
		std::replace(prtNameC.begin(), prtNameC.end(), '\\', '/');
		auto index = prtNameC.rfind("/");
		prtNameC = prtNameC.substr(index + 1, prtNameC.length() - index - 1);
	}
	auto prtNameP = pathPrtP;
	if (!prtNameP.empty())
	{
		std::replace(prtNameP.begin(), prtNameP.end(), '\\', '/');
		auto index = prtNameP.rfind("/");
		prtNameP = prtNameP.substr(index + 1, prtNameP.length() - index - 1);
	}

	CSingleton::gSkillPrtCProcessor.SetPath(skinPath + pathPrtC);
	CSingleton::gSkillPrtPProcessor.SetPath(skinPath + pathPrtP);
	m_pPreview->SetPrtName(prtNameP, prtNameC);
}

void QtSkillModelTool::refreshHeroList(std::string key/* = ""*/)
{
	QStringListModel* pSlm = (QStringListModel*)ui.list_view_heros->model();
	if (nullptr == pSlm)
	{
		pSlm = new QStringListModel(this);
		ui.list_view_heros->setModel(pSlm);
//		connect(pSlm, SIGNAL(dataChanged(const QModelIndex, const QModelIndex, const QVector<int> &)), this, SLOT(onListViewHerosIndexesMoved(const QModelIndex&, const QModelIndex&, const QVector<int>&)));
	}
	auto pSl = new QStringList();

	auto heros = CSingleton::gUnitProcessor.getHeros();
	for (auto iter = heros.begin(); iter != heros.end(); iter++)
	{
		auto iiC = -1;
		for (auto c : key)
		{
			iiC = iter->first.find(c, iiC + 1);
			if (std::string::npos == iiC)
			{
				break;
			}
		}
		if (!key.empty() && std::string::npos == iiC)
		{
			continue;
		}

		auto skins = CSingleton::gUnitProcessor.getSkins(iter->first);
		std::string protoSkin;
		for (auto iterSkin = skins.begin(); iterSkin != skins.end(); iterSkin++)
		{
			if (0 == iterSkin->compare(iter->first + "000"))
			{
				protoSkin = *iterSkin;
				break;
			}
		}
		if (protoSkin.empty() && !skins.empty())
		{
			protoSkin = skins[0];
		}
		auto protoSkinName = CSingleton::gUnitProcessor.getSkinName(protoSkin);
		protoSkinName = iter->first + " | " + protoSkinName;
		pSl->append(protoSkinName.c_str());
	}

	auto index = ui.list_view_heros->currentIndex();
	pSlm->setStringList(*pSl);
	delete pSl;

	QVariant variant = pSlm->data(index, Qt::DisplayRole);
	if (!variant.isValid())
	{
		index = pSlm->index(0, 0);
	}

	ui.list_view_heros->setCurrentIndex(index);
	onListViewHerosIndexMoved(index);
}

void QtSkillModelTool::refreshSkinList(std::string heroId)
{
	QStringListModel* pSlm = (QStringListModel*)ui.list_view_skins->model();
	if (nullptr == pSlm)
	{
		pSlm = new QStringListModel(this);
		ui.list_view_skins->setModel(pSlm);
	}
	auto pSl = new QStringList();

	auto skins = CSingleton::gUnitProcessor.getSkins(heroId);
	for (auto iter = skins.begin(); iter != skins.end(); iter++)
	{
		auto protoSkinName = CSingleton::gUnitProcessor.getSkinName(iter->c_str());
		protoSkinName = *iter + " | " + protoSkinName;
		pSl->append(protoSkinName.c_str());
	}

	pSlm->setStringList(*pSl);
	delete pSl;

	QModelIndex qIndex = pSlm->index(0, 0);
	ui.list_view_skins->setCurrentIndex(qIndex);
	onListViewSkinsIndexMoved(qIndex);
}

void QtSkillModelTool::refreshSkillList(std::string heroId)
{
	QStringListModel* pSlm = (QStringListModel*)ui.list_view_skills->model();
	if (nullptr == pSlm)
	{
		pSlm = new QStringListModel(this);
		ui.list_view_skills->setModel(pSlm);
	}
	auto pSl = new QStringList();

	auto skills = CSingleton::gSkillDataLuaProcesser.GetSkills();
	for (auto iter = skills.begin(); iter != skills.end(); iter++)
	{
		pSl->append(iter->first.c_str());
	}

	pSlm->setStringList(*pSl);
	delete pSl;

	QModelIndex qIndex = pSlm->index(0, 0);
	ui.list_view_skills->setCurrentIndex(qIndex);
	onListViewSkillsIndexMoved(qIndex);
}

void QtSkillModelTool::setEditType(EDIT_TYPE type)
{
	CSingleton::gEditType = type;

	switch (type)
	{
	case EDIT_TYPE::SKIN_MODEL:
	{
		ui.label_skill_id->setVisible(false);
		ui.list_view_skills->setVisible(false);
		ui.list_view_skins->setFixedHeight(764);
	}break;
	case EDIT_TYPE::SKILL_MODEL:
	{
		ui.label_skill_id->setVisible(true);
		ui.list_view_skills->setVisible(true);
		ui.list_view_skins->setFixedHeight(360);
	}break;
	}

	m_pPreview->RefreshTab();
	m_pPreview->RefreshContent();
}