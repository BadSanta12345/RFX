#pragma once

#include <type_traits>
#include <iostream>
#include <array>

namespace bs
{
	/*  Generic vector class. sizeof(CVector<T,dimension>) always returns sizeof(T)*dimension. 
		Not very well optimized mathematically but works fine for window managment etc. */
	template<typename T, size_t dimension, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	class CVector
	{
	public:
		CVector(const CVector& copy)
		{
			*this = copy;
		}

		CVector(const CVector&& copy)
		{
			*this = std::move(copy);
		}

		template<typename... Args>
		CVector(Args... _args) : m_elements{ { (T)_args... } }
		{
		}
/*
		template<typename TArg>
		static TArg& make_vector(TArg& arg0)
		{
			return arg0;
		}

		template<typename TArg, typename... Args>
		static auto make_vector(TArg arg0, Args... args) -> CVector<TArg, sizeof...(Args)+1>
		{
			return CVector<TArg, sizeof...(Args)+1>(arg0, args...);
		}

#define bs_x m_elements[0]
#define bs_y m_elements[1]
#define bs_z m_elements[2]
#define bs_w m_elements[3]

#define MACRO(FN, ...)\
		template< typename = typename std::enable_if<dimension >= 1>::type >\
		inline auto FN() -> decltype(make_vector(__VA_ARGS__)) {return make_vector(__VA_ARGS__);}
#undef bs_x
#undef bs_y
#undef bs_z
#undef bs_w
		*/		
		
		template< typename = typename std::enable_if<dimension >= 1>::type >
		inline T& x()
		{
			return m_elements[0];
		}
		template< typename = typename std::enable_if<dimension >= 1>::type >
		inline const T x() const
		{
			return m_elements[0];
		}

		template< typename = typename std::enable_if<dimension >= 2>::type >
		inline T& y()
		{
			return m_elements[1];
		}

		template< typename = typename std::enable_if<dimension >= 2>::type >
		inline const T y() const
		{
			return m_elements[1];
		}

		template< typename = typename std::enable_if<dimension >= 3>::type >
		inline T& z()
		{
			return m_elements[2];
		}

		template< typename = typename std::enable_if<dimension >= 3>::type >
		inline const T z() const
		{
			return m_elements[2];
		}

		template< typename = typename std::enable_if<dimension >= 4>::type >
		inline T& w()
		{
			return m_elements[3];
		}

		template< typename = typename std::enable_if<dimension >= 4>::type >
		inline const T w() const
		{
			return m_elements[3];
		}

		inline const T dot(const CVector& rhs) const
		{
			T dotProduct = T(0);
			for (size_t i = 0; i < dimension; i++)
			{
				dotProduct += m_elements[i] * rhs.m_elements[i];
			}
			return dotProduct;
		}

		inline const T sqLenght() const
		{
			return dot(*this);
		}

		inline const T lenght() const
		{
			return (T)sqrt(sqLenght());
		}

		inline CVector& normalize()
		{
			return *this /= lenght();
		}

		template< typename = typename std::enable_if<dimension == 3>::type >
		inline CVector& cross(const CVector& rhs) const
		{
			return CVector( m_elements[1] * rhs.m_elements[2] - m_elements[2] * rhs.m_elements[1],
							m_elements[2] * rhs.m_elements[0] - m_elements[0] * rhs.m_elements[2],
							m_elements[0] * rhs.m_elements[1] - m_elements[1] * rhs.m_elements[0]);
		}

		inline CVector& operator=(const CVector& rhs)
		{
			m_elements	= rhs.m_elements;
			return *this;
		}

		inline CVector& operator=(const CVector&& rhs)
		{
			m_elements = std::move(rhs.m_elements);
			return *this;
		}

		inline CVector& operator=(T rhs)
		{
			std::fill(m_elements.begin(), m_elements.end(), rhs);
			return *this;
		}

		inline const CVector operator*(const CVector& rhs) const
		{
			CVector retVal;
			for (size_t i = 0; i < dimension; i++)
			{
				retVal.m_elements[i] = m_elements[i] * rhs.m_elements[i];
			}
			return retVal;
		}

		inline const CVector operator*(T rhs) const
		{
			CVector retVal;
			for (size_t i = 0; i < dimension; i++)
			{
				retVal.m_elements[i] = m_elements[i] * rhs;
			}
			return retVal;
		}

		inline CVector& operator*=(const CVector& rhs)
		{
			return *this = *this * rhs;
		}

		inline CVector& operator*=(T rhs)
		{
			return *this = *this * rhs;
		}

		inline const CVector operator/(const CVector& rhs) const
		{
			CVector retVal;
			for (size_t i = 0; i < dimension; i++)
			{
				retVal.m_elements[i] = m_elements[i] / rhs.m_elements[i];
			}
			return retVal;
		}

		inline const CVector operator/(T rhs) const
		{
			CVector retVal;
			for (size_t i = 0; i < dimension; i++)
			{
				retVal.m_elements[i] = m_elements[i] / rhs;
			}
			return retVal;
		}

		inline CVector& operator/=(const CVector& rhs)
		{
			return *this = *this / rhs;
		}

		inline CVector& operator/=(T rhs)
		{
			return *this = *this / rhs;
		}

		inline const CVector operator+(const CVector& rhs) const
		{
			CVector retVal;
			for (size_t i = 0; i < dimension; i++)
			{
				retVal.m_elements[i] = m_elements[i] + rhs.m_elements[i];
			}
			return retVal;
		}

		inline const CVector operator+(T rhs) const
		{
			CVector retVal;
			for (size_t i = 0; i < dimension; i++)
			{
				retVal.m_elements[i] = m_elements[i] + rhs;
			}
			return retVal;
		}

		inline CVector& operator+=(const CVector& rhs)
		{
			return *this = *this + rhs;
		}

		inline CVector& operator+=(T rhs)
		{
			return *this = *this + rhs;
		}

		inline const CVector operator-(const CVector& rhs) const
		{
			CVector retVal;
			for (size_t i = 0; i < dimension; i++)
			{
				retVal.m_elements[i] = m_elements[i] - rhs.m_elements[i];
			}
			return retVal;
		}

		inline const CVector operator-(T rhs) const
		{
			CVector retVal;
			for (size_t i = 0; i < dimension; i++)
			{
				retVal.m_elements[i] = m_elements[i] - rhs;
			}
			return retVal;
		}

		inline CVector& operator-=(const CVector& rhs)
		{
			return *this = *this - rhs;
		}

		inline CVector& operator-=(T rhs)
		{
			return *this = *this - rhs;
		}

		inline T& operator[](size_t i)
		{
			return m_elements[i];
		}

		inline const T operator[](size_t i) const
		{
			return m_elements[i];
		}

		inline friend bool operator<(const CVector& lhs, const CVector& rhs)
		{
			for (size_t i = 0; i < dimension; i++)
			{
				if (lhs[i] >= rhs[i])
				{
					return false;
				}
			}
			return true;
		}

		inline friend std::ostream &operator<<(std::ostream &output, const CVector& rhs)
		{
			output  << rhs.m_elements[0];
			for (size_t i = 1; i < dimension; i++)
			{
				output << '/' << rhs.m_elements[i];
			}
			return output;
		}

		inline friend std::istream &operator>>(std::istream  &input, CVector& rhs)
		{
			for (size_t i = 0; i < dimension - 1; i++)
			{
				input >> rhs.m_elements[i];
				if (input.peek() != '/')
				{
					input.clear();
					for (size_t j = i+1; j < dimension; j++)
					{
						rhs.m_elements[j] = rhs.m_elements[i];
					}
					return input;
				}
				input.ignore();
			}
			input >> rhs.m_elements[dimension - 1];
			return input;
		}
	private:
		//T m_elements[dimension];
		std::array<T, dimension> m_elements;
	};

