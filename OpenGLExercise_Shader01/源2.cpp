#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

using namespace std;

//获取用户输入
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//两个三角形数据，6个点
//逆时针是正面，顺时针是背面,与点的定义顺序有关
float vertices[] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
};

unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

//顶点着色器,vertexShader
//按alt选中多行，然后在打 "
//location = 0代表VAO第0个位置,和下面的VAO位置对应
const char* vertexShaderSource =
"#version 330 core \n									 "
"layout (location = 0) in vec3 aPos; \n					 "
"void main(){ \n										 "
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n "
"} \n";

//片段着色器,fragmentShader
const char* fragmentShaderSource =
"#version 330 core							  \n"
"uniform vec4 ourColor;						  \n"
"void main(){								  \n"
"	FragColor = ourColor;					  \n"
"} \n";


int main() {
	//初始化glfw
	glfwInit();
	//打印版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口 窗口长宽，标题
	GLFWwindow* window = glfwCreateWindow(800, 600, "hello zf", NULL, NULL);
	if (window == NULL) {
		cout << "open window failed" << endl;
		glfwTerminate();
		return -1;
	}
	//使用当前窗口
	glfwMakeContextCurrent(window);

	//init glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cout << "open glew failed" << endl;
		glfwTerminate();
		return -1;
	}

	//可以绘制窗口的大小,左下角位置和宽高
	glViewport(0, 0, 800, 600);

	//设置是画线还是填充区域,默认填充
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//opengl是逆时针画三角形
	//剔除面
	//glEnable(GL_CULL_FACE);
	//剔除背面,正面FRONT
	//glCullFace(GL_FRONT);
	//glCullFace(GL_BACK);

	//创建VAO
	unsigned int VAO;
	//1代表1个,开辟一个VAO
	glGenVertexArrays(1, &VAO);
	//绑定VAO
	glBindVertexArray(VAO);

	//创建VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//VBO绑定
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//把三角形的数据装进VBO
	/*
	glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。
	它的第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。
	第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。
	第三个参数是我们希望发送的实际数据。
	第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
		GL_STATIC_DRAW ：数据不会或几乎不会改变。
		GL_DYNAMIC_DRAW：数据会被改变很多。
		GL_STREAM_DRAW ：数据每次绘制时都会改变。
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//链接顶点属性,分割VBO的数据,保存到VAO
	//3和3*sizeof(float)代表一次分割三个点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//打开0号VAO
	glEnableVertexAttribArray(0);

	//创建EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//把索引数组内容写进EBO,并且自动复制到绑定的VAO中
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//创建vertexShader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//创建fragmentShader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//创造着色器程序,连接顶点着色器和片段着色器
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//删除着色器对象，不再需要
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//绑定一次就可以了,不用写在循环里
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//渲染循环(Render Loop)
	while (!glfwWindowShouldClose(window))
	{
		//处理用户输入
		processInput(window);

		//渲染指令,rgb,透明值,窗口背景颜色
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// 更新uniform颜色
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//绘制物体,激活着色器
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		//绘制两个三角形，有6个点
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//通过索引画两个三角形,6个点
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//双缓冲技术，切换缓冲区
		glfwSwapBuffers(window);
		//获取用户交互事件
		glfwPollEvents();

	}

	//关闭窗口
	glfwTerminate();

	return 0;
}
