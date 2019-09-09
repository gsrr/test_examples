#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SYS_PATH_GENERIC "/sys/class/scsi_generic"
#define MAX_USB_BUS_LEN 32
#define MAX_PATH_LEN 255

int get_sg_path_info(char *sg_name, char *info, int info_len)
{
    char path[MAX_PATH_LEN];
    int ret = -1;
    snprintf(path, MAX_PATH_LEN, "%s/%s", SYS_PATH_GENERIC, sg_name);

    ret = readlink(path, info, info_len - 1);
    info[ret] = 0;

    if (ret == (info_len - 1))
    {
        return -1;
    }
    return 0;
}

void parse_usb_bus_from_path(char* path, char *usb_bus)
{
    char *s,*e;

    e = strstr(path,"host");
    printf("e:%s\n", e);
    if (!e)
        return;
    *--e = 0;
    printf("here\n");
    printf("path:%s\n", path);
    s = rindex(path,'/') + 1;
    printf("s:%s\n", s);
    strncpy(usb_bus, s, MAX_USB_BUS_LEN);
    *e = '/';

    if ((e = strchr(usb_bus, ':')) != NULL)
    {
        *e = 0;
    }
}


int main()
{
    char path[512] = {0}; 
    char buf[64] = {0};
    get_sg_path_info("sg1", path, 512);
    printf("path:%s\n", path);
    parse_usb_bus_from_path(path, buf);
    printf("buf:%s\n", buf);
    return 0;
}
