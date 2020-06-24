#include "QtConfigPreview.h"
#include "src/CSingleton.h"

QtConfigPreview::QtConfigPreview(QWidget* parent)
    : QFrame(parent)
{
    ui.setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    initUI();
    bindSignalEvent();
}

void QtConfigPreview::bindSignalEvent()
{
	connect(ui.tab_view_skill, SIGNAL(currentChanged(int)), this, SLOT(onTabCurrentChanged(int)));
	connect(ui.tab_view_skin, SIGNAL(currentChanged(int)), this, SLOT(onTabCurrentChanged(int)));
}

void QtConfigPreview::initUI()
{
    ui.tab_view_skill->setTabText(0, QString::fromUtf8("skilldata.lua"));
    ui.tab_view_skill->setTabText(1, QString::fromUtf8("skill.ini"));
    ui.tab_view_skill->setTabText(2, QString::fromUtf8("skillcondition.ini"));
    ui.tab_view_skill->setTabText(3, QString::fromUtf8("skillpresentation.ini"));
    ui.tab_view_skill->setTabText(4, QString::fromUtf8("_c.prt"));
    ui.tab_view_skill->setTabText(5, QString::fromUtf8("_p.prt"));
    ui.tab_view_skill->setCurrentIndex(0);

	ui.tab_view_skin->setTabText(0, QString::fromUtf8("units.xml"));
	ui.tab_view_skin->setTabText(1, QString::fromUtf8("roles.xml"));
	ui.tab_view_skin->setTabText(2, QString::fromUtf8("_p.prt"));
	ui.tab_view_skin->setCurrentIndex(0);

    ui.edit_content->setWordWrapMode(QTextOption::WrapAnywhere);
}

void QtConfigPreview::onTabCurrentChanged(int index)
{
	RefreshContent();
}

void QtConfigPreview::SetPrtName(std::string prtP, std::string prtC)
{
    ui.tab_view_skill->setTabText(4, QString::fromStdString(prtC));
	ui.tab_view_skill->setTabText(5, QString::fromStdString(prtP));
	ui.tab_view_skin->setTabText(2, QString::fromStdString(prtP));
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
	auto type = 0;
	switch (CSingleton::gEditType)
	{
	case EDIT_TYPE::SKIN_MODEL:
	{
		type = ui.tab_view_skin->currentIndex() + (int)CONTENT_TYPE::SKIN_START;
	}break;
	case EDIT_TYPE::SKILL_MODEL:
	{
		type = ui.tab_view_skill->currentIndex() + (int)CONTENT_TYPE::SKILL_START;
	}break;
	}

    SetContentType((CONTENT_TYPE)type);
}

void QtConfigPreview::RefreshTab()
{
	switch (CSingleton::gEditType)
	{
	case EDIT_TYPE::SKIN_MODEL:
	{
		ui.tab_view_skin->setVisible(true);
		ui.tab_view_skill->setVisible(false);
	}break;
	case EDIT_TYPE::SKILL_MODEL:
	{
		ui.tab_view_skin->setVisible(false);
		ui.tab_view_skill->setVisible(true);
	}break;
	}
}