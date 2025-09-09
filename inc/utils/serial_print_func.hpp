#ifndef SERIAL_PRINT_FUNC_HPP
#define SERIAL_PRINT_FUNC_HPP

/**
 * @file serial_print_func.hpp
 * @author Wuard
 * @brief This function wrapper the serial function to be compatible with C.
 *
 * @license For the license see file LICENSE.txt file in the root directory of this source tree.
 */

#include <Arduino.h>

extern "C" int serial_print_func(const char *format, ...);

#endif