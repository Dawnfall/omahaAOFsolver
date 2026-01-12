#include "Utils.h"

#include <filesystem>

#ifdef _WIN32
	#include <windows.h>
#elif __linux__
	#include <unistd.h>
#elif __APPLE__
	#include <mach-o/dyld.h>
#endif

namespace Utils
{
	std::filesystem::path GetExecutableDir()
	{
#ifdef _WIN32
		wchar_t buffer[MAX_PATH];
		GetModuleFileNameW(nullptr, buffer, MAX_PATH);
		return std::filesystem::path(buffer).parent_path();
#elif __linux__
		char buffer[1024];
		ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
		if (len != -1) {
			buffer[len] = '\0';
			return std::filesystem::path(buffer).parent_path();
		}
		return {};
#elif __APPLE__
		char buffer[1024];
		uint32_t size = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &size) == 0) {
			return std::filesystem::path(buffer).parent_path();
		}
		return {};
#else
		return std::filesystem::current_path();
#endif
	}
}


