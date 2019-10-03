#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <scsi/sg.h> /* take care: fetches glibc's /usr/include/scsi/sg.h */

#include "sg_command.h"


unsigned short get_cmd_len(char *cmd_str)
{
    return (strlen(cmd_str) + 1)/3;
}

unsigned short buf_len_12h(unsigned char *cmd)
{
    return (cmd[3] << 8) + cmd[4];
}

unsigned short get_buf_len(char *cmd)
{
    if(cmd[0] == 0x12)
    {
        return buf_len_12h(cmd);
    }
    return 0;
}

int chex_to_int(char c)
{
    if(c - 'a' >= 0)
    {
        return c - 'a' + 10;
    }
    return c - '0';
}

void cmd_str_to_buf(char *cmd_str, unsigned char *cmd)
{
    int i;
    for(i = 0 ; i < strlen(cmd_str) ; i += 3)
    {
        cmd[i/3] = (chex_to_int(cmd_str[i]) << 4) + chex_to_int(cmd_str[i + 1]);
    }
}

int _send_scsi_command(char *dev, unsigned short cmd_len,unsigned char *cmd, unsigned short buf_len, unsigned char *buf)
{
    int ret = 0;
    int sg_fd;
    sg_io_hdr_t io_hdr;
    unsigned char sense_buffer[32];

    if ((sg_fd = open(dev, O_RDONLY)) < 0) {
        perror("error opening given file name");
        return -1;
    }

    memset(&io_hdr, 0, sizeof(sg_io_hdr_t));
    io_hdr.interface_id = 'S';
    io_hdr.cmd_len = cmd_len;
    io_hdr.mx_sb_len = sizeof(sense_buffer);
    io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
    io_hdr.dxfer_len = buf_len;
    io_hdr.dxferp = buf;
    io_hdr.cmdp = cmd;
    io_hdr.sbp = sense_buffer;
    io_hdr.timeout = 20000;     /* 20000 millisecs == 20 seconds */

    if (ioctl(sg_fd, SG_IO, &io_hdr) < 0) {
        perror("sg_simple0: Inquiry SG_IO ioctl error");
        ret = -1;
        goto send_exit;
    }
    
    if ((io_hdr.info & SG_INFO_OK_MASK) != SG_INFO_OK) 
    {
        ret = -2;
        int j;
        for(j = 0 ; j < 32 ; j++)
        {
            printf("%02x ", sense_buffer[j]);
        }
        printf("\n");
        goto send_exit;
    }

send_exit:
    close(sg_fd);
    return ret;

}

