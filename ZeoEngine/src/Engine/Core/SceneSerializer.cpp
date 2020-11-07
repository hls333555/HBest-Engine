#include "ZEpch.h"
#include "Engine/Core/SceneSerializer.h"

#include <glm/glm.hpp>
#include <magic_enum.hpp>

#include "Engine/Renderer/Texture.h"
#include "Engine/GameFramework/Components.h"

namespace YAML {

	using namespace ZeoEngine;

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Ref<Texture2D>>
	{
		static Node encode(const Ref<Texture2D>& rhs)
		{
			Node node;
			node.push_back(rhs ? rhs->GetPath() : "");
			return node;
		}

		static bool decode(const Node& node, Ref<Texture2D>& rhs)
		{
			const auto& path = node.as<std::string>();
			if (path.empty()) return true;

			rhs = Texture2DLibrary::Get().GetOrLoad(path);
			return true;
		}
	};

}

namespace ZeoEngine {

	YAML::Emitter& operator<<(YAML::Emitter& out, const Entity& e)
	{
		out << e.GetEntityId();
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Ref<Texture2D>& texture)
	{
		out << (texture ? texture->GetPath() : "");
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene, SerializerType type)
		: m_Scene(scene)
		, m_SerializerType(type)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// Serialization /////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		YAML::Emitter out;

		auto typeName = magic_enum::enum_name(m_SerializerType).data();
		const std::string sceneName = m_Scene->GetName();
		ZE_CORE_TRACE("Serializing {0} '{1}'", typeName, sceneName);

