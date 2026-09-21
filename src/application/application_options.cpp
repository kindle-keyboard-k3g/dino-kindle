#include "application/application_options.h"

#include <cstdlib>
#include <cstring>

namespace dino::application {

namespace {

void parse_single_arg(int index,
                      int argc,
                      char* argv[],
                      ApplicationOptions& options) {
    const char* arg = argv[index];
    if (std::strcmp(arg, "--ansi") == 0) {
        options.set_ansi(true);
        return;
    }
    if (std::strcmp(arg, "--ppm") == 0) {
        options.set_ppm(true);
        return;
    }
    if (std::strcmp(arg, "--seed") == 0 && index + 1 < argc) {
        options.set_seed(static_cast<uint32_t>(std::strtoul(argv[index + 1], nullptr, 0)));
        return;
    }
    if (std::strcmp(arg, "--frames") == 0 && index + 1 < argc) {
        options.set_max_frames(static_cast<int32_t>(std::strtol(argv[index + 1], nullptr, 10)));
    }
}

}  // namespace

ApplicationOptions ApplicationOptions::parse(int argc, char* argv[]) {
    ApplicationOptions options;
    for (int i = 1; i < argc; ++i) {
        parse_single_arg(i, argc, argv, options);
    }
    return options;
}

}  // namespace dino::application
