# Pinout diagrams

## ATtinyx4A (ATtiny44A/84A)

(ATtiny24A not supported due to memory constraints.)

                |--u--|
           VCC -|1  14|- GND
      (PB0) p1 -|2  13|- tx
      (PB1) p2 -|3  12|- rx
          ~RST -|4  11|- p8 (PA2)
    (OC0A) pwm -|5  10|- p7 (PA3)
      (PA7) p3 -|6   9|- p6 (PA4)
      (PA6) p4 -|7   8|- p5 (PA5)
                |-----|


## ATtinyx5 (ATtiny45/85)

(ATtiny25 not supported due to memory constraints.)

          |--u--|
    ~RST -|1   8|- VCC
      rx -|2   7|- p2 (PB2)
      tx -|3   6|- p1 (PB1)
     GND -|4   5|- pwm (OC0A)
          |-----|
