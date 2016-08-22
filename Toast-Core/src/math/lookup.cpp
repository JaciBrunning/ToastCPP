#include "toast/math/lookup.hpp"

#include <stdlib.h>
#include <math.h>

using namespace Toast::Math::Lookup;

BinaryLookup::BinaryLookup(int size) : _size(size) {
	_store = (float *)malloc(size * 2 * sizeof(float));
}

BinaryLookup::~BinaryLookup() {
	free(_store);
}

int BinaryLookup::size() {
	return _size;
}

float BinaryLookup::get_first(int id) {
	if (id >= _size) return NAN;
	return _store[id * 2];
}

float BinaryLookup::get_second(int id) {
	if (id >= _size) return NAN;
	return _store[id * 2 + 1];
}

void BinaryLookup::insert(int id, float first, float second) {
	if (id < _size) {
		_store[id * 2]		= first;
		_store[id * 2 + 1]	= second;
	}
}

void BinaryLookup::find_between(float value, int *low, int *high, bool inverse) {
	// Simple Binary Search
	int lower = 0, upper = _size - 1, mid = (lower + upper) / 2;
	while (lower <= upper && upper < _size && lower < _size) {
		if (upper - lower == 0 || upper - lower == 1) break;
		float f = _store[mid * 2 + (inverse ? 1 : 0)];

		if (f == value) {
			lower = mid;
			upper = mid;
			break;
		} else if (f < value) {
			lower = mid;
		} else if (f > value) {
			upper = mid;
		}
		mid = (lower + upper) / 2;
	}
	*low = lower;
	*high = upper;
}

float BinaryLookup::interpolate(float first) {
	int low, high;
	BinaryLookup::find_between(first, &low, &high);
	float low_x = _store[low * 2];
	float high_x = _store[high * 2];
	float low_y = _store[low * 2 + 1];
	float high_y = _store[high * 2 + 1];
	float grad = (high_y - low_y) / (high_x - low_x);
	return grad * (first - low_x) + low_y;
}

float BinaryLookup::interpolate_inverse(float second) {
	int low, high;
	BinaryLookup::find_between(second, &low, &high, true);
	float low_x = _store[low * 2];
	float high_x = _store[high * 2];
	float low_y = _store[low * 2 + 1];
	float high_y = _store[high * 2 + 1];
	float grad = (high_y - low_y) / (high_x - low_x);
	return (second - low_y) / grad + low_x;
}