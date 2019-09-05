#include "tr.h"

void str_strip(char *buf)
{
    int i = 0;
    int j = 0;
    for(i = 0 ; i < strlen(buf) ; i++)
    {
        if(buf[i] == '\n' || buf[i] == ' ')
        {
            continue;
        }
        buf[j++] = buf[i];
    }
    buf[j] = '\0';
    
}

int hal_popen(char *cmd, char *buf)
{
    FILE *fp = NULL;
    char buf1[MAX_BUF_LEN] = {0};

    fp = popen(cmd, "r");
    if (fp)
    {
        fgets(buf1, sizeof(buf1), fp);
        pclose(fp);
    }
    else
    {
        return -1;
    }
	str_strip(buf1);
    sprintf(buf, "%s", buf1);
    return 0;
}


int get_vendor(char *pd_sys_id, char *buf)
{
    char cmd[MAX_CMD_LEN] = {0};
    sprintf(cmd, "/bin/cat /sys/class/scsi_generic/%s/device/vendor", pd_sys_id);
    hal_popen(cmd, buf);
}

int get_model(char *pd_sys_id, char *buf)
{
    char cmd[MAX_CMD_LEN] = {0};
    sprintf(cmd, "/bin/cat /sys/class/scsi_generic/%s/device/model", pd_sys_id); 
    hal_popen(cmd, buf);
}

int get_sg_path(char *pd_sys_id, char *buf)
{
    char path[MAX_PATH_LEN];
    int ret = -1;
    sprintf(path, "/sys/class/scsi_generic/%s", pd_sys_id);

    ret = readlink(path, buf, MAX_BUF_LEN);
    if(ret == -1)
        return ret;

    return 0;
}

int get_usb_bus(char *pd_sys_id, char *buf)
{
    char *s,*e;
    char tbuf[MAX_BUF_LEN] = {0};

    get_sg_path(pd_sys_id, tbuf);

    e = strstr(tbuf,"host");
    if (!e)
        return -1;

    *--e = 0;

    s = rindex(tbuf,'/') + 1;

    strcpy(buf, s);
    *e = '/';

    if ((e = strchr(buf, ':')) != NULL)
    {
        *e = 0;
    }
    return 0;
}

int get_enc_sys_id(char *pd_sys_id, char *buf)
{
    char tbuf[MAX_BUF_LEN] = {0};
    get_usb_bus(pd_sys_id, tbuf);

    sprintf(buf, "tr_%s", tbuf);
    return 0;
}

int _get_enc_usb_bus_speed(char *pd_sys_id, char *buf)
{
    char tbuf[MAX_BUF_LEN] = {0};
    char cmd[MAX_CMD_LEN] = {0};
    
    get_usb_bus(pd_sys_id, tbuf);
    
    sprintf(cmd, "/bin/cat /sys/bus/usb/devices/%s/speed", tbuf); 
    hal_popen(cmd, buf);
}

int get_usb_bus_version(char *pd_sys_id, char *buf)
{
    char tbuf[MAX_BUF_LEN] = {0};
    char cmd[MAX_CMD_LEN] = {0};
    
    get_usb_bus(pd_sys_id, tbuf);
    
    sprintf(cmd, "/bin/cat /sys/bus/usb/devices/%s/version", tbuf); 
    hal_popen(cmd, buf);
}

int tr_convert_speed(char *pd_sys_id, int default_type)
{
    char buf[MAX_BUF_LEN] = {0};
    get_usb_bus_version(pd_sys_id, buf);
	if (strstr(buf, "3.10"))
		return PD_USB_SS_3_1_Gen1;	
	return default_type;
}

int get_enc_usb_bus_speed(char *pd_sys_id, char *buf)
{
	PD_LINK_SPEED speed = PD_UNKNOWN_SPEED;
    char tbuf[MAX_BUF_LEN] = {0};
    _get_enc_usb_bus_speed(pd_sys_id, tbuf);

	if (strcmp(tbuf, "10000") == 0)
		speed = PD_USB_SSP;

	if (strcmp(tbuf, "5000") == 0)
		speed = tr_convert_speed(pd_sys_id, PD_USB_SS);

	if (strcmp(tbuf, "480") == 0)
		speed = PD_USB_HS;

	if (strcmp(tbuf, "12") == 0)
		speed = PD_USB_FS;

	if (strcmp(tbuf, "1.5") == 0)
		speed = PD_USB_LS;
	
	sprintf(buf, "%d", speed);
	return 0;
}

