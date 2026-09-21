#ifndef DINO_PERSISTENCE_FILE_HIGH_SCORE_STORE_H_
#define DINO_PERSISTENCE_FILE_HIGH_SCORE_STORE_H_

#include "persistence/high_score_store.h"

#include <string>

namespace dino::persistence {

/**
 * @brief Power-fail-safe atomic high score store with fsync and rename.
 */
class FileHighScoreStore : public HighScoreStore {
public:
    explicit FileHighScoreStore(std::string file_path);

    [[nodiscard]] domain::ScoreValue load() override;
    void save(domain::ScoreValue score) override;

private:
    void write_temp_atomic(int32_t points);

    std::string file_path_;
    std::string temp_path_;
};

}  // namespace dino::persistence

#endif  // DINO_PERSISTENCE_FILE_HIGH_SCORE_STORE_H_
