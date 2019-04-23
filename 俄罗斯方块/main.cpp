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

int starttime;			// ���Ʒ��������ƶ�ʱ��
int rotation = 0;		// ���Ƶ�ǰ�����еķ�����ת
bool gameover = false;	// ��Ϸ�������Ʊ���
int xsize = 400;		// ���ڴ�С��������Ҫ�䶯���ڴ�С����
int ysize = 720;

////////////////////������Ϸ��ǰ��ģʽ---��ͣ����Ϸ��////////////////////////////////////

const int CONTINUE = 1;	//��Ϸ��
const int PAUSE = 0;	//��ͣ
const int RESTART = 2;	//������Ϸ
const int QUIT = 3;		//�˳���Ϸ
int curMode = 1;	//��ǰģʽ

const int INIT_SPEED = 10;	//��Ϸ�ĳ�ʼ�ٶ�
int score = 0;	//��Ϸ�ķ���
int speed = INIT_SPEED;	//��Ϸ���ٶ�


// һ����ά�����ʾ���п��ܳ��ֵķ���ͷ���
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



// ���ƴ��ڵ���ɫ����
vec4 orange = vec4(1.0, 0.5, 0.0, 1.0);
vec4 white  = vec4(1.0, 1.0, 1.0, 1.0);
vec4 black  = vec4(0.0, 0.0, 0.0, 1.0);

//���÷��������
int type;

//���ø��ӵ���ɫ�������
const int colorTypeNumber = 7;

//���ø��ӵ���ɫ
vec4 tileColor[colorTypeNumber] = {
	vec4(1.0, 0.5, 0.0, 1.0),	//��ɫ
	vec3(1.0, 1.0, 0.0),  // Yellow
	vec3(0.0, 1.0, 0.0),  // Green
	vec3(0.0, 1.0, 1.0),  // Cyan
	vec3(1.0, 0.0, 1.0),  // Magenta
	vec3(1.0, 0.0, 0.0),  // Red
	vec3(0.0, 0.0, 1.0)   // Blue
};

vec2 tile[4];			// ��ʾ��ǰ�����еķ��飬����뷽������
vec2 tilepos = vec2(5, 19);	// ��ǰ�����λ�ã������̸�����½�Ϊԭ�������ϵ��

// ���������ʾ���̸��ĳλ���Ƿ񱻷�����䣬��board[x][y] = true��ʾ(x,y)�����ӱ���䡣
// �������̸�����½�Ϊԭ�������ϵ��
bool board[10][20];


// �����̸�ĳЩλ�ñ��������֮�󣬼�¼��Щλ���ϱ�������ɫ
vec4 boardcolours[1200];	//10*20*6=1200

GLuint locxsize;
GLuint locysize;

GLuint vaoIDs[3];
GLuint vboIDs[6];

//////////////////////////////////////////////////////////////////////////
// �޸����̸���posλ�õ���ɫΪcolour�����Ҹ��¶�Ӧ��VBO

