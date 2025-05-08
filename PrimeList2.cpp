/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 素数の個数を数える                                                                                      //
//                                                                                              2018/12/10 //
//                                                                                              T.Douyama  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// UTF-8で保存すること
#include <iostream>
#include <sstream>
#include <fstream>
#include <bitset>
#include "PrimeList.h"

// 立っているビットを数える
int PopCount(const uint64_t& bb)
{
	uint64_t bbTmp = bb;
	bbTmp = (bbTmp & 0x5555555555555555ULL) + ((bbTmp >> 1) & 0x5555555555555555ULL);	// 2bitごとの集計
	bbTmp = (bbTmp & 0x3333333333333333ULL) + ((bbTmp >> 2) & 0x3333333333333333ULL);	// 4bitごとの集計
	bbTmp = (bbTmp & 0x0f0f0f0f0f0f0f0fULL) + ((bbTmp >> 4) & 0x0f0f0f0f0f0f0f0fULL);	// 8bitごとの集計
	bbTmp = (bbTmp & 0x00ff00ff00ff00ffULL) + ((bbTmp >> 8) & 0x00ff00ff00ff00ffULL);	// 16bitごとの集計
	bbTmp = (bbTmp & 0x0000ffff0000ffffULL) + ((bbTmp >> 16) & 0x0000ffff0000ffffULL);	// 32bitごとの集計
	bbTmp = bbTmp + (bbTmp >> 32) & 0x007fUL;												// 全集計
	return (int)bbTmp;
}

// 区間の素数を数える
int PrimeList::GetCountPrime(int iStart, int iEnd)
{
	// 調整
	if (iStart > iEnd) return 0;
	if (iStart < 0 || iEnd < 0) return 0;
	if (iStart > 100000000 || iEnd > 100000000) return 0; // 100,000,000超過は計算しない

	// カウンタ
	int iCount = 0;
	
	// 素数リストに2は含まれていないので，範囲に2が含まれていたときはカウント+1する。
	if (iStart <= 2 && iEnd >= 2)
	{
		iCount++;
	}
	iStart = iStart / 2 + (iStart % 2 == 0);
	iEnd = (iEnd + 1) / 2;
	 
	// 素数リストの座標
	int iStartBlock = iStart / 64;
	int iStartBit = iStart % 64;
	int iEndBlock = iEnd / 64;
	int iEndBit = iEnd % 64;

	// 開始bitから63bit目までpopしているマスク
	std::bitset<64> bitMaskStart;
	for (size_t i = iStartBit; i < 64; i++)
	{
		bitMaskStart[i] = true;
	}
	uint64_t iMaskStart = bitMaskStart.to_ullong();

	// 0bitから終端bitまでpopしているマスク
	std::bitset<64> bitMaskEnd;
	for (size_t i = 0; i < iEndBit; i++)
	{
		bitMaskEnd[i] = true;
	}
	uint64_t iMaskEnd = bitMaskEnd.to_ullong();

	// １つのブロックに収まる
	if (iStartBlock == iEndBlock)
	{
		uint64_t iMask = iMaskStart & iMaskEnd;
		std::bitset<64> a;
		a = iMask;

		std::bitset<64> b = m_viPrimeList[iStartBlock] & iMask;
		a.count();
		iCount += b.count();
		return iCount;
	}

	// 開始ブロック
	std::bitset<64> iStartBlockFiltered = m_viPrimeList[iStartBlock] & iMaskStart;
	iCount += iStartBlockFiltered.count();

	// 中間ブロック
	for (size_t i = iStartBlock + 1; i < (size_t)iEndBlock; i++)
	{
		iCount += PopCount(m_viPrimeList[i]);
	}

	// 終端ブロック
	std::bitset<64> iEndBlockFiltered = m_viPrimeList[iEndBlock] & iMaskEnd;
	iCount += iEndBlockFiltered.count();

	return iCount;
}