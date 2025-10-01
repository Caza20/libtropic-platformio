#ifndef LIBTROPIC_PLATFORMIO_HPP
#define LIBTROPIC_PLATFORMIO_HPP

extern "C" {
    #include "../libtropic/include/libtropic.h"
    #include "../libtropic/include/libtropic_logging.h"
    #include "../libtropic/include/libtropic_common.h"
    #include "../libtropic/include/libtropic_examples.h"
    #include "../libtropic/include/lt_l2.h"
    #include "../libtropic/include/lt_l3.h"
    #include "../libtropic/TROPIC01_fw_update_files/boot_v_1_0_1/fw_v_1_0_0/fw_CPU.h"
    #include "../libtropic/TROPIC01_fw_update_files/boot_v_1_0_1/fw_v_1_0_0/fw_SPECT.h"
    #include "../libtropic/hal/port/rpi_pico/lt_port_rpi_pico.h"
    #include "string.h"
    #include <inttypes.h>
}

#include "functions/show_chip_id_and_fwver.hpp"
#include "functions/secure_session_and_ping.hpp"

#include "commands/cmd_chip_id_func.hpp"

/* ---------------- SPI ---------------- */
#define LT_SPI_PORT spi0
#define SPI_BAUDRATE 1000000  // 1 MHz, can be adjusted according to the device

// SPI pins (adjust them to your actual connection)
#define SPI_SCK_PIN  2   // GPIO2 → SCK
#define SPI_MOSI_PIN 3   // GPIO3 → MOSI - SDI
#define SPI_MISO_PIN 4   // GPIO4 → MISO - SDO
#define SPI_CS_PIN   5   // GPIO5 → Chip Select

/* ---------------- UART 1 ---------------- */
#define LT_UART_PORT uart1
#define UART_BAUDRATE 115200  

// UART pins (adjust them to your actual connection)
#define UART_TX_PIN 8   // GPIO8 → UART1_TX
#define UART_RX_PIN 9   // GPIO9 → UART1_RX


/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

extern lt_handle_t __lt_handle__; 

class Libtropic {
public:
    Libtropic();
    //* Initialize libtropic and UART
    void begin(int tx, int rx, uint32_t baud = 9600, uart_inst_t* uart_id = uart1);

    //* UART functions
    void sendData(const String& data);
    String readData();

    //* Commands
    void processCommand(const String &commands);

    //* libtropic functions
    void showChipIdAndFwVer();
    void secureSessionAndPing();

private:
    
    lt_dev_pico __device__;

    SerialUART* serialPort;
    String inputDataBuffer = "";

    //* Commands
    enum CommandId { 
        CMD_UNKNOWN, 
        CMD_CHIP_ID, 
        CMD_FW_VERSION, 
        CMD_SESSION_PING 
    };

    CommandId parseCommand(const String &cmd);
    void handleCommand(CommandId id, const String &originalCmd);
};

#endif
