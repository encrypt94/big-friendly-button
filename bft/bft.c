#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libusb-1.0/libusb.h>

#define USB_DEVICE_VID 0x4242
#define USB_DEVICE_PID 0xe131

#define USB_DEVICE_VENDOR_NAME  "encrypt"
#define USB_DEVICE_PRODUCT_NAME "button"

#define USB_GET_KEYCODE 0x05
#define USB_SET_KEYCODE 0x06

#define BUFFER_SIZE 1024
#define USB_TIMEOUT 500

#define REQUEST_TYPE_VENDOR LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE

void usage();
int bfb_send_command(libusb_device_handle* handle, uint8_t command, uint16_t* keycode);
libusb_device_handle* open_device(int vid, char *vendor_name, int pid, char *product_name);
void close_device(libusb_device_handle *handle);

int main(int argc, char **argv) {
  uint8_t command;
  uint16_t keycode = 0;
  if(argc < 2){
    usage();
    exit(0);
  }
  if(strcmp(argv[1], "set") == 0 && argc >= 3) {
    command = USB_SET_KEYCODE;
    keycode = (uint16_t) atoi(argv[2]);
  }
  else if(strcmp(argv[1], "get") == 0) {
    command = USB_GET_KEYCODE;
  }
  else {
    usage();
    exit(1);
  }
  libusb_device_handle* device = open_device(USB_DEVICE_VID, USB_DEVICE_VENDOR_NAME, USB_DEVICE_PID, USB_DEVICE_PRODUCT_NAME);
  if(device == NULL) {
    fputs("error: device not found\n", stderr);
    exit(1);
  }
  int nbytes = bfb_send_command(device, command, &keycode);
  if(nbytes < 0) {
    fprintf(stderr,"error: %s [%i]\n", libusb_error_name(nbytes), nbytes);
    exit(1);
  }
  else {
    printf("%i\n",keycode);
  }
  close_device(device);
  return 0;
}

void usage() {
  puts("Usage: bft COMMAND [arg...]");
  puts("\nCommands:");
  puts("   set [0...65535]  set the keycode");
  puts("   get              return current keycode");
}

int bfb_send_command(libusb_device_handle* handle, uint8_t command, uint16_t* keycode) {
  char buffer[BUFFER_SIZE];
  uint16_t k = command == USB_SET_KEYCODE ? *keycode : 0;
  int  nbytes = libusb_control_transfer(handle, REQUEST_TYPE_VENDOR, command, k, 0, (unsigned char*)buffer, sizeof(buffer), USB_TIMEOUT);
  if(nbytes > 0 && command == USB_GET_KEYCODE) {
    *keycode = buffer[0] |  buffer[1] << 8;
  }
  return nbytes;
}

libusb_device_handle* open_device(int vid, char *vendor_name, int pid, char *product_name) {
  libusb_device **devices;
  libusb_device *device;
  libusb_device_handle *handle = NULL;
  struct libusb_device_descriptor desc;
  bool found = false;
  unsigned char device_vendor_name[256], device_product_name[256];
  int i = 0;

  libusb_init(NULL);
  libusb_get_device_list(NULL, &devices);
  
  while((device = devices[i++]) != NULL) {
    if(libusb_get_device_descriptor(device, &desc) < 0) {
      fprintf(stderr, "Warning: Failed to get device descriptor");
      continue;
    }
    if(desc.idVendor == vid && desc.idProduct == pid) {
      if(libusb_open(device, &handle) < 0) {
	fprintf(stderr, "Warning: cannot open usb device\n");
	continue;
      }
      // Get Vendor name from device
      if(libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, device_vendor_name, sizeof(device_vendor_name)) < 0 ) {
	fprintf(stderr, "Warning: cannot get vendor name\n");
	libusb_close(handle);
	continue;
      }
      // Get Device name
      if(libusb_get_string_descriptor_ascii(handle, desc.iProduct, device_product_name, sizeof(device_product_name)) < 0) {
	fprintf(stderr, "Warning: cannot get vendor name\n");
	libusb_close(handle);
	continue;
      }
      if( strcmp(vendor_name, (char*) device_vendor_name) == 0 &&
          strcmp(product_name, (char*) device_product_name) == 0) {
	found = true;
	break;
      }
      else {
	libusb_close(handle);
      }
    }
  }
  libusb_free_device_list(devices, 1);
  if(found) {
    return handle;
  }
  else {
    return NULL;
  }
}

void close_device(libusb_device_handle *handle) {
  libusb_close(handle);
  libusb_exit(NULL);
}
