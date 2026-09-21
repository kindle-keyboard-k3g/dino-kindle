#include "application/application.h"
#include "application/application_options.h"
#include "util/signal_guard.h"

#include <exception>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        dino::util::SignalGuard signal_guard;
        const auto options = dino::application::ApplicationOptions::parse(argc, argv);
        dino::application::Application app(options);
        return app.run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 1;
    }
}
