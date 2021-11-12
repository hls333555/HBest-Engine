#include "Engine/GameFramework/Components.h"
#include "Engine/Core/ReflectionCore.h"

namespace ZeoEngine {

	ZE_REGISTRATION
	{
		ZCOMPONENT(CoreComponent, ZPROP(Inherent), ZPROP(HideComponentHeader))
			ZDATA(CoreComponent, Name);

		ZCOMPONENT(IDComponent, ZPROP(Inherent), ZPROP(Transient), ZPROP(HideComponentHeader));

		ZCOMPONENT(TransformComponent, ZPROP(DisplayName, ZTEXT("Transform")), ZPROP(Tooltip, ZTEXT("�任���")), ZPROP(Inherent))
			ZDATA(TransformComponent, Translation, ZPROP(Tooltip, ZTEXT("ƽ��")), ZPROP(DragSensitivity, 0.1f))
			ZDATA_SETTER_GETTER(TransformComponent, Rotation, SetRotationToRadians, GetRotationAsDegrees, ZPROP(ClampMin, -360.0f), ZPROP(ClampMax, 360.0f), ZPROP(ClampOnlyDuringDragging), ZPROP(Tooltip, ZTEXT("��ת")))
			ZDATA(TransformComponent, Scale, ZPROP(Tooltip, ZTEXT("����")), ZPROP(DragSensitivity, 0.1f));

		ZCOMPONENT(SpriteRendererComponent, ZPROP(DisplayName, ZTEXT("Sprite Renderer")), ZPROP(Tooltip, ZTEXT("������Ⱦ���")), ZPROP(Category, ZTEXT("Rendering")))
			ZDATA(SpriteRendererComponent, TintColor)
			ZDATA(SpriteRendererComponent, Texture)
			ZDATA(SpriteRendererComponent, TextureTiling)
			ZDATA(SpriteRendererComponent, SortingOrder);

		ZCOMPONENT(CircleRendererComponent, ZPROP(DisplayName, ZTEXT("Circle Renderer")), ZPROP(Tooltip, ZTEXT("Բ����Ⱦ���")), ZPROP(Category, ZTEXT("Rendering")))
			ZDATA(CircleRendererComponent, Color)
			ZDATA(CircleRendererComponent, Thickness, ZPROP(DragSensitivity, 0.025f), ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f))
			ZDATA(CircleRendererComponent, Fade, ZPROP(DragSensitivity, 0.00025f), ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f))
			ZDATA(CircleRendererComponent, SortingOrder);

		ZENUM(SceneCamera::ProjectionType)
			ZENUM_DATA(SceneCamera::ProjectionType, Perspective, ZPROP(Tooltip, ZTEXT("͸�����")))
			ZENUM_DATA(SceneCamera::ProjectionType, Orthographic, ZPROP(Tooltip, ZTEXT("�������")));

		ZCOMPONENT(CameraComponent, ZPROP(DisplayName, ZTEXT("Camera")), ZPROP(Tooltip, ZTEXT("������")), ZPROP(Category, ZTEXT("Rendering")))
			ZDATA(CameraComponent, bIsPrimary)
			ZDATA(CameraComponent, bFixedAspectRatio)
			ZDATA_SETTER_GETTER(CameraComponent, ProjectionType, SetProjectionType, GetProjectionType)
			ZDATA_SETTER_GETTER(CameraComponent, VerticalFOV, SetPerspectiveVerticalFOV, GetPerspectiveVerticalFOV, ZPROP(Category, ZTEXT("Perspective")), ZPROP(HideCondition, "ProjectionType != Perspective"))
			ZDATA_SETTER_GETTER(CameraComponent, PerspectiveNear, SetPerspectiveNearClip, GetPerspectiveNearClip, ZPROP(Category, ZTEXT("Perspective")), ZPROP(HideCondition, "ProjectionType != Perspective"))
			ZDATA_SETTER_GETTER(CameraComponent, PerspectiveFar, SetPerspectiveFarClip, GetPerspectiveFarClip, ZPROP(Category, ZTEXT("Perspective")), ZPROP(HideCondition, "ProjectionType != Perspective"))
			ZDATA_SETTER_GETTER(CameraComponent, Size, SetOrthographicSize, GetOrthographicSize, ZPROP(Category, ZTEXT("Orthographic")), ZPROP(HideCondition, "ProjectionType != Orthographic"))
			ZDATA_SETTER_GETTER(CameraComponent, OrthographicNear, SetOrthographicNearClip, GetOrthographicNearClip, ZPROP(Category, ZTEXT("Orthographic")), ZPROP(HideCondition, "ProjectionType != Orthographic"))
			ZDATA_SETTER_GETTER(CameraComponent, OrthographicFar, SetOrthographicFarClip, GetOrthographicFarClip, ZPROP(Category, ZTEXT("Orthographic")), ZPROP(HideCondition, "ProjectionType != Orthographic"));

		ZCOMPONENT(NativeScriptComponent, ZPROP(DisplayName, ZTEXT("Native Script")), ZPROP(Tooltip, ZTEXT("C++�ű����")), ZPROP(Category, ZTEXT("Scripts")));

		ZCOMPONENT(ParticleSystemComponent, ZPROP(DisplayName, ZTEXT("Particle System")), ZPROP(Tooltip, ZTEXT("����ϵͳ���")), ZPROP(Category, ZTEXT("Effects")))
			ZDATA(ParticleSystemComponent, Template)
			ZDATA(ParticleSystemComponent, PositionOffset);

		ZENUM(ParticleVariationType)
			ZENUM_DATA(ParticleVariationType, Constant)
			ZENUM_DATA(ParticleVariationType, RandomInRange)
			ZENUM_DATA(ParticleVariationType, UniformInRange);

