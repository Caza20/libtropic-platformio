#include "libtropic_platfomio.hpp"
#include <stdio.h>
#include <Arduino.h>

// Wrapper compatible con print_func
extern "C" int serial_print_func(const char *format, ...) {
    char buf[256]; // tamaño suficiente
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    Serial.print(buf);
    return len;


    // va_list args;
    // va_start(args, format);
    // int len = Serial.printf(format, args); // ESP32 soporta esto
    // va_end(args);
    // return len;
}

static void print_header_boot_v1_0_1(uint8_t *data, bank_id_t bank_id)
{
    struct header_boot_v1_t *p_h = (struct header_boot_v1_t *)data;
    switch (bank_id) {
        case FW_BANK_FW1:
            LT_LOG_INFO("    Firmware bank 1 header:");
            break;
        case FW_BANK_FW2:
            LT_LOG_INFO("    Firmware bank 2 header:");
            break;
        case FW_BANK_SPECT1:
            LT_LOG_INFO("    SPECT bank 1 header:");
            break;
        case FW_BANK_SPECT2:
            LT_LOG_INFO("    SPECT bank 2 header:");
            break;
        default:
            LT_LOG_ERROR("    Unknown bank ID: %d", (int)bank_id);
            return;
    }
    LT_LOG_INFO("      Type:      %02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8, p_h->type[3], p_h->type[2],
                p_h->type[1], p_h->type[0]);
    LT_LOG_INFO("      Version:   %02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8, p_h->version[3], p_h->version[2],
                p_h->version[1], p_h->version[0]);
    LT_LOG_INFO("      Size:      %02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8, p_h->size[3], p_h->size[2],
                p_h->size[1], p_h->size[0]);
    LT_LOG_INFO("      Git hash:  %02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8, p_h->git_hash[3], p_h->git_hash[2],
                p_h->git_hash[1], p_h->git_hash[0]);
    LT_LOG_INFO("      FW hash:   %02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8, p_h->hash[3], p_h->hash[2],
                p_h->hash[1], p_h->hash[0]);
}

static void print_all_headers_v1(lt_handle_t *h)
{
    uint8_t header[LT_L2_GET_INFO_FW_HEADER_SIZE] = {0};

    // Read header from FW_BANK_FW1
    LT_LOG_INFO("Reading firmware headers in bank %d", (int)FW_BANK_FW1);
    lt_ret_t ret = lt_get_info_fw_bank(h, FW_BANK_FW1, header, sizeof(header));
    if (ret == LT_OK) {
        print_header_boot_v1_0_1(header, FW_BANK_FW1);
    }
    else {
        LT_LOG_ERROR("Failed to get FW bank %d header, ret=%d", (int)FW_BANK_FW1, ret/*lt_ret_verbose(ret)*/);
        return;
    }

    // Read header from FW_BANK_FW2
    LT_LOG_INFO("Reading firmware headers in bank %d", (int)FW_BANK_FW2);
    memset(header, 0, sizeof(header));
    ret = lt_get_info_fw_bank(h, FW_BANK_FW2, header, sizeof(header));
    if (ret == LT_OK) {
        print_header_boot_v1_0_1(header, FW_BANK_FW2);
    }
    else {
        LT_LOG_ERROR("Failed to get FW bank %d header, ret=%d", (int)FW_BANK_FW2, ret/*lt_ret_verbose(ret)*/);
        return;
    }

    // Read header from FW_BANK_SPECT1
    LT_LOG_INFO("Reading SPECT headers in bank %d", (int)FW_BANK_SPECT1);
    memset(header, 0, sizeof(header));
    ret = lt_get_info_fw_bank(h, FW_BANK_SPECT1, header, sizeof(header));
    if (ret == LT_OK) {
        print_header_boot_v1_0_1(header, FW_BANK_SPECT1);
    }
    else {
        LT_LOG_ERROR("Failed to get SPECT bank %d header, ret=%d", (int)FW_BANK_SPECT1, ret/*lt_ret_verbose(ret)*/);
        return;
    }

    // Read header from FW_BANK_SPECT2
    LT_LOG_INFO("Reading SPECT headers in bank %d", (int)FW_BANK_SPECT2);
    memset(header, 0, sizeof(header));
    ret = lt_get_info_fw_bank(h, FW_BANK_SPECT2, header, sizeof(header));
    if (ret == LT_OK) {
        print_header_boot_v1_0_1(header, FW_BANK_SPECT2);
    }
    else {
        LT_LOG_ERROR("Failed to get SPECT bank %d header, ret=%d", (int)FW_BANK_SPECT2, ret/*lt_ret_verbose(ret)*/);
        return;
    }
}

