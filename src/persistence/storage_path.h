#ifndef DINO_PERSISTENCE_STORAGE_PATH_H_
#define DINO_PERSISTENCE_STORAGE_PATH_H_

#include <string>

namespace dino::persistence {

/**
 * @brief Resolves target directory across Kindle VFAT, user home, and local fallback.
 */
class StoragePath {
public:
    [[nodiscard]] static std::string resolve_directory();
    [[nodiscard]] static std::string resolve_file_path(const char* file_name);

private:
    [[nodiscard]] static bool is_directory_writable(const char* path);
};

}  // namespace dino::persistence

#endif  // DINO_PERSISTENCE_STORAGE_PATH_H_
