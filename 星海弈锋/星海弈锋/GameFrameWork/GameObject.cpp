#include "GameObject.h"
#include "ResManager.h"

GameObject::GameObject()
	:GameObject(nullptr, 0, 0, 0, 0, nullptr)
{
}

GameObject::GameObject(IMAGE* img, int x, int y, int w, int h, float xSpeed, float ySpeed, Scene* scene)
	: img(img)
	, x(x), y(y)
	, width(w ? w : (img ? img->getwidth() : 0))
	, height(h ? h : (img ? img->getheight() : 0))
	, xSpeed(xSpeed), ySpeed(ySpeed)
	, scene(scene)
{
}

GameObject::GameObject(IMAGE* img, int x, int y, float xSpeed, float ySpeed, Scene* scene)
	:GameObject(img, x, y, 0, 0, xSpeed, ySpeed, scene)
{
}

void GameObject::draw()
{
	if(img && active())
		drawImage(x, y, img);
}

void GameObject::update()
{
	if (active())
	{
		x += xSpeed;
		y += ySpeed;
	}
}
