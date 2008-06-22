#include <iostream>

#include "Noise.h"
#include "NoiseUtils.h"

using namespace std;
using namespace noisepp;
using namespace noisepp::utils;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cerr << "Usage: toimage [file]" << endl;
		return 1;
	}

	string filename = argv[1];
	string outfile = "out.bmp";
	// open file
	FileInStream f(filename.c_str());
	if (!f.isOpen())
	{
		cerr << "Can't open file for reading: " << filename << endl;
		return 1;
	}
	int w, h;
	w = h = 512;
	// allocate memory for height data
	Real *data = new Real[w*h];
	try
	{
		// read data
		Reader reader(f);
		if (reader.getModule() == NULL)
		{
			cerr << "File in wrong format or corrupted: " << filename << endl;
			delete[] data;
			return 1;
		}
	
		noisepp::utils::PlaneBuilder2D builder;
		builder.setModule (reader.getModule());
		builder.setSize (w, h);
		builder.setBounds (0, 0, 5, 5);
		builder.setDestination (data);
		builder.build ();
		
		// create a image
		noisepp::utils::Image img;
		img.create (w, h);
	
		// colorize
		noisepp::utils::GradientRenderer gradients;
		gradients.addGradient (-1.0000, noisepp::utils::ColourValue (  0,   0, 128)/255.f); // deeps
		gradients.addGradient (-0.2500, noisepp::utils::ColourValue (  0,   0, 255)/255.f); // shallow
		gradients.addGradient ( 0.0000, noisepp::utils::ColourValue (  0, 128, 255)/255.f); // shore
		gradients.addGradient ( 0.0625, noisepp::utils::ColourValue (240, 240,  64)/255.f); // sand
		gradients.addGradient ( 0.1250, noisepp::utils::ColourValue ( 32, 160,   0)/255.f); // grass
		gradients.addGradient ( 0.3750, noisepp::utils::ColourValue (224, 224,   0)/255.f); // dirt
		gradients.addGradient ( 0.7500, noisepp::utils::ColourValue (128, 128, 128)/255.f); // rock
		gradients.addGradient ( 1.0000, noisepp::utils::ColourValue (255, 255, 255)/255.f); // snow
		gradients.renderImage (img, data);
	
		f.close ();

		img.saveBMP (outfile.c_str());
	}
	catch (std::exception &e)
	{
		cerr << "Exception thrown: " << e.what() << endl;
	}
	delete[] data;
	data = 0;
	
	return 0;
}
