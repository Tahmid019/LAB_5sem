#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFF 4096

struct Student {
    int roll;
    char name[50];
    char dept[50];
    float marks;
};


