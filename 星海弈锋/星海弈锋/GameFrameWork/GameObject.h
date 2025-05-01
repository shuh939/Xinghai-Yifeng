#pragma once

#include <memory>
#include "Geometry.h"

class IMAGE;
class Scene;

class GameObject
{
public:
	GameObject();
	GameObject(IMAGE* img,int x,int y,int w,int h,float xSpeed,float ySpeed,Scene* scene);
	GameObject(IMAGE* img,int x,int y,float xSpeed,float ySpeed,Scene* scene);
	virtual ~GameObject() {}

	virtual void draw();
	virtual void update();

	void destroy() { m_active = false; }		//销毁对象
	bool active()const { return m_active; }		//获取活动状态
public:
	int tag{-1};		//游戏对象的标签
	IMAGE* img{};
	float x;
	float y;
	int width;
	int height;
	float xSpeed;
	float ySpeed;
	Scene* scene{};
private:
	bool m_active{true};		//游戏对象是否是活动的：不活动的就要被释放了
};

