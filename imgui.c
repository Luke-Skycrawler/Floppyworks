//===========================================================================
//
//  ��Ȩ�����ߣ�####
//
//  ����޸ģ�2019��4��15��
//            �����˿ؼ�����ɫ���������ã�
//            �Լ�������ɫ�����ӣ��ں���demoGuiALL.c��drawButtons�����
//  ����޸ģ�2019��2��26��
//            ��������ʾ�ı��༭��ʾ
//            �����˶�����ʾ
//            ������textbox �ı�����ؼ�
//            ���˲˵�����
//            �� uiGetInput Ϊ uiGetMouse,uiGetKey,uiGetKeyboard
//  ����޸ģ�2019��2��18��
//  ���δ�����2018��4�£�����<<�������ר��>>�γ̽�ѧ
//
//===========================================================================

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
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

#include <time.h>

#include "imgui.h"


#define KMOD_SHIFT 0x01
#define KMOD_CTRL  0x02


/* ���Ϳռ�״̬ */
typedef struct {
	double mousex;
	double mousey;
	int    mousedown;
	int    clickedItem;// item that was clicked
	int    actingMenu; // acting menu list
	int    kbdItem;    // item that takes keyboard
	int    lastItem;   // item that had focus just before
	int    keyPress;   // input key
	int    charInput;  // input char
	int    keyModifiers;  //  key modifier (shift, ctrl)
} UIState;

static UIState gs_UIState;

/* ��������	InitGUI
 *
 * ���ܣ���ʼ������
 *
 * �÷����ڴ��ڴ�������������֮�����
 */
void InitGUI()
{
	// doing nothing now
}

void mySetPenColor(char *color)
{
	if( color && strlen(color)>0 ) SetPenColor(color);
}

/*
 *  libgraphics Ԥ�������ɫ����
 *
 *  DefineColor("Black", 0, 0, 0);
 *  DefineColor("Dark Gray", .35, .35, .35);
 *  DefineColor("Gray", .6, .6, .6);
 *  DefineColor("Light Gray", .75, .75, .75);
 *  DefineColor("White", 1, 1, 1);
 *  DefineColor("Brown", .35, .20, .05);
 *  DefineColor("Red", 1, 0, 0);
 *  DefineColor("Orange", 1, .40, .1);
 *  DefineColor("Yellow", 1, 1, 0);
 *  DefineColor("Green", 0, 1, 0);
 *  DefineColor("Blue", 0, 0, 1);
 *  DefineColor("Violet", .93, .5, .93);
 *  DefineColor("Magenta", 1, 0, 1);
 *  DefineColor("Cyan", 0, 1, 1);
 */

/*
 *  �˵���ɫ
 */
static struct {
	char frame[32];
	char label[32];
	char hotFrame[32];
	char hotLabel[32];
	int  fillflag;
} gs_predefined_colors[] = {
	{"Blue",      "Blue",	"Red",	    "Red",   0 }, //
	{"Orange",    "Black", "Green",    "Blue",  0 }, //
	{"Orange",    "White", "Green",    "Blue",  1 }, // ���
	{"Light Gray","Black",  "Dark Gray","Blue",0 },  //
	{"Light Gray","Black",  "Dark Gray","Yellow",1 },  // ���
	{"Brown",     "Red",    "Orange",   "Blue",0 },
	{"Brown",     "Red",    "Orange",   "White",1 }   // ���
},

gs_menu_color = {
	"Dark Gray",      "White",	"Orange",	    "Black",   1 , // �����
},

gs_button_color = {
	"Dark Gray",      "White",	"Orange",	    "Black",   1 , // �����
},

gs_textbox_color = {
	"White",      "Black",	"White",	    "Black",   1 , // �����
};

void setButtonColors(char *frame, char*label, char *hotFrame, char *hotLabel, int fillflag)
{
	if(frame) strcpy(gs_button_color.frame, frame);
	if(label) strcpy(gs_button_color.label, label);
	if(hotFrame) strcpy(gs_button_color.hotFrame, hotFrame);
	if(hotLabel) strcpy(gs_button_color.hotLabel ,hotLabel);
	gs_button_color.fillflag = fillflag;
}

void setMenuColors(char *frame, char*label, char *hotFrame, char *hotLabel, int fillflag)
{
	if(frame) strcpy(gs_menu_color.frame, frame);
	if(label) strcpy(gs_menu_color.label, label);
	if(hotFrame) strcpy(gs_menu_color.hotFrame, hotFrame);
	if(hotLabel) strcpy(gs_menu_color.hotLabel ,hotLabel);
	gs_menu_color.fillflag = fillflag;
}

