/*
 *        Computer Graphics Course - Shenzhen University
 *						Mid-term Assignment
 *
 */

#include "Angel.h"

#pragma comment(lib, "glew32.lib")

#include <cstdlib>
#include <iostream>
#include<time.h>

using namespace std;

int starttime;			// 控制方块向下移动时间
int rotation = 0;		// 控制当前窗口中的方块旋转
bool gameover = false;	// 游戏结束控制变量
int xsize = 400;		// 窗口大小（尽量不要变动窗口大小！）
int ysize = 720;

////////////////////定义游戏当前的模式---暂停或游戏中////////////////////////////////////

const int CONTINUE = 1;	//游戏中
const int PAUSE = 0;	//暂停
const int RESTART = 2;	//重新游戏
const int QUIT = 3;		//退出游戏
int curMode = 1;	//当前模式

const int INIT_SPEED = 10;	//游戏的初始速度
int score = 0;	//游戏的分数
int speed = INIT_SPEED;	//游戏的速度


// 一个二维数组表示所有可能出现的方块和方向。
vec2 allRotationsshapes[7][4][4] = {
							  
							   {{vec2(0,0),vec2(1,0),vec2(0,1),vec2(1,1)},	//O
							   {vec2(0,0),vec2(1,0),vec2(0,1),vec2(1,1)},
							   {vec2(0,0),vec2(1,0),vec2(0,1),vec2(1,1)},
							   {vec2(0,0),vec2(1,0),vec2(0,1),vec2(1,1)}},

							   {{vec2(0,0),vec2(1,0),vec2(-1,0),vec2(-2,0)},	//I
							   {vec2(0,0),vec2(0,1),vec2(0,-1),vec2(0,-2)},
							   {vec2(0,0),vec2(1,0),vec2(-1,0),vec2(-2,0)},	
							   {vec2(0,0),vec2(0,1),vec2(0,-1),vec2(0,-2)}},


							   {{vec2(0,0),vec2(1,0),vec2(0,-1),vec2(-1,-1)},	//S
							   {vec2(0,0),vec2(0,1),vec2(1,-1),vec2(1,0)},
							   {vec2(0,0),vec2(1,0),vec2(0,-1),vec2(-1,-1)},	
							   {vec2(0,0),vec2(0,1),vec2(1,-1),vec2(1,0)},},

							  {{vec2(0,0),vec2(-1,0),vec2(0,-1),vec2(1,-1)},	//Z
							   {vec2(0,0),vec2(1,0),vec2(0,-1),vec2(1,1)},
							   {vec2(0,0),vec2(-1,0),vec2(0,-1),vec2(1,-1)},	
							   {vec2(0,0),vec2(1,0),vec2(0,-1),vec2(1,1)},},

							   {{vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(-1,-1)},	//   L
							   {vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(1, -1)},   //
							   {vec2(1, 1), vec2(-1,0), vec2(0, 0), vec2(1,  0)},   //
							   {vec2(-1,1), vec2(0, 1), vec2(0, 0), vec2(0, -1)}},

							   {{vec2(0,0),vec2(1,0),vec2(-1,0),vec2(1,-1)},	//J
							   {vec2(0,0),vec2(0,1),vec2(1,1),vec2(0,-1)},
							   {vec2(0,0),vec2(1,0),vec2(-1,1),vec2(-1,0)},
							   {vec2(0,0),vec2(0,-1),vec2(0,1),vec2(-1,-1)}},

							   {{vec2(0,0),vec2(1,0),vec2(0,-1),vec2(-1,0)},	//T
							   {vec2(0,0),vec2(1,0),vec2(0,1),vec2(0,-1)},
							   {vec2(0,0),vec2(1,0),vec2(0,1),vec2(-1,0)},
							   {vec2(0,0),vec2(-1,0),vec2(0,1),vec2(0,-1)}},
};



// 绘制窗口的颜色变量
vec4 orange = vec4(1.0, 0.5, 0.0, 1.0);
vec4 white  = vec4(1.0, 1.0, 1.0, 1.0);
vec4 black  = vec4(0.0, 0.0, 0.0, 1.0);

//设置方格的种类
int type;

//设置格子的颜色种类个数
const int colorTypeNumber = 7;

