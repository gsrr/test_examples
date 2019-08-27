#include <stdio.h>

extern void test_vpd_page_b2(char*, char*);

void hello()
{
    printf("Hello World\n");
    test_vpd_page_b2("/dev/sdb", "12,00,00,00,60,00");
}
