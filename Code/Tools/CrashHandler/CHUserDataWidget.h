#pragma once

#include <QWidget>
#include "CrashHandler/ui_CHUserDataWidget.h"

class CHUserDataWidget : public QWidget
{
	//Q_OBJECT

public:
	CHUserDataWidget(QWidget *parent = nullptr);
	~CHUserDataWidget();

  void SetStackTrace(const QString str);

private:
	Ui::CHUserDataWidgetClass ui;
};
