#include <Pearly.h>

class Sandbox : public Pearly::Application
{
public:
	Sandbox()
	{

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