// This function prints the header in the new format used in bootloader version 2.0.1
static void print_header_boot_v2_0_1(uint8_t *data, bank_id_t bank_id)
{
    struct header_boot_v2_t *p_h = (struct header_boot_v2_t *)data;
    switch (bank_id) {
        case FW_BANK_FW1:
            LT_LOG_INFO("    Firmware bank 1 header:");
            break;
        case FW_BANK_FW2:
            LT_LOG_INFO("    Firmware bank 2 header:");
            break;
        case FW_BANK_SPECT1:
            LT_LOG_INFO("    SPECT bank 1 header:");
            break;
        case FW_BANK_SPECT2:
            LT_LOG_INFO("    SPECT bank 2 header:");
            break;
        default:
            LT_LOG_ERROR("    Unknown bank ID: %d", (int)bank_id);
            return;
    }
    LT_LOG_INFO("      Type:               %04" PRIX16, p_h->type);
    LT_LOG_INFO("      Padding:            %02" PRIX8, p_h->padding);
    LT_LOG_INFO("      FW header version:  %02" PRIX8, p_h->header_version);
    LT_LOG_INFO("      Version:            %08" PRIX32, p_h->ver);
    LT_LOG_INFO("      Size:               %08" PRIX32, p_h->size);
    LT_LOG_INFO("      Git hash:           %08" PRIX32, p_h->git_hash);
    // Hash str has 32B
    char hash_str[32 * 2 + 1] = {0};
    for (int i = 0; i < 32; i++) {
        snprintf(hash_str + i * 2, sizeof(hash_str) - i * 2, "%02" PRIX8 "", p_h->hash[i]);
    }
    LT_LOG_INFO("      Hash:          %s", hash_str);
    LT_LOG_INFO("      Pair version:  %08" PRIX32, p_h->pair_version);
}

static void print_all_fw_headers_v2(lt_handle_t *h)
{
    uint8_t header[LT_L2_GET_INFO_FW_HEADER_SIZE] = {0};

    // Read header from FW_BANK_FW1
    LT_LOG_INFO("Reading firmware headers in bank %d", (int)FW_BANK_FW1);
    lt_ret_t ret = lt_get_info_fw_bank(h, FW_BANK_FW1, header, sizeof(header));
    if (ret == LT_OK) {
        print_header_boot_v2_0_1(header, FW_BANK_FW1);
    }
    else {
        LT_LOG_ERROR("Failed to get FW bank %d header, ret=%d", (int)FW_BANK_FW1, ret/*lt_ret_verbose(ret)*/);
        return;
    }

    // Read header from FW_BANK_FW2
    LT_LOG_INFO("Reading firmware headers in bank %d", (int)FW_BANK_FW2);
    memset(header, 0, sizeof(header));
    ret = lt_get_info_fw_bank(h, FW_BANK_FW2, header, sizeof(header));
    if (ret == LT_OK) {
        print_header_boot_v2_0_1(header, FW_BANK_FW2);
    }
    else {
        LT_LOG_ERROR("Failed to get FW bank %d header, ret=%d", (int)FW_BANK_FW2, ret/*lt_ret_verbose(ret)*/);
        return;
    }

    // Read header from FW_BANK_SPECT1
    LT_LOG_INFO("Reading SPECT headers in bank %d", (int)FW_BANK_SPECT1);
    memset(header, 0, sizeof(header));
    ret = lt_get_info_fw_bank(h, FW_BANK_SPECT1, header, sizeof(header));
    if (ret == LT_OK) {
        print_header_boot_v2_0_1(header, FW_BANK_SPECT1);
    }
    else {
        LT_LOG_ERROR("Failed to get SPECT bank %d header, ret=%d", (int)FW_BANK_SPECT1, ret/*lt_ret_verbose(ret)*/);
        return;
    }

    // Read header from FW_BANK_SPECT2
    LT_LOG_INFO("Reading SPECT headers in bank %d", (int)FW_BANK_SPECT2);
    memset(header, 0, sizeof(header));
    ret = lt_get_info_fw_bank(h, FW_BANK_SPECT2, header, sizeof(header));
    if (ret == LT_OK) {
        print_header_boot_v2_0_1(header, FW_BANK_SPECT2);
    }
    else {
        LT_LOG_ERROR("Failed to get SPECT bank %d header, ret=%d", (int)FW_BANK_SPECT2, ret/*lt_ret_verbose(ret)*/);
        return;
    }
}

