#include <iostream>

#include "Noise.h"
#include "NoiseUtils.h"

class LineJob2D : public noisepp::PipelineJob
{
	private:
		noisepp::Pipeline2D *mPipe;
		noisepp::PipelineElement2D *mElement;
		noisepp::Real x, y;
		int n;
		noisepp::Real delta;
		noisepp::Real *buffer;

	public:
		LineJob2D (noisepp::Pipeline2D *pipe, noisepp::PipelineElement2D *element, noisepp::Real x, noisepp::Real y, int n, noisepp::Real delta, noisepp::Real *buffer) :
			mPipe(pipe), mElement(element), x(x), y(y), n(n), delta(delta), buffer(buffer)
		{
		}
		void execute (noisepp::Cache *cache)
		{
			for (int i=0;i<n;++i)
			{
				// cleans the cache
				mPipe->cleanCache (cache);
				// calculates the value
				buffer[i] = mElement->getValue(x, y, cache);
				// move on
				x += delta;
			}
		}
};

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
	// read data
	Reader reader(f);
	if (reader.getModule() == NULL)
	{
		cerr << "File in wrong format or corrupted: " << filename << endl;
		return 1;
	}

	int w, h;
	w = h = 512;
	// allocate memory for height data
	Real *data = new Real[w*h];
	// create optimal pipeline (using one thread per CPU avaible)
	Pipeline2D *pipeline2D = System::createOptimalPipeline2D ();
	// add module to pipeline
	noisepp::ElementID id = reader.getModule()->addToPipeline (pipeline2D);
	// get a pointer to the main element
	noisepp::PipelineElement2D *element = pipeline2D->getElement(id);

	Real xmin = 0;
	Real ymin = 0;
	Real width = 5;
	Real height = 5;

	// calculate delta values
	Real xDelta = width / Real(w);
	Real yDelta = height / Real(h);

	Real y = ymin;
	// add the jobs to queue
	for (int yi=0;yi<h;++yi)
	{
		pipeline2D->addJob (new LineJob2D (pipeline2D, element, xmin, y, w, xDelta, data+yi*w));
		y += yDelta;
	}
	// execute the jobs
	pipeline2D->executeJobs ();
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

	delete pipeline2D;
	pipeline2D = 0;
	delete[] data;
	data = 0;
	f.close ();

	try
	{
		img.saveBMP (outfile.c_str());
	}
	catch (std::exception &e)
	{
		cerr << "Error while writing to " << outfile << ": " << e.what() << endl;
	}

    return 0;
}