//设置格子的颜色
vec4 tileColor[colorTypeNumber] = {
	vec4(1.0, 0.5, 0.0, 1.0),	//橙色
	vec3(1.0, 1.0, 0.0),  // Yellow
	vec3(0.0, 1.0, 0.0),  // Green
	vec3(0.0, 1.0, 1.0),  // Cyan
	vec3(1.0, 0.0, 1.0),  // Magenta
	vec3(1.0, 0.0, 0.0),  // Red
	vec3(0.0, 0.0, 1.0)   // Blue
};

vec2 tile[4];			// 表示当前窗口中的方块，相对与方块中心
vec2 tilepos = vec2(5, 19);	// 当前方块的位置（以棋盘格的左下角为原点的坐标系）

// 布尔数组表示棋盘格的某位置是否被方块填充，即board[x][y] = true表示(x,y)处格子被填充。
// （以棋盘格的左下角为原点的坐标系）
bool board[10][20];


// 当棋盘格某些位置被方块填充之后，记录这些位置上被填充的颜色
vec4 boardcolours[1200];	//10*20*6=1200

GLuint locxsize;
GLuint locysize;

GLuint vaoIDs[3];
GLuint vboIDs[6];

//////////////////////////////////////////////////////////////////////////
// 修改棋盘格在pos位置的颜色为colour，并且更新对应的VBO