void setTextBoxColors(char *frame, char*label, char *hotFrame, char *hotLabel, int fillflag)
{
	if(frame) strcpy(gs_textbox_color.frame, frame);
	if(label) strcpy(gs_textbox_color.label, label);
	if(hotFrame) strcpy(gs_textbox_color.hotFrame, hotFrame);
	if(hotLabel) strcpy(gs_textbox_color.hotLabel ,hotLabel);
	gs_textbox_color.fillflag = fillflag;
}

void usePredefinedColors(int k)
{
	int N = sizeof(gs_predefined_colors)/sizeof(gs_predefined_colors[0]);
	gs_menu_color    = gs_predefined_colors[k%N];
	gs_button_color  = gs_predefined_colors[k%N];
	gs_textbox_color = gs_predefined_colors[k%N];
}
void usePredefinedButtonColors(int k)
{
	int N = sizeof(gs_predefined_colors)/sizeof(gs_predefined_colors[0]);
	gs_button_color  = gs_predefined_colors[k%N];
}
void usePredefinedMenuColors(int k)
{
	int N = sizeof(gs_predefined_colors)/sizeof(gs_predefined_colors[0]);
	gs_menu_color    = gs_predefined_colors[k%N];
}
void usePredefinedTexBoxColors(int k)
{
	int N = sizeof(gs_predefined_colors)/sizeof(gs_predefined_colors[0]);
	gs_textbox_color = gs_predefined_colors[k%N];
}

/* ���øú���,�õ�����״̬ */
void uiGetMouse(int x, int y, int button, int event)
{
	 gs_UIState.mousex = ScaleXInches(x);/*pixels --> inches*/
	 gs_UIState.mousey = ScaleYInches(y);/*pixels --> inches*/

	 switch (event) {
	 case BUTTON_DOWN:
		 gs_UIState.mousedown = 1;
		 break;
	 case BUTTON_UP:
		 gs_UIState.mousedown = 0;
		 break;
	 }
}

/* ���øú���,�õ����̵����� */
void uiGetKeyboard(int key, int event)
{
	if( event==KEY_DOWN )
	{
		switch (key )
		{
			case VK_SHIFT:
				gs_UIState.keyModifiers |= KMOD_SHIFT;
				break;
			case VK_CONTROL:
				gs_UIState.keyModifiers |= KMOD_CTRL;
				break;
			default:
				gs_UIState.keyPress = key;
		}
	}
	else if( event==KEY_UP )
	{
		switch (key )
		{
			case VK_SHIFT:
				gs_UIState.keyModifiers &= ~KMOD_SHIFT;
				break;
			case VK_CONTROL:
				gs_UIState.keyModifiers &= ~KMOD_CTRL;
				break;
			default:
				gs_UIState.keyPress = 0;
		}
	}
}

/* ���øú���,�õ��ı����� */
void uiGetChar(int ch)
{
	gs_UIState.charInput = ch;
}


/* ���ԣ������(x,y)�Ƿ�λ�ڰ�Χ�� [x1,x2] X [y1,y2] �ڲ� */
static bool inBox(double x, double y, double x1, double x2, double y1, double y2)
{
	return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
}

/*
 * ��������button
 *
 * ���ܣ���ʾһ����ť��button��
 *
 * �÷���if( button(GenUUID(0),x,y,w,h,label) ) {
 *           ִ����䣬��Ӧ�û����¸ð�ť
 *       }
 *
 *   id  - Ψһ��
 *   x,y - λ��
 *   w,h - ���Ⱥ͸߶�
 *   label - ��ť�ϵ����ֱ�ǩ
 *
 * ����ֵ
 *   0 - �û�û�е�������²��ͷţ���ť
 *   1 - ����˰�ť
 */






