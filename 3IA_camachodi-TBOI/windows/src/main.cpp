#include "tboi.h"

TBOI tboi;

void init()
{

	tboi.initData();
	
}

void calculate()
{
	tboi.calculateData();
}

void draw()
{
	tboi.drawData();
}

int main()
{

	init();
	calculate();
	draw();

	return 0;
}