void changecellcolour(vec2 pos, vec4 colour)	
{
	// 每个格子是个正方形，包含两个三角形，总共6个定点，并在特定的位置赋上适当的颜色
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6*(10*pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = {colour, colour, colour, colour, colour, colour};

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

	// 计算偏移量，在适当的位置赋上颜色
	int offset = 6 * sizeof(vec4) * (int)(10*pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//该函数的功能是将color的颜色信息保存再boardcolours数组中
void changeBorderColors(vec2 pos, vec4 color) {
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6 * (10 * pos.y + pos.x) + i)] = color;
}

//获取pos方格的颜色
vec4 getColor(vec2 pos) {
	return boardcolours[6 * (int)(10 * pos.y + pos.x)];
}

//将pos方块的board设置为true
void setTrail(vec2 pos) {
	board[(int)pos.x][(int)pos.y] = true;
}

//将pos方块的board设置为flag
void setTrail(vec2 pos, bool flag) {
	board[(int)pos.x][(int)pos.y] = flag;
}

//将pos2方块的board设置为pos1的board的值
void setTrail(vec2 pos, vec2 pos1) {
	board[(int)pos.x][(int)pos.y] = board[(int)pos1.x][(int)pos1.y];
}
//将pos位置的board设置为false
void clearTrail(vec2 pos) {
	board[(int)pos.x][(int)pos.y] = false;
}

void printInformation() {
	//清屏
	system("cls");
	//输出当前的分数
	cout << "***********current score***********" << endl;
	std::cout << "score: " << score << "\n";
	cout << endl << endl;
	//输出当前的下降速度
	cout << "***********current speed***********" << endl;
	std::cout << "speed: " << speed << "\n";
	cout << endl << endl;
	//输出键盘的交互操作
	cout << "***********press keyboard***********" << endl;
	cout << "r---restart" << endl;
	cout << "q---quit" << endl;
	cout << "c---continue" << endl;
	cout << "p---pause" << endl;
	cout << "w---speed up" << endl;
	cout << "s---slow down" << endl;
	cout << endl << endl;
	//输出鼠标的交互曹祖
	cout << "***********click mouse***********" << endl;
	cout << "left button---continue" << endl;
	cout << "right button---pause" << endl;
	cout << "middle button---menu" << endl;
	cout << endl << endl;
	//如果游戏结束，则输出"gameover"提示
	if (gameover) {
		std::cout << "***********gameover***********"<<endl;
		cout << "Your final score is " << score << endl;
	}
}

//////////////////////////////////////////////////////////////////////////
// 当前方块移动或者旋转时，更新VBO

void updatetile()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

	// 每个方块包含四个格子
	for (int i = 0; i < 4; i++)
	{
		// 计算格子的坐标值
		GLfloat x = tilepos.x + tile[i].x;
		GLfloat y = tilepos.y + tile[i].y;

		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		// 每个格子包含两个三角形，所以有6个顶点坐标
		vec4 newpoints[6] = {p1, p2, p3, p2, p3, p4};
		glBufferSubData(GL_ARRAY_BUFFER, i*6*sizeof(vec4), 6*sizeof(vec4), newpoints);
	}
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// 设置当前方块为下一个即将出现的方块。在游戏开始的时候调用来创建一个初始的方块，
// 在游戏结束的时候判断，没有足够的空间来生成新的方块。

void newtile()
{
	//如果游戏结束或者当前模式为PAUSE，则不再生成新的方块
	if (gameover||curMode==PAUSE) {
		return;
	}
	// 将新方块放于棋盘格的最上行中间位置
	tilepos = vec2(5 , 19);
	//如果当前生成的方块超出边界，则将方块下移一格就不会超出边界了，downOneCell的作用就是用来记录是否需要下移一格
	bool downOneCell = false;	
	//随机生成一个方向
	rotation = rand()%4;	
	//随机生成一种方块的类型
	type = rand() % 7;	
	//生成一个随机颜色
	int randColor = rand() % colorTypeNumber;	
	//用来存储新的方块的颜色，一共有4个格子，每个格子需要6个定点，所以数组的大小为4*6=24
	vec4 newcolours[24];	

	//检测当前方块是否超出边界，如果是则需要下移一格
	for (int i = 0; i < 4; i++)
	{
		//设置当前的tile
		tile[i] = allRotationsshapes[type][rotation][i];	
		//当前生成的方块坐标
		int x = tilepos.x + tile[i].x;	
		int y = tilepos.y + tile[i].y;
		//如果超出边界
		if (y > 19) {	
			downOneCell = true;
			break;
		}
	}

	//如果需要往下移动一个格子，使得生成的方块在边界内
	if (downOneCell) {
		//当前方块下移一格
		tilepos.y--;
	}
	
	for (int i = 0; i < 4; i++)
	{
		//获取当前方块的四个格子的位置
		tile[i] = allRotationsshapes[type][rotation][i];
		int x = tilepos.x + tile[i].x;
		int y = tilepos.y + tile[i].y;
		//判断是否被填充，如果刚生成的位置都被填充的话，则游戏结束
		if (board[x][y]) {
			gameover = true;
			//输出游戏结束的信息
			printInformation();
			//直接返回，不再继续后继操作
			return;
		}
		//保存每一个新生成的格子的颜色信息
		changeBorderColors(tilepos + tile[i], tileColor[randColor]);	
		//将每一个新生成的格子的位置的board设置为true
		setTrail(tilepos + tile[i]);	
	}

	for (int i = 0; i < 24; i++) {
		newcolours[i] = tileColor[randColor];
	}
	updatetile();
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


//////////////////////////////////////////////////////////////////////////
// 游戏和OpenGL初始化

void init()
{
	//初始化随机数种子 
	srand((unsigned)time(NULL)); 
	// 初始化棋盘格，包含64个顶点坐标（总共32条线），并且每个顶点一个颜色值
	vec4 gridpoints[64];
	vec4 gridcolours[64];

	// 纵向线
	for (int i = 0; i < 11; i++)
	{
		//相对于世界坐标系，这个点是位于y=33.0的位置
		gridpoints[2*i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);	
		//693=33*21，相对于世界坐标系，这个点是位于y=33.0*21的位置，一共相差20个格子
		gridpoints[2*i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);	

	}

	// 水平线
	for (int i = 0; i < 21; i++)
	{
		//相对于世界坐标系，这个点是位于x=33.0的位置
		gridpoints[22 + 2*i] = vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
		//相对于世界坐标系，这个点是位于x=11*33=363的位置
		gridpoints[22 + 2*i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
	}

	// 将所有线赋成白色
	for (int i = 0; i < 64; i++)
		gridcolours[i] = white;

	// 初始化棋盘格，并将没有被填充的格子设置成黑色
	vec4 boardpoints[1200];
	for (int i = 0; i < 1200; i++)
		boardcolours[i] = black;

	// 对每个格子，初始化6个顶点，表示两个三角形，绘制一个正方形格子
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 10; j++)
		{
			vec4 p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);
			vec4 p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);

			boardpoints[6*(10*i + j)    ] = p1;
			boardpoints[6*(10*i + j) + 1] = p2;
			boardpoints[6*(10*i + j) + 2] = p3;
			boardpoints[6*(10*i + j) + 3] = p2;
			boardpoints[6*(10*i + j) + 4] = p3;
			boardpoints[6*(10*i + j) + 5] = p4;
		}

	// 将棋盘格所有位置的填充与否都设置为false（没有被填充）
	for (int i = 0; i < 10; i++) 
		for (int j = 0; j < 20; j++)
			board[i][j] = false;

	// 载入着色器
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(3, &vaoIDs[0]);

	// 棋盘格顶点
	glBindVertexArray(vaoIDs[0]);
	glGenBuffers(2, vboIDs);

	// 棋盘格顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// 棋盘格每个格子
	glBindVertexArray(vaoIDs[1]);
	glGenBuffers(2, &vboIDs[2]);

	// 棋盘格每个格子顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格每个格子顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// 当前方块
	glBindVertexArray(vaoIDs[2]);
	glGenBuffers(2, &vboIDs[4]);

	// 当前方块顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 当前方块顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0);
	glClearColor(0, 0, 0, 0);

	// 游戏初始化
	newtile();
	starttime = glutGet(GLUT_ELAPSED_TIME);
	printInformation();
}

