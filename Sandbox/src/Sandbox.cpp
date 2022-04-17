#include <Pearly.h>

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
		PushOverlay(new Pearly::ImGuiLayer());
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