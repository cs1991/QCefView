#include "QCefDevtoolsView.h"
QCefDevtoolsView::QCefDevtoolsView(CefRefPtr<CefBrowser>& browser,
                                   CefRefPtr<CefViewBrowserClient> pClient_,
                                   QWidget* parent)
  : QDialog(parent)
{
  setWindowTitle(QString("DevTools"));
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::WindowCloseButtonHint |
                 Qt::WindowStaysOnTopHint);
  setMaximumSize(1280, 800);
  resize(QSize(1280, 800));
  
     // Set window info
  CefWindowInfo window_info;
#if defined(OS_LINUX)
     // Don't know why, on Linux platform if we use QCefView's winId() as
     // the parent, it will complain about `BadWindow`,
     // and the browser window will not be created, this never happens
     // on Windows and macOS, so we create a temporal QWindow as the
     // parent to create CEF browser window.
     QWindow w;
     CefWindowHandle p = (CefWindowHandle)(w.winId());
     window_info.SetAsChild(p, { 0, 0, 0, 0 });
#else
#if CEF_VERSION_MAJOR >= 91
  window_info.SetAsChild((CefWindowHandle)this->winId(), { 0, 0, this->maximumWidth(), this->maximumHeight() });
#else
  window_info.SetAsChild((CefWindowHandle)this->winId(), 0, 0, this->maximumWidth(), this->maximumHeight());
#endif
#endif

     CefBrowserSettings settings;
     browser->GetHost()->ShowDevTools(window_info, pClient_, settings, CefPoint());
}

void
QCefDevtoolsView::closeEvent(QCloseEvent* event)
{
  emit OnClose();
}
