#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

int main(int argc, char** argv)
{
    void *handle1;
    void *handle2;
    void (*func)();

    handle1 = dlopen("./share/s1.so", RTLD_LAZY);
    handle2 = dlopen("./share/s2.so", RTLD_LAZY);

    if (!handle1) {
        /* fail to load the library */
        fprintf(stderr, "Error: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    func = dlsym(handle1, "is_match");
    if (!func) {
        /* no such symbol */
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle1);
        return EXIT_FAILURE;
    }

    func();
    dlclose(handle1);
    func = dlsym(handle2, "is_match");
    func();

    return EXIT_SUCCESS;
}
