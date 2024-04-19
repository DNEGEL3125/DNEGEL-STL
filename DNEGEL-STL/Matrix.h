#pragma once
#include <vector>
#include<iostream>
#include <stdexcept>

typedef long long ll;
using std::vector;
using std::pair;
using std::runtime_error;
using std::ostream;

template<typename T>
class Vector {
	vector<T> data;
	size_t len = 0;

public:
	Vector(ll len = 0, T initVal = T()) {
		data.resize(len, initVal);
		this->len = len;
	}

	Vector operator*(const Vector& other)const {
		Vector a;
		const Vector& me = *this;

		for (size_t i = 0; i < len; ++i) {
			a.data.push_back(me[i] * other[i]);
		}

		return a;
	}

	T& operator[](size_t i) {
		return data.at(i);
	}

	const T& operator[](size_t i) const {
		return data.at(i);
	}

	friend ostream& operator<<(ostream& o, const Vector& a) {
		o << "[";
		for (const auto& e : a.data) {
			o << e << '\t';
		}
		return o << "]";
	}
};

class Matrix {
	vector<vector<ll>> data;

public:
	Matrix(ll row, ll col) {
		data.resize(row, vector<ll>(col));
	}

	Matrix(const pair<size_t, size_t>& shape) {
		data.resize(shape.first, vector<ll>(shape.second));
	}

	vector<ll>& operator[](size_t index) {
		return data.at(index);
	}

	const vector<ll>& operator[](size_t row) const {
		return data.at(row);
	}

	Matrix operator*(const Matrix& other)const {
		ll col = shape().second, row = shape().first, oCol = other.shape().second, oRow = other.shape().first;
		if (col != oRow) {
			throw runtime_error("shapes are illegal");
		}
		Matrix a(row, oCol);
		for (ll i = 0; i < oCol; ++i) {
			for (ll j = 0; j < row; ++j) {
				for (ll k = 0; k < col; ++k) {
					a[j][i] = (a[j][i] + other[k][i] * data[j][k]);
				}
			}
		}
		return a;
	}

	Matrix multiply(const Matrix& other, int mod)const {
		ll col = shape().second, row = shape().first, oCol = other.shape().second, oRow = other.shape().first;
		if (col != oRow) {
			throw runtime_error("shapes are illegal");
		}
		Matrix a(row, oCol);
		for (ll i = 0; i < oCol; ++i) {
			for (ll j = 0; j < row; ++j) {
				for (ll k = 0; k < col; ++k) {
					a[j][i] = ((a[j][i] + other[k][i] * data[j][k]) % mod + mod) % mod;
				}

			}
		}
		return a;
	}

	Matrix q_pow(ll n) {
		Matrix x = *this;
		Matrix res = Matrix::eye(x.shape());
		while (n) {
			if (n & 1)
				res = res * x;
			x = x * x;
			n >>= 1;
		}
		return res;
	}

	pair<size_t, size_t> shape()const {
		return { data.size(),data[0].size() };
	}

	static Matrix eye(const pair<size_t, size_t>& shape, ll initVal = 1) {
		if (shape.first != shape.second) {
			throw runtime_error("col should equal to row");
		}
		Matrix a(shape);
		for (size_t i = 0; i < shape.first; ++i) {
			a[i][i] = initVal;
		}
		return a;
	}

	friend ostream& operator<<(ostream& o, const Matrix& a) {
		o << "\n[";
		for (const auto& row : a.data) {
			for (auto& e : row) {
				o << e << '\t';
			}
			o << "\n";
		}
		return o << "]";
	}
};