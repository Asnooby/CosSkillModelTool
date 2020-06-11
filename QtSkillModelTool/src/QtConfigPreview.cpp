#include "QtConfigPreview.h"

QtConfigPreview::QtConfigPreview(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    initUI();
    bindSignalEvent();
}

void QtConfigPreview::bindSignalEvent()
{
    connect(ui.tab_view, SIGNAL(currentChanged(int)), this, SLOT(onTabCurrentChanged(int)));
}

void QtConfigPreview::initUI()
{
    ui.tab_view->setTabText(0, QString::fromUtf8("skilldata.lua"));
    ui.tab_view->setTabText(1, QString::fromUtf8("skill.ini"));
    ui.tab_view->setTabText(2, QString::fromUtf8("skillcondition.ini"));
    ui.tab_view->setTabText(3, QString::fromUtf8("skillpresentation.ini"));
    ui.tab_view->setTabText(4, QString::fromUtf8("_c.prt"));
    ui.tab_view->setTabText(5, QString::fromUtf8("_p.prt"));
    ui.tab_view->setCurrentIndex(0);

    ui.edit_content->setWordWrapMode(QTextOption::WrapAnywhere);
    ui.edit_content->setFixedWidth(ui.edit_content->width() - 40);
}

void QtConfigPreview::onTabCurrentChanged(int index)
{
    SetContentType((CONTENT_TYPE)index);
}

void QtConfigPreview::SetPrtName(std::string prtP, std::string prtC)
{
    ui.tab_view->setTabText(4, QString::fromStdString(prtC));
    ui.tab_view->setTabText(5, QString::fromStdString(prtP));
    RefreshContent();
}

void QtConfigPreview::RegisterFuncGetContent(SignalGetContentByType func)
{
    m_pFuncGetContent = func;
}

void QtConfigPreview::SetContentType(CONTENT_TYPE type)
{
    if (m_pFuncGetContent)
    {
        auto content = m_pFuncGetContent(type);
        ui.edit_content->clear();
        ui.edit_content->setText(QString::fromUtf8(content.c_str()));
    }
}

void QtConfigPreview::RefreshContent()
{
    auto type = ui.tab_view->currentIndex();
    SetContentType((CONTENT_TYPE)type);
}