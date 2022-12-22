﻿namespace ZeoEngine
{
    public abstract class IComponent
    {
        public Entity Entity { get; internal set; }
    }

    public class TransformComponent : IComponent
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
                return translation;
            }
            set => InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
        }

        public Vector3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 rotation);
                return rotation;
            }
            set => InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
        }

        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 scale);
                return scale;
            }
            set => InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
        }
    }

    public class MeshRendererComponent : IComponent
    {
        public AssetHandle MeshAsset
        {
            get
            {
                InternalCalls.MeshRendererComponent_GetMeshAsset(Entity.ID, out AssetHandle meshAsset);
                return meshAsset;
            }
            set => InternalCalls.MeshRendererComponent_SetMeshAsset(Entity.ID, value);
        }

        public MeshInstance Instance => new MeshInstance(InternalCalls.MeshRendererComponent_GetInstance(Entity.ID));

        public AssetHandle GetMaterial(uint index)
        {
            InternalCalls.MeshRendererComponent_GetMaterialAsset(Entity.ID, index, out AssetHandle materialAsset);
            return materialAsset;
        }

        public void SetMaterial(uint index, AssetHandle materialAsset)
        {
            InternalCalls.MeshRendererComponent_SetMaterialAsset(Entity.ID, index, materialAsset);
        }
    }

    public class RigidBodyComponent : IComponent
    {
        public void SetKinematicTarget(Vector3 targetPosition, Vector3 targetRotation)
        {
            InternalCalls.RigidBodyComponent_SetKinematicTarget(Entity.ID, ref targetPosition, ref targetRotation);
        }
    }

    public class CharacterControllerComponent : IComponent
    {
        public bool bIsGrounded
        {
            get => InternalCalls.CharacterControllerComponent_IsGrounded(Entity.ID);
        }

        public void Move(Vector3 displacement)
        {
            InternalCalls.CharacterControllerComponent_Move(Entity.ID, ref displacement);
        }
    }

}