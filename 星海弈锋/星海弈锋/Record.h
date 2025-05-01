#pragma once

#include "ChessObj.h"

class Record
{
public:
	Record() = default;
	Record(ChessObj* chess, const Point& start, const Point& end, ChessObj* eatedChess = nullptr) 
		:chess(chess)
		,start(start)
		,end(end)
		,eatedChess(eatedChess)
	{ 
	}
public:
	/*����������*/
	ChessObj* chess{};
	/*��ʼ��λ��*/
	Point start;
	/*������λ��*/
	Point end;
	/*��chess�Ե�������*/
	ChessObj* eatedChess;
};
