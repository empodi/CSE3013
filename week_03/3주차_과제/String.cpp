#include "String.h"

using namespace std;

Str::Str(int leng) {
    
    if ( leng < 0 )
        cout << "Invalid Input" << endl;
    else
	    len = leng;
}
Str::Str(char* neyong) {

    len = strlen(neyong);
    str =  new char[len + 1];
    strcpy(str, neyong);
}
Str::~Str() { 
    
    delete[] str;
}
int Str::length() {

    return len;
}
char* Str::contents() {

    return str;
}

int Str::compare(class Str& a) {

    return strcmp(str, a.contents());
}
int Str::compare(char* a) {

    return strcmp(str, a);
}

void Str::operator=(char* a) {

    len = strlen(a);
    delete[] str;
    str = new char[len + 1];
    strcpy(str, a);
}
void Str::operator=(class Str& a) {

    len = a.length();
    delete[] str; 
    str = new char[len + 1];
    strcpy(str, a.contents());
}



