#include "Geometry.h"
#include <stdio.h>
Point::Point()
	:Point(0,0)
{
	
}

Point::Point(int x, int y)
	:x(x),y(y)
{
}

bool Point::isNull() const
{
	return x == 0 && y == 0;
}
