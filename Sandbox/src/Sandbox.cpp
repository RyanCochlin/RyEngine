#include "RyEngine.h"
#include "Entry.h"
#include "Core/Graphics/Mesh.h"
#include "Core/Graphics/GeometryGenerator.h"
#include "Core/Graphics/Transform.h"
#include "Core/Graphics/LightSystem.h"
#include "Core/Math/Vector.h"
#include "Core/Color.h"
#include "Core/Math/CoreMath.h"
#include "Core/Input/InputSystem.h"
#include "Core/Input/KeyCodes.h"
#include "Core/Input/Mouse.h"
#include "Core/ECS/ECS.h"

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

	void OnMouseEvent(RE::MouseEvent& mouseEvent, RE::EventType eventType);

private:
	RE::Triangle* _mTriange1;
	RE::Triangle* _mTriange2;
	RE::Triangle* _mTriange3;
	RE::Square* _mSquare;
	RE::Entity _mCubeEntity;
	RE::Cube _mCube;
	RE::Mesh* _mMesh;
	RE::OrthographicCamera* _mCamera;
	RE::PerspectiveCamera* _mPerCamera;
	RE::Vector3 _lookAt;
	RE::Light* _mLight;
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
	RE::Color ambient;
	ambient.c.r = 0.7f;
	ambient.c.g = 0.5f;
	ambient.c.b = 0.5f;
	ambient.c.a = 1.0f;
	//TODO: should NOT be directly accessing the Graphics Engine from the client. Background color should be set by the camera
	::RE::SubSystemManager::Instance().GetSubSystem<RE::GraphicsSystem>()->BackgroundColor(RE_CYAN);
	::RE::SubSystemManager::Instance().GetSubSystem<RE::GraphicsSystem>()->SetAmbientLight(ambient);

	//TODO this function binding is so ugly. Should be able to just pass the function pointer in
	auto func = std::bind(&Sandbox::OnMouseEvent, this, std::placeholders::_1, std::placeholders::_2);
	RE::Mouse::AddMouseEventHandler(func, RE::EventType::RE_MOUSE_SCROLL);

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
	_mPerCamera->SetLens(RE::Math::degToRad(100.0f), (1920.0f / 1080.0f), 1, 100);
	_lookAt = { 0.0f, 0.0f, -1.0f };
	_mPerCamera->LookAt({ 0.0f, 0.0f, 0.0f }, _lookAt, { 0.0f, 1.0f, 0.0f });
	//_mPerCamera->LookAt({ 0.0f, 0.0f, 0.0f }, { sinf(RE::Math::degToRad(45.0f)), 0.0f, -1.0f * cosf(RE::Math::degToRad(45.0f)) }, { 0.0f, 1.0f, 0.0f });
	_mPerCamera->SetPosition({ 0.0f, 20.0f, 0.0f });

	_mLight = RE::LightSystem::GetLight(RE::Directional);
	_mLight->Direction = { 1.0f, -0.5f, 0.4f };
	_mLight->Strength = { 0.8f, 0.5f, 0.5f };

	//RE::Vector3 p11{ 0.0f, 10.0f, 20.0f };
	//RE::Vector3 p21{ 10.0f, 0.0f, 20.0f };
	//RE::Vector3 p31{ -10.0f, 0.0f, 20.0f };

	RE::Vector3 p11{ 10.0f, 10.0f, 20.0f };
	RE::Vector3 p21{ 10.0f, 0.0f, 20.0f };
	RE::Vector3 p31{ -10.0f, 0.0f, 20.0f };
	//RE::Triangle t = RE::GeometryGenerator::GetTriangle(p11, p21, p31);

	//RE::Triangle2 t2(p11, p21, p31);
	/*RE::Vector3 p12{ 0.0f, 10.0f, 20.0f };
	RE::Vector3 p22{ 10.0f, 0.0f, 20.0f };
	RE::Vector3 p32{ -10.0f, 0.0f, 20.0f };
	RE::Triangle2 t22(p12, p22, p32);*/

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

	//_mCube = new RE::Cube(20, {-20.0f, 15.0f, 50.0f});
	_mCubeEntity = RE::GeometryGenerator::GetCube(10, { 0.0f, 0.0f, 0.0f });
	_mCubeEntity.transform->Translate({ -20.0f, 0.0f, 100.0f });
	_mCubeEntity.transform->Scale({ 1.5f, 1.5f, 1.5f});

	_mCubeEntity = RE::GeometryGenerator::GetCube(10, { 0.0f, 0.0f, 0.0f });
	_mCubeEntity.transform->Translate({ 20.0f, 0.0f, 100.0f });
	_mCubeEntity.transform->Scale({ 1.5f, 1.5f, 1.5f });
	//_mCube.SetColor(RE_RED);

	//_mTriange1 = new RE::Triangle(p11, p21, p31);
	//_mTriange1->SetColor(RE_YELLOW);
	//_mTriange1->Draw();

	//_mTriange2 = new RE::Triangle(p11, p21, p31);
	//_mTriange2->SetColor(RE_YELLOW);
	//_mTriange2->Draw();
}

