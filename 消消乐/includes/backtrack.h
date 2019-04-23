#define STB_IMAGE_IMPLEMENTATION
#ifndef BACKTRACK_H
#define BACKTRACK_H
#include <stdlib.h>
#include <ctime>
#include<iostream>
#include<vector>
#include<string>
#include<map>


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <common/shade.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<cell.h>
#include<grid.h>


using namespace std;

typedef glm::vec4 color4;

inline int max(int& a, int &b) {
	return a > b ? a : b;
}



/*
	Move 类是用来描述最优解的移动方案的，比如要把(3,2)和(3,3)交换，则用Move(3,2,0)表示
	@para (x,y) 相对于左上角的点的位置，由于交换有两个点，这里仅仅取其中一个: 靠左上角的点。
	@para dir: 表示交换的方向。如果为0则表示和右边的点交换，如果为1则表示和下面的点交换。-1则表示不移动即可消去。
	@para current_score: 记录本次移动之前的分数
*/

class Move {
	int x, y, dir, current_score;
public:

	Move(int x, int y, int dir, int current_score) {
		this->x = x;
		this->y = y;
		this->dir = dir;
		this->current_score = current_score;
	}

	void show() {
		string direction = "don't move";

		if (dir == 0) {
			direction = "move right";
		}
		if (dir == 1) {
			direction = "move down";
		}
		cout << "current score is " << current_score << "\t (" << x << "," << y << ") " << (direction) << " " << endl;
	}
};

class BackTrack {
	int n, m, k;

	// 窗口
	GLFWwindow* window;

	// 绘制线
	Grid grid;

	// 用来绘制格子
	Cell cells[8][4];


	// color
	color4 orange = color4(1.0, 0.5, 0.0, 1.0);
	color4 white = color4(1.0, 1.0, 1.0, 1.0);
	color4 black = color4(0.0, 0.0, 0.0, 1.0);
	color4 red = color4(1.0, 0.0, 0.0, 1.0); // red
	color4 yellow = color4(1.0, 1.0, 0.0, 1.0);  // yellow
	color4 green = color4(0.0, 1.0, 0.0, 1.0);  // green
	color4 blue = color4(0.0, 0.0, 1.0, 1.0);  // blue
	color4 magenta = color4(1.0, 0.0, 1.0, 1.0);  // magenta
	color4 cyan = color4(0.0, 1.0, 1.0, 1.0);  // cyan
	color4 background_color = color4(0.2f, 0.3f, 0.3f, 1.0f);

	glm::vec4 colors[5] = { magenta,red,yellow,green,background_color };

	// 一般情况下去0，1，2，...,k-1表示k中不同的动物头像
	// 如果没有动物头像，则值为-1
	vector<vector<int> > type;

	// 存储当前的最优解，用来分支裁剪优化
	int ans;

	// 记忆化搜索存储器
	map<vector<vector<int> >, int > maps;

public:
	// 记录当前的最优解的移动方案
	vector<Move> solution;

