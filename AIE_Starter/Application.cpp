#include "Application.h"
#include "raylib.h"


Application::Application(int a_windowWidth, int a_windowHeight): //constructor

m_windowWidth(a_windowWidth),
m_windowHeight(a_windowHeight)
{

}

Application::~Application() {} // deconstructor

void Application::Load()
{
}