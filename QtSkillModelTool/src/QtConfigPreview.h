#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ConfigPreview.h"
#include "src/processor/BaseDefine.h"

typedef std::function<std::string(CONTENT_TYPE)> SignalGetContentByType;

class QtConfigPreview : public QFrame
{
    Q_OBJECT

public:
    QtConfigPreview(QWidget *parent = Q_NULLPTR);

public:
    void SetPrtName(std::string prtP, std::string prtC);
    void RegisterFuncGetContent(SignalGetContentByType func);
    void SetContentType(CONTENT_TYPE type);
	void RefreshContent();
    void RefreshTab();

private:
    void bindSignalEvent();
    void initUI();

private slots:
    void onTabCurrentChanged(int index);

private:
    Ui::ConfigPreview ui;

    SignalGetContentByType m_pFuncGetContent;
};
