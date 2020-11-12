#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
// #include "core.h"

int main()
{
    int done = 0;

    while (done == 0)
    {
        double r = 4.0;
        double n = 5.0;

        double (*PowerOf2)(double);
        void *handle = dlopen("./build/core.dylib", RTLD_LAZY);

        if (!handle)
        {
            fprintf(stderr, "Error: %s\n", dlerror());
            return 1;
        }

        *(void **)(&PowerOf2) = dlsym(handle, "PowerOf2");
        if (!PowerOf2)
        {
            fprintf(stderr, "Error: %s\n", dlerror());
            dlclose(handle);
            return 1;
        }

        printf("-------------------------------------------------------- \n");
        // printf("Radius %.2f, area is %.2f \n", r, CircleArea(r));
        // printf("Radius %.2f, circumference is %.2f \n", r, CircleCircum(r));
        // printf("%.2f to the power of 2 is %.2f \n", n, PowerOf2(n));
        // printf("%.2f to the power of 3 is %.2f \n", n, PowerOf3(n));
        // printf("----- \n");
        printf("---> %.2f\n", PowerOf2(n));
        printf("-------------------------------------------------------- \n");

        dlclose(handle);

        sleep(2);
    }

    return 0;
}