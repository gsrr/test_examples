#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <dlfcn.h>
#define MODULE_PATH    "./module/"

struct module_struct {
    char name[64];
    void *handle;
};

struct module_struct modules[1];


int load_modules()
{
    int i = 0;
    struct dirent *de;
    DIR *dr = opendir(MODULE_PATH);
    if (dr == NULL)
    {
        return -1;
    }
    
    while ((de = readdir(dr)) != NULL)
    {
        char buf[128] = {0};
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
        {
            continue;
        }
        if(!strcmp(de->d_name, "adapter.so"))
        {
            printf("it is adapter.so\n");
            continue;
        }

        sprintf(buf, "%s/%s", MODULE_PATH, de->d_name);
        if(buf)
        {
            printf("module path:%s\n", buf);
            sprintf(modules[i].name, "%s", de->d_name);
            modules[i].handle = dlopen(buf, RTLD_LAZY);
            i++;
        }
    }
    closedir(dr);
}

#ifdef UNIT_TEST

int main(int argc, char *argv[])
{
    int i;
    load_modules();

    for(i = 0 ; i < 1 ; i++)
    {
        void (*func)(char*);
        printf("loaded module:%s\n", modules[i].name);       
        func = dlsym(modules[i].handle, "find_module_by_enc_sysid");
        func(argv[1]);
    }

}

#endif
