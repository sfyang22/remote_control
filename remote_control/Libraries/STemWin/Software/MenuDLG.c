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
#include "stdlib.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0            (GUI_ID_USER + 0x00)
#define ID_BUTTON_0            (GUI_ID_USER + 0x01)
#define ID_BUTTON_1            (GUI_ID_USER + 0x02)
#define ID_BUTTON_2            (GUI_ID_USER + 0x03)
#define ID_BUTTON_3            (GUI_ID_USER + 0x04)
#define ID_BUTTON_4            (GUI_ID_USER + 0x05)
#define ID_TEXT_0               (GUI_ID_USER + 0x06)
#define ID_CHECKBOX_0           (GUI_ID_USER + 0x07)
#define ID_TEXT_1               (GUI_ID_USER + 0x08)


// USER START (Optionally insert additional defines)
 U8 _Calibrate(void);
 WM_HWIN CreateMenu(void);
 extern WM_HWIN CreateSensor_config(void);
 extern void TopWindow_Close(void);
 extern WM_HWIN CreateWindow(void);
 extern WM_HWIN CreateLcd_blk(void);
 extern WM_HWIN Createled_config(void);
 extern void TopWindow_ReDraw(void);

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
  { FRAMEWIN_CreateIndirect, "Menu", ID_FRAMEWIN_0, 60, 20, 200, 220, WM_CF_MEMDEV, 0x0, 0 },
  { BUTTON_CreateIndirect, "Calibrate LCD", ID_BUTTON_0, 55, 5, 80, 25, WM_CF_MEMDEV_ON_REDRAW, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED Config", ID_BUTTON_1, 55, 99, 80, 25, WM_CF_MEMDEV_ON_REDRAW, 0x0, 0 },
  { BUTTON_CreateIndirect, "Sensor Config", ID_BUTTON_2, 55, 67, 80, 25, WM_CF_MEMDEV_ON_REDRAW, 0x0, 0 },
  { BUTTON_CreateIndirect, "LCD BLK", ID_BUTTON_3, 55, 37, 80, 25, WM_CF_MEMDEV_ON_REDRAW, 0x0, 0 },
  { BUTTON_CreateIndirect, "Back", ID_BUTTON_4, 55, 131, 80, 25, WM_CF_MEMDEV_ON_REDRAW, 0x0, 0 },
  { TEXT_CreateIndirect, " Please Select what you want to config", ID_TEXT_0, 1, 171, 195, 21, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "", ID_CHECKBOX_0, 5, 35, 53, 26, WM_CF_MEMDEV_ON_REDRAW, 0x0, 0 },
  { TEXT_CreateIndirect, "fbtxt", ID_TEXT_1, 5, 11, 80, 20, 0, 0x0, 0 },
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
WM_HWIN current_page;

 U8 button_flag = 0;                //代表按钮按下次数
 U8 freeback_flag = 0;              //震动反馈设置标志位
 U8 lcd_blk = 80;                   //LCD背光亮度
 U8 led_value = 1;                  //LED前照灯亮度
 U8 led_open = 0;                   //前照灯开关
 U8 air_set = 60;                   //空气质量传感器阈值
 U8 co1_set = 60;                  //CO1传感器阈值
 U8 control_power = 100;             //遥控电量
 U8 car_power = 20;                 //小车电量
 U8 wifi_connected = 0;                       //WIFI连接状态

// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  int NCode;
  int Id;
  // USER START (Optionally insert additional variables)
  WM_HWIN hItem;
  static GUI_RECT rect3 = { 160, 2, 190, 22 };
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
      WM_SetFocus(pMsg->hWin);
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
      TEXT_SetBkColor(hItem, GUI_YELLOW);
      TEXT_SetTextAlign(hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER);
      TEXT_SetTextColor(hItem, GUI_RED);
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
      TEXT_SetText(hItem, "Freeback");
      TEXT_SetTextAlign(hItem, TEXT_CF_LEFT | TEXT_CF_VCENTER);
      hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
      if (freeback_flag)
      {
          CHECKBOX_SetState(hItem, 0);
      }
      else
      {
          CHECKBOX_SetState(hItem, 1);
      }
      break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Calibrate LCD'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
       
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
            GUI_EndDialog(pMsg->hWin, 0);       //关闭菜单界面
            TopWindow_Close();                  //关闭顶部窗口
            /*
            if (_Calibrate() != 0)                      //开启校准程序
            {
                GUI_Clear();
                GUI_DispStringAt("Calibrate fail!", 100, 120);
            }
            else
            {
                GUI_DispStringAt("Calibrate suceess!", 100, 120);
            }
            */
            GUI_Delay(800);
            current_page = CreateWindow();                     //开启顶部窗口
            current_page = CreateMenu();                       //开启菜单界面
            WM_SetFocus(pMsg->hWin);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'LED Config'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
          GUI_EndDialog(pMsg->hWin, 0);
          current_page = Createled_config();
          WM_SetFocus(pMsg->hWin);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'Sensor Config'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
          GUI_EndDialog(pMsg->hWin, 0);
          current_page = CreateSensor_config();
          WM_SetFocus(pMsg->hWin);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'LCD BLK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
          GUI_EndDialog(pMsg->hWin, 0);
          current_page = CreateLcd_blk();
          WM_SetFocus(pMsg->hWin);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'Back'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
          GUI_EndDialog(pMsg->hWin, 0);         //返回退出菜单界面
          button_flag = 0;                      //清除标志位，允许menu按钮创建窗口
          WM_SetFocus(pMsg->hWin);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
    // USER START (Optionally insert additional code for further Ids)
    // USER END
       // USER START (Optionally insert additional message handling)
    // USER END
    case ID_CHECKBOX_0: // Notifications sent by 'fbcheck'
        switch (NCode) {
        case WM_NOTIFICATION_CLICKED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
        case WM_NOTIFICATION_RELEASED:
            // USER START (Optionally insert code for reacting on notification message)
            // USER END
            break;
        case WM_NOTIFICATION_VALUE_CHANGED:
            // USER START (Optionally insert code for reacting on notification message)
            hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
            freeback_flag = !CHECKBOX_IsChecked(hItem);
            TopWindow_ReDraw();
            WM_SetFocus(pMsg->hWin);
            // USER END
            break;
            // USER START (Optionally insert additional code for further notification handling)
            // USER END
        }
    }
    break;
 
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
*       CreateMenu
*/
static WM_HWIN hWin;

WM_HWIN CreateMenu(void);
WM_HWIN CreateMenu(void) {
  

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
void Menu_Close(void)
{
    GUI_EndDialog(hWin, 0);

}


// USER END

/*************************** End of file ****************************/
