#include "CHUserDataWidget.h"

CHUserDataWidget::CHUserDataWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

CHUserDataWidget::~CHUserDataWidget()
{
}

void CHUserDataWidget::SetStackTrace(const QString str)
{
  ui.TextEdit_StackTrace->setText(str);
}
