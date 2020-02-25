#ifndef __GLFW_MAIN_WINDOW_H__
#define __GLFW_MAIN_WINDOW_H__
#include "JsonManager.h"

class MainWindow
{
public:
	MainWindow(JsonManager& jsonMgr) : m_jsonManager(jsonMgr) {}

	void Run();

private:
	JsonManager& m_jsonManager;
};

#endif