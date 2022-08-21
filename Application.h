#pragma once
#include "ElectionManager.h"
#include "Action.h"
#include "Menu.h"
#include "Checks.h"
using namespace std;

class Application
{
public:
	void Run();

	Application operator=(const Application&) = delete;
	Application(const Application&) = delete;
	Application() : _election() {}
	~Application() { Dispose(); }

private:
	Election* _election;
	Menu _main_menu;
	Menu _home_menu;

	void InitializeMenus();
	void Dispose();
	bool HomeLoop();
	void MainLoop();

	class Exit
	{
	public:
		bool operator()(Application& app) { return false; }
	};
};