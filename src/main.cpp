#include "extern_includes.hpp"
#include "window.hpp"

int main() {
    window_init();
    window_mainloop();
    window_term();
    return 0;
}