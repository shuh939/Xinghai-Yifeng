#ifndef DK_GUI_H_
#define DK_GUI_H_

#include "../Global.h"

#ifdef _START_ID_
#define GUID __LINE__ + _START_ID_
#else
#define GUID __LINE__
#endif // !_START_ID_

typedef int ItemID;		//控件ID

/*!> 对齐方式 */
enum Alignment
{
	AlignTop		= 1,	/*!> 顶部对齐 */
	AlignBottom		= 2,	/*!> 底部对齐 */
	AlignLeft		= 4,	/*!> 左对齐 */
	AlignRight		= 8,	/*!> 右对齐 */
	AlignHCenter	= 16,	/*!> 水平居中对齐 */
	AlignVCenter	= 32,	/*!> 垂直居中对齐 */
	AlignCenter		= AlignHCenter | AlignVCenter
};

/*!> 方向 */
enum Orientation
{
	Horizontal,
	Vertical
};

/*!> Edit显示模式 */
enum EchoMode
{
	Normal,			/*!> 正常显示 */
	Password,		/*!> 密码 */
	NoEcho			/*!> 不显示 */
};

/*UIState*/
void GUI_Init();
void GUI_Update(ExMessage* msg);
void GUI_Prepare();
void GUI_Finish();

/*按钮*/
bool GUI_TextButton(ItemID id, const Rect&  rect, const char* text,bool lucency = false);	/*!> 文本按钮 */
bool GUI_TextureButton(ItemID id,const Point& pos, IMAGE* normalImg, IMAGE* hoverImg = nullptr);	/*!> 图片按钮 */
bool GUI_LucencyButton(ItemID id, const Rect&  rect,bool test = false);					/*!> 透明按钮 */

bool GUI_SwitchButtonEx(ItemID id, const Rect&  rect, bool* state, const char* offText = "off", const char* onText = "on");
bool GUI_SwitchButton(ItemID id, const Rect&  rect, bool* state);

typedef struct GroupID
{
	int id;					/*!> 组id */
	ItemID checkedItem;		/*!> 当前选中的itemid */
	const char* itemText;	/*!> 当前选中的item的lable */
}GroupID;
GroupID GUI_CreateGroupID();
bool GUI_RadioButton(ItemID id, const Rect&  rect, const char* label, bool* checked, GroupID* group = nullptr);
bool GUI_CheckBox(ItemID id, const Rect&  rect, const char* label, bool* checked);

/*输入框*/
enum EditSignal {Edit_Normal,Edit_Finish,Edit_TextChanged};
EditSignal GUI_Edit(ItemID id,const Rect&  rect,char* buffer,const char* placeholderText,EchoMode mode = Normal);
bool GUI_ChineseEdit(char* buffer, const char* def_value = nullptr,const char* title = nullptr, const char* hitText = NULL);

/*文本框*/
void GUI_Label(const Rect&  rect, const char* text,COLORREF color = BLACK,int aliginment = AlignCenter);
void GUI_LabelFormatEx(const Rect&  rect, COLORREF color, int aliginment, const char* fmt, ...);
void GUI_LabelFormat(const Rect&  rect, const char* fmt, ...);

/*对话框*/
bool GUI_DialogBase(ItemID id,const char* title, const Rect&  rect, Rect *  clientRect);
void GUI_DialogBegin();
void GUI_DialogEnd();

#define GUI_DialogEx(title,width,height)\
        Rect ___rect = { (getwidth() - (width)) / 2,(getheight() - (height)) / 2,(width),(height) };\
		Rect crect;\
		if(GUI_DialogBase(GUID, title, ___rect, &crect))

#define GUI_Dialog(title) GUI_DialogEx(title,350,250)

/* 滑块 */
bool GUI_SliderHorizontal(ItemID id, const Rect&  r, int maxVal, int* value);
bool GUI_SliderVertical(ItemID id, const Rect&  r, int maxVal, int* value);
bool GUI_Slider(ItemID id, const Rect&  rect, int maxVal, int* value);

/** 年选择控件 */
struct YearPickerData
{
	int year;			/*当前选择的年份*/
	int _startYear;		/*控件初始选择年份*/
};
bool GUI_YearPicker(ItemID id,const Point& pos, YearPickerData* currentYear);

/*helper*/
bool pointInRect(const Point& pos, const Rect&  rect);
void SetFont(int size, const char* family);


/*GUIStyle*/
/*颜色*/
typedef struct GUI_Color
{
	COLORREF color;
	COLORREF hoverColor;
}GUI_Color;
/*边框*/
typedef struct GUI_Border
{
	COLORREF color;
	COLORREF hoverColor;
	int width;		//边框宽度
	int radius;		//圆角半径
}GUI_Border;
/*文本*/
typedef struct GUI_Text
{
	COLORREF color;
	COLORREF hoverColor;
	int size;				//文字大小
	const char* font;		//字体
}GUI_Text;
/*样式*/
typedef struct GUIStyle
{
	GUI_Color  background;
	GUI_Text   text;
	GUI_Border border;
	GUI_Color  focus;
}GUIStyle;

extern  GUIStyle ButtonStyle;	//! 按钮样式
extern  GUIStyle EditStyle;		//! 编辑框样式

void saveStyle();			//保存样式
void restoreStyle();		//恢复样式
#endif // !DK_GUI_H_


