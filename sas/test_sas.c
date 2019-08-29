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

#include "test_sas.h"
void test_vpd_page_b2(char *dev, char *cmd_str);

unsigned short get_cmd_len(char *cmd_str)
{
    return (strlen(cmd_str) + 1)/3;
}

unsigned short buf_len_12h(unsigned char *cmd)
{
    return (cmd[3] << 8) + cmd[4];
}

unsigned short buf_len_a1h(unsigned char *cmd)
{
    return 512 * cmd[4];
}

unsigned short get_buf_len(unsigned char *cmd)
{
    if(cmd[0] == 0x12)
    {
        return buf_len_12h(cmd);
    }
    if(cmd[0] == 0xa1)
    {
        return buf_len_a1h(cmd);
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
        printf("SG info is not ok\n");
        int i;
        for(i = 0 ; i < 32 ; i++)
        {
            printf("%02x ", sense_buffer[i]);
        }
        printf("\n");
        ret = -1;
        goto send_exit;
    }

send_exit:
    close(sg_fd);
    return ret;

}

void dump_buf(unsigned char *buf)
{
    int i;
    for(i = 0 ; i < 0x100 ; i++)
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

void parsebuf_a1h(unsigned char *buf, void *data)
{
    dump_buf(buf);
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
    memset(buf, 0, buf_len);
    
    int i;
    for(i = 0 ; i < 12 ; i++)
    {
        printf("%x ", cmd[i]);
    }
    printf("\n");

    printf("cmd_len : buf_len = (%d, %d)\n", cmd_len, buf_len);
    ret = _send_scsi_command(dev, cmd_len, cmd, buf_len, buf);
    printf("ret : %d\n", ret); 
    parsefunc(buf, data);
    free(cmd);
    free(buf);
    return 0;
}


#ifdef UNIT_TEST
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

void test_ata_passthrough12(char *dev)
{
    char *cmd = "a1,08,0e,00,01,30,00,00,00,2f,00,00";
    //char *cmd = "a1,08,0e,00,01,00,00,00,00,ec,00,00";
    struct scsi_12_01_b2 data;
    send_scsi_command(dev, cmd, &data, parsebuf_a1h);
}

int main(int argc, char * argv[])
{
    /*
    //send_scsi_command(argv[1], "12,00,00,00,60,00");
    test_vpd_page_00(argv[1], argv[2]);
    //test_vpd_page_b2(argv[1], argv[2]);
    */
    test_ata_passthrough12(argv[1]);
    return 0;
}
#endif
