//
// 1.B [pseudo] 08/06/2025

#define MT64_NN 312
#define MT64_MM MT64_NN/2
#define MT64_VA 0xB5026F5AA96619E9ULL
#define MT64_UM 0xFFFFFFFF80000000ULL
#define MT64_LM 0x7FFFFFFFULL
#define MT64_K1 6364136223846793005ULL
#define MT64_K2 19650218ULL
#define MT64_K3 3935559000370003845ULL
#define MT64_K4 2862933555777941757ULL
#define MT64_G1 0x5555555555555555ULL
#define MT64_G2 0x71D67FFFEDA60000ULL
#define MT64_G3 0xFFF7EEE000000000ULL

static constexpr uch
	MT64_QJ[33] = {7,5,4,7,3,7,7,7,2,8,8,8,8,8,8,8,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0},
	MT64_EH[64] = { 0, 1, 2, 2, 4, 4, 4, 4, 8, 8, 8, 8, 8, 8, 8, 8,
				   16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
				   32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
				   32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32};

static constexpr ull MT64_01[2] = {0ULL, MT64_VA};

class MersenneTwister {

private:

	ull mxc[MT64_NN], rtc[6], seed;
	ush ixc = MT64_NN + 1;
	bool ckc[6] = {true, true, true, true, true, true};

	inline void shuffle(void) noexcept {
		ush i = this->ixc = 0;
		ull x;
		for (; i < MT64_NN - MT64_MM; i++) {
			x = (this->mxc[i] & MT64_UM) | (this->mxc[i + 1] & MT64_LM);
			this->mxc[i] = this->mxc[i + MT64_MM] ^ (x >> 1) ^ MT64_01[(int)(x & 1ULL)];
		}
		for (; i < MT64_NN - 1; i++) {
			x = (this->mxc[i] & MT64_UM) | (this->mxc[i + 1] & MT64_LM);
			this->mxc[i] = this->mxc[i + (MT64_MM - MT64_NN)] ^ (x >> 1) ^ MT64_01[(int)(x & 1ULL)];
		}
		x = (this->mxc[MT64_NN - 1] & MT64_UM) | (this->mxc[0] & MT64_LM);
		this->mxc[MT64_NN - 1] = this->mxc[MT64_MM - 1] ^ (x >> 1) ^ MT64_01[(int)(x & 1ULL)];
	}

public:

	inline void init_by_seed(void) noexcept {
		this->mxc[0] = this->seed;
		for (ush i = 1; i < MT64_NN; i++)
			this->mxc[i] = MT64_K1 * (this->mxc[i - 1] ^ (this->mxc[i - 1] >> 62)) + static_cast<ull>(i);
	}

	inline void init_by_array(const ull init_key[], const ush key_length) noexcept {
		this->init_by_seed();
		ush i = 1, j = 0, k = MT64_NN > key_length ? MT64_NN : key_length;
		for (; k; k--) {
			this->mxc[i] = (this->mxc[i] ^ ((this->mxc[i - 1] ^ (this->mxc[i - 1] >> 62)) * MT64_K3)) + init_key[j] + static_cast<ull>(j);
			i++;
			j++;
			if(i >= MT64_NN) {
				this->mxc[0] = this->mxc[MT64_NN - 1];
				i = 1;
			}
			if(j >= key_length) j = 0;
		}
		for (k = MT64_NN - 1; k; k--) {
			this->mxc[i] = (this->mxc[i] ^ ((this->mxc[i - 1] ^ (this->mxc[i - 1] >> 62)) * MT64_K4)) - static_cast<ull>(i);
			i++;
			if (i >= MT64_NN) {
				this->mxc[0] = this->mxc[MT64_NN - 1];
				i = 1;
			}
		}
		this->mxc[0] = 1ULL << 63;
	}

	inline void init_by_rand(void) noexcept {
		ush tseed = static_cast<ush>(this->seed % extra::pow_2bi[16]);
		srand(RAND_MAX ^ this->seed % extra::pow_2bi[32]);
		for (ush n = 0; n < tseed; n++) rand();
		ull keys[MT64_NN], w;
		keys[0] = ULLONG_MAX - rand();
		for(ush i = 1; i < MT64_NN; i++) {
			if (keys[i - 1] % 2ULL == 0ULL)
				w = SHRT_MAX - static_cast<ull>(i + 13);
			else
				w = MT64_MM + static_cast<ull>(i + 17);
			if (keys[i - 1] % 7ULL == 0ULL)
				keys[i] = ULLONG_MAX / w - static_cast<ull>(rand()) + w;
			else if (keys[i - 1] % 3ULL == 0ULL)
				keys[i] = LLONG_MAX + static_cast<ull>(rand()) - w;
			else {
				w *= UINT_MAX * (static_cast<ull>(rand()) + 1ULL);
				keys[i] = (w > ~w ? w : ~w);
			}
		}
		this->init_by_array(keys, MT64_NN);
	}

	explicit MersenneTwister(const ull seed/*=MT64_K2*/) noexcept {
		this->seed = seed;
		this->init_by_rand();
	}

	[[nodiscard]] inline ull x64(uch B/*=64*/) noexcept {
		if (B == 64) {
			if (this->ixc >= MT64_NN) this->shuffle();
			ull x = this->mxc[this->ixc++];
			x ^= (x >> 29) & MT64_G1 ;
			x ^= (x << 17) & MT64_G2;
			x ^= (x << 37) & MT64_G3;
			return x ^ (x >> 43);
		}
		if (B == 32 || B == 16 || B == 8 || B == 4 || B == 2 || B == 1) {
			uch g = MT64_QJ[B];
			this->ckc[g] = (!this->ckc[g]);
			if (this->ckc[g]) return this->rtc[g] / extra::pow_2bi[B];
			return (this->rtc[g] = this->x64(B * 2)) % extra::pow_2bi[B];
		}
		if (B > 2 && B < 64) {
			uch d = MT64_EH[B];
			assert(d < 65);
			return this->x64(d) + extra::pow_2bi[d] * this->x64(B - d);
		}
		return B ? ULLONG_MAX : 0ULL;
	}

	[[nodiscard]] inline sld real(uch B/*=64*/) noexcept // [0.0 - 1.0 if B=64]
		{ return static_cast<sld>(this->x64(B)) / extra::sld_ullmax; }

	[[nodiscard]] inline ull limited(ull NI, ull NF) noexcept {
		const ull D = NF - NI;
		return D < ULLONG_MAX ? (this->x64(extra::pow2f(D)) % (D + 1ULL) + NI) : this->x64(64);
	}
};
