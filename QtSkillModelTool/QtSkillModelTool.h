#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtSkillModelTool.h"
#include "src/QtConfigPreview.h"
#include "src/QtSkillModelGenerate.h"

class QtSkillModelTool : public QMainWindow
{
    Q_OBJECT

public:
    QtSkillModelTool(QWidget *parent = Q_NULLPTR);

public:
    void setSelectHero(std::string idAndName);
    void setSelectSkin(std::string idAndName);
    void refreshHeroList(std::string key = "");
    void refreshSkinList(std::string heroId);
    void refreshSkillList(std::string heroId);
    void setEditType(EDIT_TYPE type);

private:
    void bindSignalEvent();
    void initUI();

private slots:
    void onClickButtonBtnConfirm();
    void onClickButtonBtnGenerate();
    void onListViewHerosIndexMoved(QModelIndex index);
    void onListViewSkinsIndexMoved(QModelIndex index);
    void onListViewSkillsIndexMoved(QModelIndex index);
    void onListViewHerosIndexesMoved(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());
    void onEditChange(const QString& qStr);
    void onRadioEditTypeToggled(bool checked);

private:
    Ui::QtSkillModelToolClass ui;
    QtConfigPreview* m_pPreview;
    QtSkillModelGenerate* m_pDlgGenerator;
};
