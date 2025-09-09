#include "utils/serial_print_func.hpp"


/**
 * @file serial_print_func.hpp
 * @author Wuard
 * @brief This function wrapper the serial function to be compatible with C.
 *
 * @license For the license see file LICENSE.txt file in the root directory of this source tree.
 */


extern "C" int serial_print_func(const char *format, ...) {
    char buf[256];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    Serial.print(buf);
    return len;
}