	// Spezialize vector of lenght 1 to be == T
	template<typename T>
	class CVector < T, 1 >
	{
	public:
		CVector(T val) : m_val(val) {}

		operator T()
		{
			return m_val;
		}

		operator T() const
		{
			return m_val;
		}

	private:
		T m_val;
	};

	/* Generic matrix class. */
	// TODO: multiplication, invertion etc.
	template<typename T, size_t nRows, size_t nColumns, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	class CMatrix
	{
		CMatrix(){}

		template< typename = typename std::enable_if<nRows == nColumns>::type >
		CMatrix& identity()
		{
			for (size_t row = 0; row < nRows; row++)
			{
				for (size_t column = 0; column < nColumns; column++)
				{
					m_elements[row][column] = row == column ? T(1) : T(0);
				}
			}
			return *this
		}

		CMatrix<T, nColumns, nRows> getTranspose() const
		{
			CMatrix<T, nColumns, nRows> retVal;
			for (size_t row = 0; row < nRows; row++)
			{
				for (size_t column = 0; column < nColumns; column++)
				{
					retVal.m_elements[column][row] = m_elements[row][column];
				}
			}
			return retVal;
		}

		template< typename = typename std::enable_if<nRows == nColumns>::type >
		CMatrix& transpose()
		{
			for (size_t row = 0; row < nRows; row++)
			{
				for (size_t column = 0; column < nColumns; column++)
				{
					m_elements[column][row] = m_elements[row][column];
				}
			}
			return retVal;
		}

		inline CVector<T, nColumns>& operator[](size_t i)
		{
			return m_elements[i];
		}

		inline const CVector<T, nColumns>& operator[](size_t i) const
		{
			return m_elements[i];
		}

		inline friend std::ostream &operator<<(std::ostream &output, const CMatrix& rhs)
		{
			output << '{' << rhs.m_elements[0];
			for (size_t i = 1; i < nColumns; i++)
			{
				output << '\n' << rhs.m_elements[i];
			}
			return output << '}';
		}

	private:
		CVector<T, nColumns> m_elements[nRows];
	};

	/* Vector type definitions.*/
	/* 2D vector types. */
	template<typename T>
	using TVector2D = CVector <T, 2>;

	using TInt2 = TVector2D<int>;
	using TUInt2 = TVector2D<unsigned int>;
	using TFloat2 = TVector2D<float>;

	/* 3D vector types. */
	template<typename T>
	using TVector3D = CVector <T, 3>;

	using TInt3 = TVector3D<int>;
	using TUInt3 = TVector3D<unsigned int>;
	using TChar3 = TVector3D<signed char>;
	using TUChar3 = TVector3D<unsigned char>;
	//using CFloat3 = TVector3D<float>;

	/* 4D vector types. */
	template<typename T>
	using TVector4D = CVector <T, 4>;

	using TInt4 = TVector4D<int>;
	using TUInt4 = TVector4D<unsigned int>;
	using TChar4 = TVector4D<signed char>;
	using TUChar4 = TVector4D<unsigned char>;
	using TFloat4 = TVector4D<float>;

	template<typename T>
	using TMatrix4x4 = CMatrix <T, 4, 4>;

	using TFloat4x4 = TMatrix4x4<float>;
}