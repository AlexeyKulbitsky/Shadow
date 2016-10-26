namespace sh
{
	namespace math
	{
		///////////////////////////////////////////////////////////////////////////////
		// constructors
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		rect<T>::rect() : upperLeftCorner(0, 0), lowerRightCorner(0, 0)
		{
		}

		template <typename T>
		rect<T>::rect(T x, T y, T x2, T y2) : upperLeftCorner(x, y), lowerRightCorner(x2, y2)
		{
		}

		template <typename T>
		rect<T>::rect(const math::vec2<T>& upperLeft, const math::vec2<T>& lowerRight) : upperLeftCorner(upperLeft), lowerRightCorner(lowerRight)
		{
		}

		///////////////////////////////////////////////////////////////////////////////
		// methods
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		void rect<T>::set(T x, T y, T x2, T y2)
		{
			upperLeftCorner = math::vec2<T>(x, y);
			lowerRightCorner = math::vec2<T>(x2, y2);
		}

		template <typename T>
		void rect<T>::set(const math::vec2<T>& upperLeft, const math::vec2<T>& lowerRight)
		{
			upperLeftCorner = upperLeft;
			lowerRightCorner = lowerRight;
		}

		template <typename T>
		void rect<T>::scale(T scale)
		{
			upperLeftCorner *= scale;
			lowerRightCorner *= scale;
		}

		template <typename T>
		T rect<T>::getArea() const
		{
			return getWidth() * getHeight();
		}

		template <typename T>
		T rect<T>::getWidth() const
		{
			return lowerRightCorner.x - upperLeftCorner.x;
		}

		template <typename T>
		T rect<T>::getHeight() const
		{
			return lowerRightCorner.y - upperLeftCorner.y;
		}

		template <typename T>
		void rect<T>::repair()
		{
			if (lowerRightCorner.x < upperLeftCorner.x)
			{
				std::swap(lowerRightCorner.x, upperLeftCorner.x);
			}
			if (lowerRightCorner.y < upperLeftCorner.y)
			{
				std::swap(lowerRightCorner.y, upperLeftCorner.y);
			}
		}

		template <typename T>
		bool rect<T>::isValid() const
		{
			return ((lowerRightCorner.x >= upperLeftCorner.x) && (lowerRightCorner.y >= upperLeftCorner.y));
		}

		template <typename T>
		math::vec2<T> rect<T>::getCenter() const
		{
			return math::vec2<T>((upperLeftCorner.x + lowerRightCorner.x) / 2,
				(upperLeftCorner.y + lowerRightCorner.y) / 2);
		}

		template <typename T>
		math::vec2<T> rect<T>::getSize() const
		{
			return math::vec2<T>(getWidth(), getHeight());
		}

		template <typename T>
		math::vec2<T> rect<T>::getExtent() const
		{
			return lowerRightCorner - upperLeftCorner;
		}

		///////////////////////////////////////////////////////////////////////////////
		// containment methods
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		bool rect<T>::isPointInside(const math::vec2<T>& pos) const
		{
			return (
				upperLeftCorner.x <= pos.x && upperLeftCorner.y <= pos.y &&
				lowerRightCorner.x >= pos.x && lowerRightCorner.y >= pos.y
				);
		}

		template <typename T>
		bool rect<T>::isRectCollided(const rect<T>& other) const
		{
			return (
				lowerRightCorner.y > other.upperLeftCorner.y && upperLeftCorner.y  < other.lowerRightCorner.y  &&
				lowerRightCorner.x > other.upperLeftCorner.x && upperLeftCorner.x < other.lowerRightCorner.x
				);
		}

		template <typename T>
		void rect<T>::clipAgainst(const rect<T>& other)
		{
			if (other.lowerRightCorner.x < lowerRightCorner.x)
				lowerRightCorner.x = other.lowerRightCorner.x;
			if (other.lowerRightCorner.y < lowerRightCorner.y)
				lowerRightCorner.y = other.lowerRightCorner.y;

			if (other.upperLeftCorner.x > upperLeftCorner.x)
				upperLeftCorner.x = other.upperLeftCorner.x;
			if (other.upperLeftCorner.y > upperLeftCorner.y)
				upperLeftCorner.y = other.upperLeftCorner.y;

			// correct possible invalid rect resulting from clipping
			if (upperLeftCorner.y > lowerRightCorner.y)
				upperLeftCorner.y = lowerRightCorner.y;
			if (upperLeftCorner.x > lowerRightCorner.x)
				upperLeftCorner.x = lowerRightCorner.x;
		}

		template <typename T>
		bool rect<T>::constrainTo(const rect<T>& other)
		{
			if (other.getWidth() < getWidth() || other.getHeight() < getHeight())
			{
				return false;
			}

			T diff = other.lowerRightCorner.x - lowerRightCorner.x;
			if (diff < 0)
			{
				lowerRightCorner.x += diff;
				upperLeftCorner.x += diff;
			}

			diff = other.lowerRightCorner.y - lowerRightCorner.y;
			if (diff < 0)
			{
				lowerRightCorner.y += diff;
				upperLeftCorner.y += diff;
			}

			diff = upperLeftCorner.x - other.upperLeftCorner.x;
			if (diff < 0)
			{
				upperLeftCorner.x -= diff;
				lowerRightCorner.x -= diff;
			}

			diff = upperLeftCorner.y - other.upperLeftCorner.y;
			if (diff < 0)
			{
				upperLeftCorner.y -= diff;
				lowerRightCorner.y -= diff;
			}

			return true;
		}

		template <typename T>
		math::vec2<T> rect<T>::getClosestPoint(const math::vec2<T>& pt) const
		{
			math::vec2<T> result = pt;

			result.x = math::clamp(result.x, upperLeftCorner.x, lowerRightCorner.x);
			result.y = math::clamp(result.y, upperLeftCorner.y, lowerRightCorner.y);

			return result;
		}

		template <typename T>
		void rect<T>::grow(const T& size)
		{
			grow(math::vec2<T>(size));
		}

		template <typename T>
		void rect<T>::grow(const math::vec2<T>& size)
		{
			upperLeftCorner -= size;
			lowerRightCorner += size;
		}

		template <typename T>
		void rect<T>::clampToMin(const T& size)
		{
			clampToMin(math::vec2<T>(size));
		}

		template <typename T>
		void rect<T>::clampToMin(const math::vec2<T>& minSize)
		{
			T curWidth = getWidth();
			if (curWidth < minSize.x)
			{
				T widthToGrow = (minSize.x - curWidth) * (T)(0.5);
				upperLeftCorner.x -= widthToGrow;
				lowerRightCorner.x += widthToGrow;
			}

			T curHeight = getHeight();
			if (curHeight < minSize.y)
			{
				T heightToGrow = (minSize.y - curHeight) * (T)(0.5);
				upperLeftCorner.y -= heightToGrow;
				lowerRightCorner.y += heightToGrow;
			}
		}

		template <typename T>
		void rect<T>::clampToMax(const T& size)
		{
			clampToMax(math::vec2<T>(size));
		}

		template <typename T>
		void rect<T>::clampToMax(const math::vec2<T>& maxSize)
		{
			assert(maxSize.x >= 0.0f && maxSize.y >= 0.0f);

			T curWidth = getWidth();
			if (curWidth > maxSize.x)
			{
				T widthToShrink = (curWidth - maxSize.x) * (T)(0.5);
				upperLeftCorner.x += widthToShrink;
				lowerRightCorner.x -= widthToShrink;
			}

			T curHeight = getHeight();
			if (curHeight > maxSize.y)
			{
				T heightToShrink = (curHeight - maxSize.y) * (T)(0.5);
				upperLeftCorner.y += heightToShrink;
				lowerRightCorner.y -= heightToShrink;
			}
		}

		template <typename T>
		void rect<T>::addInternalPoint(const math::vec2<T>& p)
		{
			addInternalPoint(p.x, p.y);
		}

		template <typename T>
		void rect<T>::addInternalPoint(T x, T y)
		{
			lowerRightCorner = math::max(math::vec2<T>(x, y), lowerRightCorner);
			upperLeftCorner = math::min(math::vec2<T>(x, y), upperLeftCorner);
		}

		///////////////////////////////////////////////////////////////////////////////
		// operators
		///////////////////////////////////////////////////////////////////////////////

		template <typename T>
		rect<T> rect<T>::operator+(const math::vec2<T>& pos) const
		{
			rect<T> ret(*this);
			return ret += pos;
		}

		template <typename T>
		rect<T>& rect<T>::operator+=(const math::vec2<T>& pos)
		{
			upperLeftCorner += pos;
			lowerRightCorner += pos;
			return *this;
		}

		template <typename T>
		rect<T> rect<T>::operator-(const math::vec2<T>& pos) const
		{
			rect<T> ret(*this);
			return ret -= pos;
		}

		template <typename T>
		rect<T> rect<T>::operator*(const T& scale) const
		{
			rect<T> ret(upperLeftCorner*scale, lowerRightCorner*scale);
			return ret;
		}

		template <typename T>
		rect<T> rect<T>::operator/(const T& scale) const
		{
			rect<T> ret(upperLeftCorner / scale, lowerRightCorner / scale);
			return ret;
		}

		template <typename T>
		rect<T>& rect<T>::operator-=(const math::vec2<T>& pos)
		{
			upperLeftCorner -= pos;
			lowerRightCorner -= pos;
			return *this;
		}

		template <typename T>
		bool rect<T>::operator==(const rect<T>& other) const
		{
			return (upperLeftCorner == other.upperLeftCorner  &&
				lowerRightCorner == other.lowerRightCorner);
		}

		template <typename T>
		bool rect<T>::operator!=(const rect<T>& other) const
		{
			return (upperLeftCorner != other.upperLeftCorner ||
				lowerRightCorner != other.lowerRightCorner);
		}

		template <typename T>
		rect<T>& rect<T>::operator+=(const rect<T>& other)
		{
			addInternalPoint(other.upperLeftCorner);
			addInternalPoint(other.lowerRightCorner);
			return *this;
		}

		template <typename T>
		bool rect<T>::operator<(const rect<T>& other) const
		{
			return getArea() < other.getArea();
		}

	}
}