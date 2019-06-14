#include "include/graphics.h"
#include "include/extgraph.h"
#include "include/genlib.h"
#include "include/simpio.h"

#include "include/vertices.h"
#include "include/matrix.h"
#include "include/datastructure.h"

#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "include/imgui.h"

#define DEMO_MENU
#define DEMO_BUTTON
#define DEMO_EDITBOX

static double winwidth, winheight;
pressedbutton mousepressed={0,0,0};
double cx,cy,dcx,dcy;
double lc[4][2],rc[4][2];
int updated=1,initbit=1;consolebit=1,FREE=0,CTRL=0;
//------------------------
extern int GroupIndex,selectedgroup,rotatebit,selectedvertex,vertexview,rightbut;
extern double proportion;
static char c[20];
extern char currentstorage[50];
extern vertice vt_i[VOLUME_VERTICE];

void DisplayClear(void);
void startTimer(int id,int timeinterval);

char* GetDoubleStr(double value)
{
    int i;
    char buf[32]={0};
    sprintf(buf,"%.8f",value);
    int index = 0;
    int len = strlen(buf);
    for(i = len-1;i>0;i--)
    {
        if(buf[i] == '0')
            continue;
        else
        {
            if(buf[i] == '.') index = i;
            else index = i+1;
            break;
        }
    }
    buf[index] = '\0';
    return buf;
}

void displayxyz(int id)
{
	double fH = GetFontHeight();
	double h = fH*2;
	double w = winwidth/10;
	double x = winwidth/30;
	double y = winheight/2-h;
	double labwidth=fH*1.3;
//	static char numx[80]  ;
//	static char numy[80] ;
//	static char numz[80] ;
//	static char numid[80];


	//strcpy(numx,GetDoubleStr(x));
	//strcpy(numy,GetDoubleStr(y));
	//strcpy(numz,GetDoubleStr(z));
//	strcpy(numid,GetDoubleStr(id));
//
//		SetPenColor("White");
	//drawLabel(x, y-=fH*1.2, results);

	/*drawLabel(labwidth/2-TextStringWidth("x")/2, y+fH-0.06, "x");
	if(textbox(GenUIID(0), labwidth, y, w-labwidth*1.1, h, numx, sizeof(numx)))
	{

	}

	if(textbox(GenUIID(0), labwidth, y-=h*1.6, w-labwidth*1.1, h, numy, sizeof(numy)))
	{

	}
	SetPenColor("White");
	drawLabel(labwidth/2-TextStringWidth("y")/2, y+fH-0.06, "y");

	//y = winheight/2;
	if( textbox(GenUIID(0),labwidth,y-=h*1.6,w-labwidth*1.1,h,numz,sizeof(numz)) )
	{
		double value;
		sscanf(numz,"%lf",&value);
		//sprintf(newnum,"Represented value is: %.20f", value);
	}
	SetPenColor("White");
	drawLabel(labwidth/2-TextStringWidth("z")/2, y+fH-0.06, "z");*/

//	if( textbox(GenUIID(0),labwidth,y-=h*1.6,w-labwidth*1.1,h,numid,sizeof(numid)) )
//	{
//		//double value;
//		//sscanf(numz,"%lf",&value);
//		//sprintf(newnum,"Represented value is: %.20f", value);
//	}
    strcpy(c,"group:");
	SetPenColor("White");
	if(GroupIndex)sprintf(c+strlen(c),"%d",selectedgroup);
	drawLabel(x-TextStringWidth(c)/2, y+fH-0.06, c);
}

void CharEventProcess(char ch)
{
	uiGetChar(ch);
	display();
}


void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key,event);
	display();
}

void MouseEventProcess(int x, int y, int button, int event)
{

	uiGetMouse(x,y,button,event);
	display();
}


#define MY_ROTATE_TIMER  1


void TimerEventProcess(int timerID)
{
	if( timerID==MY_ROTATE_TIMER)
	{
//		rot_angle += 10;
		display();
	}
}




