#pragma once
#include<bitset>
#include<vector>
#include<deque>
#include<iostream>
#include<string>


class BitVector {
public:
	typedef unsigned long long BaseTy;

	/* iterator */
	class iterator {
	private:
		BitVector* master = nullptr;
		size_t index;

	public:
		_CONSTEXPR20 iterator(BitVector* master = nullptr, size_t index = 0) :index(index), master(master) {}

		_CONSTEXPR20 bool operator==(const iterator& other) {
			return this->master == other.master && this->index == other.index;
		}

		_CONSTEXPR20 bool operator!=(const iterator& other) {
			return !(*this == other);
		}

		_CONSTEXPR20 iterator& operator+=(size_t x) {
			index += x;
			_STL_VERIFY(index < master->size(), "cannot seek BitVector iterator after end");
			return *this;
		}

		_CONSTEXPR20 iterator& operator++() {
			return *this += 1;
		}

		_CONSTEXPR20 iterator operator+(size_t x)const {
			iterator newI = *this;
			return newI += x;
		}

		_CONSTEXPR20 iterator& operator-=(size_t x) {
			_STL_VERIFY(index < x, "cannot seek BitVector iterator before begin");
			index -= x;
			return *this;
		}

		_CONSTEXPR20 iterator& operator--() {
			return *this -= 1;
		}

		_CONSTEXPR20 iterator operator-(size_t x)const {
			iterator newI = *this;
			return newI -= x;
		}

		_CONSTEXPR20 ptrdiff_t operator-(const iterator& other)const {
			if (this->index < other.index) {
				return -static_cast<ptrdiff_t>(other.index - this->index);
			}
			return this->index - other.index;
		}

		_CONSTEXPR20 bool operator*()const {
			return this->master->test(index);
		}
	};

private:
	std::vector<BaseTy> data;
	size_t nBit = 0;
	const static size_t BASE_SIZE = sizeof(BaseTy) * CHAR_BIT;

	_CONSTEXPR20 void _trim() {
		if (this->noPadding()) {
			return;
		}
		data.back() &= (BaseTy{ 1 } << nBit % BASE_SIZE) - 1;
	}



public:
	/* constructors */
	_CONSTEXPR20 BitVector() = default;
	_CONSTEXPR20 BitVector(size_t _size) {
		this->resize(_size);
	}
	_CONSTEXPR20 BitVector(const std::string& s) {
		*this = s;
	}
	template< typename IterTy>
	_CONSTEXPR20 BitVector(IterTy begin, IterTy end) {
		for (auto iter = begin; iter != end; ++iter) {
			this->push_back(*iter);
		}
	}

	/* bitset methods */
	_CONSTEXPR20 void reset() {
		std::fill(data.begin(), data.end(), BaseTy{ 0 });
	}

	_CONSTEXPR20 auto& set() {
		std::fill(data.begin(), data.end(), ~BaseTy{ 0 });
		this->_trim();
		return *this;
	}

	_CONSTEXPR20 auto& set(size_t index, bool val = true) {
		if (index >= this->nBit) {
			this->_Xran();
		}
		auto& _Selected_word = this->data[index / BASE_SIZE];
		const auto _Bit = BaseTy{ 1 } << index % BASE_SIZE;
		if (val) {
			// 将位设置成1
			_Selected_word |= _Bit;
		}
		else {
			// 将位设置成0
			_Selected_word &= ~_Bit;
		}

		return *this;
	}

	_CONSTEXPR20 size_t size() const { return this->nBit; }

	_CONSTEXPR20 bool empty()const { return this->nBit == 0; }


	_CONSTEXPR20 bool any()const {
		for (size_t i = 0; i < data.size(); ++i) {
			if (data[i] != 0) {
				return true;
			}
		}
		return false;
	}

	_CONSTEXPR20 bool all()const {
		size_t iEnd = this->noPadding() ? data.size() : data.size() - 1;
		for (size_t i = 0; i < iEnd; ++i) {
			// BaseTy的全1形态
			if (data[i] != ~static_cast<BaseTy>(0)) {
				return false;
			}
		}
		if (this->noPadding()) {
			return true;
		}
		return data.back() == (BaseTy{ 1 } << (nBit % BASE_SIZE)) - 1;
	}

	_CONSTEXPR20 size_t count()const {
		size_t res = 0;
		for (size_t i = 0; i < data.size(); ++i) {
			auto temp = data[i];
			while (temp != 0) {
				if (temp & 1) {
					++res;
				}
				temp >>= 1;
			}
		}
		return res;
	}

