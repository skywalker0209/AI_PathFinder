#pragma once


class Application
{
public:
	Application(int a_windowWidth, int a_windowHeight);
	~Application();

	void Run();

	void Load();
	void Unload();

	void Update(float deltatime);
	void Draw();

protected:
	int m_windowWidth;
	int m_windowHeight;



};