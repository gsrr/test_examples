#include "hal_module.h"

/*
 * @param enc_sys_id system dependent enclosure identifier. (ex: sg5)
 */
int get_enc_info(char *enc_sys_id, ENCLOSURE_INFO *enc_info)
{
    int ret;
    ret = sas_expander_getinfo(enc_sys_id, enc_info);
    printf("(func, ret) = (%s, %d)\n", __func__, ret);
}




#ifdef UNIT_TEST
void test_tl_r20xxs(char *pd_sys_id)
{
	ENCLOSURE_INFO enc_info;
    printf("Hello world\n");
}

int main(int argc, char *argv[])
{
    test_tl_r20xxs(argv[1]);
}

#endif
