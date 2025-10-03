#ifndef CMD_BOOTLOADER_VERSION_FUNC_HPP
#define CMD_BOOTLOADER_VERSION_FUNC_HPP

/**
 * @file cmd_bootloader_version_func.hpp
 * @author Wuard
 * @brief This function response the bootloader version to send by uart.
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


String cmd_bootloader_version_func();


// -------

String get_headers_v1();
String header_boot_v2_0_1(uint8_t *data, bank_id_t bank_id);

String get_headers_v2();
String header_boot_v1_0_1(uint8_t *data, bank_id_t bank_id);


#endif