/*
before optimation:
calculating test value at (1.2|0.2)
Noise++ (2D): 0.413553
Noise++ (3D): 0.413553
prepare for generating 10 million points ... go!
generating using Pipeline3D .......... done
time: 3.38359 sec
2.95544 million pixels per second
generating using Pipeline2D .......... done
time: 1.76692 sec
5.65956 million pixels per second
*/

#include <iostream>
#include <iomanip>
#include <time.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

//#define COMPARE_WITH_LIBNOISE
#define DO_CPU_WARMUP

#ifdef COMPARE_WITH_LIBNOISE
#include <noise/noise.h>
#endif

#include "Noise.h"
#if NOISEPP_ENABLE_UTILS
#	include "NoiseUtils.h"
#endif

using namespace std;

const int MILLION_POINTS = 10;

double Timer_getSeconds ()
{
	double ticks;
#ifdef WIN32
	ticks=(double)GetTickCount() / 1000.0;
#else
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC,&now);
	ticks=(now.tv_sec)+(now.tv_nsec)/1000000000.0;
#endif
	return(ticks);
}

/// 2D pipeline job generating a line
class LineJob2D : public noisepp::LineJob2D
{
	private:
		bool mProgress;

	public:
		LineJob2D (noisepp::Pipeline2D *pipe, noisepp::PipelineElement2D *element, double x, double y, int n, double xDelta, double *buffer, bool progress) :
			noisepp::LineJob2D(pipe, element, x, y, n, xDelta, buffer), mProgress(progress)
		{}
		void finish ()
		{
			if (mProgress)
			{
				cout << ".";
				cout.flush ();
			}
		}
};

/// 3D pipeline job generating a line
class LineJob3D : public noisepp::LineJob3D
{
	private:
		bool mProgress;

	public:
		LineJob3D (noisepp::Pipeline3D *pipe, noisepp::PipelineElement3D *element, double x, double y, double z, int n, double xDelta, double *buffer, bool progress) :
			noisepp::LineJob3D(pipe, element, x, y, z, n, xDelta, buffer), mProgress(progress)
		{}
		void finish ()
		{
			if (mProgress)
			{
				cout << ".";
				cout.flush ();
			}
		}
};

/// calculates the maximum difference between the two arrays specified
double calcMaxDifference (double *left, double *right, int n)
{
	double dif = 0;
	for (int i=0;i<n;++i)
	{
		const double d = fabs(left[i]-right[i]);
		if (d > dif)
			dif = d;
	}
	return dif;
}

