#include <iostream>
#include "Noise.h"
#include "NoiseUtils.h"

using namespace std;

int main ()
{
 // our module
 noisepp::PerlinModule perlin;
 perlin.setOctaveCount (8);
 // size
 int w = 512;
 int h = 512;
 // create the buffer
 noisepp::Real *buffer = new noisepp::Real[w*h];
 // create a builder
 noisepp::utils::PlaneBuilder2D builder;
 // set the source module
 builder.setModule (perlin);
 // set the buffer
 builder.setDestination (buffer);
 // set the buffer size
 builder.setSize (w, h);
 // set the plane bounds - from (0.5|0) to (1.5|1)
 builder.setBounds (0.5, 0, 1.5, 1);
 // build
 builder.build ();
 
 // create an image
 noisepp::utils::Image img;
 img.create (w, h);
 // create the renderer and add some gradients to create a heat-vision like material
 noisepp::utils::GradientRenderer renderer;
 // renderer.addGradient (<value>, noisepp::utils::ColourValue(<red>, <green>, <blue>));
 renderer.addGradient (-1.0, noisepp::utils::ColourValue(0.0f, 0.0f, 0.2f));
 renderer.addGradient (-0.8, noisepp::utils::ColourValue(0.0f, 0.0f, 0.6f));
 renderer.addGradient ( 0.0, noisepp::utils::ColourValue(1.0f, 0.0f, 0.0f));
 renderer.addGradient ( 0.6, noisepp::utils::ColourValue(1.0f, 1.0f, 0.0f));
 renderer.addGradient ( 1.0, noisepp::utils::ColourValue(1.0f, 1.0f, 1.0f));
 // render the image
 renderer.renderImage (img, buffer);
 // save the image to an BMP
 img.saveBMP ("output.bmp");
 
 // free the buffer
 delete[] buffer;
 buffer = 0;
 
 return 0;
}
