#ifndef CMD_CHIP_ID_FUNC_HPP
#define CMD_CHIP_ID_FUNC_HPP

/**
 * @file cmd_chip_id_func.hpp
 * @author Wuard
 * @brief This function shows the chip id and fw version by terminal.
 *
 * @license For the license see file LICENSE.txt file in the root directory of this source tree.
 */

extern "C" {
    #include "libtropic.h"
    #include "libtropic_common.h"
    #include "libtropic_examples.h"
    #include "libtropic_logging.h"
    #include "string.h"
    #include <inttypes.h>
}


#include <Arduino.h>
#include "libtropic_platfomio.hpp"


String cmd_chip_id_func();


#endif