#pragma once
//取消unicode字符集
#ifdef UNICODE
#undef UNICODE
#endif // UNICODE

#include <easyx.h>
#include <string>

#define sApp Application::instance()
class Scene;

class Application
{
public:
	static Application* instance();
	Application(int argc, char *argv[]);
	Application(int w,int h,int argc = 0, char *argv[] = nullptr);
	~Application();
	int run();
	int exec();
	void quit() { m_isRunning = false; }

	void runScene(Scene* scene);

public:
	int width()const { return getwidth(); }
	int height()const { return getheight(); }
	bool setWindowSize(int w, int h);
	bool setWindowTitle(const std::string& title);

	const ExMessage* msg()const { return &m_msg; }
private:
	void handleMsg();
	void draw();
	void update();
private:
	bool m_isRunning{};		//游戏是否在运行
	ExMessage m_msg;		//消息结构
	Scene* m_scene;
};

