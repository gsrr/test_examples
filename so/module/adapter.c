#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>

#include "hal.h"
#include "adapter.h"
#include <fcntl.h>
#define MODULE_PATH    "/root/module/"

struct module_struct {
    char name[64];
    void *handle;
};

struct module_struct modules[1];

int loaded = 0;

int load_modules()
{
    if(loaded == 1)
    {
        return 0;;
    }
    loaded = 1;

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
            continue;
        }

        sprintf(buf, "%s/%s", MODULE_PATH, de->d_name);
        if(buf)
        {
            sprintf(modules[i].name, "%s", de->d_name);
            modules[i].handle = dlopen(buf, RTLD_LAZY);
            i++;
        }
    }
    closedir(dr);
}

void get_time(char *time_str)
{
    time_t time_val;

	time(&time_val);
	sprintf(time_str, "%s",strtok(ctime(&time_val), "\n"));
}

int get_fd(char *log_file)
{
	int fd = -1;
	if ((fd = open(log_file, O_RDWR | O_CREAT | O_APPEND, S_IRWXU)) < 0)
	{
		return -1;
	}
	if (flock(fd, LOCK_EX) < 0)
	{
		close(fd);
		return -1;
	}
	return fd;	
}

int adapter_debug_log(int flags, const char* format, ...)
{
	char *fpath = "/var/log/da_util.log";
    va_list argptr;
    int fd = 1;
    int ret = 0;
    char buffer[1024];
	char time_str[128] = {0};
	struct stat st;
	
	get_time(time_str);
	
	fd = get_fd(fpath);
	if(fd == -1)
	{
		return -1;
	}
	
	sprintf(buffer, "[%s] ", time_str);
	va_start(argptr, format);
	vsprintf(buffer + strlen(buffer), format, argptr);
	ret = write(fd, buffer, strlen(buffer));
	va_end(argptr);

	
	stat(fpath, &st);
	if(st.st_size > MAX_LOG_FILE_SIZE)
	{
		unlink(fpath);	
	}
	flock(fd, LOCK_UN);
	close(fd);
    return ret;

}

int find_module_by_enc_sysid(char *enc_sys_id)
{
    int i;
    int ret = -1;

    load_modules();
    for(i = 0 ; i < 1 ; i++)
    {
        int (*func)(char*);
        func = dlsym(modules[i].handle, "find_module_by_enc_sysid");
        ret = func(enc_sys_id);
        if(ret == 0)
        {
            return i;
        }
    }
    return ret;
}

int find_module_by_enc_id(int enc_id)
{
    int ret = -1;
    char enc_sys_id[64] = {0};
    
    ret = se_lookup_sys_id(enc_id, enc_sys_id, sizeof(enc_sys_id));
    if(ret >= 0)
    {
        return find_module_by_enc_sysid(enc_sys_id);
    }
    return ret;
}

int get_enc_info(char *enc_sys_id, ENCLOSURE_INFO *enc_info)
{
    int i;
    int (*func)(char*, ENCLOSURE_INFO*);

    i = find_module_by_enc_sysid(enc_sys_id);
    if(i < 0)
    {
        return -1;
    }
    adapter_debug_log(LOG_ERROR, "%s : (%s)\n", __func__, modules[i].name);
    func = dlsym(modules[i].handle, "get_enc_info");
    return func(enc_sys_id, enc_info); 
}


int write_enc_conf(int enc_id, ENCLOSURE_INFO *enc_info)
{
    int i;
    int (*func)(int, ENCLOSURE_INFO*);

    i = find_module_by_enc_sysid(enc_info->enc_sys_id);
    if(i < 0)
    {
        return -1;
    }
    adapter_debug_log(LOG_ERROR, "%s : (%s)\n", __func__, modules[i].name);
    func = dlsym(modules[i].handle, "write_enc_conf");
    return func(enc_id, enc_info); 
}

int pd_scan(int enc_id)
{
    int i;
    int (*func)(int);

    i = find_module_by_enc_id(enc_id);
    if(i < 0)
    {
        return -1;
    }
    adapter_debug_log(LOG_ERROR, "%s : (%s)\n", __func__, modules[i].name);
    func = dlsym(modules[i].handle, "pd_scan");
    return func(enc_id); 
}

int se_attach_specific(char *enc_sys_id, int enc_id)
{
    int i;
    int (*func)(char*, int);

    i = find_module_by_enc_sysid(enc_sys_id);
    if(i < 0)
    {
        return -1;
    }
    adapter_debug_log(LOG_ERROR, "%s : (%s)\n", __func__, modules[i].name);
    func = dlsym(modules[i].handle, "se_attach_specific");
    return func(enc_sys_id, enc_id); 
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