	_CONSTEXPR20 auto& flip() {
		for (size_t i = 0; i < data.size(); ++i) {
			data[i] = ~data[i];
		}
		this->_trim();

		return *this;
	}

	_CONSTEXPR20 auto& flip(size_t index) {
		if (index >= this->nBit) {
			this->_Xran();
		}
		this->data[index / BASE_SIZE] ^= BaseTy{ 1 } << index % BASE_SIZE;
		return *this;
	}

	_CONSTEXPR20 bool none()const {
		return !this->any();
	}

	void bs() {
		std::bitset<32> bs(""); 
	}

	_CONSTEXPR20 bool test(size_t index)const {
		if (index >= this->nBit) {
			this->_Xran();
		}

		return (BaseTy{ 1 } << (index % BASE_SIZE)) & this->data[index / BASE_SIZE];
	}

	_CONSTEXPR20 unsigned long to_ulong()const {
		return static_cast<unsigned long>(this->data.front());
	}

	_CONSTEXPR20 unsigned long long to_ullong()const {
		return static_cast<unsigned long long>(this->data.front());
	}

	_CONSTEXPR20 std::string to_string(int radix = 2) const {
		std::string s;
		s.reserve(this->size());
		for (size_t i = 0; i < this->nBit; ++i) {
			s += '0' + this->test(i);
		}
		return s;
	}

	_CONSTEXPR20 bool noPadding() const {
		return this->nBit % BASE_SIZE == 0;
	}

	/* vector methods */
	_CONSTEXPR20 void push_back(bool val) {
		if (this->noPadding()) {
			// 需要新增一个base
			this->data.emplace_back(0);
		}
		
		this->data.back() |= (BaseTy{ val } << this->nBit % BASE_SIZE);
		this->nBit++;
	}

	_CONSTEXPR20 void pop_back() {
		this->resize(nBit - 1);
	}

	_CONSTEXPR20 void resize(size_t newSize) {
		this->data.resize((newSize - 1) / BASE_SIZE + 1);
		this->nBit = newSize;
		this->_trim();
	}

	_CONSTEXPR20 void reserve(size_t _capa) {
		this->data.reserve((_capa - 1) / BASE_SIZE + 1);
	}

	/* iteration method */
	_CONSTEXPR20 iterator begin() {
		return iterator(this, 0);
	}

	_CONSTEXPR20 iterator end() {
		return iterator(this, this->size());
	}

	/* operation */
	_CONSTEXPR20 BitVector add(const BitVector& other, bool isExtend = false) const {
		BitVector newB = *this;
		short carry = 0;
		BaseTy oneAndZeros = BaseTy{ 1 } << (BASE_SIZE - 1);
		for (size_t i = 0; i < newB.data.size(); ++i) {
			BaseTy oDataI = i < other.data.size() ? other.data[i] : 0;
			auto thisHead = newB.data[i] & oneAndZeros;
			auto otherHead = oDataI & oneAndZeros;
			auto thisNoHead = newB.data[i] ^ thisHead;
			auto otherNoHead = oDataI ^ otherHead;
			auto noHeadSum = thisNoHead + otherNoHead + carry;

			carry = 0;
			for (bool check : { thisHead, otherHead, noHeadSum& oneAndZeros }) {
				if (check) {
					++carry;
				}
			}

			switch (carry) {
			case 0:
				newB.data[i] = noHeadSum;
				break;
			case 1:
				newB.data[i] = noHeadSum | oneAndZeros;
				carry = 0;
				break;
			case 2:
				newB.data[i] = noHeadSum;
				carry = 1;
				break;
			case 3:
				newB.data[i] = noHeadSum | oneAndZeros;
				carry = 1;
				break;
			default:
				std::cerr << "carry is " << carry << std::endl;
				throw std::runtime_error("strange result");
				break;
			}
		}

		if (isExtend && newB.noPadding() && carry >= 2) {

		}
		else {
			newB._trim();
		}

		return newB;
	}

	/* operators */
	_CONSTEXPR20 bool operator[](size_t index) const {
		return this->test(index);
	}

	_CONSTEXPR20 void operator=(const std::string& s) {
		this->resize(s.size());

		for (size_t i = 0; i < nBit; ++i) {
			this->set(i, s[i] != '0');
		}
	}

	_CONSTEXPR20 void operator&=(const BitVector& other) {
		size_t iEnd = std::min(other.data.size(), this->data.size());
		for (size_t i = 0; i < iEnd; ++i) {
			this->data[i] &= other.data[i];
		}
		if (iEnd <= this->data.size()) {
			return;
		}

		// other没有的位全部置0
		std::fill(this->data.begin() + iEnd, this->data.end(), BaseTy{ 0 });
	}

