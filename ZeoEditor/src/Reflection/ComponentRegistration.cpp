#include "Engine/GameFramework/Components.h"
#include "Engine/Core/ReflectionCore.h"

namespace ZeoEngine {

	ZE_REGISTRATION
	{
		ZCOMPONENT(CoreComponent, ZPROP(Inherent), ZPROP(HideComponentHeader))
			ZDATA(CoreComponent, Name)
			ZDATA(CoreComponent, bIsInternal, ZPROP(HiddenInEditor));

		ZCOMPONENT(TransformComponent, ZPROP(DisplayName, ZTEXT("Transform")), ZPROP(Tooltip, ZTEXT("变换组件")), ZPROP(Inherent))
			ZDATA(TransformComponent, Translation, ZPROP(Tooltip, ZTEXT("平移")), ZPROP(DragSensitivity, 0.1f))
			ZDATA_SETTER_GETTER(TransformComponent, Rotation, SetRotationToRadians, GetRotationAsDegrees, ZPROP(ClampMin, -360.0f), ZPROP(ClampMax, 360.0f), ZPROP(ClampOnlyDuringDragging), ZPROP(Tooltip, ZTEXT("旋转")))
			ZDATA(TransformComponent, Scale, ZPROP(Tooltip, ZTEXT("缩放")), ZPROP(DragSensitivity, 0.1f));

		ZCOMPONENT(SpriteRendererComponent, ZPROP(DisplayName, ZTEXT("Sprite Renderer")), ZPROP(Tooltip, ZTEXT("2D渲染组件")), ZPROP(Category, ZTEXT("Rendering")))
			ZDATA(SpriteRendererComponent, TintColor)
			ZDATA(SpriteRendererComponent, Texture)
			ZDATA(SpriteRendererComponent, TextureTiling)
			ZDATA(SpriteRendererComponent, SortingOrder);

		ZENUM(SceneCamera::ProjectionType)
			ZENUM_DATA(SceneCamera::ProjectionType, Perspective, ZPROP(Tooltip, ZTEXT("透视相机")))
			ZENUM_DATA(SceneCamera::ProjectionType, Orthographic, ZPROP(Tooltip, ZTEXT("正交相机")));

		ZCOMPONENT(CameraComponent, ZPROP(DisplayName, ZTEXT("Camera")), ZPROP(Tooltip, ZTEXT("相机组件")), ZPROP(Category, ZTEXT("Rendering")))
			ZDATA(CameraComponent, bIsPrimary)
			ZDATA(CameraComponent, bFixedAspectRatio)
			ZDATA_SETTER_GETTER(CameraComponent, ProjectionType, SetProjectionType, GetProjectionType)
			ZDATA_SETTER_GETTER(CameraComponent, VerticalFOV, SetPerspectiveVerticalFOV, GetPerspectiveVerticalFOV, ZPROP(Category, ZTEXT("Perspective")), ZPROP(HideCondition, "ProjectionType != Perspective"))
			ZDATA_SETTER_GETTER(CameraComponent, PerspectiveNear, SetPerspectiveNearClip, GetPerspectiveNearClip, ZPROP(Category, ZTEXT("Perspective")), ZPROP(HideCondition, "ProjectionType != Perspective"))
			ZDATA_SETTER_GETTER(CameraComponent, PerspectiveFar, SetPerspectiveFarClip, GetPerspectiveFarClip, ZPROP(Category, ZTEXT("Perspective")), ZPROP(HideCondition, "ProjectionType != Perspective"))
			ZDATA_SETTER_GETTER(CameraComponent, Size, SetOrthographicSize, GetOrthographicSize, ZPROP(Category, ZTEXT("Orthographic")), ZPROP(HideCondition, "ProjectionType != Orthographic"))
			ZDATA_SETTER_GETTER(CameraComponent, OrthographicNear, SetOrthographicNearClip, GetOrthographicNearClip, ZPROP(Category, ZTEXT("Orthographic")), ZPROP(HideCondition, "ProjectionType != Orthographic"))
			ZDATA_SETTER_GETTER(CameraComponent, OrthographicFar, SetOrthographicFarClip, GetOrthographicFarClip, ZPROP(Category, ZTEXT("Orthographic")), ZPROP(HideCondition, "ProjectionType != Orthographic"));

		ZCOMPONENT(NativeScriptComponent, ZPROP(DisplayName, ZTEXT("Native Script")), ZPROP(Tooltip, ZTEXT("C++脚本组件")), ZPROP(Category, ZTEXT("Scripts")));

		ZCOMPONENT(ParticleSystemComponent, ZPROP(DisplayName, ZTEXT("Particle System")), ZPROP(Tooltip, ZTEXT("粒子系统组件")), ZPROP(Category, ZTEXT("Effects")))
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
			ZDATA(BurstData, Time, ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f), ZPROP(DragSensitivity, 0.01f), ZPROP(Tooltip, ZTEXT("标准化的时间点：[0,1]")))
			ZDATA(BurstData, Amount, ZPROP(Tooltip, ZTEXT("在该时间点一次性生成的粒子数")));

