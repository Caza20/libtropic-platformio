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

/* ---------------- SPI ---------------- */
#define LT_SPI_PORT spi0
#define SPI_BAUDRATE 1000000  // 1 MHz, can be adjusted according to the device

// SPI pins (adjust them to your actual connection)
#define SPI_SCK_PIN  2   // GPIO2 → SCK
#define SPI_MOSI_PIN 3   // GPIO3 → MOSI
#define SPI_MISO_PIN 4   // GPIO4 → MISO
#define SPI_CS_PIN   5   // GPIO5 → Chip Select

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))


class Libtropic {
public:
    Libtropic();
    void begin();
    void showChipIdAndFwVer();
    void secureSessionAndPing();

private:
    lt_handle_t __lt_handle__;
    lt_dev_pico __device__;
};

#endif
