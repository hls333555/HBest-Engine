#pragma once

#include <filesystem>
#include <unordered_map>
#include <thread>
#include <entt.hpp>

#include "Engine/Core/Core.h"

namespace ZeoEngine {

	/** A file watcher running on a separate thread. */
	class FileWatcher
	{
	public:
		FileWatcher(const std::filesystem::path& directoryToWatch, std::chrono::duration<I32, std::milli> interval)
			: m_DirectoryToWatch(std::filesystem::canonical(directoryToWatch)) // Directory to watch must use absolute path!
			, m_Interval(interval)
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(m_DirectoryToWatch))
			{
				if (entry.is_directory()) continue;

				m_WatchedFiles[entry.path()] = std::filesystem::last_write_time(entry);
			}

			ZE_CORE_TRACE("File watcher initialized");

			Start();
		}

		~FileWatcher()
		{
			Stop();
		}
		
		void Start()
		{
			m_bIsRunning = true;
			m_FileWatcherThread = CreateScope<std::thread>(&FileWatcher::Execute, this);
			m_FileWatcherThread->detach();
		}

		void Stop()
		{
			m_bIsRunning = false;
		}

	private:
		/** Monitor "m_DirectoryToWatch" for changes and in case of a change execute the delegate. */
		void Execute()
		{
			while (m_bIsRunning)
			{
				// Wait for several milliseconds
				std::this_thread::sleep_for(m_Interval);

				auto it = m_WatchedFiles.begin();
				while (it != m_WatchedFiles.end())
				{
					// File removal
					if (!std::filesystem::exists(it->first))
					{
						m_OnFileRemovedDel.publish(it->first);
						it = m_WatchedFiles.erase(it);
					}
					else
					{
						++it;
					}
				}

				if (std::filesystem::exists(m_DirectoryToWatch))
				{
					// Check if a file was created or modified
					for (const auto& entry : std::filesystem::recursive_directory_iterator(m_DirectoryToWatch))
					{
						if (entry.is_directory()) continue;

						auto currentFileLastWriteTime = std::filesystem::last_write_time(entry);

						auto filePath = entry.path();
						// File creation
						if (m_WatchedFiles.find(filePath) == m_WatchedFiles.end())
						{
							m_WatchedFiles[filePath] = currentFileLastWriteTime;
							m_OnFileAddedDel.publish(filePath);
						}
						// File modification
						else
						{
							if (m_WatchedFiles[filePath] != currentFileLastWriteTime)
							{
								m_WatchedFiles[filePath] = currentFileLastWriteTime;
								m_OnFileModifiedDel.publish(filePath);
							}
						}
					}
				}
			}
		}

	public:
		entt::sink<entt::sigh<void(const std::filesystem::path&)>> m_OnFileAdded{ m_OnFileAddedDel };
		entt::sink<entt::sigh<void(const std::filesystem::path&)>> m_OnFileModified{ m_OnFileModifiedDel };
		entt::sink<entt::sigh<void(const std::filesystem::path&)>> m_OnFileRemoved{ m_OnFileRemovedDel };

	private:
		std::filesystem::path m_DirectoryToWatch;
		/** Time interval at which we check the base folder for changes */
		std::chrono::duration<I32, std::milli> m_Interval;

		/** A record of files from the base directory to their last modification time */
		std::unordered_map<std::filesystem::path, std::filesystem::file_time_type> m_WatchedFiles;

		bool m_bIsRunning = true;
		Scope<std::thread> m_FileWatcherThread;

		entt::sigh<void(const std::filesystem::path&)> m_OnFileAddedDel, m_OnFileModifiedDel, m_OnFileRemovedDel;
	};

}
