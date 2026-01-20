#include <Arduino.h>
#include "main.hpp"

uartFunctions uart;  // Create an instance of uartFunctions

Libtropic tropic;

// The Pico W LED is GPIO25, same for the regular Pico.
#define LED_PIN 25

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 1);

  uart.begin(UART_TX_PIN, UART_RX_PIN, UART_BAUDRATE, LT_UART_PORT);

  tropic.begin();

  // to have a moment to open the serial monitor
  delay(5000);

  uart.sendData("Hello from Raspberry Pi Pico with libtropic!\n");

  // Initiate secure session
  uart.sendData("Starting secure session...\n");
  uart.sendData(cmd_secure_session_func(true));

  // Example of sending a hash command
  // Convert message to byte array
  String input = "HelloWorld!";
  uint8_t* msg_bytes = new uint8_t[input.length()];
  for (uint16_t i = 0; i < input.length(); i++) {
    msg_bytes[i] = (uint8_t)input[i];
  }
  // Have the hash
  String hashMessage = cmd_hash_func(msg_bytes, input.length());
  // Clean "OK:hash:0x....;" response
  String hashClean = hashMessage.substring(8, hashMessage.length() - 1);
  uart.sendData("Hash of 'HelloWorld' is: " + hashClean + "\n");


  // Performance test for EDDSA signing
  String signature[64];
  int iterations = 100;

  // Convert message to byte array
  uint16_t msg_len = hashClean.length();
  uint8_t* msg_to_bytes = new uint8_t[msg_len];
  for (uint16_t i = 0; i < msg_len; i++) {
    msg_to_bytes[i] = (uint8_t)hashClean[i];
  }

  uart.sendData("Initialize performance test...\n");
  unsigned long start_time = micros(); // Use micros for higher precision

  for (int i = 0; i < iterations; i++) {
    // Call signing function
    cmd_sign_eddsa_func(ecc_slot_t(0), msg_to_bytes, msg_len);
  }

  unsigned long end_time = micros();
  unsigned long total_time = end_time - start_time;
    
  float seconds = total_time / 1000000.0;
  float tps = iterations / seconds;

  // Show results
  uart.sendData("Total time for 100 signs: ");
  uart.sendData(String(seconds, 4));
  uart.sendData(" seconds\n");
  uart.sendData("Signs for seconds: ");
  uart.sendData(String(tps));
    
  delay(10000);


}

void loop() {
    
}