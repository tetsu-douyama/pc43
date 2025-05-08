#include "stdafx.h"
#include "Millor.h"

#include <bitset>

//----------------------------------------------------------------------------------------------------------
// 繰り返し自乗法による冪剰余の計算 (b ^ e) mod mを求める
// 
// eの二進数表記(e = e_0 * 2^0+e_1 * 2^1+e_2 * 2^2+..+e_n * 2^n，e_i={0,1})を用いると，
// b^e = b^( e_0 * 2^0 )*b^( e_1 * 2^1 )*b^( e_2 * 2^2 )* .. となる。
// 冪乗余では，かけ算毎にmodを取る。
// 以下プログラムでは ,変数ibにb^2n(b^2,b^4,b^8,n^16..)順次計算し
// 変数ia に Πb^(e_i*2^i)を代入する
//
inline intt modpow(
	intt ib, // 底
	intt ie, // 指数
	intt im)	// 法
{
	intt ia = 1;
	while (ie > 0)
	{
		if ((ie & 1) == 1)  // ビットが立っていれば
		{
			ia = (ia * ib) % im; //  (a * b) mod mを計算
			if (ia == 0) return 0;
		}
		ie >>= 1; // 次のビットへ
		ib *= ib; // b -> b^2n (b^2,b^4,b^8,n^16..) 
		ib %= im; // b^2n -> b^2n mod m
		if (ib == 0) return ia;
	}

	return ia;
}

///////////////////////////////////////////////////////////////////////////
// ビットが1である最下位ビット2^n(一番右)を返す
intt GetNTZ(const intt& bb)
{
	if (bb == 0ULL)
	{
		return 0ULL;
	}

	// 一番右の立ってるビットを返す
	intt bbRet = bb & (~bb + 1);
	return bbRet;
}

///////////////////////////////////////////////////////////////////////////
// 一番右の立っているビットまでの0を数える
int GetNTZIndex(const intt& bb)
{
	static constexpr intt HASH = 0x022fdd63cc95386dULL;// ビット位置を取得するためのハッシュ値
	static const int BIT_SHIFT = 58;
	static const int HASH_TABLE[] = { //64bit用 NTZまでの位置テーブル
		 0, 1, 2,53, 3, 7,54,27,
		 4,38,41, 8,34,55,48,28,
		62, 5,39,46,44,42,22, 9,
		24,35,59,56,49,18,29,11,
		63,52, 6,26,37,40,33,47,
		61,45,43,21,23,58,17,10,
		51,25,36,32,60,20,57,16,
		50,31,19,15,30,14,13,12
	};


	if (bb == 0ULL)
	{
		return 64ULL;
	}
	unsigned long long bbNTZ = GetNTZ(bb); // 以下負値にしないためunsigned
	return HASH_TABLE[(bbNTZ * HASH) >> 58];
}


#ifdef _MirrorRabin_1kkozutu
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ミラー・ラビン素数判別法
// 戻り値 true : 素数かも? , false : 合成数
bool MillerRabinTest(const intt n) // 検定対象の自然数
{
	if (n <= 0) return false;			// 入力ミス
	if (n == 1) return false;			// 1は素数ではない
	if (n == 2) return true;			// 2は素数
	if ((n & 1) == 0) return false;	// 偶数は合成数
	bool bRet = false;

	constexpr intt aiTest[3] = { 2, 7, 61 }; // 下期の出題範囲(＜2^32)では，2,7,61で決定的に素数判別が可能となる。

	// 上記の素数リストに含まれる場合は素数
	if (n == aiTest[0] || n == aiTest[1] || n == aiTest[2]) return true;

	// 2^(s-1)*d = n-1となるs,dを求める
	intt s = GetNTZIndex(n - 1);
	intt d = (n - 1) >> s;


	for each (intt iTest in aiTest)
	{
		intt d0 = d;

		// i^d ≡ 1 mod nとなるか？
		intt n0 = modpow(iTest, d0, n);
		if (n0 == 1 || n0 == n - 1) // 素数かも?
		{
			bRet = true;
			continue;
		}

		// i^(d*2^j)≡ n-1 mod nとなるか ?
		for (int j = 0; j < s - 1; j++)
		{
			d0 <<= 1;

			intt n1 = modpow(iTest, d0, n);
			if (n1 == n - 1) // 素数かも?
			{
				bRet = true;
				break;
			}
			if (n1 == 1) // 合成数
			{
				bRet = false; 
				return bRet;
			}
			// まだ不明 
		}

		d0 <<= 1;
		intt n2 = modpow(iTest, d0, n);
		if (n2 == 1) // 素数かも?
		{
			bRet = true;
		}
		else // 合成数
		{
			bRet = false;
			return bRet;
		}
	}

	return bRet;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ミラー・ラビン素数判別法 範囲指定版
intt MillorRabinTestCount(intt iMin, intt iMax)
{
	intt iCount = 0;
	for (int n = (int)iMin; n <= (int)iMax; n++)
	{
		bool bPrime = false; 
		{

			if (n <= 1) continue;			// 0,1は素数ではない
			if (n == 2) { iCount++; continue; } // 2は素数
			if ((n & 1) == 0) continue;		// 偶数は合成数

			constexpr intt aiWitness[3] = { 2, 7, 61 }; // 下期の出題範囲(＜2^32)では，2,7,61で決定的に素数判別が可能となる。

			// 上記の素数リストに含まれる場合は素数
			if (n == aiWitness[1] || n == aiWitness[2])
			{
				iCount++;
				continue;
			}

			// 2^(s-1)*d = n-1となるs,dを求める
			intt s = GetNTZIndex(n - 1);
			intt d = (n - 1) >> s;

			for each (intt iTest in aiWitness)
			{
				intt d0 = d;

				// i^d ≡ 1 mod nとなるか？
				intt n0 = modpow(iTest, d0, n);
				if (n0 == 1 || n0 == n - 1) // 素数かも?
				{
					bPrime = true;
					continue;
				}

				// i^(d*2^j)≡ n-1 mod nとなるか ?

				for (int j = 0; j < s - 1; j++)
				{
					d0 <<= 1;

					intt n1 = modpow(iTest, d0, n);
					if (n1 == n - 1) // 素数かも?
					{
						bPrime = true;
						continue;
					}
					if (n1 == 1) // 合成数
					{
						bPrime = false;
						break;
					}
					// まだ不明 
				}

				d0 <<= 1;
				intt n2 = modpow(iTest, d0, n);
				if (n2 == 1) // 素数かも?
				{
					bPrime = true;
				}
				else // 合成数
				{
					bPrime = false;
					break;
				}
			}
		}
		if (bPrime)
		{
			iCount++;
		}
	}
	return iCount;
}