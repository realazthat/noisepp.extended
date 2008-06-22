#include <iostream>
#include "Noise.h"

using namespace std;

int main ()
{
 try
 {
  noisepp::AdditionModule add;
  noisepp::Pipeline2D pipeline;
  add.addToPipe (pipeline);
 }
 catch (noisepp::Exception &e)
 {
  cerr << "exception thrown: " << e.getDescription() << endl;
 }
 
 return 0;
}
