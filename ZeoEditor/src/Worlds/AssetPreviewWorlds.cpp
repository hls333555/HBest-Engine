#include "Worlds/AssetPreviewWorlds.h"

#include "Core/Editor.h"
#include "Engine/GameFramework/Components.h"
#include "Engine/Asset/AssetLibrary.h"
#include "Inspectors/MaterialInspector.h"
#include "SceneRenderers/AssetPreviewSceneRenderers.h"
#include "Panels/AssetViews.h"
#include "Scenes/ParticlePreviewScene.h"

namespace ZeoEngine {

	AssetPreviewWorldBase::AssetPreviewWorldBase(std::string worldName)
		: EditorPreviewWorldBase(std::move(worldName))
	{
	}

	AssetPreviewWorldBase::~AssetPreviewWorldBase() = default;

	void AssetPreviewWorldBase::PostSceneCreate(const Ref<Scene>& scene)
	{
		const Entity entity = CreatePreviewEntity(scene);
		SetContextEntity(entity);
	}

	void AssetPreviewWorldBase::OnAttach()
	{
		EditorPreviewWorldBase::OnAttach();

		m_AssetView = CreateAssetView();
	}

	void AssetPreviewWorldBase::SaveCurrentAsset() const
	{
		const AssetHandle handle = GetAsset()->GetHandle();
		const auto metadata = AssetRegistry::Get().GetAssetMetadata(handle);
		const auto& assetPath = metadata->Path;
		SaveAsset(assetPath);
	}

	void AssetPreviewWorldBase::PostAssetLoad()
	{
		GetContextEntity().UpdateBounds();
		FocusContextEntity(true);
	}

	Ref<Scene> AssetPreviewWorldBase::CreateScene()
	{
		return CreateRef<Scene>();
	}

#pragma region MaterialPreviewWorld
	void MaterialPreviewWorld::LoadAsset(const std::string& path)
	{
		GetContextEntity().PatchComponent<MaterialDetailComponent>([&path, this](auto& materialPreviewComp)
		{
			SetMaterialAsset(materialPreviewComp, AssetLibrary::LoadAsset<Material>(path));
		});
	}

	void MaterialPreviewWorld::OnAssetSaveAs(const std::string& path)
	{
		GetContextEntity().PatchComponent<MaterialDetailComponent>([&path, this](auto& materialPreviewComp)
		{
			SetMaterialAsset(materialPreviewComp, AssetLibrary::LoadAsset<Material>(path, AssetLibrary::DeserializeMode::Force));
		});
	}

	Ref<SceneRenderer> MaterialPreviewWorld::CreateSceneRenderer()
	{
		return CreateRef<MaterialPreviewSceneRenderer>();
	}

	Entity MaterialPreviewWorld::CreatePreviewEntity(const Ref<Scene>& scene)
	{
		Entity previewMaterialEntity = scene->CreateEntity("Preview Material");
		previewMaterialEntity.AddComponent<MaterialDetailComponent>();
		previewMaterialEntity.AddComponent<MeshRendererComponent>(Mesh::GetDefaultSphereMesh()->GetHandle());
		previewMaterialEntity.AddComponent<DirectionalLightComponent>();
		return previewMaterialEntity;
	}

	Scope<InspectorBase> MaterialPreviewWorld::CreateInspector()
	{
		return CreateScope<MaterialInspector>(SharedFromBase<MaterialPreviewWorld>(), entt::type_hash<MaterialDetailComponent>::value());
	}

	Scope<AssetView> MaterialPreviewWorld::CreateAssetView()
	{
		return CreateScope<AssetView>();
	}

	void MaterialPreviewWorld::SetMaterialAsset(MaterialDetailComponent& materialPreviewComp, Ref<Material> material)
	{
		materialPreviewComp.LoadedMaterial = material;
		const auto& meshComp = GetContextEntity().GetComponent<MeshRendererComponent>();
		meshComp.Instance->SetMaterial(0, materialPreviewComp.LoadedMaterial->GetHandle());
		SetAsset(std::move(material));
	}
#pragma endregion

#pragma region MeshPreviewWorld
	MeshPreviewWorld::MeshPreviewWorld(std::string worldName)
		: AssetPreviewWorldBase(std::move(worldName))
	{
		GetEditorCamera().SetFarClip(10000.0f);
	}

	void MeshPreviewWorld::LoadAsset(const std::string& path)
	{
		GetContextEntity().PatchComponent<MeshDetailComponent>([&path, this](auto& meshPreviewComp)
		{
			SetMeshAsset(meshPreviewComp, AssetLibrary::LoadAsset<Mesh>(path));
		});
	}

	void MeshPreviewWorld::OnAssetSaveAs(const std::string& path)
	{
		GetContextEntity().PatchComponent<MeshDetailComponent>([&path, this](auto& meshPreviewComp)
		{
			SetMeshAsset(meshPreviewComp, AssetLibrary::LoadAsset<Mesh>(path, AssetLibrary::DeserializeMode::Force));
		});
	}

	Ref<SceneRenderer> MeshPreviewWorld::CreateSceneRenderer()
	{
		return CreateRef<MeshPreviewSceneRenderer>();
	}