void changecellcolour(vec2 pos, vec4 colour)	
{
	// ÿ�������Ǹ������Σ��������������Σ��ܹ�6�����㣬�����ض���λ�ø����ʵ�����ɫ
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6*(10*pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = {colour, colour, colour, colour, colour, colour};

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

	// ����ƫ���������ʵ���λ�ø�����ɫ
	int offset = 6 * sizeof(vec4) * (int)(10*pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//�ú����Ĺ����ǽ�color����ɫ��Ϣ������boardcolours������
void changeBorderColors(vec2 pos, vec4 color) {
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6 * (10 * pos.y + pos.x) + i)] = color;
}

//��ȡpos�������ɫ
vec4 getColor(vec2 pos) {
	return boardcolours[6 * (int)(10 * pos.y + pos.x)];
}

//��pos�����board����Ϊtrue
void setTrail(vec2 pos) {
	board[(int)pos.x][(int)pos.y] = true;
}

//��pos�����board����Ϊflag
void setTrail(vec2 pos, bool flag) {
	board[(int)pos.x][(int)pos.y] = flag;
}

//��pos2�����board����Ϊpos1��board��ֵ
void setTrail(vec2 pos, vec2 pos1) {
	board[(int)pos.x][(int)pos.y] = board[(int)pos1.x][(int)pos1.y];
}
//��posλ�õ�board����Ϊfalse
void clearTrail(vec2 pos) {
	board[(int)pos.x][(int)pos.y] = false;
}

void printInformation() {
	//����
	system("cls");
	//�����ǰ�ķ���
	cout << "***********current score***********" << endl;
	std::cout << "score: " << score << "\n";
	cout << endl << endl;
	//�����ǰ���½��ٶ�
	cout << "***********current speed***********" << endl;
	std::cout << "speed: " << speed << "\n";
	cout << endl << endl;
	//������̵Ľ�������
	cout << "***********press keyboard***********" << endl;
	cout << "r---restart" << endl;
	cout << "q---quit" << endl;
	cout << "c---continue" << endl;
	cout << "p---pause" << endl;
	cout << "w---speed up" << endl;
	cout << "s---slow down" << endl;
	cout << endl << endl;
	//������Ľ�������
	cout << "***********click mouse***********" << endl;
	cout << "left button---continue" << endl;
	cout << "right button---pause" << endl;
	cout << "middle button---menu" << endl;
	cout << endl << endl;
	//�����Ϸ�����������"gameover"��ʾ
	if (gameover) {
		std::cout << "***********gameover***********"<<endl;
		cout << "Your final score is " << score << endl;
	}
}

//////////////////////////////////////////////////////////////////////////
// ��ǰ�����ƶ�������תʱ������VBO

void updatetile()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

	// ÿ����������ĸ�����
	for (int i = 0; i < 4; i++)
	{
		// ������ӵ�����ֵ
		GLfloat x = tilepos.x + tile[i].x;
		GLfloat y = tilepos.y + tile[i].y;

		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		// ÿ�����Ӱ������������Σ�������6����������
		vec4 newpoints[6] = {p1, p2, p3, p2, p3, p4};
		glBufferSubData(GL_ARRAY_BUFFER, i*6*sizeof(vec4), 6*sizeof(vec4), newpoints);
	}
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// ���õ�ǰ����Ϊ��һ���������ֵķ��顣����Ϸ��ʼ��ʱ�����������һ����ʼ�ķ��飬
// ����Ϸ������ʱ���жϣ�û���㹻�Ŀռ��������µķ��顣

void newtile()
{
	//�����Ϸ�������ߵ�ǰģʽΪPAUSE�����������µķ���
	if (gameover||curMode==PAUSE) {
		return;
	}
	// ���·���������̸���������м�λ��
	tilepos = vec2(5 , 19);
	//�����ǰ���ɵķ��鳬���߽磬�򽫷�������һ��Ͳ��ᳬ���߽��ˣ�downOneCell�����þ���������¼�Ƿ���Ҫ����һ��
	bool downOneCell = false;	
	//�������һ������
	rotation = rand()%4;	
	//�������һ�ַ��������
	type = rand() % 7;	
	//����һ�������ɫ
	int randColor = rand() % colorTypeNumber;	
	//�����洢�µķ������ɫ��һ����4�����ӣ�ÿ��������Ҫ6�����㣬��������Ĵ�СΪ4*6=24
	vec4 newcolours[24];	

	//��⵱ǰ�����Ƿ񳬳��߽磬���������Ҫ����һ��
	for (int i = 0; i < 4; i++)
	{
		//���õ�ǰ��tile
		tile[i] = allRotationsshapes[type][rotation][i];	
		//��ǰ���ɵķ�������
		int x = tilepos.x + tile[i].x;	
		int y = tilepos.y + tile[i].y;
		//��������߽�
		if (y > 19) {	
			downOneCell = true;
			break;
		}
	}

	//�����Ҫ�����ƶ�һ�����ӣ�ʹ�����ɵķ����ڱ߽���
	if (downOneCell) {
		//��ǰ��������һ��
		tilepos.y--;
	}
	
	for (int i = 0; i < 4; i++)
	{
		//��ȡ��ǰ������ĸ����ӵ�λ��
		tile[i] = allRotationsshapes[type][rotation][i];
		int x = tilepos.x + tile[i].x;
		int y = tilepos.y + tile[i].y;
		//�ж��Ƿ���䣬��������ɵ�λ�ö������Ļ�������Ϸ����
		if (board[x][y]) {
			gameover = true;
			//�����Ϸ��������Ϣ
			printInformation();
			//ֱ�ӷ��أ����ټ�����̲���
			return;
		}
		//����ÿһ�������ɵĸ��ӵ���ɫ��Ϣ
		changeBorderColors(tilepos + tile[i], tileColor[randColor]);	
		//��ÿһ�������ɵĸ��ӵ�λ�õ�board����Ϊtrue
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
// ��Ϸ��OpenGL��ʼ��

void init()
{
	//��ʼ����������� 
	srand((unsigned)time(NULL)); 
	// ��ʼ�����̸񣬰���64���������꣨�ܹ�32���ߣ�������ÿ������һ����ɫֵ
	vec4 gridpoints[64];
	vec4 gridcolours[64];

	// ������
	for (int i = 0; i < 11; i++)
	{
		//�������������ϵ���������λ��y=33.0��λ��
		gridpoints[2*i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);	
		//693=33*21���������������ϵ���������λ��y=33.0*21��λ�ã�һ�����20������
		gridpoints[2*i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);	

	}

	// ˮƽ��
	for (int i = 0; i < 21; i++)
	{
		//�������������ϵ���������λ��x=33.0��λ��
		gridpoints[22 + 2*i] = vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
		//�������������ϵ���������λ��x=11*33=363��λ��
		gridpoints[22 + 2*i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
	}

	// �������߸��ɰ�ɫ
	for (int i = 0; i < 64; i++)
		gridcolours[i] = white;

	// ��ʼ�����̸񣬲���û�б����ĸ������óɺ�ɫ
	vec4 boardpoints[1200];
	for (int i = 0; i < 1200; i++)
		boardcolours[i] = black;

	// ��ÿ�����ӣ���ʼ��6�����㣬��ʾ���������Σ�����һ�������θ���
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

	// �����̸�����λ�õ�����������Ϊfalse��û�б���䣩
	for (int i = 0; i < 10; i++) 
		for (int j = 0; j < 20; j++)
			board[i][j] = false;

	// ������ɫ��
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(3, &vaoIDs[0]);

	// ���̸񶥵�
	glBindVertexArray(vaoIDs[0]);
	glGenBuffers(2, vboIDs);

	// ���̸񶥵�λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸񶥵���ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// ���̸�ÿ������
	glBindVertexArray(vaoIDs[1]);
	glGenBuffers(2, &vboIDs[2]);

	// ���̸�ÿ�����Ӷ���λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸�ÿ�����Ӷ�����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// ��ǰ����
	glBindVertexArray(vaoIDs[2]);
	glGenBuffers(2, &vboIDs[4]);

	// ��ǰ���鶥��λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ��ǰ���鶥����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0);
	glClearColor(0, 0, 0, 0);

	// ��Ϸ��ʼ��
	newtile();
	starttime = glutGet(GLUT_ELAPSED_TIME);
	printInformation();
}

//////////////////////////////////////////////////////////////////////////
// �����cellposλ�õĸ����Ƿ��������Ƿ������̸�ı߽緶Χ�ڡ�

bool checkvalid(vec2 cellpos)
{
	//�����Ԫ����������
	if ((cellpos.x >= 0) && (cellpos.x < 10) && (cellpos.y >= 0) && (cellpos.y < 20))
	{
		int x = cellpos.x;
		int y = cellpos.y;
		//���û�б���䣬�򷵻�true
		return board[x][y] == false;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
// �����������㹻�ռ���������ת��ǰ����

void rotate()
{
	// ����õ���һ����ת����
	int nextrotation = (rotation + 1) % 4;
	//�ȳ���rotate��������ȥ�������Ч�ԣ������ڻ��ݷ�
	for (int i = 0; i < 4; i++) {
		clearTrail(tile[i] + tilepos);
	}
	// ��鵱ǰ��ת֮���λ�õ���Ч��
	if (checkvalid((allRotationsshapes[type][nextrotation][0]) + tilepos)
		&& checkvalid((allRotationsshapes[type][nextrotation][1]) + tilepos)
		&& checkvalid((allRotationsshapes[type][nextrotation][2]) + tilepos)
		&& checkvalid((allRotationsshapes[type][nextrotation][3]) + tilepos))
	{
		// ������ת������ǰ��������Ϊ��ת֮��ķ���
		vec4 lastColor[4];
		//ɾ����ǰ����
		for (int i = 0; i < 4; i++) {
			//������ӵ���ɫ��Ϣ
			lastColor[i] = getColor(tilepos + tile[i]);	
			//�������ĸ���ȫ�����óɺ�ɫ
			changeBorderColors(tilepos + tile[i], black);
			//��������Ϣ
			clearTrail(tile[i] + tilepos);	
		}
		//��������һ���µĸ���
		rotation = nextrotation;
		for (int i = 0; i < 4; i++) {
			//����tile
			tile[i] = allRotationsshapes[type][rotation][i];	
			//������ת�����ɫ
			changeBorderColors(tilepos + tile[i], lastColor[i]);
			//�������������Ϣ
			setTrail(tile[i] + tilepos);	
		}
		updatetile();	//����VBO
	}
	else {
		//�����ⲻ�ɹ�����ָ������Ϣ
		for (int i = 0; i < 4; i++) {
			setTrail(tile[i] + tilepos);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// ������̸���row����û�б������

bool checkfullrow(int row)
{
	//����ÿһ��xֵ
	for (int j = 0; j < 10; j++) {
		if (!board[j][row]) {
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//��ȥ��row��
void clearRow(int row) {

	//ÿ��ȥһ�У�����������speed��ƽ����speed���ԭ����1.2��
	score += speed*speed;
	speed *= 1.2;
	for (int i = row; i < 19; i++) {
		for (int j = 0; j < 10; j++) {
			//����һ��������
			setTrail(vec2(j, i), vec2(j, i + 1));	

			//ע���������������ǲ�һ����

			//����ǰ�е���ɫ�����һ�е���ɫ
			changeBorderColors(vec2(j, i), getColor(vec2(j, i + 1)));
			//���µ�ǰ��ɫ��VBO
			changecellcolour(vec2(j, i), getColor(vec2(j, i)));		
		}
	}
	printInformation();
	
}

//////////////////////////////////////////////////////////////////////////
// ���õ�ǰ���飬���Ҹ������̸��Ӧλ�ö������ɫVBO

void settile()
{
	// ÿ������
	for (int i = 0; i < 4; i++)
	{
		// ��ȡ���������̸��ϵ�����
		int x = (tile[i] + tilepos).x;
		int y = (tile[i] + tilepos).y;
		// �����Ӷ�Ӧ�����̸��ϵ�λ������Ϊ���
		board[x][y] = true;
		// ������Ӧλ�õ���ɫ�޸�
		//����VBO
		changecellcolour(vec2(x,y), boardcolours[6*(10*y+x)]);	
	}

	//��y=0��ʼɨ�裨�������ϣ����ж��Ƿ���Ҫ��ȥ�С�
	for (int row = 0; row < 19; row++) {
		if (checkfullrow(row)) {
			clearRow(row);
			//�����ȥ��һ�У���������е������������Դ�ʱrow���ؼӼ�
			row--;	
		}
	}


}


//////////////////////////////////////////////////////////////////////////
// ����λ��(x,y)���ƶ����顣��Ч���ƶ�ֵΪ(-1,0)��(1,0)��(0,-1)���ֱ��Ӧ����
// �����º������ƶ�������ƶ��ɹ�������ֵΪtrue����֮Ϊfalse��

bool movetile(vec2 direction)
{
	// �����ƶ�֮��ķ����λ������
	vec2 newtilepos[4];
	//�ȳ����ƶ�,ȥ��û���ƶ�ǰ��board����Ϣ������Ϊfalse��
	for (int i = 0; i < 4; i++) {
		newtilepos[i] = tile[i] + tilepos + direction;
		clearTrail(tile[i] + tilepos);
	}

	// ����ƶ�֮�����Ч��
	if (checkvalid(newtilepos[0])
		&& checkvalid(newtilepos[1])
		&& checkvalid(newtilepos[2])
		&& checkvalid(newtilepos[3]))
		{
			vec4 lastColor[6];	
			//�����ƶ�ǰ����ɫ
			for (int i = 0; i < 6; i++) {
				int x = (tile[i] + tilepos).x;
				int y = (tile[i] + tilepos).y;
				lastColor[i] = boardcolours[6 * (10 * y + x)];	
				
			}
			//ע����������ѭ��˳�򲻿ɻ����ߺϲ���һ��ѭ��
			//��Ϊ����������п���ԭ���ĸ��Ӻ��������ɵĸ������ص��Ĳ��֣�
			//���ͬʱ���ԭ���ĸ��Ӻ������ƶ���ĸ��ӵĻ������ܻ���������ɵĸ��Ӷ�ʧ

			//ɾ��ԭ���ĸ���
			for (int i = 0; i < 4; i++) {
				//����ǰλ�����óɺ�ɫ
				changeBorderColors(tile[i]+tilepos, black);
			}
			//���������µĸ���
			for (int i = 0; i < 4; i++) {
				setTrail(newtilepos[i]);
				changeBorderColors(newtilepos[i], lastColor[0]);
			}
			
			//���¸���tilepos��λ�ã����漰����ת�����Բ��ظ���tile
			tilepos.x = tilepos.x + direction.x;
			tilepos.y = tilepos.y + direction.y;

			//����VBO
			updatetile();
			return true;
	}
	else {
		//������ܹ��ƶ�����ָ�����������Ϣ
		for (int i = 0; i < 4; i++) {
			setTrail(tile[i] + tilepos);
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ����������Ϸ

void restart()
{
	score = 0;
	speed = INIT_SPEED;
	gameover = false;
	curMode = CONTINUE;
	init();
}

//////////////////////////////////////////////////////////////////////////
// ��Ϸ��Ⱦ����

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1i(locxsize, xsize);
	glUniform1i(locysize, ysize);

	glBindVertexArray(vaoIDs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 1200); // �������̸� (10*20*2 = 400 ��������)

	glBindVertexArray(vaoIDs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 24);	 // ���Ƶ�ǰ���� (8 ��������)

	glBindVertexArray(vaoIDs[0]);
	glDrawArrays(GL_LINES, 0, 64);		 // �������̸����


	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// �ڴ��ڱ������ʱ�򣬿������̸�Ĵ�С��ʹ֮���̶ֹ��ı�����

void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

//////////////////////////////////////////////////////////////////////////
// ������Ӧ�¼��е����ⰴ����Ӧ

void special(int key, int x, int y)
{
	if(!gameover&&curMode==CONTINUE)
	{
		switch(key)
		{
			// ���ϰ�����ת����
			case GLUT_KEY_UP:	
				rotate();
				break;
			// ���°����ƶ�����
			case GLUT_KEY_DOWN: 
			{
				//�ж��Ƿ���������ƶ�
				if (!movetile(vec2(0, -1)))
				{
					//����������ƶ�������õ�ǰ�ķ��飬�������ɷ���
					settile();
					newtile();
				}
				break;
			}
			// ���󰴼��ƶ�����
			case GLUT_KEY_LEFT: 
				movetile(vec2(-1, 0));
				break;
			// ���Ұ����ƶ�����
			case GLUT_KEY_RIGHT: 
				movetile(vec2(1, 0));
				break;
		}
	}

}

//�����������ƶ�һ��
void moveDown(int value) {
	//�����ǰģʽΪ��Ϸ��ģʽ
	if (curMode == CONTINUE) {
		//�ж��Ƿ���������ƶ�
		if (!movetile(vec2(0, -1)))
		{
			//����������ƶ�������õ�ǰ�ķ��飬�������ɷ���
			settile();
			newtile();
		}
	}
	//��ʱ������ʱ�����ƶ���1����moveDown���ݵĲ�������ʵ�����壬����glutTimerFunc�Ļص�����������Ҫһ��int�Ĳ���
	glutTimerFunc(10000 / speed, moveDown, 1);
}



//////////////////////////////////////////////////////////////////////////
// ������Ӧʱ���е���ͨ������Ӧ

void keyboard(unsigned char key, int x, int y)
{
	
	switch(key)
	{
		// ESC�� �� 'q' ���˳���Ϸ
		case 033: 
			exit(EXIT_SUCCESS);
			break;
		case 'q':
			exit (EXIT_SUCCESS);
			break;
		// 'r' ��������Ϸ
		case 'r': 
			restart();
			break;
		//'p'����ͣ��Ϸ
		case 'p':
			curMode = PAUSE;
			break;
		//'c'��������Ϸ
		case 'c':
			curMode = CONTINUE;
			break;
		//'s'�����ٷ���������ٶ�
		case 's':
			if(speed/2>0)
				speed /= 2;
			//��������ٶȵ���Ϣ
			printInformation();
			break;
		//'w'�����ٷ���������ٶ�
		case 'w':
			speed *= 2;
			//��������ٶȵ���Ϣ
			printInformation();
			break;
		//�ػ�
		glutPostRedisplay();
	}
	
}

//�����Ӧ�� ������Ϸ����ͣ��Ϸ
void mouse(int button, int state, int x, int y) {
	//������������������Ϸ
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
		//���õ�ǰģʽΪCONTINUE,����Ϸ��
		curMode = CONTINUE;
		break;
	case PAUSE:
		//���õ�ǰģʽΪPAUSE������ͣ��Ϸ
		curMode = PAUSE;
		break;
	case RESTART:
		//���¿�ʼ��Ϸ
		restart();
		break;
	case QUIT:
		//ǿ�ư���'q'������ʵ��ǿ���˳�
		keyboard('q', 0, 0);
		break;
	}
}
void setupMenu()
{
	//�����˵��ص�����
	glutCreateMenu(menuEvents);
	//Ϊ�˵����ѡ��ģʽ��ѡ��
	glutAddMenuEntry("Continue", CONTINUE);
	glutAddMenuEntry("Pause", PAUSE);
	glutAddMenuEntry("Restart", RESTART);
	glutAddMenuEntry("Quit", QUIT);
	//���˵����м�����
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}
//////////////////////////////////////////////////////////////////////////


void idle(void)
{
	//�����Ϸû�н��������ػ�ͼ��
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
	glutCreateWindow("2017152021_����Ӱ_���д���ҵ");
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
