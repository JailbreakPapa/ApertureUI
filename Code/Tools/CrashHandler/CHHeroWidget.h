#pragma once

#include <QWidget>
#include <CrashHandler/ui_CHHeroWidget.h>

class CHHeroWidget : public QWidget
{
public:


public:
	CHHeroWidget(QWidget *parent = nullptr);
	~CHHeroWidget();

  void SetHeroText(QString& in_newtext);


private:
	Ui::CHHeroWidgetClass ui;
};