	Entity MeshPreviewWorld::CreatePreviewEntity(const Ref<Scene>& scene)
	{
		Entity previewMeshEntity = scene->CreateEntity("Preview Mesh");
		previewMeshEntity.AddComponent<MeshDetailComponent>();
		previewMeshEntity.AddComponent<DirectionalLightComponent>();
		return previewMeshEntity;
	}

	Scope<InspectorBase> MeshPreviewWorld::CreateInspector()
	{
		return CreateScope<AssetInspector>(SharedFromBase<MaterialPreviewWorld>(), entt::type_hash<MeshDetailComponent>::value());
	}

	Scope<AssetView> MeshPreviewWorld::CreateAssetView()
	{
		return CreateScope<AssetView>();
	}

	void MeshPreviewWorld::SetMeshAsset(MeshDetailComponent& meshPreviewComp, Ref<Mesh> mesh)
	{
		meshPreviewComp.LoadedMesh = mesh;
		meshPreviewComp.Instance = meshPreviewComp.LoadedMesh->CreateInstance(GetContextEntity().GetScene());
		meshPreviewComp.Instance->SubmitAllTechniques();
		SetAsset(std::move(mesh));
	}
#pragma endregion

#pragma region TexturePreviewWorld
	void TexturePreviewWorld::LoadAsset(const std::string& path)
	{
		GetContextEntity().PatchComponent<TextureDetailComponent>([&path, this](auto& texturePreviewComp)
		{
			SetTextureAsset(texturePreviewComp, AssetLibrary::LoadAsset<Texture2D>(path));
		});
	}

	void TexturePreviewWorld::OnAssetSaveAs(const std::string& path)
	{
		GetContextEntity().PatchComponent<TextureDetailComponent>([&path, this](auto& texturePreviewComp)
		{
			SetTextureAsset(texturePreviewComp, AssetLibrary::LoadAsset<Texture2D>(path, AssetLibrary::DeserializeMode::Force));
		});
	}

	Ref<SceneRenderer> TexturePreviewWorld::CreateSceneRenderer()
	{
		return nullptr;
	}

	Entity TexturePreviewWorld::CreatePreviewEntity(const Ref<Scene>& scene)
	{
		Entity previewTextureEntity = scene->CreateEntity("Preview Texture");
		previewTextureEntity.AddComponent<TextureDetailComponent>();
		return previewTextureEntity;
	}

	Scope<InspectorBase> TexturePreviewWorld::CreateInspector()
	{
		return CreateScope<AssetInspector>(SharedFromBase<TexturePreviewWorld>(), entt::type_hash<TextureDetailComponent>::value());
	}

	Scope<AssetView> TexturePreviewWorld::CreateAssetView()
	{
		return CreateScope<TextureAssetView>(SharedFromBase<TexturePreviewWorld>());
	}

	void TexturePreviewWorld::SetTextureAsset(TextureDetailComponent& texturePreviewComp, Ref<Texture2D> texture)
	{
		texturePreviewComp.LoadedTexture = texture;
		SetAsset(std::move(texture));
	}
#pragma endregion

#pragma region ParticlePreviewWorld
	void ParticlePreviewWorld::LoadAsset(const std::string& path)
	{
		GetContextEntity().PatchComponent<ParticleSystemDetailComponent>([&path, this](auto& particlePreviewComp)
		{
			SetParticleAsset(particlePreviewComp, AssetLibrary::LoadAsset<ParticleTemplate>(path));
		});
	}

	void ParticlePreviewWorld::OnAssetSaveAs(const std::string& path)
	{
		GetContextEntity().PatchComponent<ParticleSystemDetailComponent>([&path, this](auto& particlePreviewComp)
		{
			SetParticleAsset(particlePreviewComp, AssetLibrary::LoadAsset<ParticleTemplate>(path, AssetLibrary::DeserializeMode::Force));
		});
	}

	Ref<Scene> ParticlePreviewWorld::CreateScene()
	{
		return CreateRef<ParticlePreviewScene>();
	}

	Ref<SceneRenderer> ParticlePreviewWorld::CreateSceneRenderer()
	{
		return CreateRef<ParticlePreviewSceneRenderer>();
	}

	Entity ParticlePreviewWorld::CreatePreviewEntity(const Ref<Scene>& scene)
	{
		Entity previewParticleEntity = scene->CreateEntity("Preview Particle");
		auto& particlePreviewComp = previewParticleEntity.AddComponent<ParticleSystemDetailComponent>();
		// TODO:
		// For loading phase, instance will be created in ParticleScene::PostLoad
		ParticleSystemInstance::Create(particlePreviewComp);
		return previewParticleEntity;
	}

	Scope<InspectorBase> ParticlePreviewWorld::CreateInspector()
	{
		return CreateScope<AssetInspector>(SharedFromBase<ParticlePreviewWorld>(), entt::type_hash<ParticleSystemDetailComponent>::value());
	}

	Scope<AssetView> ParticlePreviewWorld::CreateAssetView()
	{
		return CreateScope<AssetView>();
	}

	void ParticlePreviewWorld::SetParticleAsset(ParticleSystemDetailComponent& particlePreviewComp, Ref<ParticleTemplate> particle)
	{
		particlePreviewComp.ParticleTemplateAsset = particle;
		SetAsset(std::move(particle));
	}
#pragma endregion

}
