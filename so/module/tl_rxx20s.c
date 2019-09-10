#include "hal.h"

int find_module_by_enc_sysid(char *enc_sys_id)
{
    int ret;
    ret = comm_sys_check_is_sas_expander(enc_sys_id);
    printf("(func, ret) = (%s, %d)\n", __func__, ret);
}


/*
 * @param enc_sys_id system dependent enclosure identifier. (ex: sg5)
 */
int get_enc_info(char *enc_sys_id, ENCLOSURE_INFO *enc_info)
{
    int ret;
    printf("get_enc_info : %s\n", enc_sys_id);
    ret = sas_expander_getinfo(enc_sys_id, enc_info);
    printf("(func, ret) = (%s, %d)\n", __func__, ret);
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