int lt_ex_show_chip_id_and_fwver(lt_handle_t *h)
{
    LT_LOG_INFO("=============================================================");
    LT_LOG_INFO("==== TROPIC01 show chip ID and firmware versions example ====");
    LT_LOG_INFO("=============================================================");

    // This variable is reused on more places in this example to store different firmware versions
    uint8_t fw_ver[LT_L2_GET_INFO_RISCV_FW_SIZE] = {0};

    lt_ret_t ret = lt_init(h);
    if (ret != LT_OK) {
        LT_LOG_ERROR("Failed to initialize handle, ret=%d", ret/*lt_ret_verbose(ret)*/);
        lt_deinit(h);
        return -1;
    }

    // First we check versions of both updateable firmwares. For this we need to be in APPLICATION mode.
    // If there are valid firmwares, chip will execute them on boot. In any case we will try to reboot into application,
    // in case chip would be in maintenance mode (executing bootloader)
    LT_LOG_INFO("Rebooting into APPLICATION mode to check FW versions");
    ret = lt_reboot(h, LT_MODE_APP);
    if (ret != LT_OK) {
        LT_LOG_ERROR("lt_reboot() failed, ret=%d", ret/*lt_ret_verbose(ret)*/);
        lt_deinit(h);
        return -1;
    }

    if (h->l2.mode == LT_MODE_APP) {
        // App runs so we can see what firmwares are running
        // Getting RISCV app firmware version
        LT_LOG_INFO("Reading RISC-V FW version");
        ret = lt_get_info_riscv_fw_ver(h, fw_ver);
        if (ret == LT_OK) {
            LT_LOG_INFO("Chip is executing RISC-V application FW version: %02" PRIX8 ".%02" PRIX8 ".%02" PRIX8
                        "    (+ .%02" PRIX8 ")",
                        fw_ver[3], fw_ver[2], fw_ver[1], fw_ver[0]);
        }
        else {
            LT_LOG_ERROR("Failed to get RISC-V FW version, ret=%d", ret/*lt_ret_verbose(ret)*/);
            lt_deinit(h);
            return -1;
        }

        LT_LOG_INFO("Reading SPECT FW version");
        ret = lt_get_info_spect_fw_ver(h, fw_ver);
        if (ret == LT_OK) {
            LT_LOG_INFO("Chip is executing SPECT firmware version: %02" PRIX8 ".%02" PRIX8 ".%02" PRIX8
                        "    (+ .%02" PRIX8 ")",
                        fw_ver[3], fw_ver[2], fw_ver[1], fw_ver[0]);
        }
        else {
            LT_LOG_ERROR("Failed to get SPECT firmware version, ret=%d", ret/*lt_ret_verbose(ret)*/);
            lt_deinit(h);
            return -1;
        }
    }
    else {
        LT_LOG_ERROR("Chip couldn't get into APP mode, APP and SPECT firmwares in fw banks are not valid");
    }
    LT_LOG_LINE();

    LT_LOG_INFO("Rebooting into MAINTENANCE mode to check bootloader version and fw bank headers");
    ret = lt_reboot(h, LT_MODE_MAINTENANCE);
    if (ret != LT_OK) {
        LT_LOG_ERROR("Failed to reboot into MAINTENANCE mode, ret=%d", ret/*lt_ret_verbose(ret)*/);
        lt_deinit(h);
        return -1;
    }

    if (h->l2.mode == LT_MODE_MAINTENANCE) {
        LT_LOG_INFO("Reading RISC-V FW version for bootloader version");
        ret = lt_get_info_riscv_fw_ver(h, fw_ver);
        if (ret != LT_OK) {
            LT_LOG_ERROR("Failed to get RISC-V FW version, ret=%d", ret/*lt_ret_verbose(ret)*/);
            lt_deinit(h);
            return -1;
        }

        // Checking if bootloader version is 1.0.1
        if (((fw_ver[3] & 0x7f) == 1) && (fw_ver[2] == 0) && (fw_ver[1] == 1) && (fw_ver[0] == 0)) {
            LT_LOG_INFO("Bootloader version: %02" PRIX8 ".%02" PRIX8 ".%02" PRIX8 "    (+ .%02" PRIX8 ")",
                        fw_ver[3] & 0x7f, fw_ver[2], fw_ver[1], fw_ver[0]);

            print_all_headers_v1(h);
        }
        else {
            // Checking if bootloader version is 2.0.1
            if (((fw_ver[3] & 0x7f) == 2) && (fw_ver[2] == 0) && (fw_ver[1] == 1) && (fw_ver[0] == 0)) {
                LT_LOG_INFO("Bootloader version: %02" PRIX8 ".%02" PRIX8 ".%02" PRIX8 "    (+ .%02" PRIX8 ")",
                            fw_ver[3] & 0x7f, fw_ver[2], fw_ver[1], fw_ver[0]);
                print_all_fw_headers_v2(h);
            }
            else {
                LT_LOG_ERROR("Unknown bootloader version: %02" PRIX8 ".%02" PRIX8 ".%02" PRIX8 ".%02" PRIX8,
                             fw_ver[3] & 0x7f, fw_ver[2], fw_ver[1], fw_ver[0]);
                lt_deinit(h);
                return -1;
            }
            LT_LOG_LINE();
        }
    }
    else {
        LT_LOG_ERROR("Chip couldn't get into MAINTENANCE mode");
        lt_deinit(h);
        return -1;
    }
    LT_LOG_LINE();

    struct lt_chip_id_t chip_id = {0};

    ret = lt_get_info_chip_id(h, &chip_id);
    if (ret != LT_OK) {
        LT_LOG_ERROR("Failed to get chip ID, ret=%d", ret/*lt_ret_verbose(ret)*/);
        lt_deinit(h);
        return -1;
    }

    ret = lt_print_chip_id(&chip_id, serial_print_func);
   
    if (ret != LT_OK) {
        LT_LOG_ERROR("Failed to print chip ID, ret=%d", ret/*lt_ret_verbose(ret)*/);
        lt_deinit(h);
        return -1;
    }
    LT_LOG_LINE();

    LT_LOG_INFO("Deinitializing handle");
    ret = lt_deinit(h);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Failed to deinitialize handle, ret=%d", ret/*lt_ret_verbose(ret)*/);
        return -1;
    }

    return 0;
}


