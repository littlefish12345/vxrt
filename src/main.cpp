#include "extern_includes.hpp"
#include "window.hpp"
#include "section.hpp"
#include "block.hpp"

int main() {
    block_info_list_init();
    section_group_init();
    window_init();
    window_mainloop();
    window_term();
    return 0;
}