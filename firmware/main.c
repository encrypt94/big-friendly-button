/***

             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                     Version 2, December 2004

             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

   0. You just DO WHAT THE FUCK YOU WANT TO.

***/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usbdrv.h"
#include "hid-descriptor.h"

#define STATE_WAIT         0
#define STATE_SEND_KEY     1
#define STATE_RELEASE_KEY  2

#define USB_GET_KEYCODE 0x05
#define USB_SET_KEYCODE 0x06

#define BUTTON_PIN PB0

#define KEYCODE_ADDR 0x02

typedef struct {
        uint8_t modifier;
        uint8_t reserved;
        uint8_t keycode[6];
} keyboard_report_t;

static keyboard_report_t keyboard_report; // sent to PC
static uint8_t idleRate; // repeat rate for keyboards
static uint16_t keycode;

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
  static uint8_t rep_buffer[2];
  usbRequest_t *rq = (void *)data;

  if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
    switch(rq->bRequest) {
    case USBRQ_HID_GET_REPORT: // send "no keys pressed" if asked here
      // wValue: ReportType (highbyte), ReportID (lowbyte)
      usbMsgPtr = (void *)&keyboard_report; // we only have this one
      keyboard_report.modifier = 0;
      keyboard_report.keycode[0] = 0;
      return sizeof(keyboard_report);
    case USBRQ_HID_SET_REPORT: // if wLength == 1, should be LED state
      return (rq->wLength.word == 1) ? USB_NO_MSG : 0;
    case USBRQ_HID_GET_IDLE: // send idle rate to PC as required by spec
      usbMsgPtr = &idleRate;
      return 1;
    case USBRQ_HID_SET_IDLE: // save idle rate as required by spec
      idleRate = rq->wValue.bytes[1];
      return 0;
    }
  }
  // custom request
  switch(rq->bRequest) {
  case USB_GET_KEYCODE:
    rep_buffer[0] = keycode & 0xff; //lo
    rep_buffer[1] = keycode >> 8;   //hi
    usbMsgPtr = rep_buffer;
    return sizeof(rep_buffer);
  case USB_SET_KEYCODE:
    keycode = rq->wValue.word;
    eeprom_write_word((uint16_t*)(KEYCODE_ADDR), keycode);
    break;
  }
  return 0;
}

int main() {
  uint8_t calibrationValue, i;
  uint8_t state = STATE_WAIT;
  PORTB = _BV(BUTTON_PIN);
  calibrationValue = eeprom_read_byte(0);
  if(calibrationValue != 0xff) {
    OSCCAL = calibrationValue;
  }
  for(i = 0; i < sizeof(keyboard_report); i++) {
    ((uchar *)&keyboard_report)[i] = 0;
  }
  wdt_enable(WDTO_1S);
  usbInit();
  usbDeviceDisconnect();  
  for(i = 0; i<250; i++) {
    wdt_reset();
    _delay_ms(2);
  }
  i = 0;
  usbDeviceConnect();
  sei();
  keyboard_report.modifier = 0;
  keycode = eeprom_read_word((const uint16_t*)(KEYCODE_ADDR));
  for(;;) {
    wdt_reset(); // keep watchdog happy
    usbPoll();
    if(bit_is_set(PINB, BUTTON_PIN) && state == STATE_WAIT) {
      while(bit_is_set(PINB, BUTTON_PIN) && i < 200) {
	i++;
	_delay_ms(1);
      }
      i = 0;
      state = STATE_SEND_KEY;
    }
    if(usbInterruptIsReady() && state != STATE_WAIT) {
      switch(state) {
      case STATE_SEND_KEY:
	keyboard_report.keycode[0] = keycode & 0xff; //lo
	keyboard_report.keycode[1] = keycode >> 8; //hi
	state = STATE_RELEASE_KEY;
	break;
      case STATE_RELEASE_KEY:
	keyboard_report.keycode[0] = 0;
	keyboard_report.keycode[1] = 0;
	state = STATE_WAIT;
	break;
      }
      usbSetInterrupt((void *)&keyboard_report, sizeof(keyboard_report));
    }
  }
  return 0;
}

static void calibrateOscillator(void) {
  uchar step = 128;
  uchar trialValue = 0, optimumValue;
  int   x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);
  
  /* do a binary search: */
  do {
    OSCCAL = trialValue + step;
    x = usbMeasureFrameLength();    /* proportional to current real frequency */
    if(x < targetValue) {           /* frequency still too low */
      trialValue += step;
    }
    step >>= 1;
  } while(step > 0);
  /* We have a precision of +/- 1 for optimum OSCCAL here */
  /* now do a neighborhood search for optimum value */
  optimumValue = trialValue;
  optimumDev = x; /* this is certainly far away from optimum */
  for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++) {
    x = usbMeasureFrameLength() - targetValue;
    if(x < 0) {
      x = -x;
    }
    if(x < optimumDev) {
      optimumDev = x;
      optimumValue = OSCCAL;
    }
  }
  OSCCAL = optimumValue;
}

void hadUsbReset(void) {
  calibrateOscillator();
  eeprom_write_byte(0, OSCCAL);   /* store the calibrated value in EEPROM */
}