		out << YAML::BeginMap;
		{
			out << YAML::Key << typeName << YAML::Value << sceneName;
			out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
			{
				m_Scene->m_Registry.view<CoreComponent>().each([&](auto entityId, auto& cc)
				{
					Entity entity = { entityId, m_Scene.get() };
					if (!entity) return;

					SerializeEntity(out, entity);
				});
			}
			out << YAML::EndSeq;
		}
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filePath)
	{
		// Not implemented
		ZE_CORE_ASSERT(false);
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, const Entity entity)
	{
		out << YAML::BeginMap;
		{
			out << YAML::Key << "Entity" << YAML::Value << entity; // TODO: Entity ID goes here
			out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;
			{
				m_Scene->m_Registry.visit(entity, [&](const auto typeId)
				{
					out << YAML::BeginMap;
					{
						const auto type = entt::resolve_type(typeId);
						out << YAML::Key << "Component" << YAML::Value << typeId; // TODO: Type ID goes here
						SerializeType(out, type, entity);
					}
					out << YAML::EndMap;
				});
			}
			out << YAML::EndSeq;
		}
		out << YAML::EndMap;
	}

	void SceneSerializer::SerializeType(YAML::Emitter& out, const entt::meta_type type, const Entity entity)
	{
		const auto instance = GetTypeInstance(type, m_Scene->m_Registry, entity);
		for (auto data : type.data())
		{
			// Do not serialize transient data
			auto bDiscardSerialize = DoesPropExist(PropertyType::Transient, data);
			if (bDiscardSerialize) continue;

			if (data.type().is_sequence_container())
			{
				EvaluateSerializeSequenceContainerData(out, data, instance);
			}
			else if (data.type().is_associative_container())
			{
				EvaluateSerializeAssociativeContainerData(out, data, instance);
			}
			else
			{
				EvaluateSerializeData(out, data, instance);
			}
		}
	}

	void SceneSerializer::EvaluateSerializeSequenceContainerData(YAML::Emitter& out, const entt::meta_data data, const entt::meta_any instance)
	{
		auto& seqView = data.get(instance).as_sequence_container();
		const auto dataName = GetMetaObjectDisplayName(data);
		out << YAML::Key << *dataName << YAML::Value;
		{
			out << YAML::Flow;
			out << YAML::BeginSeq;
			{
				for (auto it = seqView.begin(); it != seqView.end(); ++it)
				{
					auto element = *it;
					if (element.type().is_integral())
					{
						EvaluateSerializeIntegralData(out, data, element, true);
					}
					else if (element.type().is_floating_point())
					{
						EvaluateSerializeFloatingPointData(out, data, element, true);
					}
					else if (element.type().is_enum())
					{
						SerializeEnumData(out, data, element, true);
					}
					else
					{
						EvaluateSerializeOtherData(out, data, element, true);
					}
				}
			}
			out << YAML::EndSeq;
		}
	}

	void SceneSerializer::EvaluateSerializeAssociativeContainerData(YAML::Emitter& out, const entt::meta_data data, const entt::meta_any instance)
	{
		
	}

	void SceneSerializer::EvaluateSerializeData(YAML::Emitter& out, const entt::meta_data data, const entt::meta_any instance)
	{
		if (data.type().is_integral())
		{
			EvaluateSerializeIntegralData(out, data, instance, false);
		}
		else if (data.type().is_floating_point())
		{
			EvaluateSerializeFloatingPointData(out, data, instance, false);
		}
		else if (data.type().is_enum())
		{
			SerializeEnumData(out, data, instance, false);
		}
		else
		{
			EvaluateSerializeOtherData(out, data, instance, false);
		}
	}

	void SceneSerializer::EvaluateSerializeIntegralData(YAML::Emitter& out, const entt::meta_data data, const entt::meta_any instance, bool bIsSeqContainer)
	{
		const auto type = bIsSeqContainer ? instance.type() : data.type();
		if (IsTypeEqual<bool>(type))
		{
			SerializeData<bool>(out, data, instance, bIsSeqContainer);
		}
		else if (IsTypeEqual<int8_t>(type))
		{
			SerializeData<int8_t>(out, data, instance, bIsSeqContainer);
		}
		else if (IsTypeEqual<int32_t>(type))
		{
			SerializeData<int32_t>(out, data, instance, bIsSeqContainer);
		}
		else if (IsTypeEqual<int64_t>(type))
		{
			SerializeData<int64_t>(out, data, instance, bIsSeqContainer);
		}
		else if (IsTypeEqual<uint8_t>(type))
		{
			SerializeData<uint8_t>(out, data, instance, bIsSeqContainer);
		}
		else if (IsTypeEqual<uint32_t>(type))
		{
			SerializeData<uint32_t>(out, data, instance, bIsSeqContainer);
		}
		else if (IsTypeEqual<uint64_t>(type))
		{
			SerializeData<uint64_t>(out, data, instance, bIsSeqContainer);
		}
	}

	void SceneSerializer::EvaluateSerializeFloatingPointData(YAML::Emitter& out, const entt::meta_data data, const entt::meta_any instance, bool bIsSeqContainer)
	{
		const auto type = bIsSeqContainer ? instance.type() : data.type();
		if (IsTypeEqual<float>(type))
		{
			SerializeData<float>(out, data, instance, bIsSeqContainer);
		}
		else if (IsTypeEqual<double>(type))
		{
			SerializeData<double>(out, data, instance, bIsSeqContainer);
		}
	}

	void SceneSerializer::EvaluateSerializeOtherData(YAML::Emitter& out, const entt::meta_data data, const entt::meta_any instance, bool bIsSeqContainer)
	{
		const auto type = bIsSeqContainer ? instance.type() : data.type();
		if (IsTypeEqual<std::string>(type))
		{
			SerializeData<std::string>(out, data, instance, bIsSeqContainer);
			return;
		}
		else if (IsTypeEqual<glm::vec2>(type))
		{
			SerializeData<glm::vec2>(out, data, instance, bIsSeqContainer);
			return;
		}
		else if (IsTypeEqual<glm::vec3>(type))
		{
			SerializeData<glm::vec3>(out, data, instance, bIsSeqContainer);
			return;
		}
		else if (IsTypeEqual<glm::vec4>(type))
		{
			SerializeData<glm::vec4>(out, data, instance, bIsSeqContainer);
			return;
		}
		else if (IsTypeEqual<Ref<Texture2D>>(type))
		{
			SerializeData<Ref<Texture2D>>(out, data, instance, bIsSeqContainer);
			return;
		}

		auto dataName = GetMetaObjectDisplayName(data);
		ZE_CORE_ASSERT_INFO(false, "Failed to serialize data: '{0}'", *dataName);
	}

	void SceneSerializer::SerializeEnumData(YAML::Emitter& out, const entt::meta_data data, const entt::meta_any instance, bool bIsSeqContainer)
	{
		if (bIsSeqContainer)
		{
			const char* enumValueName = GetEnumDisplayName(instance);
			out << enumValueName;
		}
		else
		{
			const auto dataName = GetMetaObjectDisplayName(data);
			const auto enumValue = data.get(instance);
			const char* enumValueName = GetEnumDisplayName(enumValue);
			out << YAML::Key << *dataName << YAML::Value << enumValueName;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Deserialization ///////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	bool SceneSerializer::Deserialize(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		auto typeName = magic_enum::enum_name(m_SerializerType).data();
		if (!data[typeName])
		{
			ZE_CORE_ERROR("Failed to load {0}. Unknown {0} format!", typeName);
			return false;
		}

		std::string sceneName = data[typeName].as<std::string>();
		ZE_CORE_TRACE("Deserializing {0} '{1}'", typeName, sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				DeserializeEntity(entity);
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filePath)
	{
		// Not implemented
		ZE_CORE_ASSERT(false);
		return false;
	}

	void SceneSerializer::DeserializeEntity(const YAML::Node& entity)
	{
		//uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO: UUID

		// Create a default entity
		Entity deserializedEntity = m_Scene->CreateEntity();

		auto components = entity["Components"];
		if (components)
		{
			for (auto component : components)
			{
				auto typeId = component["Component"].as<uint32_t>();
				// TODO: NativeScriptComponent deserialization
				if (typeId == entt::type_info<NativeScriptComponent>().id()) continue;

				auto type = entt::resolve_type(typeId);
				auto bIsInherentType = DoesPropExist(PropertyType::InherentType, type);
				entt::meta_any instance;
				if (bIsInherentType)
				{
					// Get inherent type from that entity as it has already been added on entity creation
					instance = GetTypeInstance(type, m_Scene->m_Registry, deserializedEntity);
				}
				else
				{
					// Add type to that entity
					instance = deserializedEntity.AddTypeById(typeId, m_Scene->m_Registry);
				}

				// Iterate all datas and deserialize values
				for (auto data : type.data())
				{
					auto dataName = GetMetaObjectDisplayName(data);
					const auto& value = component[*dataName];
					// Evaluate serialized data only
					if (value)
					{
						if (data.type().is_sequence_container())
						{
							EvaluateDeserializeSequenceContainerData(data, instance, value);
						}
						else if (data.type().is_associative_container())
						{
							EvaluateDeserializeAssociativeContainerData(data, instance, value);
						}
						else
						{
							EvaluateDeserializeData(data, instance, value);
						}
					}
				}
			}
		}
	}

	void SceneSerializer::EvaluateDeserializeSequenceContainerData(entt::meta_data data, entt::meta_any instance, const YAML::Node& value)
	{
		const auto type = data.get(instance).as_sequence_container().value_type();
		for (const auto& element : value)
		{
			if (type.is_integral())
			{
				EvaluateDeserializeIntegralData(data, instance, element, true);
			}
			else if (type.is_floating_point())
			{
				EvaluateDeserializeFloatingPointData(data, instance, element, true);
			}
			else if (type.is_enum())
			{
				DeserializeEnumData(data, instance, element, true);
			}
			else
			{
				EvaluateDeserializeOtherData(data, instance, element, true);
			}
		}
	}

	void SceneSerializer::EvaluateDeserializeAssociativeContainerData(entt::meta_data data, entt::meta_any instance, const YAML::Node& value)
	{
		
	}

	void SceneSerializer::EvaluateDeserializeData(entt::meta_data data, entt::meta_any instance, const YAML::Node& value)
	{
		if (data.type().is_integral())
		{
			EvaluateDeserializeIntegralData(data, instance, value, false);
		}
		else if (data.type().is_floating_point())
		{
			EvaluateDeserializeFloatingPointData(data, instance, value, false);
		}
		else if (data.type().is_enum())
		{
			DeserializeEnumData(data, instance, value, false);
		}
		else
		{
			EvaluateDeserializeOtherData(data, instance, value, false);
		}
	}

	void SceneSerializer::EvaluateDeserializeIntegralData(entt::meta_data data, entt::meta_any instance, const YAML::Node& value, bool bIsSeqContainer)
	{
		const auto type = bIsSeqContainer ? data.get(instance).as_sequence_container().value_type() : data.type();
		if (IsTypeEqual<bool>(type))
		{
			DeserializeData<bool>(data, instance, value, bIsSeqContainer);
		}
		else if (IsTypeEqual<int8_t>(type))
		{
			DeserializeData<int8_t>(data, instance, value, bIsSeqContainer);
		}
		else if (IsTypeEqual<int32_t>(type))
		{
			DeserializeData<int32_t>(data, instance, value, bIsSeqContainer);
		}
		else if (IsTypeEqual<int64_t>(type))
		{
			DeserializeData<int64_t>(data, instance, value, bIsSeqContainer);
		}
		else if (IsTypeEqual<uint8_t>(type))
		{
			DeserializeData<uint8_t>(data, instance, value, bIsSeqContainer);
		}
		else if (IsTypeEqual<uint32_t>(type))
		{
			DeserializeData<uint32_t>(data, instance, value, bIsSeqContainer);
		}
		else if (IsTypeEqual<uint64_t>(type))
		{
			DeserializeData<uint64_t>(data, instance, value, bIsSeqContainer);
		}
	}

	void SceneSerializer::EvaluateDeserializeFloatingPointData(entt::meta_data data, entt::meta_any instance, const YAML::Node& value, bool bIsSeqContainer)
	{
		const auto type = bIsSeqContainer ? data.get(instance).as_sequence_container().value_type() : data.type();
		if (IsTypeEqual<float>(type))
		{
			DeserializeData<float>(data, instance, value, bIsSeqContainer);
		}
		else if (IsTypeEqual<double>(type))
		{
			DeserializeData<double>(data, instance, value, bIsSeqContainer);
		}
	}

	void SceneSerializer::EvaluateDeserializeOtherData(entt::meta_data data, entt::meta_any instance, const YAML::Node& value, bool bIsSeqContainer)
	{
		const auto type = bIsSeqContainer ? data.get(instance).as_sequence_container().value_type() : data.type();
		if (IsTypeEqual<std::string>(type))
		{
			DeserializeData<std::string>(data, instance, value, bIsSeqContainer);
			return;
		}
		else if (IsTypeEqual<glm::vec2>(type))
		{
			DeserializeData<glm::vec2>(data, instance, value, bIsSeqContainer);
			return;
		}
		else if (IsTypeEqual<glm::vec3>(type))
		{
			DeserializeData<glm::vec3>(data, instance, value, bIsSeqContainer);
			return;
		}
		else if (IsTypeEqual<glm::vec4>(type))
		{
			DeserializeData<glm::vec4>(data, instance, value, bIsSeqContainer);
			return;
		}
		else if (IsTypeEqual<Ref<Texture2D>>(type))
		{
			DeserializeData<Ref<Texture2D>>(data, instance, value, bIsSeqContainer);
			return;
		}

		auto dataName = GetMetaObjectDisplayName(data);
		ZE_CORE_ASSERT_INFO(false, "Failed to deserialize data: '{0}'", *dataName);
	}

	void SceneSerializer::DeserializeEnumData(entt::meta_data data, entt::meta_any instance, const YAML::Node& value, bool bIsSeqContainer)
	{
		auto currentValueName = value.as<std::string>();
		const auto& datas = bIsSeqContainer ? data.get(instance).as_sequence_container().value_type().data() : data.type().data();
		for (auto enumData : datas)
		{
			auto valueName = GetMetaObjectDisplayName(enumData);
			if (currentValueName == valueName)
			{
				auto newValue = enumData.get({});
				if (bIsSeqContainer)
				{
					auto& seqView = data.get(instance).as_sequence_container();
					seqView.insert(seqView.end(), newValue);
				}
				else
				{
					SetDataValue(data, instance, newValue);
				}
			}
		}
	}

}