	BackTrack(int n, int m, int k, GLFWwindow* window) {
		this->n = n;
		this->m = m;
		this->k = k;
		this->window = window;

		this->ans = 0;
		this->solution.clear();

		this->type = vector<vector<int> >(n, vector<int>(m));


		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 4; j++) {
				cells[i][j].set_pos(i, j);
				//cells[i][j].set_color(colors[(rand() % 7) % 4]);
				cells[i][j].init();
			}
		}

		grid.init();

	}

	~BackTrack() {
		cout << "release memory" << endl;
		glfwTerminate();
	}

	// 随机生产n*m的矩阵数据
	void random_fill() {
		srand(unsigned(time(NULL)));
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				type[i][j] = (rand() % (k + 7)) % k;
				if (type[i][j] < 0) {
					type[i][j] = -type[i][j];
				}
			}
		}
	}

	// 返回(x,y)左边的点的个数
	int get_left(int x, int y) {
		int left = 0;
		int p = y - 1;
		while (p >= 0 && type[x][p] == type[x][y]) {
			p--;
			left++;
		}
		return left;
	}

	// 返回(x,y)右边的点的个数
	int get_right(int x, int y) {
		int right = 0;
		int p = y + 1;
		while (p < m && type[x][p] == type[x][y]) {
			p++;
			right++;
		}
		return right;
	}

	// 返回(x,y)上边的点的个数
	int get_up(int x, int y) {
		int up = 0;
		int p = x - 1;
		while (p >= 0 && type[p][y] == type[x][y]) {
			p--;
			up++;
		}
		return up;
	}

	// 返回(x,y)下边的点的个数
	int get_down(int x, int y) {
		int down = 0;
		int
			p = x + 1;
		while (p < n && type[p][y] == type[x][y]) {
			p++;
			down++;
		}
		return down;
	}

	void print_type() {

		cout << endl;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				cout << type[i][j] << ", ";
			}
			cout << "\n";
		}

		cout << endl;
	}

	// 计算清除的方块的个数
	int get_clear_number(int one_side, int another_side) {
		switch (one_side + another_side + 1) {
		case 3:
			return 3;
		case 4:
			return 4;
		case 5:
			return 5;
		default:
			return 0;
		}
	}

	// 消除(x,y)所在的方块，并返回所得到的分数
	int clear(int x, int y, int left, int right, int up, int down) {

		// 如果没有任何动物，则直接返回0
		if (type[x][y] == -1) {
			return 0;
		}

		int score = 0;

		/*

		if(left+right>2 || up+down>2){
			cout<<"----- before clear("<<x<<","<<y<<") -----"<<endl;
			//print_type();
		}
		*/

		/*
			接下来需要对(x,y)的两个方向进行判断是否消去。
		*/


		// 竖直方向
		switch (up + down + 1) {
		case 3:
			score += 1;
			break;
		case 4:
			score += 4;
			break;
		case 5:
			score += 10;
			break;
		default:
			break;
		}

		if (up + down + 1 > 2) {

			// 步长
			int stride = up + down + 1;

			// 将y=y0轴上的点，从x=0到x=x0-up往下移动stride的步长
			for (int i = x + down; i - stride >= 0 && type[i - stride][y] != -1; i--) {
				type[i][y] = type[i - stride][y];
			}

			// 找到y=y0轴上第一个为-1的点
			int p = 0;
			while (p < n&&type[p][y] == -1) {
				p++;
			}

			// 用-1填充空格
			for (int i = 0; p + i < n && i < stride; i++) {
				type[p + i][y] = -1;
			}

		}

		// 水平方向
		switch (left + right + 1) {
		case 3:
			score += 1;
			break;
		case 4:
			score += 4;
			break;
		case 5:
			score += 10;
			break;
		default:
			break;
		}

		if (left + right + 1 > 2) {
			// 下移
			for (int j = y - left; j <= y + right; j++) {

				if (up + down + 1 > 2 && j == y) {
					continue;
				}


				for (int i = x; i > 0 && type[i][j] != -1; i--) {
					type[i][j] = type[i - 1][j];
				}
			}

			// 用-1填充空白格
			for (int i = y - left; i <= y + right; i++) {
				type[0][i] = -1;
			}

		}

		/*
		if(left+right>2||up+down>2){
			cout<<"----- after clear -----"<<endl;
			print_type();
		}
		*/


		return score;
	}

	int get_score(int i, int j) {
		int left = get_left(i, j);
		int right = get_right(i, j);
		int up = get_up(i, j);
		int down = get_down(i, j);

		return clear(i, j, left, right, up, down);
	}

	int get_clear_number(int score) {
		int cnt;
		switch (score)
		{
			// 3 + 0
		case 1:
			cnt = 3;
			break;
			// 4 + 0
		case 4:
			cnt = 4;
			break;
			// 5 + 0
		case 10:
			cnt = 5;
			break;
			// 3 + 3
		case 2:
			cnt = 6;
			break;
			// 3 + 4
		case 5:
			cnt = 7;
			break;
			// 3 + 5
		case 11:
			cnt = 8;
			break;
			// 4 + 4
		case 8:
			cnt = 8;
			break;
			// 4 + 5
		case 14:
			cnt = 9;
			break;
			// 5 + 5
		case 20:
			cnt = 10;
			break;
		default:
			cnt = 0;
			break;
		}
		return cnt;
	}


	// 返回当前可行解的上界
	int get_upper_bound() {
		vector<int> count(k);

		for (int i = 0; i < k; i++) {
			count[i] = 0;
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (type[i][j] != -1) {
					//print_type();
					count[type[i][j]]++;
				}
			}
		}

		int aim = 0;

		for (int i = 0; i < k; i++) {
			if (count[i] >= 3) {
				aim += count[i] * 2;
			}
		}

		return aim;
	}

	int backtrack(int step, int cnt, int current_score, vector<Move> trace) {
		
		if(glfwWindowShouldClose(window)){
            return 0;
        }

        // 更新当前最优解，而不是等到叶子节点的时候才更新
		if (ans < current_score) {
			cout << "update ans from " << ans << " to " << current_score << endl;
			ans = current_score;
			solution = trace;
		}

		if (maps.find(type) != maps.end()) {
			cout << "the matrix has solved, stored in maps, so just return " << maps[type] << endl;
			return maps[type];
		}

		// 当前步的最大分数
		int max_score = 0;

		// 备份当前矩阵
		vector<vector<int> > backup = type;

		// 用来记录不移动的时候是否可以消除
		bool can_clear_with_no_move = false;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {

				if (type[i][j] != -1) {

					int left = get_left(i, j);
					int right = get_right(i, j);
					int up = get_up(i, j);
					int down = get_down(i, j);

					int score = clear(i, j, left, right, up, down);


					// 这里不适用上界函数是因为，如果不移动可以消除，那么肯定分数会更高
					//int upper_bound_score = get_upper_bound();

					if (score > 0) {

						Move move(i, j, -1, current_score);
						move.show();
						trace.push_back(move);
						refresh_cells();
						
						score += backtrack(step, cnt - get_clear_number(score), current_score + score, trace);

						maps[backup] = score;

						can_clear_with_no_move = true;
						// 更新max_score
						if (max_score < score) {
							max_score = score;
						}
						// 回溯到原来的状态
						trace.pop_back();
						type = backup;

						cout << "backtrack state" << endl;
						refresh_cells();
					}
				}
			}
		}

		/*
			此时的状态是必须要交换才可以消去，交换有两种：
			1. 和右边的邻近点交换
			2. 和下边的邻近点交换
		*/

		if (!can_clear_with_no_move) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {

					// 和右边的邻近点交换
					if (type[i][j] != -1 && j + 1 < m&&type[i][j + 1] != -1 && type[i][j] != type[i][j + 1]) {
						// 先尝试交换
						swap(type[i][j], type[i][j + 1]);
						// 尝试消去，得到分数score
						int score = get_score(i, j) + get_score(i, j + 1);
						// 获取此时的分数上界
						int upper_bound_score = get_upper_bound();
						// 如果当前的分数current_score + 消去的分数score + 后续步的上界分数upper_bound_score都小于
						// 当前所求解的最大分数，那么就可以剪枝了
						if (score > 0 && score + upper_bound_score + current_score > ans) {
							Move move(i, j, 0, current_score);
							trace.push_back(move);
							move.show();
							refresh_cells();
							score += backtrack(step + 1, cnt - (get_clear_number(score)), current_score + score, trace);
							maps[backup] = score;
							max_score = max(max_score, score);
							//回溯
							type = backup;
							trace.pop_back();

							cout << "backtrack: at ("<<i<<","<<j<<")\t dir: "<<0<< endl;
							refresh_cells();
						}
						else {
							if(score + upper_bound_score + current_score > ans)
								cout << "branch and bound method works" << endl;
							// 回溯
							swap(type[i][j], type[i][j + 1]);
						}
					}

					// down
					if (type[i][j] != -1 && i + 1 < n&&type[i + 1][j] != -1 && type[i][j] != type[i + 1][j]) {
						swap(type[i][j], type[i + 1][j]);

						int score = get_score(i, j) + get_score(i + 1, j);
						int upper_bound_score = get_upper_bound();

						if (score > 0 && score + upper_bound_score + current_score > ans) {
							Move move(i, j, 1, current_score);
							trace.push_back(move);
							move.show();
							refresh_cells();
							
							score += backtrack(step + 1, cnt - (get_clear_number(score)), current_score + score, trace);
							maps[backup] = score;
							trace.pop_back();
							max_score = max(max_score, score);
							//resume
							type = backup;

							cout << "backtrack: at (" << i << "," << j << ")\t dir: " << 1 << endl;
							refresh_cells();
						}
						else {
							if (score + upper_bound_score + current_score > ans)
								cout << "branch and bound method works" << endl;
							swap(type[i][j], type[i + 1][j]);
						}
					}

				}
			}
		}
        
        if(max_score==0){
		    cout << "[IMPORTANT] " << "CONNOT CLEAR IN THIS CASE, READY TO BACKTRACK" << endl;
        }
		return max_score;
	}

	void show_solution() {
		for (int i = 0; i < int(solution.size()); i++) {
			solution[i].show();
		}
	}


	void refresh_cells(bool first=false) {

        if(glfwWindowShouldClose(window)){
            return;
        }

        if(first==false)
            processInput(window);
		
        // render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 4; j++) {
				//cells[i][j].set_color(colors[(rand() % 7) % 4]);
				if (type[i][j] != -1) {
					cells[i][j].set_color(colors[type[i][j]]);
				}
				else {
					cells[i][j].set_color(background_color);
				}

				cells[i][j].display();
			}
		}

		grid.display();


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();


	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void processInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
        getchar();

	}

};




#endif // BACKTRACK_H
