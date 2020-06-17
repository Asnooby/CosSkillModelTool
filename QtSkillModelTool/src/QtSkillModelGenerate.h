#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtSkillModelGenerate.h"
#include "QtConfigPreview.h"
#include "src/processor/BaseDefine.h"
#include "src/processor/CSkillIniProcessor.h"
#include "src/processor/CSkillPrtCProcessor.h"
#include "src/processor/CSkillPrtPProcessor.h"
#include "src/processor/CSkillDataLuaProcessor.h"
#include "src/processor/CSkillConditionIniProcessor.h"
#include "src/processor/CSkillPresentationIniProcessor.h"

class QtSkillModelGenerate : public QDialog
{
    Q_OBJECT

public:
    QtSkillModelGenerate(QWidget *parent = Q_NULLPTR);

public:
    void SetModelInfo(MODEL_INFO info);

private:
    void bindSignalEvent();
    void initUI();
    void setModelInfo(std::string heroId, std::string skinId, std::string skinName);

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
};