/** @brief Message to send with Ping L3 command. */
#define PING_MSG "This is Hello World message from TROPIC01!!"
/** @brief Size of the Ping message, including '\0'. */
#define PING_MSG_SIZE 44

int lt_ex_hello_world(lt_handle_t *h)
{
    LT_LOG_INFO("======================================");
    LT_LOG_INFO("==== TROPIC01 Hello World Example ====");
    LT_LOG_INFO("======================================");

    lt_ret_t ret;

    LT_LOG_INFO("Initializing handle");
    ret = lt_init(h);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Failed to initialize handle, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }

    LT_LOG_INFO("Starting Secure Session with key %d", (int)PAIRING_KEY_SLOT_INDEX_0);
    ret = lt_verify_chip_and_start_secure_session(h, sh0priv, sh0pub, PAIRING_KEY_SLOT_INDEX_0);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Failed to start Secure Session with key %d, ret=%s", (int)PAIRING_KEY_SLOT_INDEX_0,
                     lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }
    LT_LOG_LINE();

    uint8_t recv_buf[PING_MSG_SIZE];
    LT_LOG_INFO("Sending Ping command with message:");
    LT_LOG_INFO("\t\"%s\"", PING_MSG);
    ret = lt_ping(h, (const uint8_t *)PING_MSG, recv_buf, PING_MSG_SIZE);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Ping command failed, ret=%s", lt_ret_verbose(ret));
        lt_session_abort(h);
        lt_deinit(h);
        return -1;
    }
    LT_LOG_LINE();

    LT_LOG_INFO("Message received from TROPIC01:");
    LT_LOG_INFO("\t\"%s\"", recv_buf);
    LT_LOG_LINE();

    LT_LOG_INFO("Aborting Secure Session");
    ret = lt_session_abort(h);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Failed to abort Secure Session, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }

    LT_LOG_INFO("Deinitializing handle");
    ret = lt_deinit(h);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Failed to deinitialize handle, ret=%s", lt_ret_verbose(ret));
        return -1;
    }

    return 0;
}

