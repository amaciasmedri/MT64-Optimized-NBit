#include "include.hpp"
#include "mt64.hpp"

int main(int argc, char *argv[]) {
	for (int n = 0; n < argc; n++) printf("%d [%s]\n", n, argv[n]);

	static MersenneTwister *mt_mu = new MersenneTwister(extra::system_seed(24403, 17152));
	
	for (uch B = 0; B < 64; B++) for (uch i = 0; i < 7; i++)
		printf("%hhu : %hhu => %llu\n", B, i, mt_mu->x64(B));

	return 0;
}
