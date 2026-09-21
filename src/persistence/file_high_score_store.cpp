#include "persistence/file_high_score_store.h"

#include <cstdio>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>

namespace dino::persistence {

FileHighScoreStore::FileHighScoreStore(std::string file_path)
    : file_path_(std::move(file_path)),
      temp_path_(file_path_ + ".tmp") {}

domain::ScoreValue FileHighScoreStore::load() {
    std::ifstream input(file_path_);
    if (!input.is_open()) {
        return domain::ScoreValue(0);
    }
    int32_t points = 0;
    if (!(input >> points) || points < 0) {
        return domain::ScoreValue(0);
    }
    return domain::ScoreValue(points);
}

void FileHighScoreStore::write_temp_atomic(int32_t points) {
    const int fd = ::open(temp_path_.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return;
    }
    char buffer[32];
    const int len = std::snprintf(buffer, sizeof(buffer), "%d\n", points);
    if (len > 0) {
        [[maybe_unused]] const auto written = ::write(fd, buffer, static_cast<size_t>(len));
        ::fsync(fd);
    }
    ::close(fd);
    ::rename(temp_path_.c_str(), file_path_.c_str());
}

void FileHighScoreStore::save(domain::ScoreValue score) {
    write_temp_atomic(score.points());
}

}  // namespace dino::persistence
