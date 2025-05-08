/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 範囲の素数の個数を求める                                                                                //
//                                                                                              2018/12/10 //
//                                                                                              T.Douyama  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "PrimeList.h"

int main(int argc, char* argv[])
{
	if (argc != 3) return 0;
	int iStart = 0;
	int iEnd = 0;

	iStart = atoi(argv[1]);
	iEnd = atoi(argv[2]);


	// 素数表の準備
	PrimeList pl;
	int iCount = pl.GetCountPrime(iStart, iEnd);

	std::cout << iCount ;

    return 0;
}