int lt_ex_hello_world_separate_API(lt_handle_t *h)
{
    LT_LOG_INFO("=========================================================");
    LT_LOG_INFO("====  TROPIC01 Hello World with Separate API Example ====");
    LT_LOG_INFO("=========================================================");

    lt_ret_t ret;

    LT_LOG_INFO("Initializing handle");
    ret = lt_init(h);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Failed to initialize handle, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }

    LT_LOG_INFO("Getting Certificate Store from TROPIC01");
    uint8_t cert1[LT_L2_GET_INFO_REQ_CERT_SIZE_SINGLE], cert2[LT_L2_GET_INFO_REQ_CERT_SIZE_SINGLE],
        cert3[LT_L2_GET_INFO_REQ_CERT_SIZE_SINGLE], cert4[LT_L2_GET_INFO_REQ_CERT_SIZE_SINGLE];
    struct lt_cert_store_t store
        = {.certs = {cert1, cert2, cert3, cert4},
           .buf_len = {LT_L2_GET_INFO_REQ_CERT_SIZE_SINGLE, LT_L2_GET_INFO_REQ_CERT_SIZE_SINGLE,
                       LT_L2_GET_INFO_REQ_CERT_SIZE_SINGLE, LT_L2_GET_INFO_REQ_CERT_SIZE_SINGLE}};
    ret = lt_get_info_cert_store(h, &store);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Failed to get Certificate Store, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }

    // Get only stpub, we don't verify certificate chain here
    LT_LOG_INFO("Getting stpub key from Certificate Store");
    uint8_t stpub[32];
    ret = lt_get_st_pub(&store, stpub, 32);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Failed to get stpub key, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }
    LT_LOG_LINE();

    //---------------------------------------------------------------------------------------//
    // Separated API calls for starting a secure session:
    session_state_t state = {0};

    // Inicialize session from a server side by creating state->ehpriv and state->ehpub,
    // l2 request is prepared into handle's buffer (h->l2_buff)
    LT_LOG_INFO("Executing lt_out__session_start()...");
    ret = lt_out__session_start(h, PAIRING_KEY_SLOT_INDEX_0, &state);
    if (LT_OK != ret) {
        LT_LOG_ERROR("lt_out__session_start() failed, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }

    // handle's buffer (h->l2_buff) now contains data which must be transferred over tunnel to TROPIC01

    // Following l2 functions are called on remote host
    LT_LOG_INFO("Executing lt_l2_send()...");
    ret = lt_l2_send(&h->l2);
    if (LT_OK != ret) {
        LT_LOG_ERROR("lt_l2_send() failed, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }
    LT_LOG_INFO("Executing lt_l2_receive()...");
    ret = lt_l2_receive(&h->l2);
    if (LT_OK != ret) {
        LT_LOG_ERROR("lt_l2_receive() failed, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }

    // Handle's buffer (h->l2_buff) now contains data which must be transferred over tunnel back to the server

    // Once data are back on server's side, bytes are copied into h->l2_buff
    // Then following l2 function is called on server side
    // This function establishes gcm contexts for a session
    LT_LOG_INFO("Executing lt_in__session_start()...");
    ret = lt_in__session_start(h, serial_print_func, stpub, PAIRING_KEY_SLOT_INDEX_0, sh0priv, sh0pub, &state);
    if (LT_OK != ret) {
        LT_LOG_ERROR("lt_in__session_start failed, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }
    LT_LOG_LINE();

    // Now we can use lt_ping() to send a message to TROPIC01 and receive a response, this is done with separate API
    // calls
    uint8_t recv_buf[PING_MSG_SIZE];
    LT_LOG_INFO("Executing lt_out__ping() with message:");
    LT_LOG_INFO("\t\"%s\"", PING_MSG);
    ret = lt_out__ping(h, (const uint8_t *)PING_MSG, PING_MSG_SIZE);
    if (LT_OK != ret) {
        LT_LOG_ERROR("lt_out__ping failed, ret=%s", lt_ret_verbose(ret));
        lt_session_abort(h);
        lt_deinit(h);
        return -1;
    }

    LT_LOG_INFO("Executing lt_l2_send_encrypted_cmd()...");
    ret = lt_l2_send_encrypted_cmd(&h->l2, h->l3.buff, h->l3.buff_len);
    if (LT_OK != ret) {
        LT_LOG_ERROR("lt_l2_send_encrypted_cmd failed, ret=%s", lt_ret_verbose(ret));
        lt_session_abort(h);
        lt_deinit(h);
        return -1;
    }
    LT_LOG_INFO("Executing lt_l2_recv_encrypted_res()...");
    ret = lt_l2_recv_encrypted_res(&h->l2, h->l3.buff, h->l3.buff_len);
    if (LT_OK != ret) {
        LT_LOG_ERROR("lt_l2_recv_encrypted_res failed, ret=%s", lt_ret_verbose(ret));
        lt_session_abort(h);
        lt_deinit(h);
        return -1;
    }

    LT_LOG_INFO("Executing lt_in__ping()...");
    ret = lt_in__ping(h, recv_buf, PING_MSG_SIZE);
    if (LT_OK != ret) {
        LT_LOG_ERROR("lt_in__ping failed, ret=%s", lt_ret_verbose(ret));
        lt_session_abort(h);
        lt_deinit(h);
        return -1;
    }

    LT_LOG_INFO("Message received from TROPIC01:");
    LT_LOG_INFO("\t\"%s\"", recv_buf);
    LT_LOG_LINE();

    LT_LOG_INFO("Aborting Secure Session");
    ret = lt_session_abort(h);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Failed to abort Secure Session, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }

    LT_LOG_INFO("Deinitializing handle");
    ret = lt_deinit(h);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Failed to deinitialize handle, ret=%s", lt_ret_verbose(ret));
        return -1;
    }

    return 0;
}

Libtropic::Libtropic() {}

void Libtropic::begin() {
   // libtropic related code BEGIN
   lt_handle_t __lt_handle__ = {0};

#if LT_SEPARATE_L3_BUFF
    uint8_t l3_buffer[L3_PACKET_MAX_SIZE] __attribute__((aligned(16))) = {0};
    __lt_handle__.l3.buff = l3_buffer;
    __lt_handle__.l3.buff_len = sizeof(l3_buffer);
#endif

    // Device (adapted for Pico)
    lt_dev_pico device = {0};

    device.spi_instance = LT_SPI_PORT;
    device.spi_baudrate = SPI_BAUDRATE;
    device.cs_pin = SPI_CS_PIN;
    device.pin_miso = SPI_MISO_PIN;
    device.pin_mosi = SPI_MOSI_PIN;
    device.pin_sck = SPI_SCK_PIN;

    __lt_handle__.l2.device = &device;

#ifdef LT_BUILD_EXAMPLES
    #include "lt_ex_registry.c.inc"
    UNUSED(__lt_ex_return_val__);
#endif

//    lt_ex_show_chip_id_and_fwver(&__lt_handle__);

//    delay(5000);
   
//    lt_ex_hello_world(&__lt_handle__);

   lt_ex_hello_world_separate_API(&__lt_handle__);
   // libtropic related code END

}

