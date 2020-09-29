#include "Engine/GameFramework/Components.h"
#include "Reflection/ReflectionHelper.h"

ZE_REFL_REGISTRATION
{
	using namespace ZeoEngine;

	ZE_REFL_TYPE(TagComponent, "��ǩ���")
		ZE_REFL_DATA_REF(TagComponent, Tag, "");

	ZE_REFL_TYPE(TransformComponent, "�任���")
		ZE_REFL_DATA_REF(TransformComponent, Transform, "");

	ZE_REFL_TYPE(SpriteRendererComponent, "2D��Ⱦ���")
		ZE_REFL_DATA_REF(SpriteRendererComponent, TintColor, "")
		ZE_REFL_DATA_REF(SpriteRendererComponent, Texture, "")
		ZE_REFL_DATA_REF(SpriteRendererComponent, TextureTiling, "");

	ZE_REFL_TYPE(CameraComponent, "������")
		ZE_REFL_DATA_REF(CameraComponent, bIsPrimary, "")
		ZE_REFL_DATA_REF(CameraComponent, bFixedAspectRatio, "");

	ZE_REFL_TYPE(NativeScriptComponent, "C++�ű����");

}