	_CONSTEXPR20 BitVector operator&(const BitVector& other) {
		BitVector newB = *this;
		newB &= other;
		return newB;
	}

	_CONSTEXPR20 BitVector& operator|=(const BitVector& other) {
		size_t iEnd = std::min(other.data.size(), this->data.size());
		for (size_t i = 0; i < iEnd; ++i) {
			this->data[i] |= other.data[i];
		}
		this->_trim();
		return *this;
	}

	_CONSTEXPR20 BitVector operator|(const BitVector& other) {
		BitVector newB = *this;
		newB |= other;
		return newB;
	}

	_CONSTEXPR20 void operator^=(const BitVector& other) {
		size_t iEnd = std::min(other.data.size(), this->data.size());
		for (size_t i = 0; i < iEnd; ++i) {
			this->data[i] ^= other.data[i];
		}
		this->_trim();
	}

	_CONSTEXPR20 BitVector operator^(const BitVector& other) {
		BitVector newB = *this;
		newB ^= other;
		return newB;
	}

	_CONSTEXPR20 void operator>>=(size_t shiftTimes) {
		for (size_t i = 1; i < this->data.size(); ++i) {
			this->data[i - 1] >>= shiftTimes;
			// 0101 |= (1011 & 0001) << (4 - 1)
			this->data[i - 1] |= (
				this->data[i] & (~BaseTy{ 0 } >> (BASE_SIZE - shiftTimes))
				) << (BASE_SIZE - shiftTimes);
		}
	}

	_CONSTEXPR20 BitVector operator>>(size_t shiftTimes) {
		BitVector newB = *this;
		newB >>= shiftTimes;
		return newB;
	}

	_CONSTEXPR20 void operator<<=(size_t shiftTimes) {
		if (this->empty()) {
			return;
		}
		for (size_t i = this->data.size(); i > 1;) {
			--i;
			this->data[i] <<= shiftTimes;
			// 1000 |= (1011 & 1110) >> (4 - 3)
			this->data[i] |= (
				this->data[i - 1] & (~BaseTy{ 0 } << (BASE_SIZE - shiftTimes))
				) >> (BASE_SIZE - shiftTimes);
		}
		this->data.front() <<= shiftTimes;
	}

	_CONSTEXPR20 BitVector operator<<(size_t shiftTimes) {
		BitVector newB = *this;
		newB <<= shiftTimes;
		return newB;
	}

	_CONSTEXPR20 BitVector& operator=(BaseTy val) {
		this->data.clear();
		this->data.push_back(val);
		this->nBit = BASE_SIZE;
		return *this;
	}

	_CONSTEXPR20 BitVector& operator~() {
		return this->flip();
	}

	_CONSTEXPR20 BitVector& operator+=(const BitVector& other) {
		short carry = 0;
		BaseTy oneAndZeros = BaseTy{ 1 } << (BASE_SIZE - 1);
		for (size_t i = 0; i < this->data.size(); ++i) {
			BaseTy oDataI = i < other.data.size() ? other.data[i] : 0;
			auto thisHead = this->data[i] & oneAndZeros;
			auto otherHead = oDataI & oneAndZeros;
			auto thisNoHead = this->data[i] ^ thisHead;
			auto otherNoHead = oDataI ^ otherHead;
			auto noHeadSum = thisNoHead + otherNoHead + carry;

			carry = 0;
			for (bool check : { thisHead, otherHead, noHeadSum& oneAndZeros }) {
				if (check) {
					++carry;
				}
			}

			switch (carry) {
			case 0:
				this->data[i] = noHeadSum;
				break;
			case 1:
				this->data[i] = noHeadSum | oneAndZeros;
				carry = 0;
				break;
			case 2:
				this->data[i] = noHeadSum;
				carry = 1;
				break;
			case 3:
				this->data[i] = noHeadSum | oneAndZeros;
				carry = 1;
				break;
			default:
				std::cerr << "carry is " << carry << std::endl;
				throw std::runtime_error("strange result");
				break;
			}
		}
		return *this;
	}

	_CONSTEXPR20 BitVector operator+(const BitVector& other)const {
		BitVector newB = *this;
		newB += other;
		return newB;
	}

	/* friends */
	friend std::ostream& operator<<(std::ostream& out, const BitVector& self) {
		out << self.to_string();
		return out;
	}

private:
	/* error */
	[[noreturn]] static void _Xran() {
		throw std::range_error("invalid BitVector position");
	}
};