int get_usb_layer(char *pd_sys_id, char *buf)
{
	int i;
	int layer = 0;
    char tbuf[MAX_BUF_LEN] = {0};

    get_usb_bus(pd_sys_id, tbuf);

	for (i = 0; i < strlen(tbuf); i++)
	{
		if (tbuf[i] == '.')
			layer ++;
	}
	return layer;
	
}

int get_enc_upper_usb_bus(char *pd_sys_id, char *buf)
{
	char path[MAX_PATH_LEN] = {0};
    char tbuf[MAX_BUF_LEN] = {0};
	char *s, *e;

    get_usb_bus(pd_sys_id, tbuf);
	get_sg_path(pd_sys_id, path);
	
	e = strstr(path, tbuf) - 1;
	*e = 0;
	s = rindex(path, '/') + 1;
	sprintf(buf, "%s", s);
	return 0;	
}

int get_enc_parent_sys_id(char *pd_sys_id, char *buf)
{
	int layer = get_usb_layer(pd_sys_id, NULL);	

	if(layer <= 2)
	{
		strcpy(buf, "root_usb");	
	}
	else
	{
		get_enc_upper_usb_bus(pd_sys_id, buf);	
	}
	return 0;
}

typedef int (*encfunc)(char *pd_sys_id, ENCLOSURE_INFO *enc_info);

int set_enc_tr_bus_speed(char *pd_sys_id, ENCLOSURE_INFO *enc_info)
{
	printf(__func__);
    char buf[MAX_BUF_LEN] = {0};
	
	get_enc_usb_bus_speed(pd_sys_id, buf);
    enc_info->tr_bus_speed = atoi(buf);
}

int set_enc_parent_sys_id(char *pd_sys_id, ENCLOSURE_INFO *enc_info)
{
    char buf[MAX_BUF_LEN] = {0};
	get_enc_parent_sys_id(pd_sys_id, buf);
	sprintf(enc_info->enc_parent_sys_id, "%s", buf);
}

encfunc encfarr[2] = {
	set_enc_tr_bus_speed,
	set_enc_parent_sys_id,
};

int get_enc_info(char *pd_sys_id, ENCLOSURE_INFO *enc_info)
{
	int i;
	for(i = 0 ; i < 2 ; i++)
	{
		encfarr[i](pd_sys_id, enc_info);
	}
	printf("tr_bus_speed: %d\n", enc_info->tr_bus_speed);		
	printf("tr_parent_sys_id: %s\n", enc_info->enc_parent_sys_id);		
}

int init(char *pd_sys_id, char *buf)
{
}

#ifdef UNIT_TEST
typedef int (*func)(char *pd_sys_id, char *buf);

struct func_struct {
    char name[256];
    func f;
};

static struct func_struct farr[] = {

    {"vendor", get_vendor},
    {"model", get_model},
    {"sg_path", get_sg_path},
    {"usb_buf", get_usb_bus},
    {"enc_sys_id", get_enc_sys_id},
    {"_enc_bus_speed", _get_enc_usb_bus_speed},
    {"enc_bus_speed", get_enc_usb_bus_speed},
    {"enc_bus_version", get_usb_bus_version},
    {"usb_layer", get_usb_layer},
    {"upper_usb_layer", get_enc_upper_usb_bus},
    {"", NULL},
};

void test_tr(char *pd_sys_id)
{
    int ret;
    char buf[MAX_BUF_LEN] = {0};
    int cnt = 0;
    while(farr[cnt].f != NULL)
    {
		memset(buf, 0, sizeof(buf));
		ret = farr[cnt].f(pd_sys_id, buf);
		if(strlen(buf) > 0)
		{
			str_strip(buf);
			printf("(name, ret, buf) = (%s, %d, %s)\n", farr[cnt].name, ret, buf);
		}
		else
		{
			printf("(name, ret, buf) = (%s, %d)\n", farr[cnt].name, ret);
		}
		cnt += 1;
    }
	ENCLOSURE_INFO enc_info;
	get_enc_info(pd_sys_id, &enc_info);
}

int main(int argc, char *argv[])
{
    test_tr(argv[1]);
}

#endif
