#include <CrashHandler/MainWindow.moc.h>
#include <qwindow.h>

const int g_iDockingStateVersion = 1;

nsQtMainWindow* nsQtMainWindow::s_pWidget = nullptr;

nsQtMainWindow::nsQtMainWindow()
  : QMainWindow()
{
  s_pWidget = this;

  SetAlwaysOnTop(Always);
  UpdateAlwaysOnTop();

  setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
  setupUi(this);
}

nsQtMainWindow::~nsQtMainWindow()
{

}

void nsQtMainWindow::closeEvent(QCloseEvent* pEvent)
{
  const bool bMaximized = isMaximized();
  if (bMaximized)
    showNormal();
}

void nsQtMainWindow::SetupNetworkTimer()
{
}

void nsQtMainWindow::UpdateNetwork()
{
  UpdateAlwaysOnTop();
}

void nsQtMainWindow::DockWidgetVisibilityChanged(bool bVisible)
{
  // TODO: add menu entry for qt main widget
}

void nsQtMainWindow::SetAlwaysOnTop(OnTopMode Mode)
{
  m_OnTopMode = Mode;
}

void nsQtMainWindow::UpdateAlwaysOnTop()
{
  static bool bOnTop = false;

  bool bNewState = bOnTop;
  NS_IGNORE_UNUSED(bNewState);

  if (bOnTop != bNewState)
  {
    bOnTop = bNewState;

    hide();

    if (bOnTop)
      setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    else
      setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);

    show();
  }
}

void nsQtMainWindow::ProcessTelemetry(void* pUnuseed)
{
  if (!s_pWidget)
    return;
}
