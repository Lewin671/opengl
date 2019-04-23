#ifndef CELL_H
#define CELL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>
using namespace std;
typedef glm::vec4 color4;
typedef glm::vec3 point;

class Cell {

	// 表示格子在n行m列
	int n, m;
	color4 color;

	unsigned vao, vbo;
	unsigned color_vbo;

	const int POINT_NUMBER = 4;

	point* points;
	Shader shader;

	const float WIDTH = 0.2;
	const float HEIGHT = 0.2;

	const float unit = 0.01;
public:

	Cell() {
		shader = Shader("cell_shader.vs", "cell_shader.fs");
		points = new point[POINT_NUMBER];
	}

	~Cell() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	point get_point(int n, int m) {
		return point(-0.4 + m * 0.2, 0.8 - n * 0.2, 0);
	}

	void set_color(color4 color) {
		this->color = color;
	}

	void set_pos(int n, int m) {
		points[0] = get_point(n, m);
		points[1] = get_point(n, m + 1);
		points[2] = get_point(n + 1, m + 1);
		points[3] = get_point(n + 1, m);

		// 下面的操作是为了缩小这个方格，从而能够看得出分割线
		points[0].x += unit;
		points[0].y -= unit;

		points[1].x -= unit;
		points[1].y -= unit;

		points[2].x -= unit;
		points[2].y += unit;

		points[3].x += unit;
		points[3].y += unit;

	}

	void init() {
		//cout << "init ceil object" << endl;

		//生产一个vao
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//配置对应的VBO和属性指针，之后解绑VAO供之后使用
		//生成一个vbo
		glGenBuffers(1, &vbo);
		//绑定缓冲
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//顶点数据复制到缓冲的内存
		glBufferData(GL_ARRAY_BUFFER, POINT_NUMBER * sizeof(glm::vec4), points, GL_STATIC_DRAW);

		// 设置顶点属性指针
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(1);
	}

	void display() {
		glPointSize(5.0f);
		shader.activate();
		shader.setVec4("color", color);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, POINT_NUMBER);
	}
};
#endif