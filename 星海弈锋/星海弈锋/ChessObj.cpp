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
		//�������ڡ��ߵ���ֱ�ߡ���һ��
		return isKingHome(idx) && whatLine(idx) == 1 && getStep(idx) == 1;
	}
	else if (name == "ma")
	{
		//�����ֶԽ��ߡ�û������
		return whatLine(idx) < 0 && !isLame(idx);
	}
	else if (name == "che")
	{
		//��ֱ�ߡ������ⲽ��Ŀ��������֮�䲻��������
		return whatLine(idx) > 1 && getChessCount(idx) == 0;
	}
	else if (name == "bing")
	{
		//����֮ǰֻ��ǰ��
			//return (whatLine(idx) > 1 && getStep(idx) == 1 && isForward(idx)) ||
			//	(whatLine(idx) > 1 && getStep(idx) == 1 && abs(idx.x - index.x) == 1);
		return whatLine(idx) > 1 && getStep(idx) == 1 && 
				(isForward(idx) || abs(idx.x - index.x) == 1);
	}
	return  false;
}

bool ChessObj::isKingHome(const Point& idx) const
{
	//x��Խ���ж�
	if (idx.x < 3 || idx.x >5)
		return false;
	//y��Խ���ж�
	//--�����
	if (isAbove()) {
		if (idx.y < 0 || idx.y >2)
			return false;
	}
	//--�����
	else {
		if (idx.y < 7 || idx.y >9)
			return false;
	}
	return true;
}

int ChessObj::whatLine(const Point& idx1, const Point& idx2)
{
	//��ֱ����
	if (idx1.x == idx2.x) {
		return 2;
	}
	//ˮƽ����
	if (idx1.y == idx2.y) {
		return 3;
	}
	//�Խ���
	if (abs(idx1.x - idx2.x) == abs(idx1.y - idx2.y)) {
		return 1;
	}
	//���ֶԽ���
	//��
	if (abs(idx1.x - idx2.x) == 1 && abs(idx1.y - idx2.y) == 2) {
		return -1;
	}
	//����
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
		//��ȡ���index��idx���ĵ�����
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
		//��
		if (wl == -1) {
			//������������
			p ={
				index.x,
				(idx.y + index.y)/2
			};
		}
		//����
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
	//����������
	return 1;
}

int ChessObj::getChessCount(const Point& idx) const
{
	int counter = 0;
	auto wl = whatLine(idx);
	//��ֱ
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
	//ˮƽ
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
	//������
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

