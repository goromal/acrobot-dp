#include "logging-utils-lib/filesystem.h"
#include "logging-utils-lib/logging.h"
#ifdef LOLLIPOP
#include "lollipop_dp.h"
#else
#include <iostream>
#endif

int main(int argc, char* argv[])
{
#ifdef LOLLIPOP
    dynamic_programming::DPLollipop DPL;
#else
    std::cout << "Hello, world!" << std::endl;
#endif
    return 0;
}
