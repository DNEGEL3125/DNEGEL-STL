#pragma once
#include<algorithm>
using std::is_sorted;
using std::swap;

template<typename T>
void qsort(T begin, T end) {
	size_t len = end - begin;
	if (len <= 1)
		return;
	if (is_sorted(begin, end)) return;
	auto leftPtr = begin + 1, rightPtr = end - 1;
	auto mid = begin;
	swap(*begin, *(begin + rand() % len));
	while (leftPtr < rightPtr) {
		while (leftPtr < rightPtr && *rightPtr >= *mid)
			--rightPtr;
		while (leftPtr < rightPtr && *leftPtr < *mid) {
			++leftPtr;
		}
		if (leftPtr != rightPtr) {
			swap(*leftPtr, *rightPtr);
		}
	}
	if (*mid > *leftPtr)
		swap(*mid, *leftPtr);
	qsort(begin, leftPtr);
	qsort(leftPtr, end);
}