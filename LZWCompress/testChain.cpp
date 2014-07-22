#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <cstdint>
#include "yinexception.h"
#include "linkhash.h"
#include "LZWCompress.h"



int main(int argc, char* argv[])
{
    LZWComp lc;
    lc.unitTest(argv[1]);
}