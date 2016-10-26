namespace sh
{
	namespace math
	{
		template<class T>
		uint8_t circleIntersectCircle(
			const math::vec2<T>& c0, const T r0, const math::vec2<T>& c1, const T r1, math::vec2<T>& out0, math::vec2<T>& out1)
		{
			// from http://www.geometrictools.com/LibMathematics/Intersection/Wm5IntrCircle2Circle2.cpp

			// The two circles are |X-C0| = R0 and |X-C1| = R1.  Define U = C1 - C0
			// and V = Perp(U) where Perp(x,y) = (y,-x).  Note that Dot(U,V) = 0 and
			// |V|^2 = |U|^2.  The intersection points X can be written in the form
			// X = C0+s*U+t*V and X = C1+(s-1)*U+t*V.  Squaring the circle equations
			// and substituting these formulas into them yields
			//   R0^2 = (s^2 + t^2)*|U|^2
			//   R1^2 = ((s-1)^2 + t^2)*|U|^2.
			// Subtracting and solving for s yields
			//   s = ((R0^2-R1^2)/|U|^2 + 1)/2
			// Then replace in the first equation and solve for t^2
			//   t^2 = (R0^2/|U|^2) - s^2.
			// In order for there to be solutions, the right-hand side must be
			// nonnegative.  Some algebra leads to the condition for existence of
			// solutions,
			//   (|U|^2 - (R0+R1)^2)*(|U|^2 - (R0-R1)^2) <= 0.
			// This reduces to
			//   |R0-R1| <= |U| <= |R0+R1|.
			// If |U| = |R0-R1|, then the circles are side-by-side and just tangent.
			// If |U| = |R0+R1|, then the circles are nested and just tangent.
			// If |R0-R1| < |U| < |R0+R1|, then the two circles to intersect in two
			// points.

			math::vec2<T> U = c1 - c0;
			T USqrLen = U.getLengthSq();
			T R0 = r0, R1 = r1;
			T R0mR1 = R0 - R1;

			//USqrLen < Math<Real>::ZERO_TOLERANCE
			//&&  Math<Real>::FAbs(R0mR1) < Math<Real>::ZERO_TOLERANCE)
			if (math::equals(USqrLen, 0.0f) &&
				(math::equals(math::abs(R0mR1), 0.0f)))
			{
				// Circles are essentially the same.
				//mIntersectionType = IT_OTHER;
				//mQuantity = 0;
				return 0;
			}

			T R0mR1Sqr = R0mR1*R0mR1;
			if (USqrLen < R0mR1Sqr)
			{
				//mIntersectionType = IT_EMPTY;
				//mQuantity = 0;
				return 0;
			}

			T R0pR1 = R0 + R1;
			T R0pR1Sqr = R0pR1*R0pR1;
			if (USqrLen > R0pR1Sqr)
			{
				//mIntersectionType = IT_EMPTY;
				//mQuantity = 0;
				return 0;
			}

			if (USqrLen < R0pR1Sqr)
			{
				if (R0mR1Sqr < USqrLen)
				{
					T invUSqrLen = ((T)1) / USqrLen;
					T s = ((T)0.5)*((R0*R0 - R1*R1)*invUSqrLen + (T)1);
					vec2<T> tmp = c0 + s*U;

					// In theory, discr is nonnegative.  However, numerical round-off
					// errors can make it slightly negative.  Clamp it to zero.
					T discr = R0*R0*invUSqrLen - s*s;
					if (discr < (T)0)
					{
						discr = (T)0;
					}
					T t = math::sqrt(discr);

					vec2<T> V(U.y, -U.x);
					//mQuantity = 2;
					out0 = tmp - t*V;
					out1 = tmp + t*V;

					return 2;
				}
				else
				{
					// |U| = |R0-R1|, circles are tangent.
					//mQuantity = 1;
					out0 = c0 + (R0 / R0mR1)*U;
					return 1;
				}
			}
			else
			{
				// |U| = |R0+R1|, circles are tangent.
				//mQuantity = 1;
				out0 = c0 + (R0 / R0pR1)*U;
				return 1;
			}

			//mIntersectionType = IT_POINT;
			return 0;
		}
	}
}