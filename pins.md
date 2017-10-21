# Pinout diagrams

## ATtinyx4A (ATtiny44A/84A)

(ATtiny24A not supported due to memory constraints.)

                |--u--|
           VCC -|1  14|- GND
      (PB0) p1 -|2  13|- p8 (PA0)
      (PB1) p2 -|3  12|- p7 (PA1)
          ~RST -|4  11|- p6 (PA2)
      (PB2) p3 -|5  10|- p5 (PA3)
    (OC0B) pwm -|6   9|- p4 (PA4)
     (MOSI) rx -|7   8|- tx (MISO)
                |-----|


## ATtinyx5 (ATtiny45/85)

(ATtiny25 not supported due to memory constraints.)

                |--u--|
          ~RST -|1   8|- VCC
      (PB3) p1 -|2   7|- p2 (PB2)
    (OC0B) pwm -|3   6|- tx (MISO)
           GND -|4   5|- rx (MOSI)
                |-----|
