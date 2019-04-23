#include<common/stb_image.h>
#include <glad/glad.h>
#include<iostream>
#include<string>

class Texture {
public:
	unsigned *texture;
	Texture(unsigned* texture) {
		this->texture = texture;
	}
	
	void genTexture(std::string filename) {
		glGenTextures(1, texture);

		//需要绑定它，让之后任何的纹理指令都可以配置当前绑定的纹理：
		glBindTexture(GL_TEXTURE_2D, *texture);

		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 加载并生成纹理
		//宽度、高度和颜色通道的个数
		int width, height, nrChannels;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

	}

};