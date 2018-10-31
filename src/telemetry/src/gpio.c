#include <gpio.h>

void gpio_config()
{
  /* open /dev/mem */
  if ((mem_fd = open("/dev/gpiomem", O_RDWR|O_SYNC) ) < 0) {
    printf("can't open /dev/gpiomem \n");
    exit(-1);
  }

  /* mmap GPIO */
  gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
      );

  close(mem_fd); //No need to keep mem_fd open after mmap

  if (gpio_map == MAP_FAILED) {
    printf("mmap error %d\n", (int)gpio_map);//errno also set!
    exit(-1);
  }

  // Always use volatile pointer!
  gpio = (volatile unsigned *)gpio_map;


} // setup_io
