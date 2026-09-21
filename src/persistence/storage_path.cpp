#include "persistence/storage_path.h"

#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>

namespace dino::persistence {

bool StoragePath::is_directory_writable(const char* path) {
    if (path == nullptr) {
        return false;
    }
    struct stat info {};
    if (::stat(path, &info) != 0 || !S_ISDIR(info.st_mode)) {
        return false;
    }
    return ::access(path, W_OK) == 0;
}

std::string StoragePath::resolve_directory() {
    if (is_directory_writable("/mnt/us/dino")) {
        return "/mnt/us/dino";
    }
    if (is_directory_writable("/mnt/us")) {
        return "/mnt/us";
    }
    const char* home = std::getenv("HOME");
    if (home != nullptr && is_directory_writable(home)) {
        return std::string(home);
    }
    return ".";
}

std::string StoragePath::resolve_file_path(const char* file_name) {
    const std::string dir = resolve_directory();
    return dir + "/" + file_name;
}

}  // namespace dino::persistence
