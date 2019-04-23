#ifndef GRID_H
#define GRID_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>
using namespace std;

class Grid {
	// 水平线格子的数量
	const int HORIZONTAL_NUMBER = 8;
	// 竖直线格子的数量
	const int VERTICAL_NUMBER = 4;

	// 所有的点的数量
	const int POINT_NUMBER = 2*(HORIZONTAL_NUMBER+VERTICAL_NUMBER) + 4;
	
	const float GRID_WIDTH = 0.2;
	const float GRID_HEIGHT = 0.2;

	glm::vec3* gridpoints;
	glm::vec3* gridcolors;
	
	unsigned vao, vbo;
	Shader shader;
	

public:
	Grid() {
		gridpoints = new glm::vec3[POINT_NUMBER];
		gridcolors = new glm::vec3[POINT_NUMBER];
		shader = Shader("grid_shader.vs", "grid_shader.fs");
	}

	~Grid() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		//delete gridcolors;
		//delete gridpoints;
		cout << "release grid space ok!" << endl;
	}

	void generate_grid_points() {
		cout << "generate points" << endl;

		float x_left = - VERTICAL_NUMBER* GRID_WIDTH / 2;
		float x_right = -x_left;
		float y = HORIZONTAL_NUMBER * GRID_HEIGHT / 2;

		// 水平方向的直线
		for (int i = 0; i < 2*(HORIZONTAL_NUMBER+1); i+=2) {
			gridpoints[i] = glm::vec3(x_left, y, 0);
			gridpoints[i + 1] = glm::vec3(x_right, y, 0);
			y -= GRID_HEIGHT;
		}

		float y_up = HORIZONTAL_NUMBER * GRID_HEIGHT / 2;
		float y_down = -y_up;
		float x = x_left;

		// 竖直方向的水平线
		for (int i = 2*(HORIZONTAL_NUMBER+1); i < POINT_NUMBER; i += 2) {
			gridpoints[i] = glm::vec3(x, y_up, 0);
			gridpoints[i + 1] = glm::vec3(x, y_down, 0);
			x += GRID_WIDTH;
		}

	}

	void print_points() {
		for (int i = 0; i < POINT_NUMBER; i++) {
			cout <<i<<" "<< gridpoints[i].x<<" "<<gridpoints[i].y<<" "<<gridpoints[i].z << endl;
		}
	}

	void init() {
		cout << "init grid object" << endl;

		generate_grid_points();

		//print_points();

		//生产一个vao
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//配置对应的VBO和属性指针，之后解绑VAO供之后使用
		//生成一个vbo
		glGenBuffers(1, &vbo);
		//绑定缓冲
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//顶点数据复制到缓冲的内存
		glBufferData(GL_ARRAY_BUFFER, POINT_NUMBER*sizeof(glm::vec4), gridpoints, GL_STATIC_DRAW);

		// 设置顶点属性指针
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void display() {
		glPointSize(10.0f);
		shader.activate();
		glBindVertexArray(vao);
		glDrawArrays(GL_LINES, 0, POINT_NUMBER);
		//cout << "drow lines" << endl;
	}
};

#endif // !GRID_H