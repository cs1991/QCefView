#include "CCefClientDelegate.h"

#include <QDebug>

#include "QCefViewPrivate.h"

#ifdef Q_OS_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#endif

bool
CCefClientDelegate::onPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                  const CefKeyEvent& event,
                                  CefEventHandle os_event,
                                  bool* is_keyboard_shortcut)
{
  // qDebug("===== CefKeyEvent:type=%d, modifiers=0x%08x, windows_key_code=%d, native_key_code=%d, char=0x%04x, "
  //        "unmod_char=0x%04x",
  //        event.type,
  //        event.modifiers,
  //        event.windows_key_code,
  //        event.native_key_code,
  //        event.character,
  //        event.unmodified_character);

#if defined(Q_OS_MACOS)
  if (event.modifiers & EVENTFLAG_COMMAND_DOWN && event.type == KEYEVENT_RAWKEYDOWN) {
    switch (event.native_key_code) {
      case 0: // A
        browser->GetFocusedFrame()->SelectAll();
        *is_keyboard_shortcut = true;
        break;
      case 6: // Z
        browser->GetFocusedFrame()->Undo();
        *is_keyboard_shortcut = true;
        break;
      case 7: // X
        browser->GetFocusedFrame()->Cut();
        *is_keyboard_shortcut = true;
        break;
      case 8: // C
        browser->GetFocusedFrame()->Copy();
        *is_keyboard_shortcut = true;
        break;
      case 9: // V
        browser->GetFocusedFrame()->Paste();
        *is_keyboard_shortcut = true;
        break;
      case 16: // Y
        browser->GetFocusedFrame()->Redo();
        *is_keyboard_shortcut = true;
        break;
    }
  }
#endif
#if defined(Q_OS_WIN)
    if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == VK_F12 &&
      (event.modifiers & EVENTFLAG_CONTROL_DOWN)) {
    printf("ctrl + F12 enter\n");
   /* CefWindowInfo windowInfo;
    CefBrowserSettings settings;
    windowInfo.SetAsPopup(NULL, "DevTools");
    browser->GetHost()->ShowDevTools(windowInfo, pCefViewPrivate_->pClient_, settings, CefPoint());*/
    pCefViewPrivate_->q_ptr->showDevToolsEvent();
    return true;
    } else if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == 0x44 &&
               (event.modifiers & EVENTFLAG_CONTROL_DOWN) && (event.modifiers & EVENTFLAG_ALT_DOWN)) {
      printf("ctrl + alt+ D enter\n");
      pCefViewPrivate_->q_ptr->showDevToolsEvent();
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
#else
  XKeyEvent* xevent = (XKeyEvent*)os_event;
  if (xevent == nullptr) {
    return false;
  }

  // 获取按下的键的键码
  KeySym keysym = XLookupKeysym(xevent, 0);
  if (keysym == XK_F5) {
    // F5 被按下，执行相应的操作
    // ...
    return true;
  }
#endif
        
  return false;
}

bool
CCefClientDelegate::onKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event)
{
  return false;
}
