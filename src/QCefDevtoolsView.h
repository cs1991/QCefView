#ifndef QCEFDEVTOOLS_H
#define QCEFDEVTOOLS_H
#pragma once
#include <QScopedPointer>
#include <QDialog>
#include <QWindow>
#include <CefViewBrowserClient.h>
class QCefDevtoolsView : public QDialog
{
  Q_OBJECT

public:
  QCefDevtoolsView(CefRefPtr<CefBrowser>& browser, CefRefPtr<CefViewBrowserClient> pClient_, QWidget* parent = 0);

  
  void closeEvent(QCloseEvent* event);
signals:
  void OnClose();
};
#endif
