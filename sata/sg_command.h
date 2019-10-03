

int is_sas_support_trim(char *ctrl_name);
int is_sas_support_trim_write(char *ctrl_name);
int is_sas_support_trim_read_zero(char *ctrl_name);
int get_identify_device_data(char* dev, char *page, unsigned char *buf, int buf_len);

struct scsi_12_01_00
{
    unsigned char page_code;
    unsigned short page_length;
    unsigned char *sup_pages;
};

struct scsi_12_01_b2
{
    unsigned char page_code;
    unsigned short page_length;
    unsigned char lbpw;
    unsigned char lbpr;
};
