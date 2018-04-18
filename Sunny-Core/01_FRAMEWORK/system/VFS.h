#pragma once

#include "../include.h"
#include "../utils/String.h"
#include "FileSystem.h"

namespace sunny
{
	class VFS
	{
	private:
		static VFS* s_instance;

	private:
		std::unordered_map<std::string, std::vector<std::string>> m_mountPoints;

	public:
		void Mount(const std::string& virtualPath, const std::string& physicalPath);
		void UnMount(const std::string& path);

		bool ResolvePhysicalPath(const std::string& path, std::string& outPhysicalPath);

		unsigned char* ReadFile(const std::string& path);
		std::string    ReadTextFile(const std::string& path);

		bool WriteFile(const std::string& path, unsigned char* buffer);
		bool WriteTextFile(const std::string& path, const std::string& text);

	public:
		static void Init();
		static void Shutdown();

		inline static VFS* Get() { return s_instance; }
	};
}