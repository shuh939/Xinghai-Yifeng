#pragma once
#include "GameObject.h"
#include "Geometry.h"
#include <string>

class ChessObj : public GameObject
{
public:
	//棋子阵营
	enum Player
	{
		None = -1,
		Red,
		Black
	};
public:
	ChessObj(Player player,const Point& index,const std::string& name,Scene* scene);
	void draw()override;
	void update()override;

	/**
	 * 反转棋子.
	 */
	void reverse();
	/**
	 * 坐标转下标.
	 */
	static Point pointToIndex(const Point& pos)
	{
		return Point{
			(pos.x - MARGIN + SIZE / 2) / SIZE,
			(pos.y - MARGIN + SIZE / 2) / SIZE,
		};
	}
	/**
	 * 下标转坐标.
	 */
	static Point indexToPoint(const Point& idx)
	{
		return Point{
			idx.x * SIZE + MARGIN - SIZE / 2,	//x
			idx.y * SIZE + MARGIN - SIZE / 2	//y
		};
	}

	/**
	 * 棋子能否移动到指定的索引处.
	 */
	bool isAbleMove(const Point& idx)const;

	/**
	 * 判断是不是上面.
	 */
	bool isAbove()const { return initIndex.y <= 4; }

	/**
	 * 判断idx是否在王宫内.
	 */
	bool isKingHome(const Point& idx)const;

	/*
	* 判断当前两个索引构成什么线
	* 垂直线:2
	* 水平线:3
	* 对角线:1
	* 日字对角: 日:-1 倒日:-2
	*/
	int whatLine(const Point& idx)const { return whatLine(index, idx); }
	static int whatLine(const Point& idx1,const Point& idx2);

	/**
	 * 计算index到idx之间的步数.
	 */
	int getStep(const Point& idx)const;

	/**
	 * 判断是否被蹩脚(象/相 马/馬).
	 */
	bool isLame(const Point& idx)const;

	/**
	 * 判断目标点是否过河了.
	 */
	bool isOverRiver(const Point& idx)const;

	/*
	* 获取index倒idx之间的棋子数量(不包括index和idx)
	*/
	int getChessCount(const Point& idx)const;

	/**
	 * 判断是不是前进.
	 */
	bool isForward(const Point& idx)const;

	/**
	 * 把棋子数据转成字符串.
	 */
	std::string toString()const;

	/**
	 * 从字符串创建一个棋子.
	 */
	static ChessObj* fromString(const std::string& str,Scene* scene);
public:
	std::string name;
	Point initIndex;		//棋子的初始位置(仅在初始化或者反转时改变)
	Point index;			//棋子的当前位置
	Player player{None};

	inline static const int MARGIN = 50;	//边距
	inline static const int SIZE = 80;		//格子大小
};