int button(int id, double x, double y, double w, double h, char *label)
{
	char * frameColor = gs_button_color.frame;
	char * labelColor = gs_button_color.label;
	double movement = 0.2*h;
	double shrink = 0.15*h;
	double sinkx = 0, sinky = 0;
	//int isHotItem = 0;

	if (inBox(gs_UIState.mousex, gs_UIState.mousey, x, x + w, y, y + h)) {
		frameColor = gs_button_color.hotFrame;
		labelColor = gs_button_color.hotLabel;
		gs_UIState.actingMenu = 0; // menu lose focus
		if ( gs_UIState.mousedown) {
			gs_UIState.clickedItem = id;
			sinkx =   movement;
			sinky = - movement;
		}
	}
	else {
		if ( gs_UIState.clickedItem==id )
			gs_UIState.clickedItem = 0;
	}

	// If no widget has keyboard focus, take it
	if (gs_UIState.kbdItem == 0)
		gs_UIState.kbdItem = id;
	// If we have keyboard focus, we'll need to process the keys
	if ( gs_UIState.kbdItem == id && gs_UIState.keyPress==VK_TAB )
	{
		// If tab is pressed, lose keyboard focus.
		// Next widget will grab the focus.
		gs_UIState.kbdItem = 0;
		// If shift was also pressed, we want to move focus
		// to the previous widget instead.
		if ( gs_UIState.keyModifiers & KMOD_SHIFT )
			gs_UIState.kbdItem = gs_UIState.lastItem;
		gs_UIState.keyPress = 0;
	}
	gs_UIState.lastItem = id;

	// draw the button
	mySetPenColor(frameColor);
	drawBox(x+sinkx, y+sinky, w, h, gs_button_color.fillflag,
		label, 'C', labelColor);
	if( gs_button_color.fillflag ) {
		mySetPenColor( frameColor );
		drawRectangle(x+sinkx, y+sinky, w, h, 0);
	}

	// ��������ʾ, show a small ractangle frane
	if( gs_UIState.kbdItem == id ) {
		mySetPenColor( labelColor );
		//drawRectangle(x+sinkx+shrink, y+sinky+shrink, w-2*shrink, h-2*shrink, 0);
	}

	if( gs_UIState.clickedItem==id && // must be clicked before
		! gs_UIState.mousedown )   // but now mouse button is up
	{
		gs_UIState.clickedItem = 0;
		gs_UIState.kbdItem = id;
		return 1;
	}

	return 0;
}

/*
 * ��ʾһ���˵���
 *   id  - �˵����Ψһ��
 *   x,y - �˵����λ��
 *   w,h - �˵���Ĵ�С
 *   label - �˵���ı�ǩ����
 *
 * ����ֵ
 *   0 - �û�û�е�������²��ͷţ��˲˵���
 *   1 - ����˴˲˵���
 */
static int menuItem(int id, double x, double y, double w, double h, char *label)
{
	char * frameColor = gs_menu_color.frame;
	char * labelColor = gs_menu_color.label;
	if (inBox(gs_UIState.mousex, gs_UIState.mousey, x, x + w, y, y + h)) {
		frameColor = gs_menu_color.hotFrame;
		labelColor = gs_menu_color.hotLabel;
		//if (gs_UIState.mousedown) {
		if ( (gs_UIState.clickedItem == id ||gs_UIState.clickedItem == 0) && gs_UIState.mousedown) {
			gs_UIState.clickedItem = id;
		}
	}
	else {
		if ( gs_UIState.clickedItem==id )
			gs_UIState.clickedItem = 0;
	}

	mySetPenColor(frameColor);
	drawBox(x, y, w, h, gs_menu_color.fillflag, label, 'L', labelColor);

	if( gs_UIState.clickedItem==id && // must be clicked before
		! gs_UIState.mousedown )     // but now mouse button is up
	{
		gs_UIState.clickedItem = 0;
		return 1;
	}

	return 0;
}

/*
 * ��������shortcutkey
 *
 * ���ܣ��Ӳ˵���ǩ����ȡ����ݼ�����д��ĸ
 *       Ҫ���ݼ���־ Ctrl-X λ�ڱ�ǩ�Ľ�β����
 */
static char ToUpperLetter(char c)
{
	return (c>='a' && c<='z' ? c-'a'+'A' : c);
}

static char shortcutkey(char *s)
{
	char predStr[] = "Ctrl-";
	int M = strlen(predStr)+1;
	int n = s ? strlen(s) : 0;

	if( n<M || strncmp(s+n-M, predStr, M-1)!=0 )
		return 0;

	return ToUpperLetter(s[n-1]);
}

