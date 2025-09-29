#include "libtropic_platfomio.hpp"
#include <stdio.h>
#include <Arduino.h>

// Only for FW update function
#define FW_APP_UPDATE_BANK FW_BANK_FW1
#define FW_SPECT_UPDATE_BANK FW_BANK_SPECT1


Libtropic::Libtropic() {
    serialPort = nullptr;
}

void Libtropic::begin(int tx, int rx, uint32_t baud, uart_inst_t* uart_id) {
    serialPort = new SerialUART(uart_id, tx, rx);
    serialPort->begin(baud, SERIAL_8N1);

    Serial.println("============== Inizializing TROPIC01... ==============");

   // libtropic related code BEGIN
   __lt_handle__ = {0};

#if LT_SEPARATE_L3_BUFF
    uint8_t l3_buffer[L3_PACKET_MAX_SIZE] __attribute__((aligned(16))) = {0};
    __lt_handle__.l3.buff = l3_buffer;
    __lt_handle__.l3.buff_len = sizeof(l3_buffer);
#endif

    // Device (adapted for Pico)
    __device__ = {0};

    __device__.spi_instance = LT_SPI_PORT;
    __device__.spi_baudrate = SPI_BAUDRATE;
    __device__.cs_pin = SPI_CS_PIN;
    __device__.pin_miso = SPI_MISO_PIN;
    __device__.pin_mosi = SPI_MOSI_PIN;
    __device__.pin_sck = SPI_SCK_PIN;

    __lt_handle__.l2.device = &__device__;

#ifdef LT_BUILD_EXAMPLES
    #include "lt_ex_registry.c.inc"
    UNUSED(__lt_ex_return_val__);
#endif

//    lt_show_chip_id_and_fwver(&__lt_handle__);

//    delay(5000);
   
//    lt_ex_hello_world(&__lt_handle__);

//    lt_ex_hello_world_separate_API(&__lt_handle__);

    // lt_ex_fw_update(&__lt_handle__);
   // libtropic related code END

}

//**************************************************
//*               UART functions
//************************************************** 

// Send data via UART
void Libtropic::sendData(const String& data) {
      if (serialPort) serialPort->print(data);
}

// Read data from UART
String Libtropic::readData() {
  if (serialPort) {
    while (serialPort->available()) {
      char c = (char)serialPort->read();
      if (c == ';') {  // end of the command
        String cmd = inputDataBuffer;
        inputDataBuffer = "";
        return cmd;
      } else {
        inputDataBuffer += c;
      }
    }
  }
  return "";  // no data available
}


//**************************************************
//*           libtropic functions
//************************************************** 
void Libtropic::showChipIdAndFwVer() {
    Serial.println("============== Reading Chip ID and FW version... ==============");
    lt_show_chip_id_and_fwver(&__lt_handle__);
}

void Libtropic::secureSessionAndPing() {
    Serial.println("============== Establishing secure session and ping... ==============");
    lt_secure_session_and_ping(&__lt_handle__);
}

int lt_ex_fw_update(lt_handle_t *h)
{
    LT_LOG_INFO("====================================");
    LT_LOG_INFO("==== TROPIC01 FW update Example ====");
    LT_LOG_INFO("====================================");

    lt_ret_t ret;

    LT_LOG_INFO("Initializing handle");
    ret = lt_init(h);
    if (LT_OK != ret) {
        LT_LOG_ERROR("Failed to initialize handle, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }

    // Reused variable
    uint8_t fw_ver[LT_L2_GET_INFO_RISCV_FW_SIZE] = {0};

    // For firmware update chip must be rebooted into MAINTENANCE mode.
    LT_LOG_INFO("Rebooting into Maintenance mode");
    ret = lt_reboot(h, LT_MODE_MAINTENANCE);
    if (ret != LT_OK) {
        LT_LOG_ERROR("lt_reboot() failed, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }

    if (h->l2.mode == LT_MODE_MAINTENANCE) {
        LT_LOG_INFO("Chip is executing bootloader");

        LT_LOG_INFO("Updating RISC-V FW");
        ret = lt_do_mutable_fw_update(h, fw_CPU, sizeof(fw_CPU), FW_APP_UPDATE_BANK);
        if (ret != LT_OK) {
            LT_LOG_ERROR("RISC-V FW update failed, ret=%s", lt_ret_verbose(ret));
            lt_deinit(h);
            return -1;
        }

        LT_LOG_INFO("Updating SPECT FW");
        ret = lt_do_mutable_fw_update(h, fw_SPECT, sizeof(fw_SPECT), FW_SPECT_UPDATE_BANK);
        if (ret != LT_OK) {
            LT_LOG_ERROR("SPECT FW update failed, ret=%s", lt_ret_verbose(ret));
            lt_deinit(h);
            return -1;
        }
    }
    else {
        LT_LOG_ERROR("Chip couldn't get into MAINTENANCE mode");
        lt_deinit(h);
        return -1;
    }
    LT_LOG_LINE();

    // To read firmware versions chip must be rebooted into application mode.
    LT_LOG_INFO("Rebooting into Application mode");
    ret = lt_reboot(h, LT_MODE_APP);
    if (ret != LT_OK) {
        LT_LOG_ERROR("lt_reboot() failed, ret=%s", lt_ret_verbose(ret));
        lt_deinit(h);
        return -1;
    }

    if (h->l2.mode == LT_MODE_APP) {
        LT_LOG_INFO("Reading RISC-V FW version");
        ret = lt_get_info_riscv_fw_ver(h, fw_ver);
        if (ret == LT_OK) {
            LT_LOG_INFO("Chip is executing RISC-V application FW version: %" PRIu8 ".%" PRIu8 ".%" PRIu8
                        "    (+ .%" PRIu8 ")",
                        fw_ver[3], fw_ver[2], fw_ver[1], fw_ver[0]);
        }
        else {
            LT_LOG_ERROR("Failed to get RISC-V FW version, ret=%s", lt_ret_verbose(ret));
            lt_deinit(h);
            return -1;
        }

        LT_LOG_INFO("Reading SPECT FW version");
        ret = lt_get_info_spect_fw_ver(h, fw_ver);
        if (ret == LT_OK) {
            LT_LOG_INFO("Chip is executing SPECT FW version: %" PRIu8 ".%" PRIu8 ".%" PRIu8 "    (+ .%" PRIu8 ")",
                        fw_ver[3], fw_ver[2], fw_ver[1], fw_ver[0]);
        }
        else {
            LT_LOG_ERROR("Failed to get SPECT FW version, ret=%s", lt_ret_verbose(ret));
            lt_deinit(h);
            return -1;
        }
    }
    else {
        LT_LOG_ERROR(
            "Device couldn't get into APP mode, APP and SPECT firmwares in fw banks are not valid or banks are empty");
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