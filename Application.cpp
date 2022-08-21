#include "Application.h"

void Application::Run()
{
	InitializeMenus();

	if (HomeLoop())
		MainLoop();
}

void Application::Dispose()
{
	delete _election;
}

bool Application::HomeLoop()
{
	bool res = true;
	bool done = false;
	while (!done)
	{
		_home_menu.Display(cout);
		try
		{
			res = _home_menu.Trigger(_election->GetAction());
			done = true;
		}
		catch (invalid_argument& e)
		{
			ElectionManager::ErrorHandler::Send(e);
		}
		catch (bad_alloc& e)
		{
			ElectionManager::ErrorHandler::Send(e);
		}
		catch (...) { exit(-9); }
	}
	return res;
}

void Application::MainLoop()
{
	bool isRunning = true;

	while (isRunning)
	{
		_main_menu.Display(cout);
		try
		{
			isRunning = _main_menu.Trigger(_election->GetAction());
		}
		catch (invalid_argument& e)
		{
			ElectionManager::ErrorHandler::Send(e);
		}
		catch (logic_error& e)
		{
			ElectionManager::ErrorHandler::Send(e);
		}
		catch (...)
		{
			// Most likely it won't come here.
			exception e("Unhandled error.");
			ElectionManager::ErrorHandler::Send(e);
			exit(-69);
		}
	}
}

void Application::InitializeMenus()
{
	_home_menu.Add(new MenuOption("1",  "New Election",				new Action<Election*, ElectionManager::Initialize>(&_election)));
	_home_menu.Add(new MenuOption("2",  "Load",						new Action<Election*, ElectionManager::Load>(&_election)));
	_home_menu.Add(new MenuOption("3",  "Exit",						new Action<Application, Exit>(this)));

	_main_menu.Add(new MenuOption("1",  "Add District",				new Action<Election, ElectionManager::AddDistrict>(&_election)));
	_main_menu.Add(new MenuOption("2",  "Add Citizen",				new Action<Election, ElectionManager::AddCitizen>(&_election)));
	_main_menu.Add(new MenuOption("3",  "Add Party",				new Action<Election, ElectionManager::AddParty>(&_election)));
	_main_menu.Add(new MenuOption("4",  "Add Representative",		new Action<Election, ElectionManager::AddRepresentative>(&_election)));
	_main_menu.Add(new MenuOption("5",  "Present all districts",	new Action<Election, ElectionManager::PrintDistricts>(&_election)));
	_main_menu.Add(new MenuOption("6",  "Present all citizens",		new Action<Election, ElectionManager::PrintCitizens>(&_election)));
	_main_menu.Add(new MenuOption("7",  "Present all parties",		new Action<Election, ElectionManager::PrintParties>(&_election)));
	_main_menu.Add(new MenuOption("8",  "Vote",						new Action<Election, ElectionManager::Vote>(&_election)));
	_main_menu.Add(new MenuOption("9",  "Present election results",	new Action<Election, ElectionManager::ShowResults>(&_election)));
	_main_menu.Add(new MenuOption("10", "Exit",						new Action<Application, Exit>(this)));
	_main_menu.Add(new MenuOption("11", "Save",						new Action<Election*, ElectionManager::Save>(&_election)));
	_main_menu.Add(new MenuOption("12", "Load",						new Action<Election*, ElectionManager::Load>(&_election)));
}