void consolecmd(void){
	InitConsole();
	consolebit=2;
	consolebit=cmd();
	if(consolebit==0)CloseConsole();
}
void Main()
{
    int i,j;
    RECT freeview;

	SetWindowTitle("Graphics User Interface Demo");
	//SetWindowSize(10, 10);
//	SetWindowSize(24, 13.5);
	//SetWindowSize(10, 20);
    InitGraphics();
    winwidth = GetWindowWidth();
    winheight = GetWindowHeight();
    dcx=winwidth*0.45;
    dcy=(winheight-1.5*GetFontHeight())/2;
    cx=winwidth*(0.1+0.75*0.9);
    cy=dcy/2;
//    InvalidateRect(lcorner[0][0],)
    // SetRect(&freeview,ScaleX(lcorner[0][0]),ScaleY(rcorner[0][1]),ScaleX(rcorner[0][0]),ScaleY(lcorner[0][1]));
//    DoUpdate();
    // InvalidateRect(graphicsWindow,&freeview,1);
	registerCharEvent(CharEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
	registerMouseEvent(mousecontrol);
	registerTimerEvent(TimerEventProcess);
	startTimer(MY_ROTATE_TIMER,30);
}

#if defined(DEMO_MENU)
void drawMenu()
{
	static char * menuListFile[] = {"File",
        "New   Ctrl-N",
		"Open",
		"Close ",
		"Save  Ctrl-S",
		"Save as...",
		"Exit"};
	static char * menuListTool[] = {"View",
		"",
		"Console   Ctrl-E"
		};
	static char * menuListHelp[] = {"Help",
		"About"};

	static char * selectedLabel = NULL;
	menuListTool[1]=FREE?"Divide":"Free";
	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winheight;
	double h = fH*1.5;
	double w = winwidth*0.1;
	double wlist = winwidth*0.12;
	double xindent = winheight/20;
	int    selection;
    HINSTANCE Help;
	drawMenuBar(0,y-h,winwidth,h);//here it is
	drawMenuBar(0,0,winwidth/10,winheight);

	if(FREE==0){
		SetPenSize(10);
		SetPenColor("Dark Gray");
		MovePen((winwidth+winwidth*0.1)/2,0);//here it is
		DrawLine(0,winheight-1.5*GetFontHeight());
		MovePen(0,(winheight-1.5*GetFontHeight())/2);
		DrawLine(winwidth,0);
		SetPenSize(1);
	}
	selection = menuList(GenUIID(0), x, y-h, w, w, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
	if( selection>0 ) selectedLabel = menuListFile[selection];
	switch (selection){
	    case 1:
	        kernel("");
	        initbit=0;
            break;
	    case 2:
			if(initbit){
				kernel("20V.obj");
				initbit=0;
			}
			else consolecmd();
			// else {
			// 	InitConsole();
			// 	consolebit=2;
			// 	consolebit=cmd();
			// 	// consolebit=cmd("open");
			// 	if(consolebit==0)CloseConsole();
			// }
			break;
        case 3:
			if(*currentstorage) FlushToDisk(currentstorage);
            	else FlushToDisk("data1.obj");
			kernel("");
	        initbit=0;
            break;
		case 4:
			if(initbit==0)if(*currentstorage) FlushToDisk(currentstorage);
            	else FlushToDisk("data1.obj");
			break;
		case 5:
			consolecmd();break;
		case 6:exit(-1);
	}

	selection = menuList(GenUIID(0),x+w,  y-h, wlist, wlist,h, menuListTool,sizeof(menuListTool)/sizeof(menuListTool[0]));
	if( selection>0 ) selectedLabel = menuListTool[selection];
	if( selection==1 ){
		if(FREE){
			cx=(0.75*0.9+0.1)*GetWindowWidth();
			cy/=2;
			proportion/=2;
		}
		else {
			cx=0.55*GetWindowWidth();
			cy*=2;
			proportion*=2.0;
		}
		FREE=!FREE;
	}
    else if(selection==2)consolecmd();

	//menuListHelp[1] = show_more_buttons ? "Show Less | Ctrl-M" : "Show More | Ctrl-M";
	selection = menuList(GenUIID(0),x+w+wlist,y-h, wlist, wlist, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection>0 ) selectedLabel = menuListHelp[selection];
	if( selection==1 )Help=ShellExecuteA(graphicsWindow, "open", "readme.txt", NULL, "$PWD",SW_SHOW);
//		show_more_buttons = ! show_more_buttons;
	/*selection = menuList2(GenUIID(0),0,y-2*h-1.2, w, wlist, h, menuListOne,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection>0 ) selectedLabel = menuListHelp[selection];*/

	/*x = winwidth/15;
	y = winheight/8*7;
	SetPenColor("Black");
	drawLabel(x,    y-=h, "Most recently selected menu is:");
	SetPenColor("Red");
	drawLabel(x+xindent, y-=h, selectedLabel);
	SetPenColor("Blue");
	drawLabel(x,    y-=h, "Control Variable Status");
	SetPenColor("Orange");
	drawLabel(x+xindent, y-=h, enable_rotation ? "Rotation Enabled" : "Rotation Disabled");
	drawLabel(x+xindent, y-=h, show_more_buttons ? "More Buttons" : "Less Button");*/
}
#endif // #if defined(DEMO_MENU)

//#if defined(DEMO_ROTATE)

//#include <math.h>

//#endif //#if defined(DEMO_ROTATE)

#if defined(DEMO_BUTTON)

void drawButtons()
{
	double fH = GetFontHeight();
	double h = fH*2;
	double x = 0;//winwidth/2.5;
	double y = winheight/(1.5)-h;
	double w = winwidth/10;
	/*if( button(GenUIID(0), x, y-3*h, w, h, "Random Button Colors") )
	{
		static char * c[] = {"Black","Dark Gray","Gray","Light Gray","White", "Cyan",
			"Brown", "Red", "Orange", "Yellow", "Green","Blue","Violet", "Magenta"};
		int N = sizeof(c)/sizeof(c[0]);
		static int times = 0; times++;
		setButtonColors (c[times%N], c[(times+1)%N], c[(times+2)%N], c[(times+3)%N], 1);
	}

	if( button(GenUIID(0), x, y-4.5*h, w, h, "Random EditBox Colors") )
	{
		static char * c[] = {"Black","Dark Gray","Gray","Light Gray","White", "Cyan",
			"Brown", "Red", "Orange", "Yellow", "Green","Blue","Violet", "Magenta"};
		int N = sizeof(c)/sizeof(c[0]);
		static int times = 0; times++;
		setTextBoxColors (c[times%N], c[(times+1)%N], c[(times+2)%N], c[(times+3)%N], 1);
	}
	*/
}
#endif // #if defined(DEMO_BUTTON)

#if defined(DEMO_EDITBOX)

void drawEditText()
{
	static char firstName[80] = "Xinguo";
	static char lastName[80] = "Liu";
	static char results[200] = "";
	static char memo[80]="Hello";
	static char numstr[80] = "3.1415";

	static char newnum[80] = "";
	double fH = GetFontHeight();
	double h = fH*2;
	double w = winwidth/10;
	double x = winwidth/15;
	double y = winheight/2-h;
	double labwidth=fH*1.3;

		static char numx[80] ;
		static char numy[80] ;
		static char numid[80]="0" ;

	SetPenColor("Brown");

	//if( textbox(GenUIID(0), x, y, w, h, firstName, sizeof(firstName)) )
	//	sprintf(results,"Text edit result is: %s+%s", firstName,lastName);

	//SetPenColor("Brown");

	//if( textbox(GenUIID(0), x, y, w, h, lastName, sizeof(lastName)) )
	//	sprintf(results,"Text edit result is: %s+%s", firstName,lastName);

	/*SetPenColor("White");
	//drawLabel(x, y-=fH*1.2, results);

	drawLabel(labwidth/2-TextStringWidth("x")/2, y+fH-0.06, "x");
	if(textbox(GenUIID(0), labwidth, y, w-labwidth*1.1, h, numx, sizeof(numx)))
	{

	}

	if(textbox(GenUIID(0), labwidth, y-=h*1.6, w-labwidth*1.1, h, numy, sizeof(numy)))
	{

	}
	SetPenColor("White");
	drawLabel(labwidth/2-TextStringWidth("y")/2, y+fH-0.06, "y");

	//y = winheight/2;
	if( textbox(GenUIID(0),labwidth,y-=h*1.6,w-labwidth*1.1,h,numz,sizeof(numz)) )
	{
		double value;
		sscanf(numz,"%lf",&value);
		//sprintf(newnum,"Represented value is: %.20f", value);
	}
	SetPenColor("White");
	drawLabel(labwidth/2-TextStringWidth("z")/2, y+fH-0.06, "z");*/
//strcpy(numid,GetDoubleStr(selectedgroup));

    /*if( textbox(GenUIID(0),labwidth,y-=h*1.6,w-labwidth*1.1,h,numid,sizeof(numid)) )
	{
		//double value;
		//sscanf(numz,"%lf",&value);
		//sprintf(newnum,"Represented value is: %.20f", value);
	}
	SetPenColor("White");
	drawLabel(labwidth/2-TextStringWidth("id")/2, y+fH-0.06, "id");*/



	SetPenColor("Gray");
	drawLabel(x,y-=fH*1.2, newnum);
}
#endif // #if defined(DEMO_EDITBOX)


//void displayxyz(double x,double y,double z)
//{
//	double fH = GetFontHeight();
//	double h = fH*2;
//	double w = winwidth/10;
//	//double x = winwidth/15;
//	//double y = winheight/2-h;
//	double labwidth=fH*1.3;
//	static char numx[80]  ;
//	static char numy[80] ;
//	static char numz[80] ;
//
//
//	strcpy(numx,GetDoubleStr(x));
//	strcpy(numy,GetDoubleStr(y));
//	strcpy(numz,GetDoubleStr(z));
//
//		SetPenColor("White");
//	//drawLabel(x, y-=fH*1.2, results);
//
//	drawLabel(labwidth/2-TextStringWidth("x")/2, y+fH-0.06, "x");
//	if(textbox(GenUIID(0), labwidth, y, w-labwidth*1.1, h, numx, sizeof(numx)))
//	{
//
//	}
//
//	if(textbox(GenUIID(0), labwidth, y-=h*1.6, w-labwidth*1.1, h, numy, sizeof(numy)))
//	{
//
//	}
//	SetPenColor("White");
//	drawLabel(labwidth/2-TextStringWidth("y")/2, y+fH-0.06, "y");
//
//	//y = winheight/2;
//	if( textbox(GenUIID(0),labwidth,y-=h*1.6,w-labwidth*1.1,h,numz,sizeof(numz)) )
//	{
//		double value;
//		sscanf(numz,"%lf",&value);
//		//sprintf(newnum,"Represented value is: %.20f", value);
//	}
//	SetPenColor("White");
//	drawLabel(labwidth/2-TextStringWidth("z")/2, y+fH-0.06, "z");
//}
//
//void onvertice(gs_UIState.mousedown)
//{
//	if(gs_UIState.mousedown==0)
//	return;
//	int i,j,g;
//	int flag=0;
//	vertice v;
//	for(g=0;g<GroupIndex;g++)
//        for(j=lastcount[g].vertice+1;j<lastcount[g+1].vertice+1;j++)
//	{
//		if(v.x==gs_UIState.mousex&&v.z==gs_UIState.mousey||(v.x==gs_UIState.mousex&&v.y==gs_UIState.mousey||(v.y==gs_UIState.mousex&&v.z==gs_UIState.mousey)
//		{
//			flag=1;
//			break;
//		}
//	}
//	if(flag==1)
//		displayxyz(v.x,v.y,v.z);
//
//}
void display()
{
    int i;

	DisplayClear();
    if(updated==0){
        updated=1;
    }
    display_all(FREE);
	if(rotatebit)drawaxis(selectedgroup,rotatebit-1);

	if(selectedvertex>-1){
		SetPenSize(PENBOLDFACE);
		if(mousepressed.rightbutton==0)
			SetPenColor("Orange");
			else SetPenColor("Gray");
		if(FREE==0)for(i=0;i<4;i++)showvertice(i,vt_i[selectedvertex]);
		// showvertice(vertexview,vt_i[selectedvertex]);
	}
//    for(i=0;i<1;i++)
//        clearRect(lc[i][0],lc[i][1],rc[i][0],rc[i][1]);
//    if(consolebit==2){
//        consolebit=1;
//        CloseConsole();
//    }
//    onvertice();
#if defined(DEMO_MENU)
	drawMenu();
#endif

#if defined(DEMO_ROTATE)

#endif

#if defined(DEMO_BUTTON)

	drawButtons();
#endif

#if defined(DEMO_EDITBOX)

	drawEditText();
#endif
    if(selectedgroup>=0)displayxyz(selectedgroup);
//
//	UpdateDisplay();
}
void displayfloat(double n){
    char c[6];
    sprintf(c,"%.2f  ",n);
    DrawTextString(c);
}
