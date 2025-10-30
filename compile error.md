382 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 511);
      |                                      ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:382:20: error: 'SPI_MOSI_DLEN_REG' was not declared in this scope; did you mean 'SPI_MS_DLEN_REG'?
  382 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 511);
      |                    ^~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:382:5: note: in expansion of macro 'WRITE_PERI_REG'
  382 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 511);
      |     ^~~~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:385:14: note: in expansion of macro 'READ_PERI_REG'
  385 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |              ^~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:17:25: note: in expansion of macro 'REG_SPI_BASE'
   17 | #define SPI_CMD_REG(i) (REG_SPI_BASE(i) + 0x0)
      |                         ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:385:28: note: in expansion of macro 'SPI_CMD_REG'
  385 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                            ^~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:385:40: note: in expansion of macro 'SPI_PORT'
  385 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                        ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:385:50: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  385 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:386:7: note: in expansion of macro 'WRITE_PERI_REG'
  386 |       WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1751:24: note: in expansion of macro 'REG_SPI_BASE'
 1751 | #define SPI_W0_REG(i) (REG_SPI_BASE(i) + 0x98)
      |                        ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:386:22: note: in expansion of macro 'SPI_W0_REG'
  386 |       WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:386:33: note: in expansion of macro 'SPI_PORT'
  386 |       WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  *data++);
      |                                 ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:387:7: note: in expansion of macro 'WRITE_PERI_REG'
  387 |       WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1763:24: note: in expansion of macro 'REG_SPI_BASE'
 1763 | #define SPI_W1_REG(i) (REG_SPI_BASE(i) + 0x9c)
      |                        ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:387:22: note: in expansion of macro 'SPI_W1_REG'
  387 |       WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:387:33: note: in expansion of macro 'SPI_PORT'
  387 |       WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  *data++);
      |                                 ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:388:7: note: in expansion of macro 'WRITE_PERI_REG'
  388 |       WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1775:24: note: in expansion of macro 'REG_SPI_BASE'
 1775 | #define SPI_W2_REG(i) (REG_SPI_BASE(i) + 0xa0)
      |                        ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:388:22: note: in expansion of macro 'SPI_W2_REG'
  388 |       WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:388:33: note: in expansion of macro 'SPI_PORT'
  388 |       WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  *data++);
      |                                 ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:389:7: note: in expansion of macro 'WRITE_PERI_REG'
  389 |       WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1787:24: note: in expansion of macro 'REG_SPI_BASE'
 1787 | #define SPI_W3_REG(i) (REG_SPI_BASE(i) + 0xa4)
      |                        ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:389:22: note: in expansion of macro 'SPI_W3_REG'
  389 |       WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:389:33: note: in expansion of macro 'SPI_PORT'
  389 |       WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  *data++);
      |                                 ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:390:7: note: in expansion of macro 'WRITE_PERI_REG'
  390 |       WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1799:24: note: in expansion of macro 'REG_SPI_BASE'
 1799 | #define SPI_W4_REG(i) (REG_SPI_BASE(i) + 0xa8)
      |                        ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:390:22: note: in expansion of macro 'SPI_W4_REG'
  390 |       WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:390:33: note: in expansion of macro 'SPI_PORT'
  390 |       WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  *data++);
      |                                 ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:391:7: note: in expansion of macro 'WRITE_PERI_REG'
  391 |       WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1811:24: note: in expansion of macro 'REG_SPI_BASE'
 1811 | #define SPI_W5_REG(i) (REG_SPI_BASE(i) + 0xac)
      |                        ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:391:22: note: in expansion of macro 'SPI_W5_REG'
  391 |       WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:391:33: note: in expansion of macro 'SPI_PORT'
  391 |       WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  *data++);
      |                                 ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:392:7: note: in expansion of macro 'WRITE_PERI_REG'
  392 |       WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1823:24: note: in expansion of macro 'REG_SPI_BASE'
 1823 | #define SPI_W6_REG(i) (REG_SPI_BASE(i) + 0xb0)
      |                        ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:392:22: note: in expansion of macro 'SPI_W6_REG'
  392 |       WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:392:33: note: in expansion of macro 'SPI_PORT'
  392 |       WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  *data++);
      |                                 ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:393:7: note: in expansion of macro 'WRITE_PERI_REG'
  393 |       WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1835:24: note: in expansion of macro 'REG_SPI_BASE'
 1835 | #define SPI_W7_REG(i) (REG_SPI_BASE(i) + 0xb4)
      |                        ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:393:22: note: in expansion of macro 'SPI_W7_REG'
  393 |       WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:393:33: note: in expansion of macro 'SPI_PORT'
  393 |       WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  *data++);
      |                                 ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:394:7: note: in expansion of macro 'WRITE_PERI_REG'
  394 |       WRITE_PERI_REG(SPI_W8_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1847:24: note: in expansion of macro 'REG_SPI_BASE'
 1847 | #define SPI_W8_REG(i) (REG_SPI_BASE(i) + 0xb8)
      |                        ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:394:22: note: in expansion of macro 'SPI_W8_REG'
  394 |       WRITE_PERI_REG(SPI_W8_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:394:33: note: in expansion of macro 'SPI_PORT'
  394 |       WRITE_PERI_REG(SPI_W8_REG(SPI_PORT),  *data++);
      |                                 ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:395:7: note: in expansion of macro 'WRITE_PERI_REG'
  395 |       WRITE_PERI_REG(SPI_W9_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1859:24: note: in expansion of macro 'REG_SPI_BASE'
 1859 | #define SPI_W9_REG(i) (REG_SPI_BASE(i) + 0xbc)
      |                        ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:395:22: note: in expansion of macro 'SPI_W9_REG'
  395 |       WRITE_PERI_REG(SPI_W9_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:395:33: note: in expansion of macro 'SPI_PORT'
  395 |       WRITE_PERI_REG(SPI_W9_REG(SPI_PORT),  *data++);
      |                                 ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:396:7: note: in expansion of macro 'WRITE_PERI_REG'
  396 |       WRITE_PERI_REG(SPI_W10_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1871:25: note: in expansion of macro 'REG_SPI_BASE'
 1871 | #define SPI_W10_REG(i) (REG_SPI_BASE(i) + 0xc0)
      |                         ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:396:22: note: in expansion of macro 'SPI_W10_REG'
  396 |       WRITE_PERI_REG(SPI_W10_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:396:34: note: in expansion of macro 'SPI_PORT'
  396 |       WRITE_PERI_REG(SPI_W10_REG(SPI_PORT), *data++);
      |                                  ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:397:7: note: in expansion of macro 'WRITE_PERI_REG'
  397 |       WRITE_PERI_REG(SPI_W11_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1883:25: note: in expansion of macro 'REG_SPI_BASE'
 1883 | #define SPI_W11_REG(i) (REG_SPI_BASE(i) + 0xc4)
      |                         ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:397:22: note: in expansion of macro 'SPI_W11_REG'
  397 |       WRITE_PERI_REG(SPI_W11_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:397:34: note: in expansion of macro 'SPI_PORT'
  397 |       WRITE_PERI_REG(SPI_W11_REG(SPI_PORT), *data++);
      |                                  ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:398:7: note: in expansion of macro 'WRITE_PERI_REG'
  398 |       WRITE_PERI_REG(SPI_W12_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1895:25: note: in expansion of macro 'REG_SPI_BASE'
 1895 | #define SPI_W12_REG(i) (REG_SPI_BASE(i) + 0xc8)
      |                         ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:398:22: note: in expansion of macro 'SPI_W12_REG'
  398 |       WRITE_PERI_REG(SPI_W12_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:398:34: note: in expansion of macro 'SPI_PORT'
  398 |       WRITE_PERI_REG(SPI_W12_REG(SPI_PORT), *data++);
      |                                  ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:399:7: note: in expansion of macro 'WRITE_PERI_REG'
  399 |       WRITE_PERI_REG(SPI_W13_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1907:25: note: in expansion of macro 'REG_SPI_BASE'
 1907 | #define SPI_W13_REG(i) (REG_SPI_BASE(i) + 0xcc)
      |                         ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:399:22: note: in expansion of macro 'SPI_W13_REG'
  399 |       WRITE_PERI_REG(SPI_W13_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:399:34: note: in expansion of macro 'SPI_PORT'
  399 |       WRITE_PERI_REG(SPI_W13_REG(SPI_PORT), *data++);
      |                                  ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:400:7: note: in expansion of macro 'WRITE_PERI_REG'
  400 |       WRITE_PERI_REG(SPI_W14_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1919:25: note: in expansion of macro 'REG_SPI_BASE'
 1919 | #define SPI_W14_REG(i) (REG_SPI_BASE(i) + 0xd0)
      |                         ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:400:22: note: in expansion of macro 'SPI_W14_REG'
  400 |       WRITE_PERI_REG(SPI_W14_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:400:34: note: in expansion of macro 'SPI_PORT'
  400 |       WRITE_PERI_REG(SPI_W14_REG(SPI_PORT), *data++);
      |                                  ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:401:7: note: in expansion of macro 'WRITE_PERI_REG'
  401 |       WRITE_PERI_REG(SPI_W15_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1931:25: note: in expansion of macro 'REG_SPI_BASE'
 1931 | #define SPI_W15_REG(i) (REG_SPI_BASE(i) + 0xd4)
      |                         ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:401:22: note: in expansion of macro 'SPI_W15_REG'
  401 |       WRITE_PERI_REG(SPI_W15_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:401:34: note: in expansion of macro 'SPI_PORT'
  401 |       WRITE_PERI_REG(SPI_W15_REG(SPI_PORT), *data++);
      |                                  ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:112:13: note: in expansion of macro 'WRITE_PERI_REG'
  112 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:402:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  402 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |       ^~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:17:25: note: in expansion of macro 'REG_SPI_BASE'
   17 | #define SPI_CMD_REG(i) (REG_SPI_BASE(i) + 0x0)
      |                         ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:402:25: note: in expansion of macro 'SPI_CMD_REG'
  402 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |                         ^~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:402:37: note: in expansion of macro 'SPI_PORT'
  402 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |                                     ^~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:112:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  112 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:102:76: note: in definition of macro 'WRITE_PERI_REG'
  102 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:402:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  402 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |       ^~~~~~~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:409:12: note: in expansion of macro 'READ_PERI_REG'
  409 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |            ^~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:17:25: note: in expansion of macro 'REG_SPI_BASE'
   17 | #define SPI_CMD_REG(i) (REG_SPI_BASE(i) + 0x0)
      |                         ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:409:26: note: in expansion of macro 'SPI_CMD_REG'
  409 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                          ^~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:409:38: note: in expansion of macro 'SPI_PORT'
  409 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                      ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:409:48: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  409 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                                ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:410:5: note: in expansion of macro 'WRITE_PERI_REG'
  410 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), (len << 4) - 1);
      |     ^~~~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:410:38: note: in expansion of macro 'SPI_PORT'
  410 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), (len << 4) - 1);
      |                                      ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:410:20: error: 'SPI_MOSI_DLEN_REG' was not declared in this scope; did you mean 'SPI_MS_DLEN_REG'?
  410 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), (len << 4) - 1);
      |                    ^~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:410:5: note: in expansion of macro 'WRITE_PERI_REG'
  410 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), (len << 4) - 1);
      |     ^~~~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:411:45: note: in expansion of macro 'WRITE_PERI_REG'
  411 |     for (uint32_t i=0; i <= (len<<1); i+=4) WRITE_PERI_REG((SPI_W0_REG(SPI_PORT) + i), *data++);
      |                                             ^~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:1751:24: note: in expansion of macro 'REG_SPI_BASE'
 1751 | #define SPI_W0_REG(i) (REG_SPI_BASE(i) + 0x98)
      |                        ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:411:61: note: in expansion of macro 'SPI_W0_REG'
  411 |     for (uint32_t i=0; i <= (len<<1); i+=4) WRITE_PERI_REG((SPI_W0_REG(SPI_PORT) + i), *data++);
      |                                                             ^~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:411:72: note: in expansion of macro 'SPI_PORT'
  411 |     for (uint32_t i=0; i <= (len<<1); i+=4) WRITE_PERI_REG((SPI_W0_REG(SPI_PORT) + i), *data++);
      |                                                                        ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:112:13: note: in expansion of macro 'WRITE_PERI_REG'
  112 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:412:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  412 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |     ^~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:17:25: note: in expansion of macro 'REG_SPI_BASE'
   17 | #define SPI_CMD_REG(i) (REG_SPI_BASE(i) + 0x0)
      |                         ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:412:23: note: in expansion of macro 'SPI_CMD_REG'
  412 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |                       ^~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:412:35: note: in expansion of macro 'SPI_PORT'
  412 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |                                   ^~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:112:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  112 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:102:76: note: in definition of macro 'WRITE_PERI_REG'
  102 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:412:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  412 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |     ^~~~~~~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:75:22: error: 'VSPI' was not declared in this scope; did you mean 'SPI'?
   75 |     #define SPI_PORT VSPI
      |                      ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/include/soc/soc.h:32:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   32 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:414:10: note: in expansion of macro 'READ_PERI_REG'
  414 |   while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |          ^~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/spi_reg.h:17:25: note: in expansion of macro 'REG_SPI_BASE'
   17 | #define SPI_CMD_REG(i) (REG_SPI_BASE(i) + 0x0)
      |                         ^~~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:414:24: note: in expansion of macro 'SPI_CMD_REG'
  414 |   while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                        ^~~~~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:414:36: note: in expansion of macro 'SPI_PORT'
  414 |   while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                    ^~~~~~~~
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:414:46: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  414 |   while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                              ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c: In function 'void dc_callback(spi_transaction_t*)':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:766:28: note: in expansion of macro 'DC_D'
  766 |   if ((bool)spi_tx->user) {DC_D;}
      |                            ^~~~
In file included from /Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/include/soc/gpio_periph.h:10,
                 from /Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/hal/esp32c6/include/hal/gpio_ll.h:20,
                 from /Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:14:
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.c:767:9: note: in expansion of macro 'DC_C'
  767 |   else {DC_C;}
      |         ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'void TFT_eSPI::begin_tft_write()':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:234:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  234 |         #define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:80:5: note: in expansion of macro 'CS_L'
   80 |     CS_L;
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:234:81: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  234 |         #define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
      |                                                                                 ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:80:5: note: in expansion of macro 'CS_L'
   80 |     CS_L;
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'virtual void TFT_eSPI::begin_nin_write()':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:234:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  234 |         #define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:92:5: note: in expansion of macro 'CS_L'
   92 |     CS_L;
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:234:81: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  234 |         #define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
      |                                                                                 ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:92:5: note: in expansion of macro 'CS_L'
   92 |     CS_L;
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'void TFT_eSPI::end_tft_write()':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:235:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  235 |         #define CS_H GPIO.out_w1ts = (1 << TFT_CS)//;GPIO.out_w1ts = (1 << TFT_CS)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:106:7: note: in expansion of macro 'CS_H'
  106 |       CS_H;
      |       ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'virtual void TFT_eSPI::end_nin_write()':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:235:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  235 |         #define CS_H GPIO.out_w1ts = (1 << TFT_CS)//;GPIO.out_w1ts = (1 << TFT_CS)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:121:7: note: in expansion of macro 'CS_H'
  121 |       CS_H;
      |       ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'void TFT_eSPI::begin_tft_read()':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:234:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  234 |         #define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:141:5: note: in expansion of macro 'CS_L'
  141 |     CS_L;
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:234:81: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  234 |         #define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
      |                                                                                 ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:141:5: note: in expansion of macro 'CS_L'
  141 |     CS_L;
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'void TFT_eSPI::end_tft_read()':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:235:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  235 |         #define CS_H GPIO.out_w1ts = (1 << TFT_CS)//;GPIO.out_w1ts = (1 << TFT_CS)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:161:7: note: in expansion of macro 'CS_H'
  161 |       CS_H;
      |       ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'void TFT_eSPI::writecommand(uint8_t)':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:984:3: note: in expansion of macro 'DC_C'
  984 |   DC_C;
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:988:3: note: in expansion of macro 'DC_D'
  988 |   DC_D;
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'void TFT_eSPI::writedata(uint8_t)':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:1047:3: note: in expansion of macro 'DC_D'
 1047 |   DC_D;        // Play safe, but should already be in data mode
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:234:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  234 |         #define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:1051:3: note: in expansion of macro 'CS_L'
 1051 |   CS_L;        // Allow more hold time for low VDI rail
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:234:81: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  234 |         #define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
      |                                                                                 ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:1051:3: note: in expansion of macro 'CS_L'
 1051 |   CS_L;        // Allow more hold time for low VDI rail
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'uint8_t TFT_eSPI::readcommand8(uint8_t, uint8_t)':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:1084:3: note: in expansion of macro 'DC_C'
 1084 |   DC_C; tft_Write_8(0xD9);
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:1085:3: note: in expansion of macro 'DC_D'
 1085 |   DC_D; tft_Write_8(index);
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:235:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  235 |         #define CS_H GPIO.out_w1ts = (1 << TFT_CS)//;GPIO.out_w1ts = (1 << TFT_CS)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:1087:3: note: in expansion of macro 'CS_H'
 1087 |   CS_H; // Some displays seem to need CS to be pulsed here, or is just a delay needed?
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:234:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  234 |         #define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:1088:3: note: in expansion of macro 'CS_L'
 1088 |   CS_L;
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:234:81: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  234 |         #define CS_L GPIO.out_w1tc = (1 << TFT_CS); GPIO.out_w1tc = (1 << TFT_CS)
      |                                                                                 ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:1088:3: note: in expansion of macro 'CS_L'
 1088 |   CS_L;
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:1090:3: note: in expansion of macro 'DC_C'
 1090 |   DC_C; tft_Write_8(cmd_function);
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:1091:3: note: in expansion of macro 'DC_D'
 1091 |   DC_D;
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'virtual uint16_t TFT_eSPI::readPixel(int32_t, int32_t)':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:235:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  235 |         #define CS_H GPIO.out_w1ts = (1 << TFT_CS)//;GPIO.out_w1ts = (1 << TFT_CS)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:1247:3: note: in expansion of macro 'CS_H'
 1247 |   CS_H;
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'void TFT_eSPI::readRectRGB(int32_t, int32_t, int32_t, int32_t, uint8_t*)':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:235:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  235 |         #define CS_H GPIO.out_w1ts = (1 << TFT_CS)//;GPIO.out_w1ts = (1 << TFT_CS)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:2258:3: note: in expansion of macro 'CS_H'
 2258 |   CS_H;
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'virtual void TFT_eSPI::setWindow(int32_t, int32_t, int32_t, int32_t)':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3485:5: note: in expansion of macro 'DC_C'
 3485 |     DC_C; tft_Write_8(TFT_CASET);
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3486:5: note: in expansion of macro 'DC_D'
 3486 |     DC_D; tft_Write_32C(x0, x1);
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3487:5: note: in expansion of macro 'DC_C'
 3487 |     DC_C; tft_Write_8(TFT_PASET);
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3488:5: note: in expansion of macro 'DC_D'
 3488 |     DC_D; tft_Write_32C(y0, y1);
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3489:5: note: in expansion of macro 'DC_C'
 3489 |     DC_C; tft_Write_8(TFT_RAMWR);
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3490:5: note: in expansion of macro 'DC_D'
 3490 |     DC_D;
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'void TFT_eSPI::readAddrWindow(int32_t, int32_t, int32_t, int32_t)':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3561:3: note: in expansion of macro 'DC_C'
 3561 |   DC_C; tft_Write_8(TFT_CASET);
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3562:3: note: in expansion of macro 'DC_D'
 3562 |   DC_D; tft_Write_32C(xs, xe);
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3565:3: note: in expansion of macro 'DC_C'
 3565 |   DC_C; tft_Write_8(TFT_PASET);
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3566:3: note: in expansion of macro 'DC_D'
 3566 |   DC_D; tft_Write_32C(ys, ye);
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3569:3: note: in expansion of macro 'DC_C'
 3569 |   DC_C; tft_Write_8(TFT_RAMRD);
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3571:3: note: in expansion of macro 'DC_D'
 3571 |   DC_D;
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp: In member function 'virtual void TFT_eSPI::drawPixel(int32_t, int32_t, uint32_t)':
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3761:7: note: in expansion of macro 'DC_C'
 3761 |       DC_C; tft_Write_8(TFT_CASET);
      |       ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3762:7: note: in expansion of macro 'DC_D'
 3762 |       DC_D; tft_Write_32D(x);
      |       ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3768:7: note: in expansion of macro 'DC_C'
 3768 |       DC_C; tft_Write_8(TFT_PASET);
      |       ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3769:7: note: in expansion of macro 'DC_D'
 3769 |       DC_D; tft_Write_32D(y);
      |       ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:189:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
  189 |         #define DC_C GPIO.out_w1tc = (1 << TFT_DC)//;GPIO.out_w1tc = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3774:3: note: in expansion of macro 'DC_C'
 3774 |   DC_C; tft_Write_8(TFT_RAMWR);
      |   ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(const gpio_out_w1tc_reg_t&)'
   64 | } gpio_out_w1tc_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1tc_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note: candidate: 'constexpr gpio_out_w1tc_reg_t& gpio_out_w1tc_reg_t::operator=(gpio_out_w1tc_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:64:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1tc_reg_t&&'
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32.h:190:50: error: no match for 'operator=' (operand types are 'volatile gpio_out_w1ts_reg_t' and 'int')
  190 |         #define DC_D GPIO.out_w1ts = (1 << TFT_DC)//;GPIO.out_w1ts = (1 << TFT_DC)
      |                                                  ^
/Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp:3779:5: note: in expansion of macro 'DC_D'
 3779 |     DC_D; tft_Write_16N(color);
      |     ^~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(const gpio_out_w1ts_reg_t&)'
   51 | } gpio_out_w1ts_reg_t;
      |   ^~~~~~~~~~~~~~~~~~~
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'const gpio_out_w1ts_reg_t&'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note: candidate: 'constexpr gpio_out_w1ts_reg_t& gpio_out_w1ts_reg_t::operator=(gpio_out_w1ts_reg_t&&)'
/Users/conormcmillan/Library/Arduino15/packages/esp32/tools/esp32-arduino-libs/idf-release_v5.5-07e9bf49-v1/esp32c6/include/soc/esp32c6/register/soc/gpio_struct.h:51:3: note:   no known conversion for argument 1 from 'int' to 'gpio_out_w1ts_reg_t&&'
Multiple libraries were found for "TFT_eSPI.h"
  Used: /Users/conormcmillan/Documents/Arduino/libraries/TFT_eSPI
  Not used: /Users/conormcmillan/Documents/Arduino/libraries/Seeed_GFX
exit status 1

Compilation error: exit status 1