void dump_buf(unsigned char *buf)
{
    int i;
    for(i = 0 ; i < 0x60 ; i++)
    {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

void parsebuf_12h_01h_00h(unsigned char *buf, void *data)
{
    int i;
    struct scsi_12_01_00 *vpd = (struct scsi_12_01_00*) data;

    vpd->page_code = buf[1];
    vpd->page_length = (buf[2] << 8) + buf[3];

    vpd->sup_pages = malloc(vpd->page_length);
    for(i = 0 ; i < vpd->page_length ; i++)
    {
        vpd->sup_pages[i] = buf[4 + i];
    }
}

void parsebuf_12h_01h_b2h(unsigned char *buf, void *data)
{
    int i;
    struct scsi_12_01_b2 *vpd = (struct scsi_12_01_b2*) data;

    vpd->page_code = buf[1];
    vpd->page_length = (buf[2] << 8) + buf[3];
    vpd->lbpw = (buf[5] >> 4) & 0x0e;
    vpd->lbpr = (buf[5] >> 2) & 1;
}

int send_scsi_command(char *dev, char *cmd_str, void *data, void(*parsefunc)(unsigned char*, void*))
{
    int ret = 0;
    unsigned short int cmd_len;
    unsigned short int buf_len;
    unsigned char *cmd;
    unsigned char *buf;

    cmd_len = get_cmd_len(cmd_str);
    cmd = malloc(cmd_len);
    cmd_str_to_buf(cmd_str, cmd);

    buf_len = get_buf_len(cmd);
    buf = malloc(buf_len);
    
    ret = _send_scsi_command(dev, cmd_len, cmd, buf_len, buf);
    if(ret == 0)
    {
        parsefunc(buf, data);
        //dump_buf(buf);
    }
    free(cmd);
    free(buf);
    return ret;
}

int send_scsi_command_with_buf(char *dev, char *cmd_str, unsigned char *buf, int buf_len)
{
    int ret = 0;
    unsigned short int cmd_len;
    unsigned char *cmd;

    cmd_len = get_cmd_len(cmd_str);
    cmd = malloc(cmd_len);
    cmd_str_to_buf(cmd_str, cmd);

    ret = _send_scsi_command(dev, cmd_len, cmd, buf_len, buf);
    free(cmd);

    return ret;
}

int is_sas_support_trim(char *dev)
{
    int ret = 0;
    int i;
    struct scsi_12_01_00 data;
    ret = send_scsi_command(dev, "12,01,00,00,60,00", &data, parsebuf_12h_01h_00h);
    if(ret != 0)
        return 0;

    for(i = 0 ; i < data.page_length ; i++)
    {
        if(data.sup_pages[i] == 0xb2)
            ret += 1;
        if(data.sup_pages[i] == 0xb0)
            ret += 1;
    }
    
    if(data.page_length > 0)
    {
        free(data.sup_pages);
    }

    if (ret == 2)
    {
        return 1;
    }
    return 0;
}

int is_sas_support_trim_write(char *dev)
{
    int ret = 0;
    int i;
    struct scsi_12_01_b2 data;
    ret = send_scsi_command(dev, "12,01,b2,00,08,00", &data, parsebuf_12h_01h_b2h);
    if(ret != 0)
        return 0;

    if(data.lbpw != 0)
        return 1;
    return 0;
}

int is_sas_support_trim_read_zero(char *dev)
{
    int ret = 0;
    int i;
    struct scsi_12_01_b2 data;
    ret = send_scsi_command(dev, "12,01,b2,00,08,00", &data, parsebuf_12h_01h_b2h);
    if(ret != 0)
        return 0;

    if(data.lbpr != 0)
        return 1;
    return 0;
}

int get_identify_device_data(char* dev, char *page, unsigned char *buf, int buf_len)
{
    char *base = "a1,08,0e,00,01,30,%s,00,00,2f,00,00";
    char cmd[64] = {0};
    sprintf(cmd, base, page);
    printf("%s\n",cmd);
    return send_scsi_command_with_buf(dev, cmd, buf, buf_len);
}

#ifdef UNIT_TEST
void test_get_identify_device_data(char *dev)
{
    int ret;
    unsigned char buf[512] = {0};
    char *pages[] = {"00", "02", "03", "04"};
    int i;
    for(i = 0 ; i < 4 ; i++)
    {
        printf("page: %s\n", pages[i]);
        ret = get_identify_device_data(dev, pages[i], buf, 512);
        if(ret != 0)
        {
            printf("ret : %d\n", ret);
            return;
        }
        printf("\n");
        int i;
        for(i = 0 ; i < 512 ; i++)
        {
            printf("%02x ", buf[i]);
        }
        printf("\n");
    }
}

void test_vpd_page_00(char *dev, char *cmd_str)
{
    struct scsi_12_01_00 data;
    send_scsi_command(dev, cmd_str, &data, parsebuf_12h_01h_00h);
    printf("page code:%d\n", data.page_code);
    printf("page length:%d\n", data.page_length);

    int i;
    printf("support pages: ");
    for(i = 0 ; i < data.page_length ; i++)
    {
        printf("%02x ", data.sup_pages[i]);
    }
    printf("\n");
}

void test_vpd_page_b2(char *dev, char *cmd_str)
{
    struct scsi_12_01_b2 data;
    send_scsi_command(dev, cmd_str, &data, parsebuf_12h_01h_b2h);
    printf("page code:%d\n", data.page_code);
    printf("page length:%d\n", data.page_length);
    printf("lbpw : %d\n", data.lbpw);
    printf("lbpr : %d\n", data.lbpr);
    printf("\n");
}

int test_send_scsi_command(char* dev, char *cmd)
{
    int i;
    int ret;
    unsigned char buf[512] = {0};
    ret = send_scsi_command_with_buf(dev, cmd, buf, 512);

    if(ret != 0)
    {
        printf("ret : %d\n", ret);
        return ret;
    }
    for(i = 0 ; i < 512 ; i++)
    {
        printf("%02x ", buf[i]);
    }
    printf("\n");
    return 0;
}

int main(int argc, char * argv[])
{
    //_send_scsi_command(argv[1], "12,00,00,00,60,00");
    //test_vpd_page_00(argv[1], argv[2]);
    //test_vpd_page_b2(argv[1], argv[2]);
    //test_get_identify_device_data(argv[1]);
    test_send_scsi_command(argv[1], argv[2]);
    return 0;
}
#endif
