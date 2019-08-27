

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
