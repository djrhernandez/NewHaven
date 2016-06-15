// Created by Junior on 6/13/2016.
#ifndef EXTERN_H
#define EXTERN_H
#include <stdio.h>


const char *dataLoc(const char *filename, bool doCheck = true);
const char *alterPathForPlatform(char *filename);
void printExtensions(FILE *fstream, const char *extstr_in);

#endif //EXTERN_H
