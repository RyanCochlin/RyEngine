#include "RyEngine.h"
#include "Entry.h"
#include "Core/Mesh.h"
#include "Core/Math/Vector.h"
#include "Core/Color.h"
#include "Core/Math/CoreMath.h"

#include <vector>

//TODO just a switch for camera type for now for testing
#define ORTHO 0

class Sandbox : public RE::Application
{
public:
	Sandbox() {}
	virtual ~Sandbox();

	virtual void OnStart() override;
	virtual void OnUpdate() override;

private:
	RE::Triangle* _mTriange1;
	RE::Triangle* _mTriange2;
	RE::Triangle* _mTriange3;
	RE::Square* _mSquare;
	RE::Cube* _mCube;
	RE::Mesh* _mMesh;
	RE::OrthographicCamera* _mCamera;
	RE::PerspectiveCamera* _mPerCamera;
};

Sandbox::~Sandbox()
{
#if ORTHO
	delete _mCamera;
#else
	delete _mPerCamera;
#endif
}

void Sandbox::OnStart()
{
	//TODO: should NOT be directly accessing the Graphics Engine from the client. Background color should be set by the camera
	::RE::SubSystemManager::Instance().GetSubSystem<RE::GraphicsSystem>()->BackgroundColor(RE_CYAN);

#if ORTHO
	_mCamera = new RE::OrthographicCamera();
	_mCamera->SetLens(1920, 1080, -1, 1);
	_mCamera->LookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, {0.0f, 1.0f, 0.0f});
	_mCamera->SetPosition({ 300.0f, 200.0f, 0.0f });

	RE::Vector3 p11{ 0.0f, 100.0f, 0.0f };
	RE::Vector3 p21{ 100.0f, 0.0f, 0.0f };
	RE::Vector3 p31{ -100.0f, 0.0f, 0.0f };
#else

	_mPerCamera = new RE::PerspectiveCamera();
	_mPerCamera->SetLens(RE::Math::degToRad(90.0f), (1920.0f / 1080.0f), 1, 100);
	_mPerCamera->LookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f });
	//_mPerCamera->LookAt({ 0.0f, 0.0f, 0.0f }, { sinf(RE::Math::degToRad(45.0f)), 0.0f, -1.0f * cosf(RE::Math::degToRad(45.0f)) }, { 0.0f, 1.0f, 0.0f });
	_mPerCamera->SetPosition({ 0.0f, 0.0f, 0.0f });

	//RE::Vector3 p11{ 0.0f, 10.0f, 20.0f };
	//RE::Vector3 p21{ 10.0f, 0.0f, 20.0f };
	//RE::Vector3 p31{ -10.0f, 0.0f, 20.0f };

	RE::Vector3 p11{ 10.0f, 10.0f, 20.0f };
	RE::Vector3 p21{ 10.0f, 0.0f, 20.0f };
	RE::Vector3 p31{ -10.0f, 0.0f, 20.0f };

	//RE::Vector3 p11{ 0.0f, 10.0f, 20.0f };
	//RE::Vector3 p21{ 10.0f, 0.0f, 20.0f };
	//RE::Vector3 p31{ -10.0f, 0.0f, 20.0f };

#endif

	/*std::vector<RE::Vector3> v;
	v.push_back(p11);
	v.push_back(p21);
	v.push_back(p31);
	std::vector<RE_INDEX> i;
	i.push_back(0);
	i.push_back(1);
	i.push_back(2);*/

	//v.push_back(p12);
	//v.push_back(p22);
	//v.push_back(p32);
	//v.push_back(p13);
	//v.push_back(p23);
	//v.push_back(p33);

	//_mMesh = new RE::Mesh(v, i);
	//_mMesh->SetColor(RE_YELLOW);
	//_mMesh->Draw();

	//_mSquare = new RE::Square(80, 40, { 0.0f, 0.0f, 20.0f });
	//_mSquare->SetColor(RE_RED);
	//_mSquare->Draw();

	_mCube = new RE::Cube(20, {-20.0f, 15.0f, 40.0f});
	_mCube->SetColor(RE_RED);
	_mCube->Draw();

	//_mTriange1 = new RE::Triangle(p11, p21, p31);
	//_mTriange1->SetColor(RE_YELLOW);
	//_mTriange1->Draw();

	//_mTriange2 = new RE::Triangle(p11, p21, p31);
	//_mTriange2->SetColor(RE_YELLOW);
	//_mTriange2->Draw();
}

void Sandbox::OnUpdate()
{
}

RE::Application* RE::GetApplication()
{
	return new Sandbox();
}