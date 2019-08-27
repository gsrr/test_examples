
#define _XOPEN_SOURCE


#include "hal.h"
#include "hal_event.h"


#define ULINK_TEMPLATE_SIZE 512

struct option *options;

typedef struct _cmd_param {
    const char *param_name;
    char *param_value;
} cmd_param;

struct paras_in {
    int enc_id;
    int port_id;
    char *value;
};

static cmd_param param_ary[] =
{
    { "enc_id", NULL},
    { "port_id", NULL},
    { "value", NULL},
    { NULL, NULL}
};

typedef int (*fobj) (struct paras_in);

/*== start:function_declare_area  ==*/
int da_iotest(struct paras_in);
int da_sas(struct paras_in);
int da_enable(struct paras_in);
int da_enable_by_cfg(struct paras_in);
int da_system_reset_cnt(struct paras_in);
int da_system_raid_info(struct paras_in);
int da_gen_header(struct paras_in);
int da_ata_cnt_plus(struct paras_in);

/*== end:function_declare_area ==*/

struct FuncStruct {
    char param[256];
    fobj func;
};

/*== start:function_para_area  ==*/
static struct FuncStruct func_array[] = {

    {"iotest", da_iotest},
    {"sas", da_sas},
    {"enable", da_enable},
    {"enable_by_cfg", da_enable_by_cfg},
    {"system_reset_cnt", da_system_reset_cnt},
    {"system_raid_info", da_system_raid_info},
    {"gen_header", da_gen_header},
    {"ata_cnt_plus", da_ata_cnt_plus},
    {"", NULL},
};

/*== end:function_para_area ==*/

