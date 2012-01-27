#ifndef _MATH_H_
#define _MATH_H_

#include <complex>

namespace System
{
	/**
			Helper math class.  Templated class for type safety.

\code
float absValue = Math<float>::Abs ( -1 * Math<float>::PI );
int max = Math<int>::Max ( 25, 67 ); 
int min = Math<int>::Min ( 25, 67 );
\endcode

	*/
	template <class t>
	class Math
	{
	public:

		/**
			Computes the absolute value of a variable

			@param a Original value

			@returns The absolute value of a
		*/
		static t Abs(const t& a)
		{
			return (a < (t)0) ? (-a) : a;
		}

		/**
			Computes the maximum value of two values

			@param a Value to compare to b
			@param b Value to compare to a

			@returns Maximum of a and b
		*/
		static t Max(const t& a, const t& b)
		{
			return (a > b) ? a : b;
		}

		/**
			Computes the minimum value of two values

			@param a Value to compare to b
			@param b Value to compare to a

			@returns Minimum of a and b
		*/
		static t Min(const t& a, const t& b)
		{
			return (a < b) ? a : b;
		}

		static t SquareRoot(const t& a)
		{
			return (std::sqrt ( std::complex<t> ( a ) ) ).real ( );
		}

		/// Constant PI
		static const t PI;
		
		/// Constant E
		static const t E;
	};

	template <class t>
	const t Math<t>::PI = (t)3.14159265358979323846;

	template <class t>
	const t Math<t>::E	 = (t)2.7182818284590452354;
}

#endif
