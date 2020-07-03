#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtSkillModelGenerate.h"
#include "QtConfigPreview.h"
#include "src/processor/CSkinProcessor.h"
#include "src/processor/CSkillIniProcessor.h"
#include "src/processor/CSkillPrtCProcessor.h"
#include "src/processor/CSkillPrtPProcessor.h"
#include "src/processor/CSkillDataLuaProcessor.h"
#include "src/processor/CSkinSpDescribeProcessor.h"
#include "src/processor/CSkillConditionIniProcessor.h"
#include "src/processor/CSkillPresentationIniProcessor.h"
#include "src/processor/CUnitsXmlProcessor.h"
#include "src/processor/CRolesXmlProcessor.h"

class QtSkillModelGenerate : public QDialog
{
    Q_OBJECT

public:
    QtSkillModelGenerate(QWidget *parent = Q_NULLPTR);

public:
    void SetModelInfo(MODEL_INFO info);

private:
	void showEvent(QShowEvent*) override;
    void bindSignalEvent();
    void initUI();
	void setModelInfo(std::string heroId, std::string skinId, std::string skinName);
	void exportSkillConfig();
	void exportSkinConfig();
	void completeSkinModelFiles();

private slots:
    void onBtnGenerateClicked();
    void onBtnExportClicked();

private:
    Ui::QtSkillModelGenerate ui;
    QtConfigPreview* m_pPreview;
    MODEL_INFO m_info;
    MODEL_INFO m_infoGenerate;

    CSkillIniProcessor m_SkillIniProcessor;
    CSkillPrtCProcessor m_SkillPrtCProcessor;
    CSkillPrtPProcessor m_SkillPrtPProcessor;
    CSkillDataLuaProcessor m_SkillDataLuaProcesser;
    CSkillConditionIniProcessor m_SkillConditionIniProcessor;
	CSkillPresentationIniProcessor m_SkillPresentationIniProcessor;
    CUnitsXmlProcessor m_UnitsXmlProcessor;
    CRolesXmlProcessor m_RolesXmlProcessor;
};
