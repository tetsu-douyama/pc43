///////////////////////////////////////////////////////////////////////////////
// 素数を数える                                                              //
//                                                                 T.DOUYAMA //
//                                                                 2018.12   //
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <math.h>

typedef  uint64_t intt;

int main(int argc, char* argv[])
{
	if (argc != 3) return 0;
	intt iMin = atoi(argv[1]);					// 数列の最大値取得
	intt iMax = atoi(argv[2]);					// 数列の最大値取得
	if (iMin > iMax) return 0;
	int iCount = 0;								// iMin～iMaxまでの素数の個数

	std::vector<bool> viNum(iMax + 1);			// 数列 最終的に合成数なら1，素数なら0 
	intt iSqr = (intt)sqrt(iMax)+1;				// 素因数のiMax
	viNum[0] = true;								// [0]は素数でない
	viNum[1] = true;								// [1]は素数でない

	// 2～sqrt(iMax)までの素数列の作成-------------------------------------------------------------------------
	// 偶数
	{
		for ( intt ii = 2*2; ii<= iMax; ii+=2 )
		{
			viNum[ii]= true;							// iの倍数はすべて合成数
		}
	}
	// 3倍数以上
	for (intt i = 3; i < iSqr; i += 2)
	{
		if (viNum[i] != 0) continue;				// 合成数
		for (intt ii = i * i; ii <= iMax; ii += i * 2)		// i^2より小さい値は篩い済み
		{
			viNum[ii] = true;							// iの倍数はすべて合成数
		}
	}
	// iMin～iMaxまでの素数を集計------------------------------------------------------------------------
	std::for_each(viNum.begin() + iMin, viNum.end(), [&iCount](bool n) {iCount += !n; });
	//---------------------------------------------------------------------------------------------------------
	// 答え
	std::cout << iCount << std::endl;
	return 0;
}