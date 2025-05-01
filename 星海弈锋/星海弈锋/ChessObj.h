#pragma once
#include "GameObject.h"
#include "Geometry.h"
#include <string>

class ChessObj : public GameObject
{
public:
	//������Ӫ
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
	 * ��ת����.
	 */
	void reverse();
	/**
	 * ����ת�±�.
	 */
	static Point pointToIndex(const Point& pos)
	{
		return Point{
			(pos.x - MARGIN + SIZE / 2) / SIZE,
			(pos.y - MARGIN + SIZE / 2) / SIZE,
		};
	}
	/**
	 * �±�ת����.
	 */
	static Point indexToPoint(const Point& idx)
	{
		return Point{
			idx.x * SIZE + MARGIN - SIZE / 2,	//x
			idx.y * SIZE + MARGIN - SIZE / 2	//y
		};
	}

	/**
	 * �����ܷ��ƶ���ָ����������.
	 */
	bool isAbleMove(const Point& idx)const;

	/**
	 * �ж��ǲ�������.
	 */
	bool isAbove()const { return initIndex.y <= 4; }

	/**
	 * �ж�idx�Ƿ���������.
	 */
	bool isKingHome(const Point& idx)const;

	/*
	* �жϵ�ǰ������������ʲô��
	* ��ֱ��:2
	* ˮƽ��:3
	* �Խ���:1
	* ���ֶԽ�: ��:-1 ����:-2
	*/
	int whatLine(const Point& idx)const { return whatLine(index, idx); }
	static int whatLine(const Point& idx1,const Point& idx2);

	/**
	 * ����index��idx֮��Ĳ���.
	 */
	int getStep(const Point& idx)const;

	/**
	 * �ж��Ƿ�����(��/�� ��/�R).
	 */
	bool isLame(const Point& idx)const;

	/**
	 * �ж�Ŀ����Ƿ������.
	 */
	bool isOverRiver(const Point& idx)const;

	/*
	* ��ȡindex��idx֮�����������(������index��idx)
	*/
	int getChessCount(const Point& idx)const;

	/**
	 * �ж��ǲ���ǰ��.
	 */
	bool isForward(const Point& idx)const;

	/**
	 * ����������ת���ַ���.
	 */
	std::string toString()const;

	/**
	 * ���ַ�������һ������.
	 */
	static ChessObj* fromString(const std::string& str,Scene* scene);
public:
	std::string name;
	Point initIndex;		//���ӵĳ�ʼλ��(���ڳ�ʼ�����߷�תʱ�ı�)
	Point index;			//���ӵĵ�ǰλ��
	Player player{None};

	inline static const int MARGIN = 50;	//�߾�
	inline static const int SIZE = 80;		//���Ӵ�С
};

