#ifndef __usbconfig_h_included__
#define __usbconfig_h_included__

#define USB_CFG_IOPORTNAME      B
#define USB_CFG_DMINUS_BIT      1
#define USB_CFG_DPLUS_BIT       2
#define USB_CFG_CLOCK_KHZ       (F_CPU/1000)

#define USB_CFG_CHECK_CRC       0
#define USB_CFG_HAVE_INTRIN_ENDPOINT    1
#define USB_CFG_HAVE_INTRIN_ENDPOINT3   0
#define USB_CFG_EP3_NUMBER              3
#define USB_CFG_IMPLEMENT_HALT          0
#define USB_CFG_SUPPRESS_INTR_CODE      0
#define USB_CFG_INTR_POLL_INTERVAL      100
#define USB_CFG_IS_SELF_POWERED         0
#define USB_CFG_MAX_BUS_POWER           100
#define USB_CFG_IMPLEMENT_FN_WRITE      0
#define USB_CFG_IMPLEMENT_FN_READ       0
#define USB_CFG_IMPLEMENT_FN_WRITEOUT   0
#define USB_CFG_HAVE_FLOWCONTROL        0

#define USB_CFG_DRIVER_FLASH_PAGE       0

#define USB_CFG_LONG_TRANSFERS          0

#define USB_RESET_HOOK(resetStarts)     if(!resetStarts){hadUsbReset();} 

#ifndef __ASSEMBLER__
extern void hadUsbReset(void);
#endif

#define USB_COUNT_SOF                   0

#define USB_CFG_CHECK_DATA_TOGGLING     0

#define USB_CFG_HAVE_MEASURE_FRAME_LENGTH   1

#define USB_USE_FAST_CRC                0

#define  USB_CFG_VENDOR_ID       0x42, 0x42 /* = 0x16c0 = 5824 = voti.nl */

#define  USB_CFG_DEVICE_ID       0x31, 0xe1 /* = 0x05dc = 1500 */

#define USB_CFG_DEVICE_VERSION  0x00, 0x01

#define USB_CFG_VENDOR_NAME     'e', 'n', 'c', 'r', 'y', 'p', 't'
#define USB_CFG_VENDOR_NAME_LEN 7

#define USB_CFG_DEVICE_NAME     'b', 'u', 't', 't', 'o', 'n'
#define USB_CFG_DEVICE_NAME_LEN 6

#define USB_CFG_DEVICE_CLASS        0    /* set to 0 if deferred to interface */
#define USB_CFG_DEVICE_SUBCLASS     0

#define USB_CFG_INTERFACE_CLASS     3   /* define class here if not at device level */
#define USB_CFG_INTERFACE_SUBCLASS  1
#define USB_CFG_INTERFACE_PROTOCOL  1
#define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH    63
#define USB_CFG_DESCR_PROPS_DEVICE                  0
#define USB_CFG_DESCR_PROPS_CONFIGURATION           0
#define USB_CFG_DESCR_PROPS_STRINGS                 0
#define USB_CFG_DESCR_PROPS_STRING_0                0
#define USB_CFG_DESCR_PROPS_STRING_VENDOR           0
#define USB_CFG_DESCR_PROPS_STRING_PRODUCT          0
#define USB_CFG_DESCR_PROPS_STRING_SERIAL_NUMBER    0
#define USB_CFG_DESCR_PROPS_HID                     0
#define USB_CFG_DESCR_PROPS_HID_REPORT              0
#define USB_CFG_DESCR_PROPS_UNKNOWN                 0

/* ----------------------- Optional MCU Description ------------------------ */

/* The following configurations have working defaults in usbdrv.h. You
 * usually don't need to set them explicitly. Only if you want to run
 * the driver on a device which is not yet supported or with a compiler
 * which is not fully supported (such as IAR C) or if you use a differnt
 * interrupt than INT0, you may have to define some of these.
 */
/* #define USB_INTR_CFG            MCUCR */
/* #define USB_INTR_CFG_SET        ((1 << ISC00) | (1 << ISC01)) */
/* #define USB_INTR_CFG_CLR        0 */
/* #define USB_INTR_ENABLE         GIMSK */
/* #define USB_INTR_ENABLE_BIT     INT0 */
/* #define USB_INTR_PENDING        GIFR */
/* #define USB_INTR_PENDING_BIT    INTF0 */
/* #define USB_INTR_VECTOR         INT0_vect */

#endif /* __usbconfig_h_included__ */
