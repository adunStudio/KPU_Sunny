#include "VFS.h"

namespace sunny
{
	VFS* VFS::s_instance = nullptr;

	void VFS::Init()
	{
		s_instance = new VFS();
	}

	void VFS::Shutdown()
	{
		delete s_instance;
	}

	void VFS::Mount(const std::string& virtualPath, const std::string& physicalPath)
	{
		if (!s_instance)
		{
			// Debug System
		}

		m_mountPoints[virtualPath].push_back(physicalPath);
	}

	void VFS::UnMount(const std::string& path)
	{
		if (!s_instance)
		{
			// Debug System
		}

		m_mountPoints[path].clear();
	}

	bool VFS::ResolvePhysicalPath(const std::string& path, std::string& outPhysicalPath)
	{

		if (path[0] != '/')
		{
			outPhysicalPath = path;
			return system::FileSystem::FileExists(path);
		}

		std::vector<std::string> dirs = utils::SplitString(path, '/');

		const std::string& virtualDir = dirs.front();

		std::cout << virtualDir << std::endl;

		if (m_mountPoints.find(virtualDir) == m_mountPoints.end() || m_mountPoints[virtualDir].empty())
			return false;


		std::string remainder = path.substr(virtualDir.size() + 1, path.size() - virtualDir.size());

		std::cout << "remainder: " << remainder << std::endl;
		for (const std::string& physicalPath : m_mountPoints[virtualDir])
		{
			std::string path = physicalPath + remainder;

			std::cout << path << std::endl;
			if (system::FileSystem::FileExists(path))
			{
				outPhysicalPath = path;

				return true;
			}
		}

		return false;
	}

	unsigned char* VFS::ReadFile(const std::string& path)
	{
		if (!s_instance)
		{
			// Debug System
		}
		std::string physicalPath;

		return ResolvePhysicalPath(path, physicalPath) ? system::FileSystem::ReadFile(physicalPath) : nullptr;
	}

	std::string VFS::ReadTextFile(const std::string& path)
	{
		if (!s_instance)
		{
			// Debug System
		}

		std::string physicalPath;
		
		return ResolvePhysicalPath(path, physicalPath) ? system::FileSystem::ReadTextFile(physicalPath) : nullptr;
	}

	bool VFS::WriteFile(const std::string& path, unsigned char* buffer)
	{
		if (!s_instance)
		{
			// Debug System
		}

		std::string physicalPath;

		return ResolvePhysicalPath(path, physicalPath) ? system::FileSystem::WriteFile(physicalPath, buffer) : nullptr;
	}

	bool VFS::WriteTextFile(const std::string& path, const std::string& text)
	{
		if (!s_instance)
		{
			// Debug System
		}

		std::string physicalPath;

		return ResolvePhysicalPath(path, physicalPath) ? system::FileSystem::WriteTextFile(physicalPath, text) : nullptr;
	}
	
}