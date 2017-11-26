#include <stdlib.h>

#include <iostream>

#define GLEW_STATIC
#include <glew.h>

#include <glfw3.h>

#include "LadderGenerator.h"
#include "LoadShaders.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

void handleKeyLeftPress();
void handleKeyRightPress();

enum TransformOperation { SCALING, TRANSLATING, ROTATING };
enum Axis { X, Y, Z };

const float D_TRANSLATING = 0.1f;
const float D_ROTATING = 5.0f;
const float D_SCALING = 0.5f;

struct {
	float dx = 0;
	float dy = 0;
	float dz = 0;
	float xAngle = 0;
	float yAngle = 0;
	float zAngle = 0;
	float scale = 1;
} transformOptions;

struct {
	Axis axis = X;
	TransformOperation operation = TRANSLATING;
} operations;

void* handleError(int code, char* description) {
	std::cout << code << std::endl;
	std::cout << description << std::endl;
	int temp;
	std::cin >> temp;
	return 0;
}

void handleKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//if (action == GLFW_KEY_DOWN) {
		switch (key) {
			case GLFW_KEY_X: {
				operations.axis = X;
				break;
			}
			case GLFW_KEY_Y: {
				operations.axis = Y;
				break;
			}
			case GLFW_KEY_Z: {
				operations.axis = Z;
				break;
			}
			case GLFW_KEY_T: {
				operations.operation = TRANSLATING;
				break;
			}
			case GLFW_KEY_R: {
				operations.operation = ROTATING;
				break;
			}
			case GLFW_KEY_S: {
				operations.operation = SCALING;
				break;
			}
			case GLFW_KEY_LEFT: {
				handleKeyLeftPress();
				break;
			}
			case GLFW_KEY_RIGHT: {
				handleKeyRightPress();
				break;
			}
		}
	//}

}

void handleKeyLeftPress() {
	if (operations.operation == TRANSLATING) {
		float* val = NULL;

		switch (operations.axis) {
		case X:
			val = &transformOptions.dx;
			break;
		case Y:
			val = &transformOptions.dy;
			break;
		case Z:
			val = &transformOptions.dz;
			break;
		}

		*val -= D_TRANSLATING;
	}
	if (operations.operation == ROTATING) {
		float* val = NULL;

		switch (operations.axis) {
		case X:
			val = &transformOptions.xAngle;
			break;
		case Y:
			val = &transformOptions.yAngle;
			break;
		case Z:
			val = &transformOptions.zAngle;
			break;
		}

		*val -= D_ROTATING;
	}
	if (operations.operation == SCALING) {
		transformOptions.scale -= D_SCALING;
	}
}

void handleKeyRightPress() {
	if (operations.operation == TRANSLATING) {
		float* val = NULL;

		switch (operations.axis) {
		case X:
			val = &transformOptions.dx;
			break;
		case Y:
			val = &transformOptions.dy;
			break;
		case Z:
			val = &transformOptions.dz;
			break;
		}

		*val += D_TRANSLATING;
	}
	if (operations.operation == ROTATING) {
		float* val = NULL;

		switch (operations.axis) {
		case X:
			val = &transformOptions.xAngle;
			break;
		case Y:
			val = &transformOptions.yAngle;
			break;
		case Z:
			val = &transformOptions.zAngle;
			break;
		}

		*val += D_ROTATING;
	}
	if (operations.operation == SCALING) {
		transformOptions.scale += D_SCALING;
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	LadderGenerator* ladder_generator = new LadderGenerator(1, 1, 1, 3);
	Coordinates* coordinates = ladder_generator->generate();

	FILE* f;
	fopen_s(&f, "result.txt", "w");

	for (int i = 0; i < coordinates->count; i++) {
		fprintf_s(f, "%.5f,", coordinates->values[i]);
	}

	fclose(f);

	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwSetErrorCallback((GLFWerrorfun)handleError);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Practice", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coordinates->count, coordinates->values, GL_STATIC_DRAW);
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	GLuint programID = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glfwSetKeyCallback(window, handleKeyboardEvent);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Проекционная матрица : 45deg; поле обзора, 4:3 соотношение сторон, диапазон : 0.1 юнит <-> 100 юнитов
		glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		// Матрица камеры
		glm::mat4 View = glm::lookAt(
			glm::vec3(10, 10, 10), // Камера находится в мировых координатах (4,3,3)
			glm::vec3(0, 0, 0), // И направлена в начало координат
			glm::vec3(0, 1, 0)  // "Голова" находится сверху
		);
		// Матрица модели : единичная матрица (Модель находится в начале координат)
		glm::mat4 Model = glm::translate(glm::mat4(), glm::vec3(transformOptions.dx, transformOptions.dy, transformOptions.dz));
		Model = glm::scale(Model, glm::vec3(transformOptions.scale));
		Model = glm::rotate(Model, glm::radians(transformOptions.xAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::rotate(Model, glm::radians(transformOptions.yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		Model = glm::rotate(Model, glm::radians(transformOptions.zAngle), glm::vec3(0.0f, 0.0f, 1.0f));

		// Итоговая матрица ModelViewProjection, которая является результатом перемножения наших трех матриц
		glm::mat4 MVP = Projection * View * Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


		glUseProgram(programID);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, coordinates->count / 3);
		
		glDisableVertexAttribArray(0);
	}

	return 0;
}