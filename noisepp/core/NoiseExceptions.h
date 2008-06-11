#ifndef NOISEPP_EXCEPTIONS_H
#define NOISEPP_EXCEPTIONS_H

#include "NoiseStdHeaders.h"
#include "NoisePlatform.h"

namespace noisepp
{

/// Exception thrown by Noise++ in case of an error.
class Exception : public std::exception
{
	private:
		std::string mDescription;
	public:
		/// Constructor.
		Exception (const std::string &desc) : mDescription(desc)
		{}
		/// Destructor.
		~Exception() throw() {}
		/// Returns the error description.
		const std::string &getDescription () const
		{
			return mDescription;
		}
		/// Returns the error description.
		virtual const char* what() const throw ()
		{
		 	return mDescription.c_str();
		}
};

/// Thrown in case of calling an unimplemented function.
class NotImplementedException : public Exception
{
	public:
		NotImplementedException(const std::string &func) : Exception("function not implemented: '" + func + "'")
		{}
};

/// Thrown if there are unset source modules.
class NoModuleException : public Exception
{
	public:
		NoModuleException(const std::string &func) : Exception("source module is not set in '" + func + "'")
		{}
};

/// Thrown in case of an invalid parameter.
class ParamInvalidException : public Exception
{
	public:
		ParamInvalidException(const std::string &paramName, const std::string &desc, const std::string &func) : Exception("invalid parameter for '" + paramName + "' in '" + ( desc.empty() ? func + "'" : func + "': " + desc ))
		{}
};

/// Thrown if you request an array element that is out of its range.
class OutOfRangeException : public Exception
{
	public:
		OutOfRangeException(const std::string &paramName, const std::string &func) : Exception("parameter '" + paramName + "' is out of range in '" + func + "'")
		{}
};

#define NoiseAssert(eval, name) if (!(eval)) \
	throw ParamInvalidException(#name, std::string("assertion '") + #eval + std::string("' failed"), NOISEPP_CURRENT_FUNCTION);
#define NoiseAssertRange(param, range) if (param >= range) \
	throw OutOfRangeException(#param, NOISEPP_CURRENT_FUNCTION);
#define NoiseThrowNotImplementedException \
	throw NotImplementedException (NOISEPP_CURRENT_FUNCTION)
#define NoiseThrowNoModuleException \
	throw NoModuleException (NOISEPP_CURRENT_FUNCTION)

};

#endif // NOISEEXCEPTIONS_H
