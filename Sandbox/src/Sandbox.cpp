#include <Pearly.h>

#include "imgui/imgui.h"

class ExampleLayer : public Pearly::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}

	void OnUpdate() override
	{
	//	PR_INFO("example layer update");
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test: ");
		ImGui::Text("Hello World!");
		ImGui::End();
	}	

	virtual void OnEvent(Pearly::Event& event) override
	{
		//PR_INFO("event {0}", event);
	}
};

class Sandbox : public Pearly::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}

private:

};


Pearly::Application* Pearly::CreateApplication()
{
	return new Sandbox();
}