#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "AdvancedGraphics.h"
#include "conio.h"
#include "extgraph.h"
#include "genlib.h"
#include "graphics.h"
#include "simpio.h"

#include <mmsystem.h>
#include <ocidl.h>
#include <ole2.h>
#include <olectl.h>
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>

/*判断点(x0,y0)是否在矩形包围盒(x1, y1) --> (x2, y2)范围内*/
bool inBox(double x0, double y0, double x1, double x2, double y1, double y2);
void MouseEventProcess(int x, int y, int button,
                       int event); /*鼠标消息回调函数*/

void KeyboardEventProcess(int key, int event);
static double windowW,windowH;

void Main() /*仅初始化执行一次*/
{
    InitConsole();
    InitGraphics();

    windowW = GetWindowWidth();
    windowH = GetWindowHeight();
    printf("Resolution: (%f, %f)\n",GetXResolution(),GetYResolution());

    SetPenColor("Dark Gray"), SetPenSize(4);
    char s[20];
    MovePen(windowW / 2, windowH / 2);
    sprintf(s, "%.2f , %.2f", windowW / 2, windowH / 2);
    DrawTextString(s);
    DrawGrid(0, 0, 1.0, 1.0, 20, 20);

    SetPenColor("Dark Gray");
    SetPenSize(1);
    DrawGrid(0, 0, 0.1, 0.1, 100, 70);

    SetPenSize(10);
    SetPenColor("Magenta");
    // SetEraseMode(TRUE);
    // DrawBox(2,2,1,1);
    SetEraseMode(FALSE);
    DrawBox(2,2,1,1);
    // for (int i=1;i<=10;i++){//prove that all consoles use the same handle.
    //     InitConsole();
    //     printf("Console%d Handle:%p\n",i,MyConsoleWindow());
    //     scanf("%s",s);
    //     CloseConsole();
    // }
    registerKeyboardEvent(KeyboardEventProcess);
    // printf("2\n");
    // scanf("%s",s);

    SetPenColor("Red");
    registerMouseEvent(MouseEventProcess);
    return;
}
bool inBox(double x0, double y0, double x1, double x2, double y1, double y2) {
    return (x0 >= x1 && x0 <= x2 && y0 >= y1 && y0 <= y2);
}
void KeyboardEventProcess(int key,int event){
    switch (event){
        case KEY_DOWN:
        switch (key){
            case VK_RETURN:
                // {
                // int a;
                // a=SetForegroundWindow(MyConsoleWindow());
                // void* p=SetActiveWindow(MyConsoleWindow());
                // p=SetFocus(MyConsoleWindow());
                // a=BringWindowToTop(MyConsoleWindow());
                // //none of the above will work...
                // printf("Active window: %p\n", GetActiveWindow());
                // printf("Focus: %p\n", GetFocus());
                // printf("Console Handle: %p\n", MyConsoleWindow());
                // printf("Graphics Handle: %p\n", MyGraphicsWindow());
                // char s[20];
                // scanf("%s", s);
                // // SetActiveWindow(MyGraphicsWindow());
                // // SetFocus(MyGraphicsWindow());
                // printf("Active window: %p\n", GetActiveWindow());
                // printf("Focus: %p\n", GetFocus());
                // printf("Console Handle: %p\n", MyConsoleWindow());
                // printf("Graphics Handle: %p\n", MyGraphicsWindow());
                // break;}
                // ShowWindow(consoleWindow,SW_SHOWNORMAL);
                // printf("Active window: %p\n", GetActiveWindow());
                // printf("Focus: %p\n", GetFocus());
                // char s[20];
                clearRect(windowW/2,windowH/2,windowW,0);
                break;
            case VK_BACK:
                DisplayClear();
                break;
            case VK_SPACE:
                SetPenColor("Dark Gray");
                SetPenSize(10);
                DrawGrid(0,0,1,1,10,7);
                break;
        }
        break;
    }
}
void printEvent(int x, int y, int button, int event) {
    const char buttonName[][20] = {
        "NO_BUTTON",
        "LEFT_BUTTON",
        "MIDDLE_BUTTON",
        "RIGHT_BUTTON"
    };
    const char eventName[][20] = {
        "BUTTON_DOWN",
        "BUTTON_DOUBLECLICK",
        "BUTTON_UP",
        "ROLL_UP",
        "ROLL_DOWN",
        "MOUSEMOVE"
    };
    printf("pixel:(%d, %d),inch:(%.3f,%.3f)\n%s(%d), %s\n\n",x,y,
    ScaleXInches(x),ScaleYInches(y),buttonName[button],button,eventName[event]);
    return;
}
void MouseEventProcess(int x, int y, int button, int event) {
    static bool isDraw = FALSE;
    static double ox, oy;  // originalX, originalY.
    static int i=0;
    double mx, my;         // myX, myY.
    mx = ScaleXInches(x);
    my = ScaleYInches(y);
    switch (event) {
        case BUTTON_DOWN:
            if (button == LEFT_BUTTON) {
                isDraw = TRUE;
                MovePen(mx, my);
                ox = mx;
                oy = my;
            }else if(button==RIGHT_BUTTON){
                // InitGraphics();
                // MovePen(GetWindowWidth()/2.0,GetWindowHeight()/2.0);
                DrawTextString("Hello,");
                DrawTextString("Right Button!");
                /**You can't generate several consoles at the same time.
                 * Probably because of the sync with stdin.
                */
                InitConsole();
                printf("%d",++i);

            }
            break;
        case BUTTON_UP:
            if (button == LEFT_BUTTON) isDraw = FALSE;
            break;
        case MOUSEMOVE:
            if (isDraw) {
                DrawLineTo(mx, my);
                ox = mx;
                oy = my;
            }
            break;
        case BUTTON_DOUBLECLICK:
            /*if (button == LEFT_BUTTON) */ {
                char s[50];
                sprintf(s, "inch:(%.2f , %.2f); pixel:(%d, %d)", mx, my, x, y);
                MovePen(mx, my);
                DrawTextString(s);
                ox = mx;
                oy = my;
            }
            break;
    }
    if(event!=MOUSEMOVE)printEvent(x, y, button, event);
    return;
}
