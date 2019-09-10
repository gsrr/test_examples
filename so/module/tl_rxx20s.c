#include "hal.h"

int find_module_by_enc_sysid(char *enc_sys_id)
{
    int ret;
    ret = comm_sys_check_is_sas_expander(enc_sys_id);
    printf("(func, ret) = (%s, %s, %d)\n", __func__, enc_sys_id, ret);
    return ret;
}


/*
 * @param enc_sys_id system dependent enclosure identifier. (ex: sg5)
 */
int get_enc_info(char *enc_sys_id, ENCLOSURE_INFO *enc_info)
{
    int ret;
    printf("Hello World\n");
    ret = sas_expander_getinfo(enc_sys_id, enc_info);
    printf("(func, enc_sys_id, ret) = (%s, %s, %d)\n", __func__, enc_sys_id, ret);
    return ret;
}

int write_enc_conf(int enc_id, ENCLOSURE_INFO *enc_info)
{
    int ret = -1;
    printf("(func, enc_id, ret) = (%s, %d, %d)\n", __func__, enc_id, ret);
    return ret;
}

int pd_scan(int enc_id)
{
    int ret = -1;
    printf("(func, enc_id, ret) = (%s, %d, %d)\n", __func__, enc_id, ret);
    return ret;
}

int se_attach_specific(char *enc_sys_id, int enc_id)
{
    int ret = -1;
    ret = se_sys_set_7_segment_led(enc_sys_id, enc_id); 
    printf("(func, enc_id, ret) = (%s, %d, %d)\n", __func__, enc_id, ret);
    return 0;
}

#ifdef UNIT_TEST
void test_tl_r20xxs(char *enc_sys_id)
{
	ENCLOSURE_INFO enc_info;
    //get_enc_info(enc_sys_id, &enc_info);
    se_sys_getinfo(enc_sys_id);
}

int main(int argc, char *argv[])
{
    test_tl_r20xxs(argv[1]);
}

#endif
