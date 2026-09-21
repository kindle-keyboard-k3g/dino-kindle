#ifndef DINO_PERSISTENCE_HIGH_SCORE_STORE_H_
#define DINO_PERSISTENCE_HIGH_SCORE_STORE_H_

#include "domain/game_values.h"

namespace dino::persistence {

/**
 * @brief Abstract contract for high score retrieval and storage.
 */
class HighScoreStore {
public:
    virtual ~HighScoreStore() = default;

    [[nodiscard]] virtual domain::ScoreValue load() = 0;
    virtual void save(domain::ScoreValue score) = 0;
};

}  // namespace dino::persistence

#endif  // DINO_PERSISTENCE_HIGH_SCORE_STORE_H_