void test (noisepp::Module &module, bool compare)
{
	double x, y;
	double xdelta = 0.1;
	double ydelta = 0.1;

	// generate output arrays
#ifdef COMPARE_WITH_LIBNOISE
	double *libnoise_results = 0;
	if (compare)
		libnoise_results = new double[1000000];
#endif
	double *noisepp_results = new double[1000000];

#ifdef COMPARE_WITH_LIBNOISE
	// the libnoise module for comparision
	noise::module::Perlin libnoisePerlin;
#endif

	// create default pipelines and fill them with our modules
	noisepp::Pipeline2D pipeline2D;
	noisepp::ElementID noiseID = module.addToPipe (pipeline2D);
	noisepp::Pipeline3D pipeline3D;
	noisepp::ElementID noiseID3D = module.addToPipe (pipeline3D);

	// create threaded pipelines and fill them with modules
#if NOISEPP_ENABLE_THREADS
#if NOISEPP_ENABLE_UTILS
	const int threadCount = noisepp::utils::System::getNumberOfCPUs ();
#else
	const int threadCount = 2;
#endif
	if (threadCount > 1)
		cout << "Using " << threadCount << " threads for generation with threaded pipeline." << endl;
	else
		cout << "Your system does not have a multi-core CPU ... skipping generation with threaded pipeline." << endl;

	noisepp::ThreadedPipeline2D pipeline2D_mt(threadCount);
	noisepp::ElementID noiseID2D_mt = module.addToPipe (pipeline2D_mt);
	noisepp::ThreadedPipeline3D pipeline3D_mt(threadCount);
	noisepp::ElementID noiseID3D_mt = module.addToPipe (pipeline3D_mt);
#endif

	// creates the cache
	noisepp::Cache *cache = pipeline2D.createCache();
	noisepp::Cache *cache3D = pipeline3D.createCache();

	// calculate a perlin noise value using an test coordinate (you can change this to any value)
	const double testValX = 1.2;
	const double testValY = 0.2;

	cout << "calculating test value at (" << testValX << "|" << testValY << ")" << endl;
#ifdef COMPARE_WITH_LIBNOISE
	cout << "libnoise: " << libnoisePerlin.GetValue (testValX, testValY, 0) << endl;
#endif
	cout << "Noise++ (2D): " << pipeline2D.getElement(noiseID)->getValue (testValX, testValY, cache) << endl;
	cout << "Noise++ (3D): " << pipeline3D.getElement(noiseID3D)->getValue (testValX, testValY, 0, cache3D) << endl;

	// calculates some square roots to bring the CPU(s) to maximum frequency - i know, not really professional solution
	// (you can skip this step but at least on my PC I get more accurate benchmark results with it)
#ifdef DO_CPU_WARMUP
	cout << "prepare for generating " << MILLION_POINTS << " million points ... ";
	cout.flush ();

	double *test = new double[1000000];
	for (int n=0;n<100;++n)
	{
		for (int i=0;i<1000000;++i)
		{
			test[i] = sqrt((double)rand()/RAND_MAX);
		}
	}
	delete[] test;
#endif

	cout << "go!" << endl;

	// generate the points with libnoise
	double seconds;
	double mpix = MILLION_POINTS;
#ifdef COMPARE_WITH_LIBNOISE
	double libnoiseTime = 0;
	if (compare)
	{
		cout << "generating using libnoise ";
		cout.flush ();
		double lnoiseStart = Timer_getSeconds();
		for (int i=0;i<MILLION_POINTS;++i)
		{
			cout << ".";
			cout.flush ();
			double *res = libnoise_results;
			y = 0;
			for (int yy=0;yy<1000;++yy)
			{
				x = 0;
				for (int xx=0;xx<1000;++xx)
				{
					*res++ = libnoisePerlin.GetValue (x, y, 0);
					x += xdelta;
				}
				y += ydelta;
			}
		}
		cout << " done" << endl;
		// calculate stats
		seconds = Timer_getSeconds()-lnoiseStart;
		libnoiseTime = seconds * 100.0;
		cout << "time: " << seconds << " sec" << endl;
		cout << mpix / seconds << " million pixels per second" << endl;
	}
#endif

	// get the pointers to the pipeline elements
	noisepp::PipelineElement2D *element = pipeline2D.getElement(noiseID);
	noisepp::PipelineElement3D *element3D = pipeline3D.getElement(noiseID3D);
#if NOISEPP_ENABLE_THREADS
	noisepp::PipelineElement2D *element2D_mt = pipeline2D.getElement(noiseID2D_mt);
	noisepp::PipelineElement3D *element3D_mt = pipeline3D.getElement(noiseID3D_mt);
#endif

	// generate using the default 3D pipeline
	cout << "generating using Pipeline3D ";
	cout.flush ();
	double noiseppStart3D = Timer_getSeconds();
	for (int i=0;i<MILLION_POINTS;++i)
	{
		cout << ".";
		cout.flush ();
		double *res = noisepp_results;
		y = 0;
		for (int yy=0;yy<1000;++yy)
		{
			x = 0;
			for (int xx=0;xx<1000;++xx)
			{
				// clean cache
				pipeline3D.cleanCache(cache3D);
				// calculate value
				*res++ = element3D->getValue (x, y, 0, cache3D);
				// move on
				x += xdelta;
			}
			y += ydelta;
		}
	}
	cout << " done" << endl;
	// calculate stats
	seconds = Timer_getSeconds()-noiseppStart3D;
	cout << "time: " << seconds << " sec" << endl;
	cout << mpix / seconds << " million pixels per second";
#ifdef COMPARE_WITH_LIBNOISE
	if (compare)
	{
		cout << " (" << libnoiseTime / seconds - 100.0 << "% faster than libnoise)";
		if (calcMaxDifference(noisepp_results, libnoise_results, 1000*1000) > 1.0e-07)
			cout << "WARNING: different result!" << endl;
	}
#endif
	cout << endl;

	// generate using the default 2D pipeline
	cout << "generating using Pipeline2D ";
	cout.flush ();
	double noiseppStart2D = Timer_getSeconds();
	for (int i=0;i<MILLION_POINTS;++i)
	{
		cout << ".";
		cout.flush ();
		double *res = noisepp_results;
		y = 0;
		for (int yy=0;yy<1000;++yy)
		{
			x = 0;
			for (int xx=0;xx<1000;++xx)
			{
				pipeline2D.cleanCache(cache);
				*res++ = element->getValue (x, y, cache);
				x += xdelta;
			}
			y += ydelta;
		}
	}
	cout << " done" << endl;
	// calculate stats
	seconds = Timer_getSeconds()-noiseppStart2D;
	cout << "time: " << seconds << " sec" << endl;
	cout << mpix / seconds << " million pixels per second";
#ifdef COMPARE_WITH_LIBNOISE
	if (compare)
	{
		cout << " (" << libnoiseTime / seconds - 100.0 << "% faster than libnoise)";
		if (calcMaxDifference(noisepp_results, libnoise_results, 1000*1000) > 1.0e-07)
			cout << "WARNING: different result!" << endl;
	}
#endif
	cout << endl;

#if NOISEPP_ENABLE_THREADS
	if (threadCount > 1)
	{
		// generate using the threaded 3D pipeline
		cout << "generating using ThreadedPipeline3D ";
		cout.flush ();
		double noiseppStart3D_mt = Timer_getSeconds();
		for (int i=0;i<MILLION_POINTS;++i)
		{
			double *res = noisepp_results;
			y = 0;
			for (int yy=0;yy<1000;++yy)
			{
				pipeline3D_mt.addJob (new LineJob3D(&pipeline3D_mt, element3D_mt, 0, y, 0, 1000, xdelta, res, (yy==0)));
				res += 1000;
				y += ydelta;
			}
			pipeline3D_mt.executeJobs ();
		}
		cout << " done" << endl;
		// calculate stats
		seconds = Timer_getSeconds()-noiseppStart3D_mt;
		cout << "time: " << seconds << " sec" << endl;
		cout << mpix / seconds << " million pixels per second";
#ifdef COMPARE_WITH_LIBNOISE
		if (compare)
		{
			cout << " (" << libnoiseTime / seconds - 100.0 << "% faster than libnoise)";
			if (calcMaxDifference(noisepp_results, libnoise_results, 1000*1000) > 1.0e-07)
				cout << "WARNING: different result!" << endl;
		}
#endif
		cout << endl;

		// generate using the threaded 2D pipeline
		cout << "generating using ThreadedPipeline2D ";
		cout.flush ();
		double noiseppStart2D_mt = Timer_getSeconds();
		for (int i=0;i<MILLION_POINTS;++i)
		{
			double *res = noisepp_results;
			y = 0;
			for (int yy=0;yy<1000;++yy)
			{
				pipeline2D_mt.addJob (new LineJob2D(&pipeline2D_mt, element2D_mt, 0, y, 1000, xdelta, res, (yy==0)));
				res += 1000;
				y += ydelta;
			}
			pipeline2D_mt.executeJobs ();
		}
		cout << " done" << endl;
		// calculate stats
		seconds = Timer_getSeconds()-noiseppStart2D_mt;
		cout << "time: " << seconds << " sec" << endl;
		cout << mpix / seconds << " million pixels per second";
#ifdef COMPARE_WITH_LIBNOISE
		if (compare)
		{
			cout << " (" << libnoiseTime / seconds - 100.0 << "% faster than libnoise)";
			if (calcMaxDifference(noisepp_results, libnoise_results, 1000*1000) > 1.0e-07)
				cout << "WARNING: different result!" << endl;
		}
#endif
		cout << endl;
	}
#endif

#ifdef COMPARE_WITH_LIBNOISE
	if (compare)
	{
		delete[] libnoise_results;
	}
#endif
	delete[] noisepp_results;

	// frees the cache
	pipeline2D.freeCache(cache);
	pipeline3D.freeCache(cache3D);
}

int main ()
{
	int ret = 0;
	try
	{
		// the Noise++ module
		noisepp::PerlinModule noiseppPerlin;
		// run the test with default perlin noise settings (which are compatible to libnoise, so do also compare results)
		cout << "--- RUNNING TEST WITH A PERLIN NOISE MODULE USING DEFAULT SETTINGS ---" << endl;
		test(noiseppPerlin, true);
		// now set the module to use fast noise
		noiseppPerlin.setQuality(noisepp::NOISE_QUALITY_FAST_STD);
		// and run the test again but without comparision
		cout << "--- NOW RUNNING TEST WITH A PERLIN NOISE MODULE USING FAST NOISE ---" << endl;
		test(noiseppPerlin, false);
	}
	catch (noisepp::Exception &e)
	{
		cerr << "exception thrown: " << e.getDescription() << endl;
		ret = 1;
	}
#if NOISEPP_PLATFORM == NOISEPP_PLATFORM_WINDOWS
	cout << "Press any key ..." << endl;
	cin.get();
#endif
	return ret;
}
