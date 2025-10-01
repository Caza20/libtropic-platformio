#include "commands/cmd_fw_version_func.hpp"

/**
 * @file cmd_fw_version_func.cpp
 * @author Wuard
 * @brief This function response the firmware version to send by uart.
 *
 * @license For the license see file LICENSE.txt file in the root directory of this source tree.
 */

//**********************************************************************
//*                     Principal functions                            *
//**********************************************************************



String cmd_fw_version_func() {
    String response = "";

    // This variable is reused on more places in this example to store different firmware versions
    uint8_t fw_ver[LT_L2_GET_INFO_RISCV_FW_SIZE] = {0};

    // First we check versions of both updateable firmwares. For this we need to be in APPLICATION mode.
    // If there are valid firmwares, chip will execute them on boot. In any case we will try to reboot into application,
    // in case chip would be in maintenance mode (executing bootloader)
    if (lt_reboot(&__lt_handle__, LT_MODE_APP) != LT_OK) {
        response = "ERR:LT_REBOOT_FAILED;\n";
        lt_deinit(&__lt_handle__);
        return response;
    }

    // App runs so we can see what firmwares are running
    // Getting RISCV app firmware version
    if (__lt_handle__.l2.mode == LT_MODE_APP) {
        // Getting RISCV app firmware version
        if (lt_get_info_riscv_fw_ver(&__lt_handle__, fw_ver) == LT_OK) {
            char buff_2X[3]; 
            sprintf(buff_2X, "%02X", fw_ver[3]); 
            String fw_ver_3 = String(buff_2X);
            sprintf(buff_2X, "%02X", fw_ver[2]); 
            String fw_ver_2 = String(buff_2X);
            sprintf(buff_2X, "%02X", fw_ver[1]); 
            String fw_ver_1 = String(buff_2X);
            sprintf(buff_2X, "%02X", fw_ver[0]); 
            String fw_ver_0 = String(buff_2X);
            response = "RISC-V application FW version = " + fw_ver_3 + "." + fw_ver_2 + "." + fw_ver_1 + " (+ ." + fw_ver_0 + "):";
        }
        else {
            response = "ERR:FAILED_TO_GET_RISCV_FW_VERSION;\n";
            lt_deinit(&__lt_handle__);
            return response;
        }

        
        if (lt_get_info_spect_fw_ver(&__lt_handle__, fw_ver) == LT_OK) {
            char buff_2X[3]; 
            sprintf(buff_2X, "%02X", fw_ver[3]); 
            String fw_ver_3 = String(buff_2X);
            sprintf(buff_2X, "%02X", fw_ver[2]); 
            String fw_ver_2 = String(buff_2X);
            sprintf(buff_2X, "%02X", fw_ver[1]); 
            String fw_ver_1 = String(buff_2X);
            sprintf(buff_2X, "%02X", fw_ver[0]); 
            String fw_ver_0 = String(buff_2X);
            response += "SPECT firmware version= " + fw_ver_3 + "." + fw_ver_2 + "." + fw_ver_1 + "  (+ ." + fw_ver_0 + "):\n";
        }
        else {
            response = "ERR:FAILED_TO_GET_SPECT_FW_VERSION;\n";
            lt_deinit(&__lt_handle__);
            return response;
        }
    }
    else {
        response = "ERR:CHIP_COULD_NOT_GET_INTO_APP_MODE;\n";
    }

    return response;
}