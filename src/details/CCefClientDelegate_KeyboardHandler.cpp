#include "CCefClientDelegate.h"

#include <QDebug>

#include "QCefViewPrivate.h"
#include <include/capi/views/cef_window_capi.h>
#ifdef Q_OS_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#endif

static inline QString getKeyChar(int native, int windowscode){
    QString code;
    switch (native) {
    case 96:
        code = "F12";
        break;
    case 95:
        code = "F11";
        break;
    case 76:
        code = "F10";
        break;
    case 75:
        code = "F9";
        break;
    case 74:
        code = "F8";
        break;
    case 73:
        code = "F7";
        break;
    case 72:
        code = "F6";
        break;
    case 71:
        code = "F5";
        break;
    case 70:
        code = "F4";
        break;
    case 69:
        code = "F3";
        break;
    case 68:
        code = "F2";
        break;
    case 67:
        code = "F1";
        break;
    default:
        code = QChar(windowscode);
        break;
    }
    qDebug() <<"keyevent: code="<<code;
    return code;
}

bool
CCefClientDelegate::onPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                  const CefKeyEvent& event,
                                  CefEventHandle os_event,
                                  bool* is_keyboard_shortcut)
{
   qDebug("===== CefKeyEvent:type=%d, modifiers=0x%08x, windows_key_code=%d, native_key_code=%d, char=0x%04x, "
          "unmod_char=0x%04x",
          event.type,
          event.modifiers,
          event.windows_key_code,
          event.native_key_code,
          event.character,
          event.unmodified_character);
    //printf("event.type=%d, rawkeydown=%d, KEYEVENT_KEYDOWN=%d\n",event.type,KEYEVENT_RAWKEYDOWN,KEYEVENT_KEYDOWN);
    if(event.type != KEYEVENT_RAWKEYDOWN && event.type != KEYEVENT_KEYDOWN){
        return false;
    }

#if defined(Q_OS_MACOS)
  // it seems Qt breaks the macOS shortcut key, so we need to fix it
  if (event.modifiers & EVENTFLAG_COMMAND_DOWN && event.type == KEYEVENT_RAWKEYDOWN) {
    switch (event.native_key_code) {
      case 0:  // A SelectAll
      case 6:  // Z Undo
      case 7:  // X Cut
      case 8:  // C Copy
      case 9:  // V Paste
      case 16: // Y Redo
        *is_keyboard_shortcut = true;
        break;
    }
  }
#endif
#if defined(Q_OS_WIN)
    if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == VK_F12 &&
      (event.modifiers & EVENTFLAG_CONTROL_DOWN)) {
    printf("ctrl + F12 enter\n");
    pCefViewPrivate_->q_ptr->showDevTools();
    return true;
    } else if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == 0x44 &&
               (event.modifiers & EVENTFLAG_CONTROL_DOWN) && (event.modifiers & EVENTFLAG_ALT_DOWN)) {
      printf("ctrl + alt+ D enter\n");
      pCefViewPrivate_->q_ptr->showDevTools();
      return true;
    } else if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == VK_F5 &&
             (event.modifiers & EVENTFLAG_CONTROL_DOWN)) {
    printf("ctrl + F5 enter\n");
    browser->Reload();
    return true;
    } else if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == 0x52 &&
             (event.modifiers & EVENTFLAG_CONTROL_DOWN) && (event.modifiers & EVENTFLAG_ALT_DOWN)) {
    printf("ctrl + alt+ R enter\n");
    browser->Reload();
    return true;
  } else if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == 0x51 &&
             (event.modifiers & EVENTFLAG_ALT_DOWN)) {
    printf("ctrl + Q enter\n");
    pCefViewPrivate_->q_ptr->quitKeyEvent();
    return true;
  } else if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == 0x4D &&
             (event.modifiers & EVENTFLAG_ALT_DOWN)) {
    printf("ctrl + M enter\n");
    pCefViewPrivate_->q_ptr->minKeyEvent();
    return true;
  } else if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == VK_LWIN) {
    return true;
  }
  pCefViewPrivate_->q_ptr->keyBoardEvent(event.modifiers & EVENTFLAG_CONTROL_DOWN,
                                         event.modifiers & EVENTFLAG_SHIFT_DOWN,
                                         event.modifiers & EVENTFLAG_ALT_DOWN,
                                         event.windows_key_code == VK_LWIN, getKeyChar(event.native_key_code,event.windows_key_code));
#else

  //printf("<modifiers=%d>,<native_key_code=%d>,<windows_key_code=%c>\n",event.modifiers,event.native_key_code,event.windows_key_code);
  //EVENTFLAG_ALT_DOWN alt
  if (((event.modifiers == EVENTFLAG_CONTROL_DOWN) && event.native_key_code == 96) ||
          ((event.modifiers & EVENTFLAG_CONTROL_DOWN) && (event.windows_key_code == 'D' || event.windows_key_code == 'd'))) {
      printf("ctrl + F12 || ctrl + D enter\n");
      pCefViewPrivate_->q_ptr->showDevTools();
      return true;
  } else if (((event.modifiers & EVENTFLAG_CONTROL_DOWN) && event.native_key_code == 71) ||
            ((event.modifiers & EVENTFLAG_CONTROL_DOWN)  && (event.windows_key_code == 'r' || event.windows_key_code == 'R'))) {
      printf("ctrl + F5 || ctrl + R enter\n");
      browser->Reload();
      return true;
  } else if ((event.modifiers & EVENTFLAG_CONTROL_DOWN) && (event.windows_key_code == 'Q' || event.windows_key_code == 'q')) {
      printf("ctrl + Q enter\n");
      pCefViewPrivate_->q_ptr->quitKeyEvent();
      return true;
  } else if ((event.modifiers & EVENTFLAG_CONTROL_DOWN) && (event.windows_key_code == 'M' || event.windows_key_code == 'm')) {
      printf("ctrl + M enter\n");
      pCefViewPrivate_->q_ptr->minKeyEvent();
      return true;
  } else if (event.modifiers & EVENTFLAG_COMMAND_DOWN) {//windows鍵
      return true;
  }
  pCefViewPrivate_->q_ptr->keyBoardEvent(event.modifiers & EVENTFLAG_CONTROL_DOWN,
                                         event.modifiers & EVENTFLAG_SHIFT_DOWN,
                                         event.modifiers & EVENTFLAG_ALT_DOWN,
                                         event.modifiers & EVENTFLAG_COMMAND_DOWN, getKeyChar(event.native_key_code, event.windows_key_code));
#endif
        
  return false;
}

bool
CCefClientDelegate::onKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event)
{
#if defined(Q_OS_MACOS)
  // it seems Qt breaks the macOS shortcut key, so we need to fix it
  if (event.modifiers & EVENTFLAG_COMMAND_DOWN && event.type == KEYEVENT_RAWKEYDOWN) {
    switch (event.native_key_code) {
      case 0: // A
        browser->GetFocusedFrame()->SelectAll();
        break;
      case 6: // Z
        browser->GetFocusedFrame()->Undo();
        break;
      case 7: // X
        browser->GetFocusedFrame()->Cut();
        break;
      case 8: // C
        browser->GetFocusedFrame()->Copy();
        break;
      case 9: // V
        browser->GetFocusedFrame()->Paste();
        break;
      case 16: // Y
        browser->GetFocusedFrame()->Redo();
        break;
    }
  }
#endif

  return false;
}
