#pragma once
#include <glm/glm.hpp>

namespace Pearly {
	class Layout
	{
	public:
		static void Space(float size);
		static void Indent(float size);
		static void EndIndent();
	};
	
}