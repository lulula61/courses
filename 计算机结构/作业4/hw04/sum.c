#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif


#define CLOCK_RATE_GHZ 2.26e9

static __inline__ uint64_t RDTSC()
{
    uint32_t hi, lo;
    __asm__ volatile
	(
		"rdtsc"
		: "=a"(lo), "=d"(hi)
	);
	return (((uint64_t)hi) << 32) | ((uint64_t)lo);
}

static int sum_naive(int n, int *a)
{
    int sum = 0;
	for (int i = 0; i < n; i++)
	{
		if(a[i] > 0) sum += a[i];
	}
	return sum;
}

static int sum_unrolled(int n, int *a)
{
    int sum = 0;

    // unrolled loop
	for (int i = 0; i < n / 4 * 4; i += 4)
    {
        sum += a[i+0];
        sum += a[i+1];
        sum += a[i+2];
        sum += a[i+3];

    }

    // tail case
	for (int i = n / 4 * 4; i < n; i++)
	{
		sum += a[i];
	}

    return sum;
}

static int sum_vectorized(int n, int *a)
{
    // WRITE YOUR VECTORIZED CODE HERE
	__m256i sum = _mm256_setzero_si256();
    int ans;
	int s[8];
	for (int i = 0; i < n / 8; i++) {
		__m256i va = _mm_loadu_si256(a+i*8);
		sum = _mm_add_epi32(sum, va);
	}
	_mm_storeu_si256(s,sum);
	for (int i = 0; i < 8; i++) ans += s[i];
	for (int i = n / 8 * 8; i < n; i++) {
		ans += a[i];
	}
	return ans;
}

static int sum_vectorized_unrolled(int n, int *a)
{
    // UNROLL YOUR VECTORIZED CODE HERE
	__m128i* aa = (__m128i*)a;
	__m128i sum0 = _mm_setzero_si128();
	__m128i sum1 = _mm_setzero_si128();
	__m128i sum2 = _mm_setzero_si128();
	__m128i sum3 = _mm_setzero_si128();
	int ans;
	int s[4];
	int k = 0;
	for (int i = 0; i < n/16; i++) {
		__m128i va = _mm_loadu_si128(&aa[k]);
		__m128i vb = _mm_loadu_si128(&aa[k + 1]);
		__m128i vc = _mm_loadu_si128(&aa[k + 2]);
		__m128i vd = _mm_loadu_si128(&aa[k + 3]);
		sum0 = _mm_add_epi32(sum0, va);
		sum1 = _mm_add_epi32(sum1, vb);
		sum2 = _mm_add_epi32(sum2, vc);
		sum3 = _mm_add_epi32(sum3, vd);

		k += 4;
	}
	sum0 = _mm_add_epi32(sum0, sum1);
	sum2 = _mm_add_epi32(sum2, sum3);
	sum0 = _mm_add_epi32(sum0, sum2);
	_mm_storeu_si128(s,sum0);
	ans = s[0]+s[1]+s[2]+s[3];

	for (int i = n/16*16; i < n; i++) {
		ans += a[i];
	}
	return ans;
}

void benchmark(int n, int *a, int(*computeSum)(int, int*), char *name)
{
    // warm up cache
    int sum = computeSum(n, a);

    // measure
    uint64_t beginCycle = RDTSC();
    sum += computeSum(n, a);
	uint64_t cycles = RDTSC() - beginCycle;

    double microseconds = cycles/CLOCK_RATE_GHZ*1e6;

    // print results
	printf("%20s: ", name);
	if (sum == 2 * sum_naive(n, a))
	{
		printf("%.2f microseconds\n", microseconds);
	}
	else
	{
		printf("ERROR!\n");
	}
}

int main(int argc, char **argv)
{
    const int n = 9999;

    // initialize the array with random values
	srand48(time(NULL));
	int a[n] __attribute__((aligned(16)));
	for (int i = 0; i < n; i++)
	{
		a[i] = lrand48();
	}

    // benchmark series of codes
	benchmark(n, a, sum_naive, "naive");
	benchmark(n, a, sum_unrolled, "unrolled");
	benchmark(n, a, sum_vectorized, "vectorized");
	benchmark(n, a, sum_vectorized_unrolled, "vectorized unrolled");

    return 0;
}
