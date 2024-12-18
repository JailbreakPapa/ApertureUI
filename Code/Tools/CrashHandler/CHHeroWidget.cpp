#include "CHHeroWidget.h"

CHHeroWidget::CHHeroWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
  QString t = "Aperture UI Crashed!";
  // Note: Example. Remove this.
  SetHeroText(t);
}

CHHeroWidget::~CHHeroWidget()
{
}

void CHHeroWidget::SetHeroText(QString& in_newtext)
{
  ui.CrashHero->setText(in_newtext);
}
