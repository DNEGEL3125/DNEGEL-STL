#include"Matrix.h"
#include"BitVector.h"
#include<list>

using std::cout, std::endl;

int main() {
	BitVector bv;
	bv = "00000001001101010100110"; 

	bv.reserve(512);
	for (size_t i = 0; i < 512; ++i) {
		bv.push_back(i & 1);
	}
	
	cout << bv << endl;
	cout << (bv + bv) << endl;
	cout << ((bv + bv) & ~bv) << endl;
}