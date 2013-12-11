#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

unsigned int value;

int main(int argc, char **argv)
{

  if(argc == 2) {
    if(argv[1][1]=='6') {
      printf("writing 16 bit colors\n");
      fbutil(16);
    } else if(argv[1][1]=='4') {
      printf("writing 24 bit colors\n");
      fbutil(18);
    } else {
      printf("you are an idiot stop giving me wrong args");
    }
  }
  return 0;
}

int fbutil(unsigned int depth)
{
    int fd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    unsigned short *fbp = 0;
    int x = 0, y = 0;
    long int location = 0;
    int count = 0;
    printf("Gathering info...\n");
        // Open the file for reading and writing
    fd = open("/dev/fb0", O_RDWR);
    if (fd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");
    
    // Get fixed screen information
    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }
    
    // Get variable screen information
    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printVarInfo(vinfo);

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    printf("xoffset: %d yoffset: %d\n", vinfo.xoffset, vinfo.yoffset);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
        
        //printf("Screensize: %lu\n",screensize);
        
        // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");
        
        unsigned int hex=0x1;
        unsigned int max;
        if(depth==16) {
          max=0xf800;
        } else {
          max=0xf800;
        }
        while ( hex < max ) {
          count = 0;
          x = 0; y = 0; /* Where we are going to put the pixel */
          /* Figure out where in memory to put the pixel */
          location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
          //printf("Starting at location: %lu\n", location);        
          while ( location < (screensize/2))
          {
                  *(fbp + location) = hex;
                  location += 1;
                  count++;
          }
          hex += 0xf;
          printf("Hex value: 0x%08X\n", hex);
        }
        
        printf("location: %lu count: %d\n",location,count);
        
        munmap(fbp, screensize);
        close(fd);
        
        return 0;
}

void printVarInfo(struct fb_var_screeninfo  vinfo) {
printf("vinfo.xres=%d\n", vinfo.xres);
printf("vinfo.yres=%d\n", vinfo.yres);
printf("vinfo.xres_virtual=%d\n", vinfo.xres_virtual);
printf("vinfo.yres_virtual=%d\n", vinfo.yres_virtual);
printf("vinfo.xoffset=%d\n", vinfo.xoffset);
printf("vinfo.yoffset=%d\n", vinfo.yoffset);
printf("vinfo.bits_per_pixel=%d\n", vinfo.bits_per_pixel);
printf("vinfo.grayscale=%d\n", vinfo.grayscale);
printf("vinfo.nonstd=%d\n", vinfo.nonstd);
printf("vinfo.activate=%d\n", vinfo.activate);
printf("vinfo.height=%d\n", vinfo.height);
printf("vinfo.width=%d\n", vinfo.width);
printf("vinfo.accel_flags=%d\n", vinfo.accel_flags);
printf("vinfo.pixclock=%d\n", vinfo.pixclock);
printf("vinfo.left_margin=%d\n", vinfo.left_margin);
printf("vinfo.right_margin=%d\n", vinfo.right_margin);
printf("vinfo.upper_margin=%d\n", vinfo.upper_margin);
printf("vinfo.lower_margin=%d\n", vinfo.lower_margin);
printf("vinfo.hsync_len=%d\n", vinfo.hsync_len);
printf("vinfo.vsync_len=%d\n", vinfo.vsync_len);
printf("vinfo.sync=%d\n", vinfo.sync);
printf("vinfo.vmode=%d\n", vinfo.vmode);
printf("vinfo.rotate=%d\n", vinfo.rotate);
printf("vinfo.red.offset=%d\n", vinfo.red.offset);
printf("vinfo.red.length=%d\n", vinfo.red.length);
printf("vinfo.red.msb_right=%d\n", vinfo.red.msb_right);
printf("vinfo.green.offset=%d\n", vinfo.green.offset);
printf("vinfo.green.length=%d\n", vinfo.green.length);
printf("vinfo.green.msb_right=%d\n", vinfo.green.msb_right);
printf("vinfo.blue.offset=%d\n", vinfo.blue.offset);
printf("vinfo.blue.length=%d\n", vinfo.blue.length);
printf("vinfo.blue.msb_right=%d\n", vinfo.blue.msb_right);
printf("vinfo.transp.offset=%d\n", vinfo.transp.offset);
printf("vinfo.transp.length=%d\n", vinfo.transp.length);
printf("vinfo.transp.msb_right=%d\n", vinfo.transp.msb_right);
}
