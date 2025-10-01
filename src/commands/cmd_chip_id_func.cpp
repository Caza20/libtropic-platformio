#include "commands/cmd_chip_id_func.hpp"

/**
 * @file cmd_chip_id_func.cpp
 * @author Wuard
 * @brief This function response the chip id to send by uart.
 *
 * @license For the license see file LICENSE.txt file in the root directory of this source tree.
 */

//**********************************************************************
//*                     Principal functions                            *
//**********************************************************************

String cmd_chip_id_func() {
    String response = "";

    // lt_init(&__lt_handle__);
    
    struct lt_chip_id_t chip_id = {0};
    //* read chip ID
    if (lt_get_info_chip_id(&__lt_handle__, &chip_id) != LT_OK) {
        response = "ERR:FAILED_TO_GET_CHIP_ID\n";
        return response;
    } 

    char print_bytes_buff[CHIP_ID_FIELD_MAX_SIZE];

    //* get chip id version as string
    if (lt_print_bytes(chip_id.chip_id_ver, sizeof(chip_id.chip_id_ver), print_bytes_buff,sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_CHIP_ID_VER\n";
        return response;
    } 

    response = "OK:chip_id:ver = 0x" + String(print_bytes_buff) + " " + "(v" + chip_id.chip_id_ver[0] + "." + chip_id.chip_id_ver[1] + "." + chip_id.chip_id_ver[2] + "." + chip_id.chip_id_ver[3] + ")" + ":";

    //* get fl chip info as string
    if (lt_print_bytes(chip_id.fl_chip_info, sizeof(chip_id.fl_chip_info), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_FL_PROD_DATA\n";
        return response;
    } 

    String type_fl_prod_data;
    chip_id.fl_chip_info[0] == 0x01 ? type_fl_prod_data = "PASSED" : type_fl_prod_data = "N/A";

    response += "FL_PROD_DATA = 0x" + String(print_bytes_buff) + " " + "(" + type_fl_prod_data + ")" + ":";

    //* get man func test as string
    if (lt_print_bytes(chip_id.func_test_info, sizeof(chip_id.func_test_info), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_MAN_FUNC_TEST\n";
        return response;
    } 
    
    String type_man_func_test;
    chip_id.func_test_info[0] == 0x01 ? type_man_func_test = "PASSED" : type_man_func_test = "N/A";

    response += "MAN_FUNC_TEST = 0x" + String(print_bytes_buff) + " " + "(" + type_man_func_test + ")" + ":";

    //* get silicon rev as string
    if (lt_print_bytes(chip_id.silicon_rev, sizeof(chip_id.silicon_rev), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_SILICON_REV\n";
        return response;
    } 

    response += "Silicon rev = 0x" + String(print_bytes_buff) + chip_id.silicon_rev[0] + chip_id.silicon_rev[1] + chip_id.silicon_rev[2] + chip_id.silicon_rev[3] + ":"; 

    //* get package type id as string
    uint16_t packg_type_id = ((uint16_t)chip_id.packg_type_id[0] << 8) | ((uint16_t)chip_id.packg_type_id[1]);
    if (lt_print_bytes(chip_id.packg_type_id, sizeof(chip_id.packg_type_id), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_PACKAGE_TYPE_ID\n";
        return response;
    } 
    
    char packg_type_id_str[17];
    switch (packg_type_id) {
        case CHIP_PKG_BARE_SILICON_ID:
            strcpy(packg_type_id_str, "Bare silicon die");
            break;

        case CHIP_PKG_QFN32_ID:
            strcpy(packg_type_id_str, "QFN32, 4x4mm");
            break;

        default:
            strcpy(packg_type_id_str, "N/A");
            break;
    }

    response += "Package ID = 0x" + String(print_bytes_buff) + " (" + String(packg_type_id_str) + ")" + ":";

    //* get prov info ver as string
    char buff_2X[3]; 
    sprintf(buff_2X, "%02X", chip_id.prov_ver_fab_id_pn[0]); 
    String prov_ver_fab_id_str = String(buff_2X);

    response += "Prov info ver = 0x" + prov_ver_fab_id_str + "(v" + chip_id.prov_ver_fab_id_pn[0] + ")" + ":"; 

    //* get fab id as string
    uint16_t parsed_fab_id = ((chip_id.prov_ver_fab_id_pn[1] << 4) | (chip_id.prov_ver_fab_id_pn[2] >> 4)) & 0xfff;
    String fab_id = "";
    switch (parsed_fab_id) {
        case FAB_ID_TROPIC_SQUARE_LAB:
            fab_id = "Tropic Square Lab";
            break;

        case FAB_ID_EPS_BRNO:
            fab_id = "EPS Global - Brno";
            break;

        default:
            fab_id = "N/A";
            break;
    }
    char buff_3X[4];  
    sprintf(buff_3X, "%03X", parsed_fab_id); 
    String fab_id_str = String(buff_3X);

    response += "Fab ID = 0x" + fab_id_str + " (" + fab_id + ")" + ":";

    //* get P/N ID (short P/N) as string
    uint16_t parsed_short_pn = ((chip_id.prov_ver_fab_id_pn[2] << 8) | (chip_id.prov_ver_fab_id_pn[3])) & 0xfff;
    sprintf(buff_3X, "%03X", parsed_short_pn); 
    String parsed_short_str = String(buff_3X);

    response += "P/N ID (short P/N) = 0x" + parsed_short_str + ":";

    //* get prov date as string
    if (lt_print_bytes(chip_id.provisioning_date, sizeof(chip_id.provisioning_date), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_PROVISIONING_DATE\n";
        return response;
    }

    response += "Prov date = 0x" + String(print_bytes_buff) + ":"; 

    //* get HSM HW/FW/SW ver as string
    if (lt_print_bytes(chip_id.hsm_ver, sizeof(chip_id.hsm_ver), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_HSM\n";
        return response;
    }

    response += "HSM HW/FW/SW ver = 0x" + String(print_bytes_buff) + ":";

    //* get programmer ver as string
    if (lt_print_bytes(chip_id.prog_ver, sizeof(chip_id.prog_ver), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_PROGRAMMER_VER\n";
        return response;
    }

    response +=  "Programmer ver = 0x" + String(print_bytes_buff) + ":";

    //* get S/N as string
    if (lt_print_bytes((uint8_t *)&chip_id.ser_num, sizeof(chip_id.ser_num), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_SER_NUM\n";
        return response;
    }

    response += "S/N = 0x" + String(print_bytes_buff) + ":";

    //* get P/N long as string
    uint8_t pn_len = chip_id.part_num_data[0];
    uint8_t pn_data[16];  // 15B for data, last byte for '\0'
    memcpy(pn_data, &chip_id.part_num_data[1], pn_len);
    pn_data[pn_len] = '\0';

    if (lt_print_bytes(chip_id.part_num_data, sizeof(chip_id.part_num_data), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_P_N\n";
        return response;
    }

    response += "P/N (long) = 0x" + String(print_bytes_buff) + " (" + String((char*)pn_data) + ")" + ":";

    //* get prov template ver as string
    if (lt_print_bytes(chip_id.prov_templ_ver, sizeof(chip_id.prov_templ_ver), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_PROV_TEMPLATE_VER\n";
        return response;
    }

    response += "Prov template ver = 0x" + String(print_bytes_buff) + " (v" + chip_id.prov_templ_ver[0] + "." + chip_id.prov_templ_ver[1] + ")" + ":";

    //* get prov template tag as string
    if (lt_print_bytes(chip_id.prov_templ_tag, sizeof(chip_id.prov_templ_tag), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_PROV_TEMPLATE_TAG\n";
        return response;
    }

    response += "Prov template tag = 0x" + String(print_bytes_buff) + ":";

    //* get prov specification ver as string
    if (lt_print_bytes(chip_id.prov_spec_ver, sizeof(chip_id.prov_spec_ver), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_PROV_SPECIFICATION_VER\n";
        return response;
    }

    response += "Prov specification ver = 0x" + String(print_bytes_buff) + ":";

    //* get prov specification tag as string
    if (lt_print_bytes(chip_id.prov_spec_tag, sizeof(chip_id.prov_spec_tag), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_PROV_SPECIFICATION_TAG\n";
        return response;
    }

    response += "Prov specification tag = 0x" + String(print_bytes_buff) + ":";

    //* get batch ID as string
    if (lt_print_bytes(chip_id.batch_id, sizeof(chip_id.batch_id), print_bytes_buff, sizeof(print_bytes_buff)) != LT_OK) {
        response = "ERR:FAILED_TO_READ_PROV_SPECIFICATION_TAG\n";
        return response;
    }

    response += "Batch ID = 0x" + String(print_bytes_buff) + ";\n";

    return response;
}