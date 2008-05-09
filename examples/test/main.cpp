#include <iostream>
#include <time.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "Noise.h"
#include <noise/noise.h>

using namespace std;

const size_t THREAD_COUNT = 2;
const int MILLION_POINTS = 100;

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
class LineJob2D : public noisepp::PipelineJob
{
	private:
		noisepp::Pipeline2D *mPipe;
		noisepp::PipelineElement2D *mElement;
		double x, y;
		int n;
		double xDelta;
		double *buffer;

	public:
		LineJob2D (noisepp::Pipeline2D *pipe, noisepp::PipelineElement2D *element, double x, double y, int n, double xDelta, double *buffer) :
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

/// 3D pipeline job generating a line
class LineJob3D : public noisepp::PipelineJob
{
	private:
		noisepp::Pipeline3D *mPipe;
		noisepp::PipelineElement3D *mElement;
		double x, y, z;
		int n;
		double xDelta;
		double *buffer;

	public:
		LineJob3D (noisepp::Pipeline3D *pipe, noisepp::PipelineElement3D *element, double x, double y, double z, int n, double xDelta, double *buffer) :
			mPipe(pipe), mElement(element), x(x), y(y), z(z), n(n), xDelta(xDelta), buffer(buffer)
		{}
		void execute (noisepp::Cache *cache)
		{
			for (int i=0;i<n;++i)
			{
				// cleans the cache
				mPipe->cleanCache (cache);
				// calculates the value
				buffer[i] = mElement->getValue(x, y, z, cache);
				// move on
				x += xDelta;
			}
		}
};

/// calculates the mean difference between the two arrays specified
double calcMeanDifference (double *left, double *right, int n)
{
	double dif = 0;
	for (int i=0;i<n;++i)
	{
		dif += fabs(left[i]-right[i]);
	}
	return dif / double(n);
}

int main ()
{
	double x, y;
	double xdelta = 0.1;
	double ydelta = 0.1;

	// generate output arrays
	double *libnoise_results = new double[1000000];
	double *noisepp_results = new double[1000000];

	// the libnoise module for comparision
	noise::module::Perlin libnoisePerlin;

	// the Noise++ modules
	noisepp::PerlinModule2D noiseppPerlin2D;
	noisepp::PerlinModule3D noiseppPerlin3D;

	// create default pipelines and fill them with our modules
	noisepp::Pipeline2D pipeline2D;
	noisepp::ElementID noiseID = noiseppPerlin2D.addToPipe (pipeline2D);
	noisepp::Pipeline3D pipeline3D;
	noisepp::ElementID noiseID3D = noiseppPerlin3D.addToPipe (pipeline3D);

	// create threaded pipelines and fill them with modules
#if NOISEPP_ENABLE_THREADS
	noisepp::ThreadedPipeline2D pipeline2D_mt(THREAD_COUNT);
	noisepp::ElementID noiseID2D_mt = noiseppPerlin2D.addToPipe (pipeline2D_mt);
	noisepp::ThreadedPipeline3D pipeline3D_mt(THREAD_COUNT);
	noisepp::ElementID noiseID3D_mt = noiseppPerlin3D.addToPipe (pipeline3D_mt);
#endif

	// creates the cache
	noisepp::Cache *cache = pipeline2D.createCache();
	noisepp::Cache *cache3D = pipeline3D.createCache();

	// calculate a perlin noise value
	cout << "libnoise: " << libnoisePerlin.GetValue (1.2, 0.2, 0) << std::endl;
	cout << "Noise++ (2D): " << pipeline2D.getElement(noiseID)->getValue (1.2, 0.2, cache) << std::endl;
	cout << "Noise++ (3D): " << pipeline3D.getElement(noiseID3D)->getValue (1.2, 0.2, 0, cache3D) << std::endl;

	// calculates some square roots to bring the CPU(s) to maximum frequency - i know, not really professional solution
	// (you can skip this step but at least on my PC I get more accurate benchmark results with it)
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

	cout << "go!" << endl;

	// generate the points with libnoise
	double seconds;
	double mpix = MILLION_POINTS;
	cout << "generating using libnoise ... ";
	cout.flush ();
	double lnoiseStart = Timer_getSeconds();
	for (int i=0;i<MILLION_POINTS;++i)
	{
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
	cout << "done" << endl;
	// calculate stats
	seconds = Timer_getSeconds()-lnoiseStart;
	double libnoiseTime = seconds * 100.0;
	cout << "time: " << seconds << " sec" << endl;
	cout << mpix / seconds << " million pixels per second" << endl;

	// get the pointers to the pipeline elements
	noisepp::PipelineElement2D *element = pipeline2D.getElement(noiseID);
	noisepp::PipelineElement3D *element3D = pipeline3D.getElement(noiseID3D);
#if NOISEPP_ENABLE_THREADS
	noisepp::PipelineElement2D *element2D_mt = pipeline2D.getElement(noiseID2D_mt);
	noisepp::PipelineElement3D *element3D_mt = pipeline3D.getElement(noiseID3D_mt);
#endif

	// generate using the default 3D pipeline
	cout << "generating using Pipeline3D ... ";
	cout.flush ();
	double noiseppStart3D = Timer_getSeconds();
	for (int i=0;i<MILLION_POINTS;++i)
	{
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
	cout << "done" << endl;
	// calculate stats
	seconds = Timer_getSeconds()-noiseppStart3D;
	cout << "time: " << seconds << " sec" << endl;
	cout << mpix / seconds << " million pixels per second (" << libnoiseTime / seconds - 100.0 << "% faster than libnoise)" << endl;
	if (calcMeanDifference(noisepp_results, libnoise_results, 1000*1000) > 1.0e-07)
		cout << "WARNING: different result!" << endl;

	// generate using the default 2D pipeline
	cout << "generating using Pipeline2D ... ";
	cout.flush ();
	double noiseppStart2D = Timer_getSeconds();
	for (int i=0;i<MILLION_POINTS;++i)
	{
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
	cout << "done" << endl;
	// calculate stats
	seconds = Timer_getSeconds()-noiseppStart2D;
	cout << "time: " << seconds << " sec" << endl;
	cout << mpix / seconds << " million pixels per second (" << libnoiseTime / seconds - 100.0 << "% faster than libnoise)" << endl;
	// check for difference
	if (calcMeanDifference(noisepp_results, libnoise_results, 1000*1000) > 1.0e-07)
		cout << "WARNING: different result!" << endl;

#if NOISEPP_ENABLE_THREADS
	// generate using the threaded 3D pipeline
	cout << "generating using ThreadedPipeline3D ... ";
	cout.flush ();
	double noiseppStart3D_mt = Timer_getSeconds();
	for (int i=0;i<MILLION_POINTS;++i)
	{
		double *res = noisepp_results;
		y = 0;
		for (int yy=0;yy<1000;++yy)
		{
			pipeline3D_mt.addJob (new LineJob3D(&pipeline3D_mt, element3D_mt, 0, y, 0, 1000, xdelta, res));
			res += 1000;
			y += ydelta;
		}
		pipeline3D_mt.executeJobs ();
	}
	cout << "done" << endl;
	// calculate stats
	seconds = Timer_getSeconds()-noiseppStart3D_mt;
	cout << "time: " << seconds << " sec" << endl;
	cout << mpix / seconds << " million pixels per second (" << libnoiseTime / seconds - 100.0 << "% faster than libnoise)" << endl;
	// check for difference
	if (calcMeanDifference(noisepp_results, libnoise_results, 1000*1000) > 1.0e-07)
		cout << "WARNING: different result!" << endl;

	// generate using the threaded 2D pipeline
	cout << "generating using ThreadedPipeline2D ... ";
	cout.flush ();
	double noiseppStart2D_mt = Timer_getSeconds();
	for (int i=0;i<MILLION_POINTS;++i)
	{
		double *res = noisepp_results;
		y = 0;
		for (int yy=0;yy<1000;++yy)
		{
			pipeline2D_mt.addJob (new LineJob2D(&pipeline2D_mt, element2D_mt, 0, y, 1000, xdelta, res));
			res += 1000;
			y += ydelta;
		}
		pipeline2D_mt.executeJobs ();
	}
	cout << "done" << endl;
	// calculate stats
	seconds = Timer_getSeconds()-noiseppStart2D_mt;
	cout << "time: " << seconds << " sec" << endl;
	cout << mpix / seconds << " million pixels per second (" << libnoiseTime / seconds - 100.0 << "% faster than libnoise)" << endl;
	// check for difference
	if (calcMeanDifference(noisepp_results, libnoise_results, 1000*1000) > 1.0e-07)
		cout << "WARNING: different result!" << endl;
#endif

	delete[] libnoise_results;
	delete[] noisepp_results;

	// frees the cache
	pipeline2D.freeCache(cache);
	pipeline3D.freeCache(cache3D);

#ifdef WIN32
	cout << "Press any key ..." << endl;
	cin.get();
#endif

	return 0;
}
