#include <avr/io.h>

class LGSerial
{
public:
  // usart_init - Initialize the USART port
  static void init(int);
  
  // Send data to the serial port
  static void put(char*);
  static void put(char);
  
  // Send data with carriage return
  static void print(char*);
  
  // Get data from the port
  static char get(); // Wait for data and return a byte
  static int get(char* buf, int len);
  
  // Test if data is available
  static bool available();
  
// private:
//   LGSerial (arguments);
//   virtual ~LGSerial ();
};