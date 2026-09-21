#include "test_framework.h"
#include "persistence/file_high_score_store.h"

#include <cstdio>
#include <fstream>

using namespace dino;

DINO_TEST_CASE(TestHighScoreSaveAndLoad) {
    const char* path = "tmp_test_highscore.dat";
    ::remove(path);

    persistence::FileHighScoreStore store(path);
    ASSERT_EQ(store.load().points(), 0);

    store.save(domain::ScoreValue(1250));
    ASSERT_EQ(store.load().points(), 1250);

    // Reopening with new store instance retrieves saved score
    persistence::FileHighScoreStore store2(path);
    ASSERT_EQ(store2.load().points(), 1250);

    ::remove(path);
}

DINO_TEST_CASE(TestHighScoreCorruptedFileFallback) {
    const char* path = "tmp_corrupt_highscore.dat";
    {
        std::ofstream out(path);
        out << "corrupted_garbage_data";
    }

    persistence::FileHighScoreStore store(path);
    ASSERT_EQ(store.load().points(), 0);

    ::remove(path);
}