void Sandbox::OnUpdate()
{
	RE::Camera* cam;

#if ORTHO 
	cam = _mCamera;
#else
	cam = _mPerCamera;
#endif

	RE::Vector3 rotation = _mCubeEntity.transform->GetRotation();
	rotation.y += 1.0f;
	if (rotation.y >= 360.0f)
		rotation.y = 0.0f;
	_mCubeEntity.transform->Rotate(rotation);
	if (RE::Keyboard::KeyDown(RE_UP))
	{
		//TODO define += for Vectors
		RE::Vector3 position = _mCubeEntity.transform->GetTranslation();//cam->GetPosition();
		position = position + RE::Vector3{ 0.0f, 0.5f, 0.0f };
		_mCubeEntity.transform->Translate(position);
		//cam->SetPosition(position);
		/*RE::Vector3 position = cam->GetPosition();
		position = position + RE::Vector3{ 0.0f, 0.5f, 0.0f };
		_lookAt = { _lookAt.x, sin(_lookAt.y - 0.01f), cos(_lookAt.z - 0.01f)};
		cam->LookAt(position, _lookAt, { 0.0f, 1.0f, 0.0f });*/
	}

	if (RE::Keyboard::KeyDown(RE_DOWN))
	{
		//TODO define += for Vectors
		RE::Vector3 position = _mCubeEntity.transform->GetTranslation();//cam->GetPosition();
		position = position + RE::Vector3{ 0.0f, -0.5f, 0.0f };
		_mCubeEntity.transform->Translate(position);
		//cam->SetPosition(position);
	}

	if (RE::Keyboard::KeyDown(RE_LEFT))
	{
		//TODO define += for Vectors
		RE::Vector3 position = _mCubeEntity.transform->GetTranslation();//cam->GetPosition();
		position = position + RE::Vector3{ -0.5f, 0.0f, 0.0f };
		_mCubeEntity.transform->Translate(position);
		//cam->SetPosition(position);
	}

	if (RE::Keyboard::KeyDown(RE_RIGHT))
	{
		//TODO define += for Vectors
		RE::Vector3 position = _mCubeEntity.transform->GetTranslation();//cam->GetPosition();
		position = position + RE::Vector3{ 0.5f, 0.0f, 0.0f };
		//cam->SetPosition(position);
		_mCubeEntity.transform->Translate(position);
	}

	if (RE::Keyboard::KeyDown(RE_R))
	{
		RE::Color aColor = ::RE::SubSystemManager::Instance().GetSubSystem<RE::GraphicsSystem>()->GetAmbientLight();
		aColor.c.r += 0.1f;
		if (aColor.c.r > 1.0f) aColor.c.r = 0.0f;
		::RE::SubSystemManager::Instance().GetSubSystem<RE::GraphicsSystem>()->SetAmbientLight(aColor);
	}

	if (RE::Keyboard::KeyDown(RE_G))
	{
		RE::Color aColor = ::RE::SubSystemManager::Instance().GetSubSystem<RE::GraphicsSystem>()->GetAmbientLight();
		aColor.c.g += 0.1f;
		if (aColor.c.g > 1.0f) aColor.c.g = 0.0f;
		::RE::SubSystemManager::Instance().GetSubSystem<RE::GraphicsSystem>()->SetAmbientLight(aColor);
	}

	if (RE::Keyboard::KeyDown(RE_B))
	{
		RE::Color aColor = ::RE::SubSystemManager::Instance().GetSubSystem<RE::GraphicsSystem>()->GetAmbientLight();
		aColor.c.b += 0.1f;
		if (aColor.c.b > 1.0f) aColor.c.b = 0.0f;
		::RE::SubSystemManager::Instance().GetSubSystem<RE::GraphicsSystem>()->SetAmbientLight(aColor);
	}
}

void Sandbox::OnMouseEvent(RE::MouseEvent& mouseEvent, RE::EventType eventType)
{
	if (eventType == RE::EventType::RE_MOUSE_SCROLL)
	{
		RE::MouseScrollEvent& scrollEvent = dynamic_cast<RE::MouseScrollEvent&>(mouseEvent);
		int wheelDelta = (int)scrollEvent.GetWheelDelta();
#if ORTHO
		RE::Vector3 position = _mCamera->GetPosition();
#else
		RE::Vector3 position = _mPerCamera->GetPosition();
#endif

		if (wheelDelta > 0)
		{
			position = position + RE::Vector3{ 0.0f, 0.0f, 5.0f };
		}
		else
		{
			position = position + RE::Vector3{ 0.0f, 0.0f, -5.0f };
		}
#if ORTHO
		_mCamera->SetPosition(position);
#else
		_mPerCamera->SetPosition(position);
#endif
	}
}

RE::Application* RE::GetApplication()
{
	return new Sandbox();
}