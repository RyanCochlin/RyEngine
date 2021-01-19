#include "RyEngine.h"
#include "Entry.h"
#include "Core/Mesh.h"
#include "Core/CoreMath.h"
#include "Core/Color.h"

#include <vector>

class Sandbox : public RE::Application
{
public:
	Sandbox() {}
	~Sandbox() {}

	virtual void OnStart() override;
	virtual void OnUpdate() override;

private:
	RE::Triangle* _mTriange1;
	RE::Triangle* _mTriange2;
	RE::Triangle* _mTriange3;
	RE::Mesh* _mMesh;
};

void Sandbox::OnStart()
{
	RE_GRAPHICS->BackgroundColor(RE_CYAN);

	RE::Vector3 p11{ 0.0f, 0.5f, 0.0f };
	RE::Vector3 p21{ 0.25f, 0.25f, 0.0f };
	RE::Vector3 p31{ -0.25f, 0.25f, 0.0f };

	RE::Vector3 p12{ 0.25f, 0.25f, 0.0f };
	RE::Vector3 p22{ 0.5f, 0.0f, 0.0f };
	RE::Vector3 p32{ 0.0f, 0.0f, 0.0f };

	RE::Vector3 p13{ -0.25f, 0.25f, 0.0f };
	RE::Vector3 p23{ 0.0f, 0.0f, 0.0f };
	RE::Vector3 p33{ -0.5f, 0.0f, 0.0f };

	std::vector<RE::Vector3> v;
	v.push_back(p11);
	v.push_back(p21);
	v.push_back(p31);
	v.push_back(p12);
	v.push_back(p22);
	v.push_back(p32);
	v.push_back(p13);
	v.push_back(p23);

	v.push_back(p33);

	_mMesh = new RE::Mesh(v);
	_mMesh->SetColor(RE_YELLOW);
	_mMesh->Draw();

	/*_mTriange1 = new RE::Triangle(p11, p21, p31);
	_mTriange1->SetColor(RE_YELLOW);
	_mTriange1->Draw();

	_mTriange2 = new RE::Triangle(p12, p22, p32);
	_mTriange2->SetColor(RE_YELLOW);
	_mTriange2->Draw();*/
}

void Sandbox::OnUpdate()
{
}

RE::Application* RE::GetApplication()
{
	return new Sandbox();
}