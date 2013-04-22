#ifndef LGSERIAL_H
#define LGSERIAL_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUDRATE 9600
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

class LGSerial
{
public:
  // usart_init - Initialize the USART port
  static void init();

  // Send data to the serial port
  static void put(char*);
  static void put(char);

  // Send data with carriage return
  static void print(char*);
  static void print(int);
  static void clear_screen();
  void print_hex(uint64_t);

  // Get data from the port
  static char get(); // Wait for data and return a byte
  static bool get_with_timeout(char *c, unsigned int timeout_ms);
  static int get(char* buf, int len);
  static int get(char* buf, char stopchar, int maxlen);

  // Test if data is available
  static bool available();

  // Clear serial buffer
  static void clear();

// private:
//   LGSerial (arguments);
//   virtual ~LGSerial ();
};

#endif
