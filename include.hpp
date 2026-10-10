#include <stdlib.h>
#include <climits>
#include <cassert>
#include <unistd.h>
#include <ctime>
#include <cstdio>

typedef unsigned long long	ull;
typedef unsigned int		uin;
typedef unsigned short		ush;
typedef unsigned char		uch;
typedef long double			sld;

namespace extra {

	static constexpr ull pow_2bi[65] =
{1ULL    , 1ULL<<1 , 1ULL<<2 , 1ULL<<3 , 1ULL<<4 , 1ULL<<5 , 1ULL<<6 , 1ULL<<7 ,
 1ULL<<8 , 1ULL<<9 , 1ULL<<10, 1ULL<<11, 1ULL<<12, 1ULL<<13, 1ULL<<14, 1ULL<<15,
 1ULL<<16, 1ULL<<17, 1ULL<<18, 1ULL<<19, 1ULL<<20, 1ULL<<21, 1ULL<<22, 1ULL<<23,
 1ULL<<24, 1ULL<<25, 1ULL<<26, 1ULL<<27, 1ULL<<28, 1ULL<<29, 1ULL<<30, 1ULL<<31,
 1ULL<<32, 1ULL<<33, 1ULL<<34, 1ULL<<35, 1ULL<<36, 1ULL<<37, 1ULL<<38, 1ULL<<39,
 1ULL<<40, 1ULL<<41, 1ULL<<42, 1ULL<<43, 1ULL<<44, 1ULL<<45, 1ULL<<46, 1ULL<<47,
 1ULL<<48, 1ULL<<49, 1ULL<<50, 1ULL<<51, 1ULL<<52, 1ULL<<53, 1ULL<<54, 1ULL<<55,
 1ULL<<56, 1ULL<<57, 1ULL<<58, 1ULL<<59, 1ULL<<60, 1ULL<<61, 1ULL<<62, 1ULL<<63, 0ULL};

	static constexpr sld sld_ullmax = static_cast<sld>(ULLONG_MAX);

	[[nodiscard]] inline uch pow2f(ull M/*maximum value*/) noexcept {
		if (M == 0ULL) return UCHAR_MAX;/*this wouldn't make sense*/
		if (M == 1ULL) return 0;
		for (uch k = 1; k < 64; k++)
			if (M > extra::pow_2bi[k-1] && M <= extra::pow_2bi[k]) return k;
		return 64;
	}

	[[nodiscard]] static ull system_seed(ush K1/*=0*/, ush K2/*=0*/) {
		//return 9438923982ULL;
		ull gamma = ULLONG_MAX
			- (50ULL * static_cast<ull>(getpid()) + static_cast<ull>(K1) + 10319ULL)
			* (static_cast<ull>(K2) + 21ULL * static_cast<ull>(time(0)) + 10914ULL);
		return ((gamma >> 32) ^ (gamma << 32)) | (extra::pow_2bi[65] + 1ULL);
	}

};
