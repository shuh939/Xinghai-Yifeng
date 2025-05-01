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
	/*操作的棋子*/
	ChessObj* chess{};
	/*开始的位置*/
	Point start;
	/*结束的位置*/
	Point end;
	/*被chess吃掉的棋子*/
	ChessObj* eatedChess;
};
