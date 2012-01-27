//#include <iostream>     
#include <sstream>     
#include <iomanip>

#include "Float.h"

namespace System
{
	namespace Types
	{

		///////////////////////////////////////////////////////////////////////
		Float::Float ( ) : value ( 0.0f )
		///////////////////////////////////////////////////////////////////////
		{

		}

		///////////////////////////////////////////////////////////////////////
		Float::Float ( float value )
		///////////////////////////////////////////////////////////////////////
		{
			this->value = value;
		}

		///////////////////////////////////////////////////////////////////////
		Float::Float ( std::string value )
		///////////////////////////////////////////////////////////////////////
		{
			this->value = Float::ParseFloat ( value );
		}

		///////////////////////////////////////////////////////////////////////
		Float::~Float(void)
		///////////////////////////////////////////////////////////////////////
		{
		}

		///////////////////////////////////////////////////////////////////////
		float Float::ParseFloat ( std::string value )
		///////////////////////////////////////////////////////////////////////
		{
			return (float)atof ( value.c_str () );
		}

		///////////////////////////////////////////////////////////////////////
		std::string Float::ToString ( float value, int precision )
		///////////////////////////////////////////////////////////////////////
		{
			std::ostringstream ostr;

			// Set the number of decimal points
			if ( precision != 0 )
				ostr << std::setprecision ( precision );

			ostr << value;     

			return ostr.str ();
		}
		
		///////////////////////////////////////////////////////////////////////
		bool Float::operator == ( const Float &right )
		///////////////////////////////////////////////////////////////////////
		{
			return ( this->value == right.value );
		}

		///////////////////////////////////////////////////////////////////////
		bool Float::operator != ( const Float &right )
		///////////////////////////////////////////////////////////////////////
		{
			return ( this->value != right.value );
		}
	}
}
