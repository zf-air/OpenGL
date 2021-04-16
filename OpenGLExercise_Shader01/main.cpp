#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

using namespace std;

//��ȡ�û�����
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//�������������ݣ�6����
//��ʱ�������棬˳ʱ���Ǳ���,���Ķ���˳���й�
float vertices[] = {
	// λ��              // ��ɫ
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
};


//������ɫ��,vertexShader
//��altѡ�ж��У�Ȼ���ڴ� "
//location = 0����VAO��0��λ��,�������VAOλ�ö�Ӧ
const char* vertexShaderSource =
"#version 330 core \n									 "
"layout (location = 0) in vec3 aPos; \n					 "
"layout (location = 1) in vec3 aColor; \n				 "
"out vec4 vertexColor; \n                                "
"void main(){ \n										 "
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n "
"    vertexColor = vec4(aColor.x, aColor.y, aColor.z, 1.0);\n"
"} \n";

//Ƭ����ɫ��,fragmentShader
const char* fragmentShaderSource =
"#version 330 core							  \n"
"out vec4 FragColor;						  \n"
"in vec4 vertexColor;                         \n"
"uniform vec4 ourColor;                       \n"
"void main(){								  \n"
"	FragColor = vertexColor;					  \n"
"} \n";


int main() {
	//��ʼ��glfw
	glfwInit();
	//��ӡ�汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//�������� ���ڳ�������
	GLFWwindow* window = glfwCreateWindow(800, 600, "hello zf", NULL, NULL);
	if (window == NULL) {
		cout << "open window failed" << endl;
		glfwTerminate();
		return -1;
	}
	//ʹ�õ�ǰ����
	glfwMakeContextCurrent(window);

	//init glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cout << "open glew failed" << endl;
		glfwTerminate();
		return -1;
	}

	//���Ի��ƴ��ڵĴ�С,���½�λ�úͿ��
	glViewport(0, 0, 800, 600);

	//�����ǻ��߻����������,Ĭ�����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//opengl����ʱ�뻭������
	//�޳���
	//glEnable(GL_CULL_FACE);
	//�޳�����,����FRONT
	//glCullFace(GL_FRONT);
	//glCullFace(GL_BACK);

	//����VAO
	unsigned int VAO;
	//1����1��,����һ��VAO
	glGenVertexArrays(1, &VAO);
	//��VAO
	glBindVertexArray(VAO);

	//����VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//VBO��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//�������ε�����װ��VBO
	/*
	glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ�����
	���ĵ�һ��������Ŀ�껺������ͣ����㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ���ϡ�
	�ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�
	����������������ϣ�����͵�ʵ�����ݡ�
	���ĸ�����ָ��������ϣ���Կ���ι�����������ݡ�����������ʽ��
		GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
		GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
		GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//���Ӷ�������,�ָ�VBO������,���浽VAO
	//3��3*sizeof(float)����һ�ηָ�������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//��0��VAO
	glEnableVertexAttribArray(0);
	//������ɫ��Ϣ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	

	//����vertexShader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//����fragmentShader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//������ɫ������,���Ӷ�����ɫ����Ƭ����ɫ��
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//ɾ����ɫ�����󣬲�����Ҫ
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//��һ�ξͿ�����,����д��ѭ����
	glBindVertexArray(VAO);
	//��Ⱦѭ��(Render Loop)
	while (!glfwWindowShouldClose(window))
	{
		//�����û�����
		processInput(window);

		//��Ⱦָ��,rgb,͸��ֵ,���ڱ�����ɫ
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//��������,������ɫ��
		glUseProgram(shaderProgram);

		// ����uniform��ɫ
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");		
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		//���������Σ���3����
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//˫���弼�����л�������
		glfwSwapBuffers(window);
		//��ȡ�û������¼�
		glfwPollEvents();

	}

	//�رմ���
	glfwTerminate();

	return 0;
}
