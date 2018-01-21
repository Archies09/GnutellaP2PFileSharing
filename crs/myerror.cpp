#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string>
#include<iostream>

using namespace std;

int catcherror(string message)
{
    cerr<<message<<endl;
    exit(1);
    return 1;
}
