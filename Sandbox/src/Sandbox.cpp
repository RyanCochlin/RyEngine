#include "RyEngine.h"
#include "Entry.h"

class Sandbox : public RE::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

RE::Application* RE::GetApplication()
{
	return new Sandbox();
}