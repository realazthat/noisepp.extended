#include <iostream>
#include "Noise.h"

int main ()
{
 // We use a perlin module here
 noisepp::PerlinModule perlin;
 // now we change the frequency parameter
 perlin.setFrequency (2.0);
 // We use a 2D pipeline here
 noisepp::Pipeline2D *pipeline = new noisepp::Pipeline2D;
 // Add the module to the pipeline and store the ID of the created pipeline element.
 noisepp::ElementID id = perlin.addToPipeline(pipeline);
 // Now we get an pointer to the element using its ID
 noisepp::PipelineElement2D *element = pipeline->getElement(id);
 // Ask the pipeline for a cache buffer
 noisepp::Cache *cache = pipeline->createCache ();
 noisepp::Real value;
 // Calculate the value at (1.2|0.2)
 value = element->getValue (1.2, 0.2, cache);
 // and print it
 std::cout << value << std::endl;
 // clean the cache
 pipeline->cleanCache (cache);
 // now generate another value at (1.3|0.2)
 value = element->getValue (1.3, 0.2, cache);
 // and print it
 std::cout << value << std::endl;
 // free the cache
 pipeline->freeCache (cache);
 cache = 0;
 // free the pipeline (this will also free all elements in the pipeline)
 delete pipeline;
 pipeline = 0;
 
 return 0;
}

