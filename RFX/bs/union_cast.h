#pragma once
/*
 * Defines a cast function which can cast between any types.
 * This can be very dangerous and should probably never be used.
 * I use this sometimes when doing reverse engineering.
*/
namespace bs
{
	template < typename R, typename V >
	R union_cast(V v_)
	{
		union { V v; R r; } u;
		u.v = v_;
		return u.r;
	}
}