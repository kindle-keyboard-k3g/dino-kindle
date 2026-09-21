#include "test_framework.h"

int main() {
    return dino::testing::TestRegistry::instance().run_all();
}
