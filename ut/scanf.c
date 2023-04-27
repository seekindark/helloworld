#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
    const char * str = "afesim      dsg rand 10.0f";
    char buf[10][20] = {{0,0,}, };
    float f1, f2;

    printf("argc=%d, argv[1]=%s \n", argc, argv[1]);
    int n = sscanf(argv[1], "%s %s %s %f %f", buf[0], buf[1], buf[2], &f1, &f2);
    printf("n=%d\n", n);

    for (int i=0; i < 3; i++)
        printf("buf[%d] = %s\n", i, buf[i]);
    printf("f1=%f, f2=%f\n", f1, f2);


    printf("aoti(-10) = %d\n", atoi("-10"));
    printf("atof(10.12f)=%f\n", atof("10.12"));
    printf("atof(10) = %f\n", atof("10"));
    printf("atof(10.0) = %f\n", atof("10.0"));
    printf("atof(%s) = %f\n", buf[3],  atof(buf[3]));
    printf("atof(%s) = %f\n", buf[4], atof(buf[4]));
    return 0;
}
