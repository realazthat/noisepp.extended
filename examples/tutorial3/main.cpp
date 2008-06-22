#include <iostream>
#include "Noise.h"
#if NOISEPP_ENABLE_UTILS
# include "NoiseUtils.h"
#endif

using namespace std;

class LineJob2D : public noisepp::PipelineJob
{
 private:
  noisepp::Pipeline2D *mPipe;
  noisepp::PipelineElement2D *mElement;
  noisepp::Real x, y;
  int n;
  noisepp::Real xDelta;
  noisepp::Real *buffer;

 public:
  /// Constructor.
  /// @param pipe The pipeline.
  /// @param element A pointer to the pipeline element.
  /// @param x The start x-coordinate.
  /// @param y The y-coordinate.
  /// @param n The number of pixels to build.
  /// @param xDelta The delta value the x-coordinate will change each pixel.
  /// @param buffer A pointer to the output buffer.
  LineJob2D (noisepp::Pipeline2D *pipe, noisepp::PipelineElement2D *element, noisepp::Real x, noisepp::Real y, int n, noisepp::Real xDelta, noisepp::Real *buffer) :
   mPipe(pipe), mElement(element), x(x), y(y), n(n), xDelta(xDelta), buffer(buffer)
  {}
  void execute (noisepp::Cache *cache)
  {
   for (int i=0;i<n;++i)
   {
    // cleans the cache
    mPipe->cleanCache (cache);
    // calculates the value
    buffer[i] = mElement->getValue(x, y, cache);
    // move on
    x += xDelta;
   }
  }
};

int main ()
{
 noisepp::PerlinModule perlin;
#if NOISEPP_ENABLE_UTILS
 noisepp::Pipeline2D *pipeline = noisepp::utils::System::createOptimalPipeline2D();
#elif NOISEPP_ENABLE_THREADS
 noisepp::Pipeline2D *pipeline = new noisepp::ThreadedPipeline2D(2);
#else
 noisepp::Pipeline2D *pipeline = new noisepp::Pipeline2D;
#endif
 noisepp::PipelineElement2D *element = pipeline->getElement(perlin.addToPipeline(pipeline));
 
 int w = 512;
 int h = 512;
 // create a buffer
 noisepp::Real *buffer = new noisepp::Real[w*h]; 
 noisepp::Real xPos = 0;
 noisepp::Real yPos = 0;
 noisepp::Real xDelta = 1.0/noisepp::Real(w);
 noisepp::Real yDelta = 1.0/noisepp::Real(h);
 // add the jobs
 for (int y=0;y<h;++y)
 {
  pipeline->addJob (new LineJob2D(pipeline, element, xPos, yPos, w, xDelta, buffer+(y*w)));
  // move along the y-axis
  yPos += xDelta;
 }
 // execute the jobs
 pipeline->executeJobs ();
 // do something with the buffer
 // ...
 cout << buffer[0] << endl;
 // ...

 // free buffer
 delete[] buffer;
 buffer = 0;
 // free the pipeline
 delete pipeline;
 pipeline = 0;

 return 0;
}
