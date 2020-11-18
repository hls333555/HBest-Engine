#include "Engine/GameFramework/Components.h"
#include "Engine/Core/ReflectionHelper.h"

ZE_REFL_REGISTRATION
{
using namespace ZeoEngine;

#if ENABLE_TEST
	ZE_REFL_ENUM(TestComponent::TestEnum)
		ZE_REFL_ENUM_DATA(TestComponent::TestEnum, TestEnum1)
		ZE_REFL_ENUM_DATA(TestComponent::TestEnum, TestEnum2)
		ZE_REFL_ENUM_DATA(TestComponent::TestEnum, TestEnum3);

	ZE_REFL_ENUM(TestComponent::TestEnumClass)
		ZE_REFL_ENUM_DATA(TestComponent::TestEnumClass, TestEnumClass1)
		ZE_REFL_ENUM_DATA(TestComponent::TestEnumClass, TestEnumClass2)
		ZE_REFL_ENUM_DATA(TestComponent::TestEnumClass, TestEnumClass3);

	ZE_REFL_TYPE(TestComponent, ZE_REFL_PROP_PAIR(DisplayName, ZE_TEXT("Test")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("�������")), ZE_REFL_PROP(InherentType))
		ZE_REFL_DATA_REF(TestComponent, BoolVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")))
		ZE_REFL_DATA_REF(TestComponent, Ui8Var, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")), ZE_REFL_PROP_PAIR(ClampMin, 1))
		ZE_REFL_DATA_REF(TestComponent, Ui32Var, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")), ZE_REFL_PROP_PAIR(ClampMin, 1))
		ZE_REFL_DATA_REF(TestComponent, Ui64Var, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")), ZE_REFL_PROP_PAIR(ClampMin, 1))
		ZE_REFL_DATA_REF(TestComponent, I8Var, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")), ZE_REFL_PROP_PAIR(ClampMin, -1))
		ZE_REFL_DATA_REF(TestComponent, I32Var, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")), ZE_REFL_PROP_PAIR(ClampMin, -1))
		ZE_REFL_DATA_REF(TestComponent, I64Var, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")), ZE_REFL_PROP_PAIR(ClampMin, -1))
		ZE_REFL_DATA_REF(TestComponent, FloatVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")), ZE_REFL_PROP_PAIR(ClampMin, 1.0f), ZE_REFL_PROP_PAIR(ClampMax, 360.0f), ZE_REFL_PROP(ClampOnlyDuringDragging))
		ZE_REFL_DATA_REF(TestComponent, DoubleVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")), ZE_REFL_PROP_PAIR(ClampMin, 1.0))
		ZE_REFL_DATA_REF(TestComponent, EnumVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")))
		ZE_REFL_DATA_REF(TestComponent, EnumClassVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")))
		ZE_REFL_DATA_REF(TestComponent, StringVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")))
		ZE_REFL_DATA_REF(TestComponent, Vec2Var, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")), ZE_REFL_PROP_PAIR(ClampMin, 5.0f))
		ZE_REFL_DATA_REF(TestComponent, Vec3Var, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")), ZE_REFL_PROP_PAIR(ClampMin, 5.0f))
		ZE_REFL_DATA_REF(TestComponent, ColorVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")))
		ZE_REFL_DATA_REF(TestComponent, Texture2DVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")))
		ZE_REFL_DATA_REF(TestComponent, ParticleTemplateVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Basic")))
		ZE_REFL_DATA_REF(TestComponent, BoolDeqVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, Ui8VecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, Ui32VecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, Ui64VecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, I8VecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")), ZE_REFL_PROP_PAIR(ClampMin, 1))
		ZE_REFL_DATA_REF(TestComponent, I32VecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, I64VecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, FloatVecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, DoubleVecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, EnumVecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, StringVecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, Vec2VecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, Vec3VecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, ColorVecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")))
		ZE_REFL_DATA_REF(TestComponent, ParticleTemplateVecVar, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Sequence Container")));
#endif

	ZE_REFL_TYPE(CoreComponent, ZE_REFL_PROP(HideTypeHeader), ZE_REFL_PROP(InherentType))
		ZE_REFL_DATA_REF(CoreComponent, Name)
		ZE_REFL_DATA_REF(CoreComponent, bIsInternal, ZE_REFL_PROP(HiddenInEditor));

	ZE_REFL_TYPE(TransformComponent, ZE_REFL_PROP_PAIR(DisplayName, ZE_TEXT("Transform")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("�任���")), ZE_REFL_PROP(InherentType))
		ZE_REFL_DATA_REF(TransformComponent, Translation, ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("ƽ��")), ZE_REFL_PROP_PAIR(DragSensitivity, 0.1f))
		ZE_REFL_DATA_SETTER_GETTER(TransformComponent, Rotation, SetRotationToRadians, GetRotationAsDegrees, ZE_REFL_PROP_PAIR(ClampMin, 0.0f), ZE_REFL_PROP_PAIR(ClampMax, 360.0f), ZE_REFL_PROP(ClampOnlyDuringDragging), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("��ת")))
		ZE_REFL_DATA_REF(TransformComponent, Scale, ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("����")), ZE_REFL_PROP_PAIR(DragSensitivity, 0.1f));

	ZE_REFL_TYPE(SpriteRendererComponent, ZE_REFL_PROP_PAIR(DisplayName, ZE_TEXT("Sprite Renderer")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("2D��Ⱦ���")), ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Rendering")))
		ZE_REFL_DATA_REF(SpriteRendererComponent, TintColor)
		ZE_REFL_DATA_REF(SpriteRendererComponent, Texture)
		ZE_REFL_DATA_REF(SpriteRendererComponent, TextureTiling);

	ZE_REFL_ENUM(SceneCamera::ProjectionType)
		ZE_REFL_ENUM_DATA(SceneCamera::ProjectionType, Perspective, ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("͸�����")))
		ZE_REFL_ENUM_DATA(SceneCamera::ProjectionType, Orthographic, ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("�������")));

	ZE_REFL_TYPE(CameraComponent, ZE_REFL_PROP_PAIR(DisplayName, ZE_TEXT("Camera")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("������")), ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Rendering")))
		ZE_REFL_DATA_REF(CameraComponent, bIsPrimary)
		ZE_REFL_DATA_REF(CameraComponent, bFixedAspectRatio)
		ZE_REFL_DATA_SETTER_GETTER(CameraComponent, ProjectionType, SetProjectionType, GetProjectionType)
		ZE_REFL_DATA_SETTER_GETTER(CameraComponent, VerticalFOV, SetPerspectiveVerticalFOV, GetPerspectiveVerticalFOV, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Perspective")), ZE_REFL_PROP_PAIR(HideCondition, "ProjectionType != Perspective"))
		ZE_REFL_DATA_SETTER_GETTER(CameraComponent, PerspectiveNear, SetPerspectiveNearClip, GetPerspectiveNearClip, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Perspective")), ZE_REFL_PROP_PAIR(HideCondition, "ProjectionType != Perspective"))
		ZE_REFL_DATA_SETTER_GETTER(CameraComponent, PerspectiveFar, SetPerspectiveFarClip, GetPerspectiveFarClip, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Perspective")), ZE_REFL_PROP_PAIR(HideCondition, "ProjectionType != Perspective"))
		ZE_REFL_DATA_SETTER_GETTER(CameraComponent, Size, SetOrthographicSize, GetOrthographicSize, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Orthographic")), ZE_REFL_PROP_PAIR(HideCondition, "ProjectionType != Orthographic"))
		ZE_REFL_DATA_SETTER_GETTER(CameraComponent, OrthographicNear, SetOrthographicNearClip, GetOrthographicNearClip, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Orthographic")), ZE_REFL_PROP_PAIR(HideCondition, "ProjectionType != Orthographic"))
		ZE_REFL_DATA_SETTER_GETTER(CameraComponent, OrthographicFar, SetOrthographicFarClip, GetOrthographicFarClip, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Orthographic")), ZE_REFL_PROP_PAIR(HideCondition, "ProjectionType != Orthographic"));

	ZE_REFL_TYPE(NativeScriptComponent, ZE_REFL_PROP_PAIR(DisplayName, ZE_TEXT("Native Script")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("C++�ű����")), ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Scripts")));

	ZE_REFL_TYPE(ParticleSystemComponent, ZE_REFL_PROP_PAIR(DisplayName, ZE_TEXT("Particle System")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("����ϵͳ���")), ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Effects")))
		ZE_REFL_DATA_REF(ParticleSystemComponent, Template)
		ZE_REFL_DATA_REF(ParticleSystemComponent, PositionOffset);

	ZE_REFL_ENUM(ParticleVariationType)
		ZE_REFL_ENUM_DATA(ParticleVariationType, Constant)
		ZE_REFL_ENUM_DATA(ParticleVariationType, RandomInRange)
		ZE_REFL_ENUM_DATA(ParticleVariationType, UniformInRange);

#define ZE_REFL_PARTICLE_VARIATION_TYPE(_type)											\
	ZE_REFL_TYPE_NESTED(_type)															\
		ZE_REFL_DATA_REF(_type, VariationType)											\
		ZE_REFL_DATA_REF(_type, Val1, ZE_REFL_PROP_PAIR(DisplayName, ZE_TEXT("Value")))	\
		ZE_REFL_DATA_REF(_type, Val2, ZE_REFL_PROP_PAIR(DisplayName, ZE_TEXT("ValueHigh")), ZE_REFL_PROP_PAIR(HideCondition, "VariationType == Constant"))

	ZE_REFL_PARTICLE_VARIATION_TYPE(ParticleInt);
	ZE_REFL_PARTICLE_VARIATION_TYPE(ParticleFloat);
	ZE_REFL_PARTICLE_VARIATION_TYPE(ParticleVec2);
	ZE_REFL_PARTICLE_VARIATION_TYPE(ParticleVec3);
	ZE_REFL_PARTICLE_VARIATION_TYPE(ParticleColor);

	ZE_REFL_TYPE_NESTED(BurstData)
		ZE_REFL_DATA_REF(BurstData, Time, ZE_REFL_PROP_PAIR(ClampMin, 0.0f), ZE_REFL_PROP_PAIR(ClampMax, 1.0f), ZE_REFL_PROP_PAIR(DragSensitivity, 0.01f), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("��׼����ʱ��㣺[0,1]")))
		ZE_REFL_DATA_REF(BurstData, Amount, ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("�ڸ�ʱ���һ�������ɵ�������")));

	ZE_REFL_TYPE(ParticleSystemPreviewComponent, ZE_REFL_PROP(HideTypeHeader), ZE_REFL_PROP(InherentType))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, IsLocalSpace, SetLocalSpace, IsLocalSpace, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Emitter")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("�Ƿ��ھֲ��ռ�ģ������")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, LoopCount, SetLoopCount, GetLoopCount, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Emitter")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("ѭ����������С�ڵ���0, ��Ϊ����ѭ��")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, LoopDuration, SetLoopDuration, GetLoopDuration, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Emitter")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("ÿ��ѭ����ʱ��")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, SpawnRate, SetSpawnRate, GetSpawnRate, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Emitter")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("ÿ���ܹ����ɵ����������ñ����������ӵ������ٶȡ���С�ڵ���0��������")))
		ZE_REFL_DATA_SETTER_GETTER_REF(ParticleSystemPreviewComponent, BurstList, GetBurstList, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Emitter")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("ÿ��ʱ���һ�������ɵ��������б�")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, InitialPosition, SetInitialPosition, GetInitialPosition, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Particle")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("���ӵĳ�ʼλ��")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, InitialRotation, SetInitialRotation, GetInitialRotation, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Particle")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("���ӵĳ�ʼ��ת")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, RotationRate, SetRotationRate, GetRotationRate, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Particle")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("���ӵ���ת�ٶ�")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, SizeBegin, SetSizeBegin, GetSizeBegin, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Particle")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("���ӵĳ�ʼ��С")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, SizeEnd, SetSizeEnd, GetSizeEnd, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Particle")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("���ӵ����մ�С")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, InitialVelocity, SetInitialVelocity, GetInitialVelocity, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Particle")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("���ӵĳ�ʼ�ٶ�")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, InheritVelocity, SetInheritVelocity, GetInheritVelocity, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Particle")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("�����������ܼ̳е��ٶȡ��ñ���ֻ���ڷǾֲ��ռ���Ч")), ZE_REFL_PROP_PAIR(ClampMin, 0.0f), ZE_REFL_PROP_PAIR(ClampMax, 1.0f))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, ColorBegin, SetColorBegin, GetColorBegin, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Particle")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("���ӵĳ�ʼ��ɫ")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, ColorEnd, SetColorEnd, GetColorEnd, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Particle")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("���ӵ�������ɫ")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, Lifetime, SetLifetime, GetLifetime, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Particle")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("���ӵ���������")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, Texture, SetTexture, GetTexture, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Renderer")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("���ӵĲ�����ͼ")))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, SubImageSize, SetSubImageSize, GetSubImageSize, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Renderer")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("������ηָ���ͼ������UV������xΪ������yΪ����")), ZE_REFL_PROP_PAIR(ClampMin, 0.0f))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, MaxDrawParticles, SetMaxDrawParticles, GetMaxDrawParticles, ZE_REFL_PROP_PAIR(Category, ZE_TEXT("Emitter")), ZE_REFL_PROP_PAIR(Tooltip, ZE_TEXT("������ɵ�������")), ZE_REFL_PROP_PAIR(ClampMin, 0))
		ZE_REFL_DATA_SETTER_GETTER(ParticleSystemPreviewComponent, PreviewThumbnail, SetPreviewThumbnail, GetPreviewThumbnail, ZE_REFL_PROP(HiddenInEditor));

}
