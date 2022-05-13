#include "prpch.h"
#include "SceneSerializer.h"


#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include <stb_image.h>
#include "Entity.h"
#include "Components.h"

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& vec)
		{
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& vec)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& vec)
		{
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& vec)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			vec.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Pearly {

	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec2& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec4& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "123456789";// entity id

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << transform.Position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;

			out << YAML::EndMap;
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRenderer.Color;
			std::string path;
			if (spriteRenderer.Texture)
				path = spriteRenderer.Texture->GetPath();
			out << YAML::Key << "TexturePath" << YAML::Value << path;
			out << YAML::Key << "TilingFactor" << YAML::Value << spriteRenderer.TilingFactor;
			out << YAML::Key << "ZIndex" << YAML::Value << spriteRenderer.ZIndex;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			CameraComponent& camera = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "Camera";
			out << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "OrthographicSize" << camera.Camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << camera.Camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << camera.Camera.GetOrthographicFarClip();
			out << YAML::EndMap;
			out << YAML::Key << "Primary" << camera.Primary;
			out << YAML::Key << "FixedAspectRatio" << camera.FixedAspectRatio;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Scene Name";
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](entt::entity entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}
	void SceneSerializer::SerializeBinary(const std::string& filepath)
	{
		PR_CORE_ASSERT(false, "Method not implemented yet!");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream sstream;
		sstream << stream.rdbuf();

		YAML::Node data = YAML::Load(sstream.str());
		if (data.size() == 0)
			return false;
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		PR_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		const YAML::Node& entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64 uuid = entity["Entity"].as<uint64>();

				std::string name;
				const YAML::Node& tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				const YAML::Node& transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					TransformComponent& transform = deserializedEntity.GetComponent<TransformComponent>();
					transform.Position = transformComponent["Position"].as<glm::vec2>();
					transform.Rotation = transformComponent["Rotation"].as<float>();
					transform.Scale = transformComponent["Scale"].as<glm::vec2>();
				}

				const YAML::Node& spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					SpriteRendererComponent& spriteRenderer = deserializedEntity.AddComponent<SpriteRendererComponent>();
					spriteRenderer.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					std::string path = spriteRendererComponent["TexturePath"].as<std::string>();
					spriteRenderer.Texture = nullptr;
					int x, y, comp;
					if (stbi_info(path.c_str(), &x, &y, &comp))
					{
						spriteRenderer.Texture = Texture2D::Create(path);
					}
					else if (!path.empty())
					{
						return false;
					}
					spriteRenderer.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
					spriteRenderer.ZIndex = spriteRendererComponent["ZIndex"].as<float>();
				}

				const YAML::Node& cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					CameraComponent& camera = deserializedEntity.AddComponent<CameraComponent>();
					
					auto& cameraProps = cameraComponent["Camera"];
					camera.Camera.SetOrthographic(
						cameraProps["OrthographicSize"].as<float>(),
						cameraProps["OrthographicNear"].as<float>(),
						cameraProps["OrthographicFar"].as<float>()
					);

					camera.Primary = cameraComponent["Primary"].as<bool>();
					camera.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}
			}
		}

		return true;
	}
	bool SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		PR_CORE_ASSERT(false, "Method not implemented yet!");
		return false;
	}
}