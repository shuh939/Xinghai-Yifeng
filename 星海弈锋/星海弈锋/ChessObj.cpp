#include "ChessObj.h"
#include "ResManager.h"
#include "GameScene.h"
#include <sstream>
ChessObj::ChessObj(Player player, const Point& index, const std::string& name, Scene* scene)
	:GameObject(nullptr,-1,-1,65,65,0,0,scene)
	,player(player)
	,initIndex(index)
	,index(index)
	,name(name)
{
}

void ChessObj::draw()
{
	auto path = name + std::to_string(player) + ".png";
	auto img = ResManager::instance()->cacheImage(path, path);
	if (img) {
		width = img->getwidth();
		height = img->getheight();
		drawImage(x, y, img);
	}
}

void ChessObj::update()
{
	x = index.x * SIZE + MARGIN - SIZE / 2;
	y = index.y * SIZE + MARGIN - SIZE / 2;
}

void ChessObj::reverse()
{
	index.x = 9 - index.x - 1;
	index.y = 10 - index.y - 1;
	initIndex = index;
}

bool ChessObj::isAbleMove(const Point& idx) const
{
	if (name == "king")
	{
		//在王宫内、走的是直线、走一步
		return isKingHome(idx) && whatLine(idx) == 1 && getStep(idx) == 1;
	}
	else if (name == "ma")
	{
		//走日字对角线、没有蹩脚
		return whatLine(idx) < 0 && !isLame(idx);
	}
	else if (name == "che")
	{
		//走直线、走任意步、目标点与起点之间不能有棋子
		return whatLine(idx) > 1 && getChessCount(idx) == 0;
	}
	else if (name == "bing")
	{
		//过河之前只能前行
			//return (whatLine(idx) > 1 && getStep(idx) == 1 && isForward(idx)) ||
			//	(whatLine(idx) > 1 && getStep(idx) == 1 && abs(idx.x - index.x) == 1);
		return whatLine(idx) > 1 && getStep(idx) == 1 && 
				(isForward(idx) || abs(idx.x - index.x) == 1);
	}
	return  false;
}

bool ChessObj::isKingHome(const Point& idx) const
{
	//x轴越界判断
	if (idx.x < 3 || idx.x >5)
		return false;
	//y轴越界判断
	//--上面的
	if (isAbove()) {
		if (idx.y < 0 || idx.y >2)
			return false;
	}
	//--下面的
	else {
		if (idx.y < 7 || idx.y >9)
			return false;
	}
	return true;
}

int ChessObj::whatLine(const Point& idx1, const Point& idx2)
{
	//垂直方向
	if (idx1.x == idx2.x) {
		return 2;
	}
	//水平方向
	if (idx1.y == idx2.y) {
		return 3;
	}
	//对角线
	if (abs(idx1.x - idx2.x) == abs(idx1.y - idx2.y)) {
		return 1;
	}
	//日字对角线
	//日
	if (abs(idx1.x - idx2.x) == 1 && abs(idx1.y - idx2.y) == 2) {
		return -1;
	}
	//倒日
	if (abs(idx1.x - idx2.x) == 2 && abs(idx1.y - idx2.y) == 1) {
		return -2;
	}
	return 0;
}

int ChessObj::getStep(const Point& idx) const
{
	auto wl = whatLine(idx);
	if (wl == 2) {
		return abs(index.y - idx.y);
	}
	else if (wl == 3) {
		return abs(index.x - idx.x);
	}
	else if (wl == 1) {
		return abs(index.x - idx.x);
	}
	return 0;
}

bool ChessObj::isLame(const Point& idx) const
{
	if (name == "xiang")
	{
		//获取象的index和idx中心点索引
		Point p{
			(index.x +idx.x)/2,
			(index.y +idx.y)/2
		};
		return static_cast<GameScene*>(scene)->getChessByIndex(p);
	}
	else if (name == "ma")
	{
		Point p;
		auto wl = whatLine(idx);
		//日
		if (wl == -1) {
			//计算蹩脚索引
			p ={
				index.x,
				(idx.y + index.y)/2
			};
		}
		//倒日
		else if (wl == -2) {
			p = {
				(idx.x + index.x)/2,
				index.y
			};
		}
		return static_cast<GameScene*>(scene)->getChessByIndex(p);
	}
	return false;
}

bool ChessObj::isOverRiver(const Point& idx) const
{
	//本来在上面
	return 1;
}

int ChessObj::getChessCount(const Point& idx) const
{
	int counter = 0;
	auto wl = whatLine(idx);
	//垂直
	if (wl == 2)
	{
		int startY = min(idx.y, index.y)+1;
		int endY = max(idx.y,index.y);

		for (int i = startY; i < endY; i++) {
			if (static_cast<GameScene*>(scene)->getChessByIndex({ index.x,i })) {
				counter++;
			}
		}
	}
	//水平
	else if (wl == 3)
	{
		int startX = min(idx.x, index.x)+1;
		int endX = max(idx.x,index.x);

		for (int i = startX; i < endX; i++) {
			if (static_cast<GameScene*>(scene)->getChessByIndex({ i,index.y })) {
				counter++;
			}
		}
	}
	return counter;
}

bool ChessObj::isForward(const Point& idx) const
{
	//在上面
	if (isAbove()) {
		return idx.y > index.y;
	}
	else {
		return idx.y < index.y;
	}
	return false;
}

std::string ChessObj::toString() const
{
	std::stringstream ss;
	ss << name << " " << index.x << " " << index.y << " " << player;
	return ss.str();
}

ChessObj* ChessObj::fromString(const std::string& str,Scene* scene)
{
	std::string name;
	Point idx;
	int player;
	std::stringstream ss(str);
	if (!(ss >> name >> idx.x >> idx.y >> player)) {
		return nullptr;
	}

	return new ChessObj(ChessObj::Player(player),idx,name,scene);
}

