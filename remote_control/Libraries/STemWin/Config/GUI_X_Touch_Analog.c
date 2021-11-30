#include "GUI.h"
#include "touch.h"
#include "stdlib.h"


void GUI_TOUCH_X_ActivateX(void)
{
    //__nop
}

void GUI_TOUCH_X_ActivateY(void)
{
    //__nop
}

int GUI_TOUCH_X_MeasureX(void)
{
    return XPT2046_ReadAdc_Fliter(X_OFFSET);
}

int GUI_TOUCH_X_MeasureY(void)
{
    return XPT2046_ReadAdc_Fliter(Y_OFFSET);
}

void TOUCH_First_Calibrate(void)
{
    GUI_CURSOR_Show();
    GUI_CURSOR_Select(&GUI_CursorCrossL);
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);
    GUI_Clear();
    GUI_DispStringAt("Measurement of\nA/D converter values\n", 0, 10);

    while (1)
    {
        GUI_PID_STATE TouchState;
        int xPhys, yPhys;
        GUI_TOUCH_GetState(&TouchState); /* Get the touch position in? ,→pixel */
        xPhys = GUI_TOUCH_GetxPhys(); /* Get the A/D mesurement result in? ,→x */
        yPhys = GUI_TOUCH_GetyPhys(); /* Get the A/D mesurement result in? ,→y */
        /* Display the measurement result */
        GUI_SetColor(GUI_BLUE);
        GUI_DispStringAt("Analog input:\n", 10, 30);
        GUI_GotoY(GUI_GetDispPosY() + 2);
        GUI_DispString("x:");
        GUI_DispDec(xPhys, 4);
        GUI_DispString(", y:");
        GUI_DispDec(yPhys, 4);
        /* Display the according position */
        GUI_SetColor(GUI_RED);
        GUI_GotoY(GUI_GetDispPosY() + 4);
        GUI_DispString("\nPosition:\n");
        GUI_GotoY(GUI_GetDispPosY() + 2);
        GUI_DispString("x:");
        GUI_DispDec(TouchState.x, 4);
        GUI_DispString(", y:");
        GUI_DispDec(TouchState.y, 4);
        /* Wait a while */
        GUI_Delay(100);
    };
}

 #define NUM_CALIB_POINTS 5 //校准点数	

static int _aRefX[NUM_CALIB_POINTS];
static int _aRefY[NUM_CALIB_POINTS];
static int _aSamX[NUM_CALIB_POINTS];
static int _aSamY[NUM_CALIB_POINTS];

uint8_t _Calibrate(void)
{

    GUI_PID_STATE State;
    int i;
    int xSize, ySize;
	int x_error, y_error;
	

    GUI_Delay(10);

    xSize = LCD_GetXSize();
    ySize = LCD_GetYSize();

    /* 根据 LCD 尺寸计算参考点 */
    _aRefX[0] = (xSize * 5) / 100;
    _aRefY[0] = (ySize * 5) / 100;
    _aRefX[1] = xSize - (xSize * 5) / 100;
    _aRefY[1] = _aRefY[0];
    _aRefX[2] = _aRefX[1];
    _aRefY[2] = ySize - (ySize * 5) / 100;
    _aRefX[3] = _aRefX[0];
    _aRefY[3] = _aRefY[2];
    _aRefX[4] = xSize / 2;
    _aRefY[4] = ySize / 2;

    /* 在 LCD 上绘制参考点 */
    GUI_TOUCH_GetState(&State);
    State.Pressed = 0;
    GUI_SetPenSize(3);

    for (i = 0; i < NUM_CALIB_POINTS; i++)
    {
        GUI_Clear();
        GUI_DispStringHCenterAt("Please touch the point", LCD_GetXSize(
        ) / 2, LCD_GetYSize() / 2 - 60);
        GUI_DrawCircle(_aRefX[i], _aRefY[i], 5);

        while (State.Pressed != 1)
        {
            GUI_Delay(100);
            GUI_TOUCH_GetState(&State);

        }

        if (State.Pressed == 1)
        {
            /* 储存采样点 */
            _aSamX[i] = GUI_TOUCH_GetxPhys();
            _aSamY[i] = GUI_TOUCH_GetyPhys();
			x_error = abs(_aRefX[i] * (3678 - 180) / 320 + 180 - _aSamX[i]);
			y_error = abs(_aRefY[i] * (3900 - 320) / 240 + 320 - _aSamY[i]);

			if (x_error > 110 || y_error > 110)
            {
                return -1;					//若采样点与参考点相差太远，返回错误
            }
        }	

        State.Pressed = 0;
        GUI_Delay(500);
    }

    /* 将测量点传递给 emWin */
    GUI_TOUCH_CalcCoefficients(NUM_CALIB_POINTS, _aRefX, _aRefY,
        _aSamX, _aSamY, xSize, ySize);

    GUI_Clear();
	GUI_Delay(200);

	return 0;
}







