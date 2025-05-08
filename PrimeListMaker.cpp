///////////////////////////////////////////////////////////////////////////////
// 素数を数える                                                              //
//                                                                 T.DOUYAMA //
//                                                                 2018.12   //
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#ifdef _CLOCK // 時間計測用
#include <chrono>
#endif

#include "Millor.h"
#ifdef _MIRROR_TEST_MODE
#include <fstream>
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// エラトステネスのふるいにより，範囲[iMin,iMax]にある素数の個数を返す
// 
int Eratos(intt iMin, intt iMax)
{
	int iCount = 0;

	std::vector<bool> viNum(iMax + 1);			// 数列 最終的に合成数なら1，素数なら0 
	intt iSqr = (intt)sqrt(iMax) + 1;			// 素因数のiMax
	viNum[0] = true;							// [0]は素数でない
	viNum[1] = true;							// [1]は素数でない

	// 2～sqrt(iMax)までの素数列の作成-------------------------------------------------------------------------
	// 偶数
#pragma omp parallel for
	for (intt ii = 2 * 2; ii <= iMax; ii += 2)
	{
		viNum[ii] = true;
	}
	// 3の倍数
#pragma omp parallel for
	for (intt ii = 3 * 3; ii <= iMax; ii += 3 * 2)
	{
		viNum[ii] = true;
	}

	// 5以上の倍数
	for (intt i = 5; i < iSqr; i += 2)
	{
		if (viNum[i] != 0) continue;				// 合成数

		viNum[i*i] = true; // i^2は合成数

		// ループAの初期値 i^2を超えた 最初のi*n mod 6=5となる値
		intt istA = i * (i + 6 - 1 - (i % 6));
		// ループBの初期値 i^2を超えた 最初のi*n mod 6=1となる値
		intt istB = i * (i + 6 + 1 - (i % 6));

#pragma omp parallel for
		for (intt ii = istA; ii <= iMax; ii += i * 6)
		{
			viNum[ii] = true;
		}
#pragma omp parallel for
		for (intt ii = istB; ii <= iMax; ii += i * 6)
		{
			viNum[ii] = true;
		}
	}

	// iMin～iMaxまでの素数を集計------------------------------------------------------------------------
	std::for_each(viNum.begin() + iMin, viNum.end(), [&iCount](bool n) {iCount += !n; });
	//---------------------------------------------------------------------------------------------------------
	return iCount;
}
intt PrimeCounter(intt iMin, intt iMax)
{
	intt iCount = 0;

	if (sqrt(iMax)< (iMax - iMin))
	{
		//----------------------
		// エラトステネスのふるいにより集計
		iCount = Eratos(iMin, iMax);
	}
	else
	{
		//----------------------
		// ミラーラビン素数判別法
		if (iMax > 1000000000) // 64bit剰余計算上限
		{
			iCount = 0;
		}
		else
		{
			iCount = MillorRabinTestCount(iMin, iMax);
		}
	}
	return iCount;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	if (argc < 3) return 0;
	intt iMin = atoi(argv[1]);					// 数列の最大値取得
	intt iMax = atoi(argv[2]);					// 数列の最大値取得
	if (iMin > iMax) return 0;

#ifdef _MIRROR_TEST_MODE
	// 1～300000000までの値を出力
	constexpr intt TEST_MIN = 1;
	constexpr intt TEST_MAX = 300000000;
	std::ofstream ofs("mirror.txt");
	for (size_t i = TEST_MIN; i < TEST_MAX; i++)
	{
		bool bPrime = MillerRabinTest(i);
		bool bPrimeE = Eratos(i, i) == 1;
		if (bPrime != bPrimeE)
		ofs << i << ",";

	}
	ofs.close();
	return 0;
#endif

#ifdef _CLOCK // 実効時間計測
	std::chrono::system_clock::time_point chrono_start, chrono_end;
	chrono_start = std::chrono::system_clock::now();
#endif

	// 素数カウント開始
	intt iCount = PrimeCounter(iMin, iMax);
#ifdef _CLOCK // 実効時間計測
	chrono_end = std::chrono::system_clock::now();
	double dTime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(chrono_end - chrono_start).count() / 1000.0);
#endif

	// 答え
	std::cout << iCount << std::endl;
	return 0;
}