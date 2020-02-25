#include "GlfwMainWindow.h"
#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "linmath.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "ft2build.h"
#include FT_FREETYPE_H
#include <future>

static bool m_updateJson = false;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_LEFT:
		case GLFW_KEY_RIGHT:
			m_updateJson = true;
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		default:
			break;
		}
	}
}

static void resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void MainWindow::Run()
{
	if (!glfwInit())
		return;

	char * json_data;
	// need to populate this with a rest call
	auto readJsonFunc = std::async(&JsonManager::parseRequest, &m_jsonManager, json_data);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "DSS Coding Assignment", monitor, NULL);

	FILE * file = fopen("..\\resources\\mlb_background.jpg", "rb");
	if (file == NULL)
		return;
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char * data = (unsigned char*)malloc(len);
	fread(data, 1, len, file);
	fclose(file);

	FT_Library ft;
	if(FT_Init_FreeType(&ft))
		return;

	FT_Face face;
	FT_Error err = FT_New_Face(ft, "fonts/arial.ttf", 0, &face);
	//if(err)
		//return;

	if (monitor)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetWindowSizeCallback(window, resize_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		if (m_updateJson)
		{
			m_updateJson = false;
			readJsonFunc.get();
		}

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		//glClearColor(0, 0, 1, 1);
		// display background
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Set headline
		//FT_Load_Char(face, m_jsonManager.GetMostRecentGame()->GetHeadline().c_str(), FT_LOAD_RENDER);

		// Should be thumbnail
		std::string pic = m_jsonManager.GetMostRecentGame()->GetThumbnail();
		glBegin(GL_QUADS);
		glVertex3f(-0.1f, 0.1f, -1.0f);
		glVertex3f(0.1f, 0.1f, -1.0f);
		glVertex3f(0.1f, -0.1f, -1.0f);
		glVertex3f(-0.1f, -0.1f, -1.0f);
		glEnd();

		// Set summary
		//FT_Load_Char(face, m_jsonManager.GetMostRecentGame()->GetSummary().c_str(), FT_LOAD_RENDER);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//free(data);
	glfwDestroyWindow(window);
	glfwTerminate();
}