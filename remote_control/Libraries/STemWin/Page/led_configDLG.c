/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.44                          *
*        Compiled Nov 10 2017, 08:53:57                              *
*        (c) 2017 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include "stdio.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0         (GUI_ID_USER + 0x06)
#define ID_SLIDER_0         (GUI_ID_USER + 0x07)
#define ID_CHECKBOX_0         (GUI_ID_USER + 0x08)
#define ID_TEXT_0         (GUI_ID_USER + 0x09)
#define ID_BUTTON_0         (GUI_ID_USER + 0x0A)


// USER START (Optionally insert additional defines)
extern WM_HWIN CreateMenu(void);
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "led_config", ID_FRAMEWIN_0, 60, 20, 200, 220, WM_CF_MEMDEV, 0x0, 0 },
  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 47, 43, 94, 34, WM_CF_MEMDEV_ON_REDRAW, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "open", ID_CHECKBOX_0, 45, 104, 100, 31, WM_CF_MEMDEV_ON_REDRAW, 0x0, 0 },
  { TEXT_CreateIndirect, "value:", ID_TEXT_0, 48, 17, 42, 19, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Back", ID_BUTTON_0, 53, 154, 80, 25, WM_CF_MEMDEV_ON_REDRAW, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
extern  U8 led_value;                  //LEDǰ�յ�����
extern  U8 led_open;                   //LED����
extern WM_HWIN current_page;
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  char txt[4];
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'open'
    //
    WM_SetFocus(pMsg->hWin);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
    CHECKBOX_SetText(hItem, "LED open");
    // USER START (Optionally insert additional code for further widget initialization)
    GUI_X_Lock();
    CHECKBOX_SetState(hItem, led_open);
    GUI_X_Unlock();
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
    SLIDER_SetRange(hItem, 1, 100);
    SLIDER_SetNumTicks(hItem, 100);
    GUI_X_Lock();
    SLIDER_SetValue(hItem, led_value);
    GUI_X_Unlock();
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    GUI_X_Lock();
    sprintf(txt, "%d", led_value);
    GUI_X_Unlock();
    TEXT_SetText(hItem, txt);

    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_SLIDER_0: // Notifications sent by 'Slider'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
          WM_SetFocus(pMsg->hWin);
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
          hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
          GUI_X_Lock();
          led_value = SLIDER_GetValue(hItem);
          GUI_X_Unlock();
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
          GUI_X_Lock();
          sprintf(txt, "%d", led_value);
          GUI_X_Unlock();
          TEXT_SetText(hItem, txt);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_0: // Notifications sent by 'open'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
          WM_SetFocus(pMsg->hWin);
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
          hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
          GUI_X_Lock();
          led_open = CHECKBOX_GetState(hItem);
          GUI_X_Unlock();
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'Back'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
          GUI_EndDialog(pMsg->hWin, 0);
          GUI_X_Lock();
          current_page = CreateMenu();
          GUI_X_Unlock();
          WM_SetFocus(pMsg->hWin);
          
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       Createled_config
*/
WM_HWIN Createled_config(void);
WM_HWIN Createled_config(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/