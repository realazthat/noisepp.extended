NOISE++ LIBRARY

I. DEPENDENCIES
	Boost Thread Library (not needed if threads are disabled)


II. CONFIGURE

To configure Noise++ edit the noisepp/core/NoiseConfig.h file.


III. BUILD

Currently Noise++ is a header-only library so you don't have to build it. Just add the noisepp/core folder to your include directories.


IV. BUILD EXAMPLES

There is only one simple example application included for now. It is a simple performance comparision between libnoise and Noise++. To build it in Linux just go to the examples/test folder and type "make". In Windows create a new console application and add the examples/test/main.cpp file. Then add the noisepp/core folder and the libnoise headers to your include directories and link it against libnoise and boost_thread.


V. BASIC USAGE

To include Noise++ into your C++ application add the noisepp/core folder to your include directories and type:
#include <Noise.h>

In Noise++ the noise generation process is different to other libraries. Instead of calling a noise generation function from your module instances directly, you add your modules to a so called Pipeline. The pipeline internally creates pipeline elements from the specified noise modules. Then you create a cache and start generating.

// create the module
noisepp::PerlinModule3D perlinModule;
// set some module options
perlinModule.setSeed (3);
perlinModule.setFrequency (2.0);
// create the pipeline
noisepp::Pipeline3D pipeline;
// add the module to the pipeline
noisepp::PipelineElement3D *perlinElement = pipeline.getElement(perlinModule.addToPipe(pipeline));
// create the cache
noisepp::Cache *cache = pipeline.createCache ();
double x = 0.0;
double xDelta = 0.1;
for (int n=0;n<10;++n)
{
	// clean the cache
	pipeline.cleanCache (cache);
	// calculate
	std::cout << perlinElement->getValue (x, 0, 0, cache) << std::endl;
	
	x += xDelta;
}
// free the cache
pipeline.freeCache (cache);

There are also some modules that modificate the values they get from their source modules. Here is an example of a module adding together two perlin noise modules:

// module 0
noisepp::PerlinModule3D perlinModule0;
// module 1
noisepp::PerlinModule3D perlinModule1;
perlinModule1.setSeed (3);
perlinModule1.setFrequency (2.0);
// add them together
noisepp::AdditionModule3D addModule;
addModule.setSourceModule (0, perlinModule0);
addModule.setSourceModule (1, perlinModule1);

You can now use this module to calculate values like shown in the first example.
Noise++ includes a multi-threaded Pipeline. See the libnoise comparision example for usage.

VI. LICENSE

The Noise++ Library is licensed under BSD-license (see below).
The Noise++ examples are public domain.


Noise++ Library
Copyright (c) 2008, Urs C. Hanselmann
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Noise++ Library's copyright owner nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
