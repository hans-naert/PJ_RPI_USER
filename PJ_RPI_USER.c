#include "PJ_RPI_USER.h"

struct bcm2835_peripheral gpio = {GPIO_BASE};

// Exposes the physical address defined in the passed structure using mmap on /dev/mem
int map_peripheral(struct bcm2835_peripheral *p)
{
   // Open /dev/mem
   if ((p->mem_fd = open("/dev/gpiomem", O_RDWR|O_SYNC) ) < 0) {
      printf("Failed to open /dev/gpiomem, try checking permissions.\n");
      return -1;
   }

   p->map = mmap(
      NULL,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED,
      p->mem_fd,  // File descriptor to physical memory virtual file '/dev/mem'
      p->addr_p      // Address in physical map that we want this memory block to expose
   );

   if (p->map == MAP_FAILED) {
        perror("mmap");
        return -1;
   }

   p->addr = (volatile unsigned int *)p->map;

   return 0;
}

void unmap_peripheral(struct bcm2835_peripheral *p) {

    munmap(p->map, BLOCK_SIZE);
    close(p->mem_fd);
}

// Priority 

int SetProgramPriority(int priorityLevel)
{
    struct sched_param sched;

    memset (&sched, 0, sizeof(sched));

    if (priorityLevel > sched_get_priority_max (SCHED_RR))
        priorityLevel = sched_get_priority_max (SCHED_RR);

    sched.sched_priority = priorityLevel;

    return sched_setscheduler (0, SCHED_RR, &sched);
}
