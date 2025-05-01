#include "Application.h"
#include "Scene.h"
#include "gui/gui.h"
#include <thread>
#include <chrono>
#include "../TimingUtils.h" // 包含通用时间工具头文件

// 检查是否已定义_WIN32_WINNT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601 // Windows 7 or later
#endif

#include <windows.h>
#pragma comment(lib, "kernel32.lib")

// 自定义GetTickCount64函数的实现已移至TimingUtils.h

static Application* global_ins = nullptr;

Application* Application::instance()
{
	if (!global_ins) {
		printf("You must create an Application instance!\n");
	}
	return global_ins;
}

Application::Application(int argc, char* argv[])
	:Application(640,480,argc,argv)
{
}

Application::Application(int w, int h,int argc, char* argv[])
	: m_isRunning(false), m_scene(nullptr)
{
	if (global_ins) {
		printf("There can be only one Application instance!\n");
		abort();
		return;
	}
	global_ins = this;
	//创建窗口
	if (initgraph(w, h, EX_SHOWCONSOLE)) {
		m_isRunning = true;
	}
    
    // 初始化消息结构
    memset(&m_msg, 0, sizeof(ExMessage));
}

Application::~Application()
{
	if (m_scene)
		delete m_scene;
}

int Application::run()
{
	//游戏帧率控制 帧率每秒绘制的帧数 1000/120 
	float fps = 1000.0f / 120;
	ULONGLONG startTicks = MyGetTickCount64();	//获取电脑开机到调用这个函数所经过的毫秒数
	ULONGLONG realTicks = 0;

	GUI_Init();
	BeginBatchDraw();
	while (m_isRunning)
	{
		startTicks = MyGetTickCount64();
		GUI_Prepare();
		//处理消息
		handleMsg();

		//更新
		update();

		//绘图
		draw();
		FlushBatchDraw();

		//对整个循环进行控制
		realTicks = MyGetTickCount64() - startTicks;	//计算当前帧实际执行时间ms
		if (realTicks < static_cast<ULONGLONG>(fps)){
			printf("realTicks:%llu\n", realTicks);
			Sleep(static_cast<DWORD>(fps - realTicks));
		}
		GUI_Finish();
	}
	EndBatchDraw();
    return 0;
}

int Application::exec()
{
	//游戏帧率控制 帧率每秒绘制的帧数 1000/120 
	float fps = 1000.0f / 120;
	ULONGLONG startTicks = MyGetTickCount64();	//获取电脑开机到调用这个函数所经过的毫秒数

	GUI_Init();
	BeginBatchDraw();
	while (m_isRunning)
	{
		GUI_Prepare();
		//处理消息
		handleMsg();

		//更新
		update();

		//绘图
		if (MyGetTickCount64() - startTicks >= static_cast<ULONGLONG>(fps)) {
			draw();
			FlushBatchDraw();
			startTicks = MyGetTickCount64();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		GUI_Finish();
	}
	EndBatchDraw();
    return 0;
}

void Application::runScene(Scene* scene)
{
	m_scene = scene;
}

bool Application::setWindowSize(int w, int h)
{
	//获取屏幕的宽度和高度
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);
	//指定窗口的宽度和高度
	int width = w;
	int height = h;
	return SetWindowPos(GetHWnd(), HWND_TOP,(screen_width - width)/2, (screen_height - height)/2,
		width,height, SWP_SHOWWINDOW);
}

bool Application::setWindowTitle(const std::string& title)
{
	return SetWindowText(GetHWnd(), title.data());
}

void Application::handleMsg()
{
	//先清空消息
	m_msg.message = 0;
	//获取消息
	while(peekmessage(&m_msg)) {
		GUI_Update(&m_msg);
		flushmessage();
	}
}


void Application::draw()
{
	setbkcolor(RGB(230, 231, 232));
	cleardevice();
	if (m_scene) m_scene->draw();
}

void Application::update()
{
	if (m_scene) m_scene->update();
}