		ZCOMPONENT(ParticleSystemPreviewComponent, ZPROP(Inherent), ZPROP(HideComponentHeader))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, IsLocalSpace, SetLocalSpace, IsLocalSpace, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("是否在局部空间模拟粒子")))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, LoopCount, SetLoopCount, GetLoopCount, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("循环次数。若小于等于0, 则为无限循环")))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, LoopDuration, SetLoopDuration, GetLoopDuration, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("每次循环的时长")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, SpawnRate, GetSpawnRate, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("每秒总共生成的粒子数，该变量决定粒子的生成速度。若小于等于0，则不生成")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, BurstList, GetBurstList, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("每个时间点一次性生成的粒子数列表")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, InitialPosition, GetInitialPosition, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("粒子的初始位置")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, InitialRotation, GetInitialRotation, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("粒子的初始旋转")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, RotationRate, GetRotationRate, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("粒子的旋转速度")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, SizeBegin, GetSizeBegin, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("粒子的初始大小")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, SizeEnd, GetSizeEnd, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("粒子的最终大小")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, InitialVelocity, GetInitialVelocity, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("粒子的初始速度")))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, InheritVelocityRatio, SetInheritVelocityRatio, GetInheritVelocityRatio, ZPROP(DisplayName, ZTEXT("InheritVelocity")), ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("决定粒子所能从发射器继承的速度比率。该变量只有在非局部空间有效")), ZPROP(ClampMin, 0.0f), ZPROP(ClampMax, 1.0f))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, ColorBegin, GetColorBegin, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("粒子的初始颜色")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, ColorEnd, GetColorEnd, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("粒子的最终颜色")))
			ZDATA_GETTER_REF(ParticleSystemPreviewComponent, Lifetime, GetLifetime, ZPROP(Category, ZTEXT("Particle")), ZPROP(Tooltip, ZTEXT("粒子的生命周期")))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, Texture, SetTexture, GetTexture, ZPROP(Category, ZTEXT("Renderer")), ZPROP(Tooltip, ZTEXT("粒子的材质贴图")))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, SubImageSize, SetSubImageSize, GetSubImageSize, ZPROP(Category, ZTEXT("Renderer")), ZPROP(Tooltip, ZTEXT("决定如何分割贴图来用于UV动画。x为列数，y为行数")), ZPROP(ClampMin, 0.0f))
			ZDATA_SETTER_GETTER(ParticleSystemPreviewComponent, MaxParticles, SetMaxParticles, GetMaxParticles, ZPROP(Category, ZTEXT("Emitter")), ZPROP(Tooltip, ZTEXT("最多生成的粒子数")), ZPROP(ClampMin, 0));
	}

}
