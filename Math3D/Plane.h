#ifndef _PLANE_H_
#define _PLANE_H_

#include "../Math.h"
#include "../Exception.h"
#include "Vector3D.h"

namespace System
{
	namespace Math3D
	{
		enum PlaneSide
		{
			PlaneSide_Front,
			PlaneSide_Back,
			PlaneSide_Planar
		};

		template <class t>
		class Plane
		{
		public:
			Plane ( ) : normal ((t)0,(t)0,(t)0), d((t)0) {}
			Plane ( t x, t y, t z, t d ) : normal (x, y, z), d(d) {}
			Plane ( Vector3D<t> v, t d ) : normal (v), d(d) {}
			Plane ( Plane<t>& plane ) : normal (plane.normal), d(plane.d) {}
			Plane ( Vector3D<t> normal, Vector3D<t> point ) : normal (normal)
			{
				d = Vector3D<t>::Dot (point, normal);
			}

			///////////////////////////////////////////////////////////////////
			// Operations
			///////////////////////////////////////////////////////////////////
			t Distance (Vector3D<t> point)
			///////////////////////////////////////////////////////////////////
			{
				return Vector3D<t>::Dot(normal, point) + d;
			}

			///////////////////////////////////////////////////////////////////
			PlaneSide GetSide (Vector3D<t> point)
			///////////////////////////////////////////////////////////////////
			{
				t distance = Distance (point);

				// If within Epsilon, the point is on the plane
				if (Math<t>::Abs (distance) <= (t)0.0001)
					return PlaneSide_Planar;

				// If the distance is negative, the point is behind the plane
				if (distance < (t)0.0f)
					return PlaneSide_Back;

				return PlaneSide_Front;
			}

			Vector3D<t>	normal;
			float		d;
		};

		///////////////////////////////////////////////////////////////////////
		template <class t>
		bool operator == ( Plane<t> p1, Plane<t> p2 )
		///////////////////////////////////////////////////////////////////////
		{
			return (p1.normal == p2.normal) && (p1.d == p2.d);
		}

		///////////////////////////////////////////////////////////////////////
		template <class t>
		bool operator != ( Plane<t> p1, Plane<t> p2 )
		///////////////////////////////////////////////////////////////////////
		{
			return !(p1 == p2);
		}

	}
}

#endif // _PLANE_H_