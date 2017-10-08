#ifndef IO_H
#define IO_H


int cfg_pin(int pin, int input, int pullup);
int set_pin(int pin, int val);
int get_pin(int pin);
int pwm(int duty, int length);
int wait(int ms);
void load_from_eeprom(void);
void save_to_eeprom(void);


#endif