//////////////////////////////////////////////////////////////////////////
// 检查在cellpos位置的格子是否被填充或者是否在棋盘格的边界范围内。

bool checkvalid(vec2 cellpos)
{
	//如果单元格在棋盘内
	if ((cellpos.x >= 0) && (cellpos.x < 10) && (cellpos.y >= 0) && (cellpos.y < 20))
	{
		int x = cellpos.x;
		int y = cellpos.y;
		//如果没有被填充，则返回true
		return board[x][y] == false;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
// 在棋盘上有足够空间的情况下旋转当前方块

void rotate()
{
	// 计算得到下一个旋转方向
	int nextrotation = (rotation + 1) % 4;
	//先尝试rotate，这样再去检测有有效性，类似于回溯法
	for (int i = 0; i < 4; i++) {
		clearTrail(tile[i] + tilepos);
	}
	// 检查当前旋转之后的位置的有效性
	if (checkvalid((allRotationsshapes[type][nextrotation][0]) + tilepos)
		&& checkvalid((allRotationsshapes[type][nextrotation][1]) + tilepos)
		&& checkvalid((allRotationsshapes[type][nextrotation][2]) + tilepos)
		&& checkvalid((allRotationsshapes[type][nextrotation][3]) + tilepos))
	{
		// 更新旋转，将当前方块设置为旋转之后的方块
		vec4 lastColor[4];
		//删除当前格子
		for (int i = 0; i < 4; i++) {
			//保存格子的颜色信息
			lastColor[i] = getColor(tilepos + tile[i]);	
			//这个方块的格子全部设置成黑色
			changeBorderColors(tilepos + tile[i], black);
			//清除填充信息
			clearTrail(tile[i] + tilepos);	
		}
		//重新生成一个新的格子
		rotation = nextrotation;
		for (int i = 0; i < 4; i++) {
			//更新tile
			tile[i] = allRotationsshapes[type][rotation][i];	
			//设置旋转后的颜色
			changeBorderColors(tilepos + tile[i], lastColor[i]);
			//重新设置填充信息
			setTrail(tile[i] + tilepos);	
		}
		updatetile();	//更新VBO
	}
	else {
		//如果检测不成功，则恢复填充信息
		for (int i = 0; i < 4; i++) {
			setTrail(tile[i] + tilepos);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 检查棋盘格在row行有没有被填充满

bool checkfullrow(int row)
{
	//遍历每一个x值
	for (int j = 0; j < 10; j++) {
		if (!board[j][row]) {
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//消去第row行
void clearRow(int row) {

	//每消去一行，分数都加上speed的平方，speed变成原来的1.2倍
	score += speed*speed;
	speed *= 1.2;
	for (int i = row; i < 19; i++) {
		for (int j = 0; j < 10; j++) {
			//将上一行往下移
			setTrail(vec2(j, i), vec2(j, i + 1));	

			//注意下面两个函数是不一样的

			//将当前行的颜色变成上一行的颜色
			changeBorderColors(vec2(j, i), getColor(vec2(j, i + 1)));
			//更新当前颜色的VBO
			changecellcolour(vec2(j, i), getColor(vec2(j, i)));		
		}
	}
	printInformation();
	
}

//////////////////////////////////////////////////////////////////////////
// 放置当前方块，并且更新棋盘格对应位置顶点的颜色VBO

void settile()
{
	// 每个格子
	for (int i = 0; i < 4; i++)
	{
		// 获取格子在棋盘格上的坐标
		int x = (tile[i] + tilepos).x;
		int y = (tile[i] + tilepos).y;
		// 将格子对应在棋盘格上的位置设置为填充
		board[x][y] = true;
		// 并将相应位置的颜色修改
		//更新VBO
		changecellcolour(vec2(x,y), boardcolours[6*(10*y+x)]);	
	}

	//从y=0开始扫描（从下往上），判断是否需要消去行。
	for (int row = 0; row < 19; row++) {
		if (checkfullrow(row)) {
			clearRow(row);
			//如果消去了一行，则上面的行掉了下来，所以此时row不必加加
			row--;	
		}
	}


}


//////////////////////////////////////////////////////////////////////////
// 给定位置(x,y)，移动方块。有效的移动值为(-1,0)，(1,0)，(0,-1)，分别对应于向
// 左，向下和向右移动。如果移动成功，返回值为true，反之为false。

bool movetile(vec2 direction)
{
	// 计算移动之后的方块的位置坐标
	vec2 newtilepos[4];
	//先尝试移动,去除没有移动前的board的信息（设置为false）
	for (int i = 0; i < 4; i++) {
		newtilepos[i] = tile[i] + tilepos + direction;
		clearTrail(tile[i] + tilepos);
	}

	// 检查移动之后的有效性
	if (checkvalid(newtilepos[0])
		&& checkvalid(newtilepos[1])
		&& checkvalid(newtilepos[2])
		&& checkvalid(newtilepos[3]))
		{
			vec4 lastColor[6];	
			//保存移动前的颜色
			for (int i = 0; i < 6; i++) {
				int x = (tile[i] + tilepos).x;
				int y = (tile[i] + tilepos).y;
				lastColor[i] = boardcolours[6 * (10 * y + x)];	
				
			}
			//注意下面两个循环顺序不可换或者合并成一个循环
			//因为在这个过程中可能原来的格子和重新生成的格子有重叠的部分，
			//如果同时清除原来的格子和生成移动后的格子的话，可能会造成新生成的格子丢失

			//删除原来的格子
			for (int i = 0; i < 4; i++) {
				//将当前位置设置成黑色
				changeBorderColors(tile[i]+tilepos, black);
			}
			//重新生成新的格子
			for (int i = 0; i < 4; i++) {
				setTrail(newtilepos[i]);
				changeBorderColors(newtilepos[i], lastColor[0]);
			}
			
			//重新更新tilepos的位置，不涉及到旋转，所以不必更新tile
			tilepos.x = tilepos.x + direction.x;
			tilepos.y = tilepos.y + direction.y;

			//更新VBO
			updatetile();
			return true;
	}
	else {
		//如果不能够移动，则恢复方块的填充信息
		for (int i = 0; i < 4; i++) {
			setTrail(tile[i] + tilepos);
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// 重新启动游戏

void restart()
{
	score = 0;
	speed = INIT_SPEED;
	gameover = false;
	curMode = CONTINUE;
	init();
}

//////////////////////////////////////////////////////////////////////////
// 游戏渲染部分

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1i(locxsize, xsize);
	glUniform1i(locysize, ysize);

	glBindVertexArray(vaoIDs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 1200); // 绘制棋盘格 (10*20*2 = 400 个三角形)

	glBindVertexArray(vaoIDs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 24);	 // 绘制当前方块 (8 个三角形)

	glBindVertexArray(vaoIDs[0]);
	glDrawArrays(GL_LINES, 0, 64);		 // 绘制棋盘格的线


	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// 在窗口被拉伸的时候，控制棋盘格的大小，使之保持固定的比例。

void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

//////////////////////////////////////////////////////////////////////////
// 键盘响应事件中的特殊按键响应

void special(int key, int x, int y)
{
	if(!gameover&&curMode==CONTINUE)
	{
		switch(key)
		{
			// 向上按键旋转方块
			case GLUT_KEY_UP:	
				rotate();
				break;
			// 向下按键移动方块
			case GLUT_KEY_DOWN: 
			{
				//判断是否可以向下移动
				if (!movetile(vec2(0, -1)))
				{
					//如果能向下移动，则放置当前的方块，重新生成方块
					settile();
					newtile();
				}
				break;
			}
			// 向左按键移动方块
			case GLUT_KEY_LEFT: 
				movetile(vec2(-1, 0));
				break;
			// 向右按键移动方块
			case GLUT_KEY_RIGHT: 
				movetile(vec2(1, 0));
				break;
		}
	}

}

//将方块向下移动一格
void moveDown(int value) {
	//如果当前模式为游戏中模式
	if (curMode == CONTINUE) {
		//判断是否可以向下移动
		if (!movetile(vec2(0, -1)))
		{
			//如果能向下移动，则放置当前的方块，重新生成方块
			settile();
			newtile();
		}
	}
	//定时器，定时向下移动，1是向moveDown传递的参数，无实际意义，但是glutTimerFunc的回调函数必须需要一格int的参数
	glutTimerFunc(10000 / speed, moveDown, 1);
}



//////////////////////////////////////////////////////////////////////////
// 键盘响应时间中的普通按键响应

void keyboard(unsigned char key, int x, int y)
{
	
	switch(key)
	{
		// ESC键 和 'q' 键退出游戏
		case 033: 
			exit(EXIT_SUCCESS);
			break;
		case 'q':
			exit (EXIT_SUCCESS);
			break;
		// 'r' 键重启游戏
		case 'r': 
			restart();
			break;
		//'p'键暂停游戏
		case 'p':
			curMode = PAUSE;
			break;
		//'c'键继续游戏
		case 'c':
			curMode = CONTINUE;
			break;
		//'s'键减速方块下落的速度
		case 's':
			if(speed/2>0)
				speed /= 2;
			//重新输出速度的信息
			printInformation();
			break;
		//'w'键加速方块下落的速度
		case 'w':
			speed *= 2;
			//重新输出速度的信息
			printInformation();
			break;
		//重绘
		glutPostRedisplay();
	}
	
}

//鼠标响应： 继续游戏和暂停游戏
void mouse(int button, int state, int x, int y) {
	//如果点击左键，则继续游戏
	if (button == GLUT_LEFT_BUTTON) {
		curMode = CONTINUE;
	}
	if (button== GLUT_RIGHT_BUTTON) {
		curMode = PAUSE;
	}

}

void menuEvents(int menuChoice)
{
	switch (menuChoice)
	{
	case CONTINUE:
		//设置当前模式为CONTINUE,即游戏中
		curMode = CONTINUE;
		break;
	case PAUSE:
		//设置当前模式为PAUSE，即暂停游戏
		curMode = PAUSE;
		break;
	case RESTART:
		//重新开始游戏
		restart();
		break;
	case QUIT:
		//强制按下'q'键，以实现强制退出
		keyboard('q', 0, 0);
		break;
	}
}
void setupMenu()
{
	//创建菜单回调函数
	glutCreateMenu(menuEvents);
	//为菜单添加选择模式的选项
	glutAddMenuEntry("Continue", CONTINUE);
	glutAddMenuEntry("Pause", PAUSE);
	glutAddMenuEntry("Restart", RESTART);
	glutAddMenuEntry("Quit", QUIT);
	//将菜单和中键关联
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}
//////////////////////////////////////////////////////////////////////////


void idle(void)
{
	//如果游戏没有结束，则重绘图形
	if (!gameover) {
		glutPostRedisplay();
	}
}

//////////////////////////////////////////////////////////////////////////


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(680, 178);
	glutCreateWindow("2017152021_刘清影_期中大作业");
	glewInit();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	setupMenu();
	glutIdleFunc(idle);
	moveDown(1);
	glutMainLoop();
	return 0;
}
