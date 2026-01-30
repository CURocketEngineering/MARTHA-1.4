// The MIT License (MIT)
// Copyright (c) 2019 Ha Thach for Adafruit Industries
#include <unity.h>

#include <SPI.h>
#include <SdFat.h>

#include <Adafruit_SPIFlash.h>

// for flashTransport definition
#include "flash_config.h"

#define TEST_ADDRESS 0x00010000 // Example address for testing
#define TEST_DATA 0xA5         // Example test data pattern

Adafruit_SPIFlash flash(&flashTransport);

void test_flash_init() {
    TEST_ASSERT_EQUAL(true, flash.begin());
}

// Test JEDEC ID
void test_flash_correct_JEDEC_ID() {
    TEST_ASSERT_EQUAL(EXPECTED_JEDEC_ID, flash.getJEDECID());
}

// Test flash size
void test_flash_size_greater_than_zero() {
    TEST_ASSERT_GREATER_THAN(0, flash.size());
}

// Test write and read
void test_flash_write_read() {
    uint8_t write_data = TEST_DATA;
    uint8_t read_data = 0;

    // Erase a sector to ensure clean write
    TEST_ASSERT_EQUAL(true, flash.eraseSector(TEST_ADDRESS / SFLASH_SECTOR_SIZE));

    // Write data to the flash
    TEST_ASSERT_EQUAL(true, flash.writeBuffer(TEST_ADDRESS, &write_data, 1));

    // Read back the data
    TEST_ASSERT_EQUAL(true, flash.readBuffer(TEST_ADDRESS, &read_data, 1));

    // Verify the written and read data are the same
    TEST_ASSERT_EQUAL(write_data, read_data);
}

// NOR flash cannot change 0 bits back to 1 without erase.
// This catches “not actually flash” behavior and some wiring issues.
void test_flash_cannot_program_zeros_back_to_ones_without_erase() {
  uint8_t v = 0;

  TEST_ASSERT_TRUE(flash.eraseSector(TEST_ADDRESS / SFLASH_SECTOR_SIZE));

  uint8_t zero = 0x00;
  TEST_ASSERT_TRUE(flash.writeBuffer(TEST_ADDRESS, &zero, 1));
  TEST_ASSERT_TRUE(flash.readBuffer(TEST_ADDRESS, &v, 1));
  TEST_ASSERT_EQUAL_HEX8(0x00, v);

  uint8_t ones = 0xFF;
  TEST_ASSERT_TRUE(flash.writeBuffer(TEST_ADDRESS, &ones, 1));
  TEST_ASSERT_TRUE(flash.readBuffer(TEST_ADDRESS, &v, 1));

  // Expect it to still be 0x00 (or at least not 0xFF). Most parts will read 0x00 here.
  TEST_ASSERT_EQUAL_HEX8(0x00, v);
}

// Two-sector test: ensures addressing and sector decoding look sane
void test_flash_two_sectors_independent() {
  // Skip if device is too small for the second sector test region.
  if (flash.size() < (TEST_ADDRESS_2 + kTestLen)) {
    TEST_IGNORE_MESSAGE("Flash too small for two-sector test");
    return;
  }

  uint8_t w1[kTestLen], r1[kTestLen];
  uint8_t w2[kTestLen], r2[kTestLen];

  fill_pattern(w1, kTestLen, 0x11);
  fill_pattern(w2, kTestLen, 0x22);
  memset(r1, 0, sizeof(r1));
  memset(r2, 0, sizeof(r2));

  TEST_ASSERT_TRUE(flash.eraseSector(TEST_ADDRESS / SFLASH_SECTOR_SIZE));
  TEST_ASSERT_TRUE(flash.eraseSector(TEST_ADDRESS_2 / SFLASH_SECTOR_SIZE));

  TEST_ASSERT_TRUE(flash.writeBuffer(TEST_ADDRESS, w1, kTestLen));
  TEST_ASSERT_TRUE(flash.writeBuffer(TEST_ADDRESS_2, w2, kTestLen));

  TEST_ASSERT_TRUE(flash.readBuffer(TEST_ADDRESS, r1, kTestLen));
  TEST_ASSERT_TRUE(flash.readBuffer(TEST_ADDRESS_2, r2, kTestLen));

  assert_buf_eq(w1, r1, kTestLen);
  assert_buf_eq(w2, r2, kTestLen);
}

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(100); // wait for native usb
    }
    
    UNITY_BEGIN();
    RUN_TEST(test_flash_init);
    RUN_TEST(test_flash_correct_JEDEC_ID);
    RUN_TEST(test_flash_size_greater_than_zero);
    RUN_TEST(test_flash_write_read);

    Serial.println("Adafruit Serial Flash Info example");
    uint32_t jedec_id = flash.getJEDECID();
    Serial.print("JEDEC ID: 0x");
    Serial.println(jedec_id, HEX);
    Serial.print("Flash size (usable): ");
    Serial.print(flash.size() / 1024);
    Serial.println(" KB");

    UNITY_END();
}

void loop() {
  // nothing to do
}