/*
 * ��������menuList
 *
 * ���ܣ���ʾһ����˵�
 *
 * �÷���choice = menuList(GenUUID(0),x,y,w,h,labels,n);
 *
 *   id  - �˵���Ψһ��
 *   x,y,w,h - �˵�����λ�úʹ�С
 *   wlist,h - �˵��б��Ŀ��Ⱥ͸߶�
 *   labels[] - ��ǩ���֣�[0]�ǲ˵����[1...n-1]�ǲ˵��б�
 *   n   - �˵���ĸ���
 *
 * ����ֵ
 *   -1    - �û�û�е�������²��ͷţ���ť
 *   >=0   - �û�ѡ�еĲ˵��� index ����labels[]�У�
 *
 */
int menuList(int id, double x, double y, double w, double wlist, double h, char *labels[], int n)
{
	static int unfoldMenu = 0;
	int result = 0;
	int k = -1;

	// 处理快捷键

	if( gs_UIState.keyModifiers & KMOD_CTRL ) {
		for( k=1; k<n; k++ ) {
			int kp = ToUpperLetter( gs_UIState.keyPress );
			if( kp && kp == shortcutkey(labels[k]) ) {
				gs_UIState.keyPress = 0;
				break;
			}
		}
	}

	if( k>0 && k<n )
	{	// 成功匹配快捷键
        gs_UIState.keyModifiers =0;
		unfoldMenu = 0;
		return k;
	}

	// 处理鼠标

	if( inBox(gs_UIState.mousex, gs_UIState.mousey, x, x + w, y, y + h) )
		gs_UIState.actingMenu = id;

	if( menuItem(id, x, y, w, h, labels[0]) )
		unfoldMenu = ! unfoldMenu;

	if( gs_UIState.actingMenu == id && unfoldMenu  ) {
		int k;
		gs_UIState.charInput = 0; // disable text editing
		gs_UIState.keyPress = 0;  // disable text editing
		for( k=1; k<n; k++ ) {
			if ( menuItem(GenUIID(k)+id, x, y-k*h, wlist, h, labels[k]) ) {
				unfoldMenu = 0;
				result = k;
			}
		}
	}
	return result;
}

//�����ŵĲ˵�
//int menuList1(int id, double x, double y, double w, double wlist, double h, char *labels[], int n)
//{
//	static int unfoldMenu = 0;
//	int result = 0;
//	int k = -1;
//	// ������ݼ�
//
//	if( gs_UIState.keyModifiers & KMOD_CTRL ) {
//		for( k=1; k<n; k++ ) {
//			int kp = ToUpperLetter( gs_UIState.keyPress );
//			if( kp && kp == shortcutkey(labels[k]) ) {
//				gs_UIState.keyPress = 0;
//				break;
//			}
//		}
//	}
//
//	if( k>0 && k<n )
//	{	// �ɹ�ƥ���ݼ�
//		unfoldMenu = 0;
//		return k;
//	}
//
//	// �������
//
//	if( inBox(gs_UIState.mousex, gs_UIState.mousey, x, x + w, y, y + h) )
//		gs_UIState.actingMenu = id;
//
//	if( menuItem(id, x, y, w, h, labels[0]) )
//		unfoldMenu = ! unfoldMenu;
//
//	if( gs_UIState.actingMenu == id && unfoldMenu  ) {
//		int k;
//		gs_UIState.charInput = 0; // disable text editing
//		gs_UIState.keyPress = 0;  // disable text editing
//		for( k=1; k<n; k++ ) {
//			if ( menuItem(GenUIID(k)+id, x+w, y-k*h+h, wlist, h, labels[k]) ) {
//				unfoldMenu = 0;
//				result = k;
//			}
//		}
//	}
//	return result;
//}

void drawMenuBar(double x, double y, double w, double h)
{
	mySetPenColor(gs_menu_color.frame);
	drawRectangle(x,y,w,h,gs_menu_color.fillflag);
}


/*
 * ��������textbox
 *
 * ���ܣ���ʾһ���ı��༭����ʾ�ͱ༭�ı��ַ���
 *
 * �÷���textbox(GenUUID(0),x,y,w,h,textbuf,buflen);
 *       ����
         if( textbox(GenUUID(0),x,y,w,h,textbuf,buflen) ) {
 *           �ı��ַ������༭�޸��ˣ�ִ����Ӧ���
 *       }
 *
 *   id  - Ψһ�ţ�һ����GenUUID(0), ����GenUUID��k)��k��ѭ��������
 *   x,y - �ı���λ��
 *   w,h - �ı���Ŀ��Ⱥ͸߶�
 *   textbuf - ���༭���ı��ַ�������\0��β��
 *   buflen - �ɴ洢���ı��ַ�������󳤶�
 * ����ֵ
 *   0 - �ı�û�б��༭
 *   1 - ���༭��
 */
