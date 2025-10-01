#ifndef CMD_FW_VERSION_FUNC_HPP
#define CMD_FW_VERSION_FUNC_HPP

/**
 * @file cmd_fw_version_func.hpp
 * @author Wuard
 * @brief This function response the firmware version to send by uart.
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


String cmd_fw_version_func();


#endif