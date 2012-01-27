#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

#include "../Math.h"
#include "../Exception.h"

namespace System
{
	namespace Math3D
	{
		template <class t>
		class Vector3D
		{
		public:
			t x, y, z;

			Vector3D ( ) : x((t)0.0), y((t)0.0), z((t)0.0)			{}
			Vector3D ( t x, t y, t z ) : x(x), y(y), z(z)			{}
			Vector3D ( Vector3D<t>& v ) : x(v.x), y(v.y), z(v.z)	{}

			virtual ~Vector3D ( ) {}

			///////////////////////////////////////////////////////////////////
			t SquaredLength ( )
			///////////////////////////////////////////////////////////////////
			{
				return (x * x) + (y * y) + (z * z);
			}

			///////////////////////////////////////////////////////////////////
			t Length ( )
			///////////////////////////////////////////////////////////////////
			{
				return Math<t>::SquareRoot ( SquaredLength ( ) );
			}

			///////////////////////////////////////////////////////////////////
			t& operator[] ( int index )
			///////////////////////////////////////////////////////////////////
			{
				switch ( index )
				{
					case 0:
						return x;
					case 1:
						return y;
					case 2:
						return z;
				}

				throw IndexOutOfRangeException( );

				return x;
			}

			///////////////////////////////////////////////////////////////////
			static t Dot ( Vector3D<t> v1, Vector3D<t> v2 )
			///////////////////////////////////////////////////////////////////
			{
				return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
			}

			///////////////////////////////////////////////////////////////////
			static Vector3D<t> Cross ( Vector3D<t> v1, Vector3D<t> v2 )
			///////////////////////////////////////////////////////////////////
			{
				return Vector3D<t> (
					(v1.y * v2.z) - (v1.z * v2.y),
					(v1.z * v2.x) - (v1.x * v2.z),
					(v1.x * v2.y) - (v1.y * v2.x));
			}

			///////////////////////////////////////////////////////////////////
			Vector3D<t> Normalized ()
			///////////////////////////////////////////////////////////////////
			{
				t length = this->Length();

				// Will also work for zero-sized vectors, but will change nothing
				if ( length > (t)0.0001f ) 
				{
					t inverseLength = (t)(1.0f) / length;
					return Vector3D<t> ( x * inverseLength, y * inverseLength, z * inverseLength );
				}

				return *this;
			}

			/*

			/// <summary>
			/// Reflects a vector on a plane
			/// </summary>
			/// <param name="plane">The plane to reflect from</param>
			/// <returns>A reflected vector</returns>
			public Vector3 Reflection(Plane plane) 
			{
				return this - (2 * Vector3.Dot(this, plane.Normal) * plane.Normal);
			}

			#endregion
		*/
		};

		///////////////////////////////////////////////////////////////////////
		template <class t>
		inline bool operator == ( Vector3D<t> v1, Vector3D<t> v2 )
		///////////////////////////////////////////////////////////////////////
		{
			return (v1.x == v2.x) && (v2.y == v2.y) && ( v1.z == v2.z );
		}

		///////////////////////////////////////////////////////////////////////
		template <class t>
		inline bool operator != ( Vector3D<t> v1, Vector3D<t> v2 )
		///////////////////////////////////////////////////////////////////////
		{
			return !(v1 == v2 );
		}

		///////////////////////////////////////////////////////////////////////
		template <class t>
		inline Vector3D<t> operator + (Vector3D<t> lhs, Vector3D<t> rhs)
		///////////////////////////////////////////////////////////////////////
		{
			return Vector3D<t> (lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
		}

		///////////////////////////////////////////////////////////////////////
		template <class t>
		inline Vector3D<t> operator - (Vector3D<t> lhs, Vector3D<t> rhs)
		///////////////////////////////////////////////////////////////////////
		{
			return Vector3D<t> (lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
		}

		///////////////////////////////////////////////////////////////////////
		template <class t>
		inline Vector3D<t> operator * (Vector3D<t> lhs, t rhs)
		///////////////////////////////////////////////////////////////////////
		{
			return Vector3D<t> (lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
		}

		///////////////////////////////////////////////////////////////////////
		template <class t>
		inline Vector3D<t> operator * (t lhs, Vector3D<t> rhs)
		///////////////////////////////////////////////////////////////////////
		{
			return rhs * lhs;
		}

		///////////////////////////////////////////////////////////////////////
		template <class t>
		inline t operator * (Vector3D<t> lhs, Vector3D<t> rhs)
		///////////////////////////////////////////////////////////////////////
		{
			return Vector3D<t>::Dot (lhs, rhs);
		}

		///////////////////////////////////////////////////////////////////////
		template <class t>
		inline Vector3D<t> operator % (Vector3D<t> lhs, Vector3D<t> rhs)
		///////////////////////////////////////////////////////////////////////
		{
			return Vector3D<t>::Cross (lhs, rhs);
		}

		///////////////////////////////////////////////////////////////////////
		template <class t>
		inline Vector3D<t> operator / (Vector3D<t> lhs, t rhs)
		///////////////////////////////////////////////////////////////////////
		{
			return Vector3D<t> (lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
		}

		///////////////////////////////////////////////////////////////////////
		template <class t>
		inline Vector3D<t> operator / (t lhs, Vector3D<t> rhs)
		///////////////////////////////////////////////////////////////////////
		{
			return rhs / lhs;
		}

	}
}

#endif // _VECTOR3D_H_