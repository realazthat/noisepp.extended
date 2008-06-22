#include <iostream>
#include "Noise.h"

using namespace std;

int main ()
{
 // create the perlin module (we will keep the default parameters here)
 noisepp::PerlinModule perlin;
 // create the constant module
 noisepp::ConstantModule constant;
 // set a constant value of 0.5
 constant.setValue (0.5);
 // create the addition module
 noisepp::AdditionModule add;
 // set the two source modules
 add.setSourceModule (0, perlin);
 add.setSourceModule (1, constant);
 // create the pipeline
 noisepp::Pipeline2D *pipeline = new noisepp::Pipeline2D;
 // add the addition module to the pipeline. this will also add all source modules recursively.
 noisepp::ElementID id = add.addToPipeline (pipeline);
 // get a pointer to the element of the addition module
 noisepp::PipelineElement2D *addElement = pipeline->getElement(id);
 // get pointers to the other elements
 noisepp::PipelineElement2D *perlinElement = pipeline->getElementPtr(perlin);
 noisepp::PipelineElement2D *constantElement = pipeline->getElementPtr(constant);
 // create the cache
 noisepp::Cache *cache = pipeline->createCache();
 // calculate the values
 noisepp::Real x = 0.2;
 noisepp::Real y = 0.1;
 noisepp::Real value;
 value = perlinElement->getValue (x, y, cache);
 cout << "perlin: " << value << endl;
 value = constantElement->getValue (x, y, cache);
 cout << "constant: " << value << endl;
 value = addElement->getValue (x, y, cache);
 cout << "add: " << value << endl;
 
 noisepp::ElementID perlinID = pipeline->getElementID(perlin);
 noisepp::ElementID constantID = pipeline->getElementID(constant);
 // get the value of the perlin module
 value = cache[perlinID].value;
 cout << "perlin(cached): " << value << endl;
 // get the value of the constant module
 value = cache[constantID].value;
 cout << "constant(cached): " << value << endl;
 
 // free the cache
 pipeline->freeCache (cache);
 cache = 0;
 // free the pipeline (this will also free all elements in the pipeline)
 delete pipeline;
 pipeline = 0;
 
 return 0;
}
