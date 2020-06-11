#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ConfigPreview.h"

enum class CONTENT_TYPE
{
    SKILLDATA_LUA,
    SKILL_INI,
    SKILLCONDITION_INI,
    SKILLPRESENTATION_INI,
    PRT_C_INI,
    PRT_P_INI
};
typedef std::function<std::string(CONTENT_TYPE)> SignalGetContentByType;

class QtConfigPreview : public QMainWindow
{
    Q_OBJECT

public:
    QtConfigPreview(QWidget *parent = Q_NULLPTR);

public:
    void SetPrtName(std::string prtP, std::string prtC);
    void RegisterFuncGetContent(SignalGetContentByType func);
    void SetContentType(CONTENT_TYPE type);
    void RefreshContent();

private:
    void bindSignalEvent();
    void initUI();

private slots:
    void onTabCurrentChanged(int index);

private:
    Ui::ConfigPreview ui;

    SignalGetContentByType m_pFuncGetContent;
};
