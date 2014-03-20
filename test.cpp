
#include <iostream>
#include "GDS.hpp"

int main(int argc,char* argv[])
{
  // load
  GDS::LIB lib(argv[1]);

  // print
  lib.print();
}
