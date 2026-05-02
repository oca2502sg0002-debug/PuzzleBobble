#ifndef __CONST_H__
#define __CONST_H__
	
const int WINDOW_WIDTH = 574;
const int WINDOW_HEIGHT = 824;

class Float2
{
public:
	float x;
	float y;
};

class Ball
{
public:
	Float2 pos;
	bool mode;
	float vx;
	float vy;

	void Init();
};
Ball ball;

class Player
{
public:
	Float2 pos;
	float rot;
};
Player player;


#endif