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
#include "glm/gtc/type_ptr.hpp"

void handleKeyLeftPress();
void handleKeyRightPress();

enum TransformOperation { SCALING, TRANSLATING, ROTATING, SHININESS_CHANGING };
enum Axis { X, Y, Z };

const float D_TRANSLATING = 0.1f;
const float D_ROTATING = 5.0f;
const float D_SCALING = 0.5f;
const float D_SHININESS = 1.5f;

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
	float shininess = 32.0f;
	glm::vec3 ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
} materialOptions;

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
			case GLFW_KEY_H: {
				operations.operation = SHININESS_CHANGING;
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
	if (operations.operation == SHININESS_CHANGING) {
		materialOptions.shininess -= D_SHININESS;
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
	if (operations.operation == SHININESS_CHANGING) {
		materialOptions.shininess += D_SHININESS;
	}
}

void writeToFileCoordintates(Coordinates* coordinates, char* filename) {
	FILE* f;
	fopen_s(&f, filename, "w");

	for (int i = 0; i < coordinates->count; i++) {
		fprintf_s(f, "%.5f ", coordinates->values[i]);
	}

	fclose(f);
}

Coordinates* calculateNormalsAndToVerticesArray(Coordinates* coordinates) {
	Coordinates* resultCoordinates = (Coordinates*)malloc(sizeof(Coordinates));
	resultCoordinates->count = coordinates->count * 2;
	resultCoordinates->values = (float*)malloc(sizeof(float) * resultCoordinates->count);

	for (int i = 0; i < coordinates->count / 3; i++) {
		int idx = i * 2 * 3;
		resultCoordinates->values[idx] = coordinates->values[i * 3];
		resultCoordinates->values[idx + 1] = coordinates->values[i * 3 + 1];
		resultCoordinates->values[idx + 2] = coordinates->values[i * 3 + 2];
	}

	glm::vec3 normal = glm::vec3(.0f, .0f, .0f);

	for (int i = 0; i < coordinates->count / 3 - 1;) {
		glm::vec3 a = glm::vec3(coordinates->values[i * 3], coordinates->values[i * 3 + 1], coordinates->values[i * 3 + 2]);
		glm::vec3 b = glm::vec3(coordinates->values[i * 3 + 3], coordinates->values[i * 3 + 4], coordinates->values[i * 3 + 5]);
		glm::vec3 c = glm::vec3(coordinates->values[i * 3 + 6], coordinates->values[i * 3 + 7], coordinates->values[i * 3 + 8]);
		
		glm::vec3 normal = glm::cross(b - a, c - a);
		normal = normal / glm::length(normal);
		//normal = glm::abs(normal);

		int idx = (i++ * 2 + 1) * 3;
		resultCoordinates->values[idx] = normal.x;
		resultCoordinates->values[idx + 1] = normal.y;
		resultCoordinates->values[idx + 2] = normal.z;

		idx = (i++ * 2 + 1) * 3;
		resultCoordinates->values[idx] = normal.x;
		resultCoordinates->values[idx + 1] = normal.y;
		resultCoordinates->values[idx + 2] = normal.z;

		idx = (i++ * 2 + 1) * 3;
		resultCoordinates->values[idx] = normal.x;
		resultCoordinates->values[idx + 1] = normal.y;
		resultCoordinates->values[idx + 2] = normal.z;
	}

	return resultCoordinates;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	LadderGenerator* ladder_generator = new LadderGenerator(1, 1, 1, 10);
	Coordinates* coordinates = ladder_generator->generate();

	writeToFileCoordintates(coordinates, "result.txt");

	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwSetErrorCallback((GLFWerrorfun)handleError);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Practice", NULL, NULL);
	if (window == NULL)
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
/*
	coordinates = calculateNormalsAndToVerticesArray(coordinates);
	writeToFileCoordintates(coordinates, "result1.txt");
*/
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

	GLuint modelID = glGetUniformLocation(programID, "model");
	GLuint viewID = glGetUniformLocation(programID, "view");
	GLuint projectionID = glGetUniformLocation(programID, "projection");

	GLint viewPosLoc = glGetUniformLocation(programID, "viewPos");
	glUniform3f(viewPosLoc, 100, 100, 100);

	GLint lightPosID = glGetUniformLocation(programID, "lightPos");
	glUniform3f(lightPosID, 100, 100, 100);

	GLint materialAmbientID = glGetUniformLocation(programID, "materialAmbient");
	GLint materialDiffuseID = glGetUniformLocation(programID, "materialDiffuse");
	GLint materialSpecularID = glGetUniformLocation(programID, "materialSpecular");
	GLint materialShininessID = glGetUniformLocation(programID, "materialShininess");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glfwSetKeyCallback(window, handleKeyboardEvent);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window))
	{
		glUniform3f(materialAmbientID, materialOptions.ambient.x, materialOptions.ambient.y, materialOptions.ambient.z);
		glUniform3f(materialDiffuseID, materialOptions.diffuse.x, materialOptions.diffuse.y, materialOptions.diffuse.z);
		glUniform3f(materialSpecularID, materialOptions.specular.x, materialOptions.specular.y, materialOptions.specular.z);
		glUniform1f(materialShininessID, materialOptions.shininess);

		glfwPollEvents();
		glfwSwapBuffers(window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 View = glm::lookAt(
			glm::vec3(10, 10, 10), // Камера находится в мировых координатах
			glm::vec3(0, 0, 0), // И направлена в начало координат
			glm::vec3(0, 1, 0)  // Голова" находится сверху
		);
		// Матрица модели : единичная матрица (Модель находится в начале координат)
		glm::mat4 Model = glm::translate(glm::mat4(), glm::vec3(transformOptions.dx, transformOptions.dy, transformOptions.dz));
		Model = glm::scale(Model, glm::vec3(transformOptions.scale));
		Model = glm::rotate(Model, glm::radians(transformOptions.xAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::rotate(Model, glm::radians(transformOptions.yAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		Model = glm::rotate(Model, glm::radians(transformOptions.zAngle), glm::vec3(0.0f, 0.0f, 1.0f));

		// Итоговая матрица ModelViewProjection, которая является результатом перемножения наших трех матриц
		glm::mat4 MVP = Projection * View * Model;

		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(Model));
		glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(Projection));

		glUseProgram(programID);

		glDrawArrays(GL_TRIANGLES, 0, coordinates->count / 3);
	}

	return 0;
}