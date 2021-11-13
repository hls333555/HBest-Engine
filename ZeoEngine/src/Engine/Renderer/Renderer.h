#pragma once

#include "Engine/Renderer/EditorCamera.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Mesh.h"

namespace ZeoEngine {

	struct MeshRendererComponent;

	struct RenderData
	{
		Ref<VertexArray> VAO;
		std::vector<MeshEntry>* MeshEntries;
		struct ModelData
		{
			glm::mat4 Transform;
			glm::mat4 NormalMatrix;

			// Editor-only
			int32_t EntityID;
		};
		ModelData ModelBuffer;
		Ref<UniformBuffer> ModelUniformBuffer;

		RenderData(const Ref<VertexArray>& vao, std::vector<MeshEntry>* meshEntries, const glm::mat4& transform, int32_t entityID)
			: VAO(vao), MeshEntries(meshEntries)
		{
			ModelBuffer.Transform = transform;
			ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
			ModelBuffer.EntityID = entityID;
		}

		void Bind()
		{
			ModelUniformBuffer = UniformBuffer::Create(sizeof(ModelData), 1);
			ModelUniformBuffer->SetData(&ModelBuffer, sizeof(ModelData));
		}

	};

	struct Statistics
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;

		Entity HoveredEntity;

		uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
		uint32_t GetTotalIndexCount() const { return QuadCount * 6; }

		void Reset()
		{
			DrawCalls = 0;
			QuadCount = 0;
			HoveredEntity = {};
		}
	};

	struct RendererData
	{
		Ref<Shader> DefaultShader;
		std::vector<RenderData> RenderQueue;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;

		// Data alignment is required for uniform buffers!
		struct LightData
		{
			glm::vec4 DirectionalLightColor;
			glm::vec3 DirectionalLightDirection;
			float DirectionalLightIntensity;

			void Reset()
			{
				DirectionalLightColor = glm::vec4{ 0.0f };
				DirectionalLightDirection = glm::vec3{ 0.0f };
				DirectionalLightIntensity = 0.0f;
			}
		};
		LightData LightBuffer;
		Ref<UniformBuffer> LightUniformBuffer;

		Statistics Stats;
	};

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void Prepare();
		static void Submit();

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void SetupDirectionalLight(const glm::vec3& rotation, const Ref<DirectionalLight>& directionalLight);

		static void DrawMesh(const glm::mat4& transform, const MeshRendererComponent& meshComp, int32_t entityID);

		// TODO:
		static Statistics& GetStats();
		static void ResetStats();

	private:
		static RendererData s_Data;
	};

}
