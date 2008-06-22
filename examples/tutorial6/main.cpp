#include <iostream>
#include "Noise.h"
#include "NoiseUtils.h"

using namespace std;

int main ()
{
 // create file out stream
 noisepp::utils::FileOutStream f;
 // open file
 if (!f.open ("test.pipeline"))
 {
  cerr << "Can't open file for writing" << endl;
  return 1;
 }
 noisepp::PerlinModule perlin;
 perlin.setOctaveCount(7);
 perlin.setQuality(noisepp::NOISE_QUALITY_HIGH);
 // define the writer
 noisepp::utils::Writer writer(f);
 // add the module
 writer.addModule (perlin);
 // write it
 writer.writePipeline ();
 // close the file
 f.close ();
 
 return 0;
}
