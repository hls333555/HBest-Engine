#include "Editors/MaterialEditor.h"

#include "Engine/GameFramework/Components.h"
#include "EditorUIRenderers/MaterialEditorUIRenderer.h"
#include "Scenes/MaterialEditorScene.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Material.h"

namespace ZeoEngine {

	void MaterialEditor::OnAttach()
	{
		m_PostSceneCreate.connect<&MaterialEditor::CreatePreviewMaterial>(this);

		EditorBase::OnAttach();
	}

	Ref<EditorUIRendererBase> MaterialEditor::CreateEditorUIRenderer()
	{
		return CreateRef<MaterialEditorUIRenderer>(SharedFromBase<MaterialEditor>());
	}

	Ref<Scene> MaterialEditor::CreateScene()
	{
		return CreateRef<MaterialEditorScene>();
	}

	AssetTypeId MaterialEditor::GetAssetTypeId() const
	{
		return MaterialAsset::TypeId();
	}

	AssetHandle<IAsset> MaterialEditor::GetAsset() const
	{
		return GetContextEntity().GetComponent<MaterialPreviewComponent>().Template;
	}

	void MaterialEditor::LoadAsset(const std::string& path)
	{
		GetContextEntity().PatchComponent<MaterialPreviewComponent>([&path](auto& materialPreviewComp)
		{
			materialPreviewComp.Template = MaterialAssetLibrary::Get().LoadAsset(path);
		});
	}

	void MaterialEditor::SaveAsset(const std::string& path)
	{
		auto& materialPreviewComp = GetContextEntity().GetComponent<MaterialPreviewComponent>();
		materialPreviewComp.Template->Serialize(path);
	}

	void MaterialEditor::CreatePreviewMaterial(bool bIsFromLoad)
	{
		Entity previewMaterialEntity = GetScene()->CreateEntity("Preview Material");
		auto& materialPreviewComp = previewMaterialEntity.AddComponent<MaterialPreviewComponent>();
		auto& meshComp = previewMaterialEntity.AddComponent<MeshRendererComponent>();
		auto& lightComp = previewMaterialEntity.AddComponent<LightComponent>(LightComponent::LightType::DirectionalLight);
		meshComp.Mesh = MeshAssetLibrary::GetDefaultSphereMesh();
		SetContextEntity(previewMaterialEntity);
	}

}