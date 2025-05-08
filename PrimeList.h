/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 素数リスト                                                                                              //
//                                                                                              2018/12/10 //
//                                                                                              T.Douyama  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UTF-8で保存すること

#include <vector>
class PrimeList
{
private:
	std::vector<unsigned long long> m_viPrimeList;
	//std::vector<unsigned long long> m_viCountList;
public:

	PrimeList();

	// 区間の素数を数える
	int GetCountPrime(int iStart, int iEnd);
};