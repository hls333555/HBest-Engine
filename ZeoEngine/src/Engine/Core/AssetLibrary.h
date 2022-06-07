#pragma once

#include <entt.hpp>

#include "Engine/Core/AssetRegistry.h"
#include "Engine/Core/EngineTypes.h"
#include "Engine/Utils/PathUtils.h"

namespace ZeoEngine {

	namespace Utils {

		/** Returns true if ID begins with "assets". */
		static bool IsPath(const std::string& ID)
		{
			const char* assetRoot = AssetRegistry::GetAssetRootDirectory();
			return ID.find(assetRoot) == 0;
		}

		static bool Validate(std::string& ID)
		{
			if (IsPath(ID))
			{
				ID = PathUtils::GetNormalizedAssetPath(ID);
				if (!PathUtils::DoesPathExist(ID)) return false;
			}
			return true;
		}

		static U32 GetIDFromString(const std::string& ID)
		{
			return entt::hashed_string{ ID.c_str() };
		}

	}

	template<typename AssetLibraryClass, typename AssetClass, typename AssetLoaderClass>
	class AssetLibrary : private entt::resource_cache<AssetClass, AssetLoaderClass>
	{
	public:
		AssetLibrary(const AssetLibrary&) = delete;
		AssetLibrary& operator=(const AssetLibrary&) = delete;

		static AssetLibraryClass& Get()
		{
			static AssetLibraryClass instance;
			return instance;
		}

		template<typename... Args>
		AssetHandle<AssetClass> LoadAsset(std::string ID, Args &&... args)
		{
			if (!Utils::Validate(ID)) return {};

			auto ret = load(Utils::GetIDFromString(ID), ID, std::forward<Args>(args)...);
			return ret.first->second;
		}

		void ReloadAsset(std::string ID)
		{
			if (!HasAsset(ID)) return;

			auto asset = LoadAsset(std::move(ID));
			asset->Reload();
			asset->m_OnAssetReloadedDel.publish();
		}

		void UnloadAsset(std::string ID)
		{
			if (!Utils::Validate(ID)) return;

			erase(Utils::GetIDFromString(ID));
		}

		bool HasAsset(std::string ID)
		{
			if (!Utils::Validate(ID)) return false;

			return contains(Utils::GetIDFromString(ID));
		}

	protected:
		AssetLibrary() = default;
	};

}
