#include "Float2.h"

namespace bs
{
	inline std::ostream &operator<<(std::ostream &lhs, const CFloat2& rhs)
	{
		return lhs << rhs.x << '/' << rhs.y;
	}

	inline std::istream& operator >> (std::istream& lhs, CFloat2& rhs)
	{
		char c;

		lhs >> rhs.x;
		lhs >> c;
		if (c != '/')
		{
			rhs.y = rhs.x;
			return lhs;
		}

		lhs >> rhs.y;

		return lhs;
	}
}