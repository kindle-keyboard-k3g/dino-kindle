#ifndef DINO_PERSISTENCE_STORAGE_PATH_H_
#define DINO_PERSISTENCE_STORAGE_PATH_H_

#include <string>

namespace dino::persistence {

/**
 * @brief Resolves target directory across Kindle VFAT, user home, and local fallback.
 */
class StoragePath {
public:
    /**
     * @brief Resolves first available writable directory for user data.
     * @return Absolute directory path.
     */
    [[nodiscard]] static std::string resolve_directory();

    /**
     * @brief Resolves full file path for given file name within writable storage.
     * @param file_name File name (e.g. "highscore.dat").
     * @return Full path to file.
     */
    [[nodiscard]] static std::string resolve_file_path(const char* file_name);

private:
    [[nodiscard]] static bool is_directory_writable(const char* path);
};

}  // namespace dino::persistence

#endif  // DINO_PERSISTENCE_STORAGE_PATH_H_
