#pragma once

/**
* 坐标点类
*/
class Point
{
public:
	Point();
	Point(int x, int y);
	bool isNull()const;
	bool operator==(const Point& other) { return x == other.x && y == other.y; }
public:
	int x;
	int y;
};

class Size
{
public:
	int w;
	int h;
};

class Rect
{
public:
	int x;
	int y;
	int w;
	int h;
};

