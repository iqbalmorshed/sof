#include<iostream>
#include "BWT.h"
using namespace std;

int main(){

    const BWT bwt(std::string(indexPrefix + BWT_EXT), opt::sampleRate);
    numstr = bwt.getNumStrings();
    bwtlen = bwt.getBWLen();
    bwt.printInfo();
}