int textbox(int id, double x, double y, double w, double h, char textbuf[], int buflen)
{
	char * frameColor = gs_textbox_color.frame;
	char * labelColor = gs_textbox_color.label;
	int textChanged = 0;
	int len = strlen(textbuf);
	double indent = GetFontAscent()/2;
	double textPosY = y+h/2-GetFontAscent()/2;

	if ( inBox(gs_UIState.mousex, gs_UIState.mousey, x, x + w, y, y + h) )
	{
		frameColor = gs_textbox_color.hotFrame;
		labelColor = gs_textbox_color.hotLabel;
		gs_UIState.actingMenu = 0; // menu lose focus
		if ( gs_UIState.mousedown) {
			gs_UIState.clickedItem = id;
		}
	}

	// If no widget has keyboard focus, take it
	if (gs_UIState.kbdItem == 0)
		gs_UIState.kbdItem = id;

	if (gs_UIState.kbdItem == id)
		labelColor = gs_textbox_color.hotLabel;

	// Render the text box
	mySetPenColor(frameColor);
	drawRectangle(x, y, w, h, gs_textbox_color.fillflag);
	// show text
	mySetPenColor(labelColor);
	MovePen(x+indent, textPosY);
	DrawTextString(textbuf);
	// add cursor if we have keyboard focus
	if ( gs_UIState.kbdItem == id && (clock() >> 8) & 1)
	{
		//MovePen(x+indent+TextStringWidth(textbuf), textPosY);
		DrawTextString("_");
	}

	// If we have keyboard focus, we'll need to process the keys
	if ( gs_UIState.kbdItem == id )
	{
		switch (gs_UIState.keyPress)
		{
		case VK_RETURN:
		case VK_TAB:
			// lose keyboard focus.
			gs_UIState.kbdItem = 0;
			// If shift was also pressed, we want to move focus
			// to the previous widget instead.
			if ( gs_UIState.keyModifiers & KMOD_SHIFT )
				gs_UIState.kbdItem = gs_UIState.lastItem;
			// Also clear the key so that next widget won't process it
			gs_UIState.keyPress = 0;
			break;
		case VK_BACK:
			if( len > 0 ) {
				textbuf[--len] = 0;
				textChanged = 1;
			}
			gs_UIState.keyPress = 0;
			break;
		}
		// char input
		if (gs_UIState.charInput >= 32 && gs_UIState.charInput < 127 && len+1 < buflen ) {
			textbuf[len] = gs_UIState.charInput;
			textbuf[++len] = 0;
			gs_UIState.charInput = 0;
			textChanged = 1;
		}
	}

	gs_UIState.lastItem = id;

	if( gs_UIState.clickedItem==id && // must be clicked before
		! gs_UIState.mousedown )     // but now mouse button is up
	{
		gs_UIState.clickedItem = 0;
		gs_UIState.kbdItem = id;
	}

	return textChanged;
}


/* ��һ������ */
void drawRectangle(double x, double y, double w, double h, int fillflag)
{
	MovePen(x, y);
	if( fillflag ) StartFilledRegion(1);
	{
		DrawLine(0, h);
		DrawLine(w, 0);
		DrawLine(0, -h);
		DrawLine(-w, 0);
	}
	if( fillflag ) EndFilledRegion();
}

/* ��һ�����Σ��������ڲ�������ʾһ���ַ�����ǩlabel */
void drawBox(double x, double y, double w, double h, int fillflag, char *label, char labelAlignment, char *labelColor)
{
	double fa = GetFontAscent();
	// rect
	drawRectangle(x,y,w,h,fillflag);
	// text
	if( label && strlen(label)>0 ) {
		mySetPenColor(labelColor);
		if( labelAlignment=='L' )
			MovePen(x+fa/2, y+h/2-fa/2);
		else if( labelAlignment=='R' )
			MovePen(x+w-fa/2-TextStringWidth(label), y+h/2-fa/2);
		else // if( labelAlignment=='C'
			MovePen(x+(w-TextStringWidth(label))/2, y+h/2-fa/2);
		DrawTextString(label);
	}
}



/* ��ʾ�ַ�����ǩ */
void drawLabel(double x, double y, char *label)
{
	if( label && strlen(label)>0 ) {
		MovePen(x,y);
		DrawTextString(label);
	}
}
