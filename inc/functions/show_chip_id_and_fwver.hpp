#ifndef SHOW_CHIP_ID_AND_FWVER_HPP
#define SHOW_CHIP_ID_AND_FWVER_HPP

/**
 * @file show_chip_id_and_fwver.hpp
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
#include "utils/serial_print_func.hpp"


int lt_show_chip_id_and_fwver(lt_handle_t *h);


#endif