#define ZSTRUCT_PARTICLE_VARIATION_TYPE(_type)						\
		ZSTRUCT(_type)												\
			ZDATA(_type, VariationType)								\
			ZDATA(_type, Val1, ZPROP(DisplayName, ZTEXT("Value")))	\
			ZDATA(_type, Val2, ZPROP(DisplayName, ZTEXT("ValueHigh")), ZPROP(HideCondition, "VariationType == Constant"))

		ZSTRUCT_PARTICLE_VARIATION_TYPE(ParticleInt);
		ZSTRUCT_PARTICLE_VARIATION_TYPE(ParticleFloat);
		ZSTRUCT_PARTICLE_VARIATION_TYPE(ParticleVec2);
		ZSTRUCT_PARTICLE_VARIATION_TYPE(ParticleVec3);
		ZSTRUCT_PARTICLE_VARIATION_TYPE(ParticleColor);

		ZSTRUCT(BurstData)
			ZDATA(BurstData, Time, ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f), ZPROP(DragSensitivity, 0.01f), ZPROP(Tooltip, ZTEXT("��׼����ʱ��㣺[0,1]")))
			ZDATA(BurstData, Amount, ZPROP(Tooltip, ZTEXT("�ڸ�ʱ���һ�������ɵ�������")));

		ZCOMPONENT(ParticleSystemPreviewComponent, ZPROP(Inherent), ZPROP(HideComponentHeader))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, IsLocalSpace, SetLocalSpace, IsLocalSpace, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("�Ƿ��ھֲ��ռ�ģ������")))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, LoopCount, SetLoopCount, GetLoopCount, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("ѭ����������С�ڵ���0, ��Ϊ����ѭ��")))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, LoopDuration, SetLoopDuration, GetLoopDuration, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("ÿ��ѭ����ʱ��")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, SpawnRate, GetSpawnRate, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("ÿ���ܹ����ɵ����������ñ����������ӵ������ٶȡ���С�ڵ���0��������")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, BurstList, GetBurstList, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("ÿ��ʱ���һ�������ɵ��������б�")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, InitialPosition, GetInitialPosition, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("���ӵĳ�ʼλ��")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, InitialRotation, GetInitialRotation, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("���ӵĳ�ʼ��ת")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, RotationRate, GetRotationRate, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("���ӵ���ת�ٶ�")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, SizeBegin, GetSizeBegin, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("���ӵĳ�ʼ��С")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, SizeEnd, GetSizeEnd, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("���ӵ����մ�С")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, InitialVelocity, GetInitialVelocity, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("���ӵĳ�ʼ�ٶ�")))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, InheritVelocityRatio, SetInheritVelocityRatio, GetInheritVelocityRatio, ZPROP(DisplayName, ZTEXT("InheritVelocity")), ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("�����������ܴӷ������̳е��ٶȱ��ʡ��ñ���ֻ���ڷǾֲ��ռ���Ч")), ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, ColorBegin, GetColorBegin, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("���ӵĳ�ʼ��ɫ")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, ColorEnd, GetColorEnd, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("���ӵ�������ɫ")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, Lifetime, GetLifetime, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("���ӵ���������")))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, Texture, SetTexture, GetTexture, ZPROP(Category, ZTEXT("Renderer")), ZPROP(Tooltip, ZTEXT("���ӵĲ�����ͼ")))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, SubImageSize, SetSubImageSize, GetSubImageSize, ZPROP(Category, ZTEXT("Renderer")), ZPROP(Tooltip, ZTEXT("������ηָ���ͼ������UV������xΪ������yΪ����")), ZPROP(ClampMin, 0.0f))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, MaxParticles, SetMaxParticles, GetMaxParticles, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("������ɵ�������")), ZPROP(ClampMin, 0));

		ZENUM(Rigidbody2DComponent::BodyType)
			ZENUM_DATA(Rigidbody2DComponent::BodyType, Static, ZPROP(Tooltip, ZTEXT("��̬")))
			ZENUM_DATA(Rigidbody2DComponent::BodyType, Dynamic, ZPROP(Tooltip, ZTEXT("��̬")))
			ZENUM_DATA(Rigidbody2DComponent::BodyType, Kinematic, ZPROP(Tooltip, ZTEXT("�˶�ѧ")));

		ZCOMPONENT(Rigidbody2DComponent, ZPROP(DisplayName, ZTEXT("Rigidbody 2D")), ZPROP(Tooltip, ZTEXT("2D�������")), ZPROP(Category, ZTEXT("Physics")))
			ZDATA(Rigidbody2DComponent, Type, ZPROP(Tooltip, ZTEXT("��������")))
			ZDATA(Rigidbody2DComponent, bFixedRotation);

		ZCOMPONENT(BoxCollider2DComponent, ZPROP(DisplayName, ZTEXT("Box Collider 2D")), ZPROP(Tooltip, ZTEXT("2D��ײ�����")), ZPROP(Category, ZTEXT("Physics")))
			ZDATA(BoxCollider2DComponent, Offset)
			ZDATA(BoxCollider2DComponent, Size, ZPROP(Tooltip, ZTEXT("��ײ��С����ֵΪ���ű���")))
			ZDATA(BoxCollider2DComponent, Density, ZPROP(Tooltip, ZTEXT("�ܶ�")), ZPROP(Category, ZTEXT("Physics Material")), ZPROP(DragSensitivity, 0.01f), ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f))
			ZDATA(BoxCollider2DComponent, Friction, ZPROP(Tooltip, ZTEXT("Ħ����")), ZPROP(Category, ZTEXT("Physics Material")), ZPROP(DragSensitivity, 0.01f), ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f))
			ZDATA(BoxCollider2DComponent, Restitution, ZPROP(Tooltip, ZTEXT("����")), ZPROP(Category, ZTEXT("Physics Material")), ZPROP(DragSensitivity, 0.01f), ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f))
			ZDATA(BoxCollider2DComponent, RestitutionThreshold, ZPROP(Tooltip, ZTEXT("������ֵ���ٶȸ��ڸ�ֵʱ����ײ���ᷴ��")), ZPROP(Category, ZTEXT("Physics Material")), ZPROP(DragSensitivity, 0.01f), ZPROP(ClampMin, 0.0f));

		ZCOMPONENT(CircleCollider2DComponent, ZPROP(DisplayName, ZTEXT("Circle Collider 2D")), ZPROP(Tooltip, ZTEXT("2D��ײԲ���")), ZPROP(Category, ZTEXT("Physics")))
			ZDATA(CircleCollider2DComponent, Offset)
			ZDATA(CircleCollider2DComponent, Radius, ZPROP(Tooltip, ZTEXT("��ײ�뾶")))
			ZDATA(CircleCollider2DComponent, Density, ZPROP(Tooltip, ZTEXT("�ܶ�")), ZPROP(Category, ZTEXT("Physics Material")), ZPROP(DragSensitivity, 0.01f), ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f))
			ZDATA(CircleCollider2DComponent, Friction, ZPROP(Tooltip, ZTEXT("Ħ����")), ZPROP(Category, ZTEXT("Physics Material")), ZPROP(DragSensitivity, 0.01f), ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f))
			ZDATA(CircleCollider2DComponent, Restitution, ZPROP(Tooltip, ZTEXT("����")), ZPROP(Category, ZTEXT("Physics Material")), ZPROP(DragSensitivity, 0.01f), ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f))
			ZDATA(CircleCollider2DComponent, RestitutionThreshold, ZPROP(Tooltip, ZTEXT("������ֵ���ٶȸ��ڸ�ֵʱ����ײ���ᷴ��")), ZPROP(Category, ZTEXT("Physics Material")), ZPROP(DragSensitivity, 0.01f), ZPROP(ClampMin, 0.0f));
	}

}
