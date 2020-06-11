#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtSkillModelTool.h"
#include "src/QtConfigPreview.h"

class QtSkillModelTool : public QMainWindow
{
    Q_OBJECT

public:
    QtSkillModelTool(QWidget *parent = Q_NULLPTR);

public:
    void setSelectHero(std::string idAndName);
    void setSelectSkin(std::string idAndName);
    void refreshHeroList();
    void refreshSkinList(std::string heroId);
    void refreshSkillList(std::string heroId);

private:
    void bindSignalEvent();
    void initUI();

private slots:
    void onClickButtonBtnConfirm();
    void onListViewHerosIndexMoved(QModelIndex index);
    void onListViewSkinsIndexMoved(QModelIndex index);
    void onListViewSkillsIndexMoved(QModelIndex index);

private:
    Ui::QtSkillModelToolClass ui;
    QtConfigPreview* m_pPreview;
};
