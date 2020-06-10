#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtSkillModelTool.h"

class QtSkillModelTool : public QMainWindow
{
    Q_OBJECT

public:
    QtSkillModelTool(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtSkillModelToolClass ui;
};
