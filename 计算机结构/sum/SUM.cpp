#include<emmintrin.h>
#include<iostream>
#include <immintrin.h>


using namespace std;

int sum_vectorized(int n, int* a)
{
	// WRITE YOUR VECTORIZED CODE HERE
	__m256i sum = _mm256_setzero_si256();
	__m256i* aa = (__m256i*)a;
	int ans = 0;
	
	for (int i = 0; i < n / 8; i++) {
		__m256i va = _mm256_loadu_si256(&aa[i]);
		sum = _mm256_add_epi32(sum, va);
	}
	//_mm256_storeu_si256(s, sum);
	int* s = (int*)&sum;
	for (int i = 0; i < 8; i++) ans += s[i];
	
	for (int i = n / 8 * 8; i < n; i++) {
		ans += a[i];
	}
	return ans;
}

int sum_vectorized_unrolled(int n, int* a)
{
	// UNROLL YOUR VECTORIZED CODE HERE
	__m128i* aa = (__m128i*)a;
	__m128i sum0 = _mm_setzero_si128();
	__m128i sum1 = _mm_setzero_si128();
	__m128i sum2 = _mm_setzero_si128();
	__m128i sum3 = _mm_setzero_si128();
	__m128i sum4 = _mm_setzero_si128();
	__m128i sum5 = _mm_setzero_si128();
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
	int* p = (int*)&sum0;
	int s = p[0] + p[1] + p[2] + p[3];

	for (int i = n/16*16; i < n; i++) {
		s += a[i];
	}
	return s;
}

int main() {
	int* data = new int[43];
	for (int i = 0; i < 43; i++) {
		data[i] = i+1;
	}
	int sum = sum_vectorized(43, data);
	int sum2 = sum_vectorized_unrolled(43, data);
	cout << sum << "  " << sum2 << endl;
	return 0;

}