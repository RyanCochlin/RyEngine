#pragma once

#include "Application.h"

#ifdef RE_WINDOWS

extern RE::Application* RE::GetApplication();

int main(int argc, char** argv)
{
	RE::Application* app = RE::GetApplication();
	app->Start();
	app->Run();
	delete app;
}

#else
//TODO other entry points
#endif