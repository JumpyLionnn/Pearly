#pragma once
#include "Scene.h"

namespace Pearly {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeBinary(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeBinary(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};

}