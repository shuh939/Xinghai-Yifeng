#pragma once
#include <memory>

class IMAGE;
/*
* 场景
*/
class Scene
{
public:
	using ptr = std::shared_ptr<Scene>;
public:
	virtual ~Scene() {}
	virtual void draw() = 0;
	virtual void update() = 0;
};

