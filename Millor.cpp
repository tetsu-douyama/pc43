#include "stdafx.h"
#include "Millor.h"

#include <bitset>

//----------------------------------------------------------------------------------------------------------
// �J��Ԃ�����@�ɂ��p��]�̌v�Z (b ^ e) mod m�����߂�
// 
// e�̓�i���\�L(e = e_0 * 2^0+e_1 * 2^1+e_2 * 2^2+..+e_n * 2^n�Ce_i={0,1})��p����ƁC
// b^e = b^( e_0 * 2^0 )*b^( e_1 * 2^1 )*b^( e_2 * 2^2 )* .. �ƂȂ�B
// �p��]�ł́C�����Z����mod�����B
// �ȉ��v���O�����ł� ,�ϐ�ib��b^2n(b^2,b^4,b^8,n^16..)�����v�Z��
// �ϐ�ia �� ��b^(e_i*2^i)��������
//
inline intt modpow(
	intt ib, // ��
	intt ie, // �w��
	intt im)	// �@
{
	intt ia = 1;
	while (ie > 0)
	{
		if ((ie & 1) == 1)  // �r�b�g�������Ă����
		{
			ia = (ia * ib) % im; //  (a * b) mod m���v�Z
			if (ia == 0) return 0;
		}
		ie >>= 1; // ���̃r�b�g��
		ib *= ib; // b -> b^2n (b^2,b^4,b^8,n^16..) 
		ib %= im; // b^2n -> b^2n mod m
		if (ib == 0) return ia;
	}

	return ia;
}

///////////////////////////////////////////////////////////////////////////
// �r�b�g��1�ł���ŉ��ʃr�b�g2^n(��ԉE)��Ԃ�
intt GetNTZ(const intt& bb)
{
	if (bb == 0ULL)
	{
		return 0ULL;
	}

	// ��ԉE�̗����Ă�r�b�g��Ԃ�
	intt bbRet = bb & (~bb + 1);
	return bbRet;
}

///////////////////////////////////////////////////////////////////////////
// ��ԉE�̗����Ă���r�b�g�܂ł�0�𐔂���
int GetNTZIndex(const intt& bb)
{
	static constexpr intt HASH = 0x022fdd63cc95386dULL;// �r�b�g�ʒu���擾���邽�߂̃n�b�V���l
	static const int BIT_SHIFT = 58;
	static const int HASH_TABLE[] = { //64bit�p NTZ�܂ł̈ʒu�e�[�u��
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
	unsigned long long bbNTZ = GetNTZ(bb); // �ȉ����l�ɂ��Ȃ�����unsigned
	return HASH_TABLE[(bbNTZ * HASH) >> 58];
}


#ifdef _MirrorRabin_1kkozutu
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �~���[�E���r���f�����ʖ@
// �߂�l true : �f������? , false : ������
bool MillerRabinTest(const intt n) // ����Ώۂ̎��R��
{
	if (n <= 0) return false;			// ���̓~�X
	if (n == 1) return false;			// 1�͑f���ł͂Ȃ�
	if (n == 2) return true;			// 2�͑f��
	if ((n & 1) == 0) return false;	// �����͍�����
	bool bRet = false;

	constexpr intt aiTest[3] = { 2, 7, 61 }; // �����̏o��͈�(��2^32)�ł́C2,7,61�Ō���I�ɑf�����ʂ��\�ƂȂ�B

	// ��L�̑f�����X�g�Ɋ܂܂��ꍇ�͑f��
	if (n == aiTest[0] || n == aiTest[1] || n == aiTest[2]) return true;

	// 2^(s-1)*d = n-1�ƂȂ�s,d�����߂�
	intt s = GetNTZIndex(n - 1);
	intt d = (n - 1) >> s;


	for each (intt iTest in aiTest)
	{
		intt d0 = d;

		// i^d �� 1 mod n�ƂȂ邩�H
		intt n0 = modpow(iTest, d0, n);
		if (n0 == 1 || n0 == n - 1) // �f������?
		{
			bRet = true;
			continue;
		}

		// i^(d*2^j)�� n-1 mod n�ƂȂ邩 ?
		for (int j = 0; j < s - 1; j++)
		{
			d0 <<= 1;

			intt n1 = modpow(iTest, d0, n);
			if (n1 == n - 1) // �f������?
			{
				bRet = true;
				break;
			}
			if (n1 == 1) // ������
			{
				bRet = false; 
				return bRet;
			}
			// �܂��s�� 
		}

		d0 <<= 1;
		intt n2 = modpow(iTest, d0, n);
		if (n2 == 1) // �f������?
		{
			bRet = true;
		}
		else // ������
		{
			bRet = false;
			return bRet;
		}
	}

	return bRet;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �~���[�E���r���f�����ʖ@ �͈͎w���
intt MillorRabinTestCount(intt iMin, intt iMax)
{
	intt iCount = 0;
	for (int n = (int)iMin; n <= (int)iMax; n++)
	{
		bool bPrime = false; 
		{

			if (n <= 1) continue;			// 0,1�͑f���ł͂Ȃ�
			if (n == 2) { iCount++; continue; } // 2�͑f��
			if ((n & 1) == 0) continue;		// �����͍�����

			constexpr intt aiWitness[3] = { 2, 7, 61 }; // �����̏o��͈�(��2^32)�ł́C2,7,61�Ō���I�ɑf�����ʂ��\�ƂȂ�B

			// ��L�̑f�����X�g�Ɋ܂܂��ꍇ�͑f��
			if (n == aiWitness[1] || n == aiWitness[2])
			{
				iCount++;
				continue;
			}

			// 2^(s-1)*d = n-1�ƂȂ�s,d�����߂�
			intt s = GetNTZIndex(n - 1);
			intt d = (n - 1) >> s;

			for each (intt iTest in aiWitness)
			{
				intt d0 = d;

				// i^d �� 1 mod n�ƂȂ邩�H
				intt n0 = modpow(iTest, d0, n);
				if (n0 == 1 || n0 == n - 1) // �f������?
				{
					bPrime = true;
					continue;
				}

				// i^(d*2^j)�� n-1 mod n�ƂȂ邩 ?

				for (int j = 0; j < s - 1; j++)
				{
					d0 <<= 1;

					intt n1 = modpow(iTest, d0, n);
					if (n1 == n - 1) // �f������?
					{
						bPrime = true;
						continue;
					}
					if (n1 == 1) // ������
					{
						bPrime = false;
						break;
					}
					// �܂��s�� 
				}

				d0 <<= 1;
				intt n2 = modpow(iTest, d0, n);
				if (n2 == 1) // �f������?
				{
					bPrime = true;
				}
				else // ������
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