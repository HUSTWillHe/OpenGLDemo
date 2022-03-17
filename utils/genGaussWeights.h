#ifndef GEN_GAUSS_WEIGHTS_H
#define GEN_GAUSS_WEIGHTS_H

#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
#include <algorithm>

std::vector<float> genGaussWeight(int radius) {
	if (radius == 0) {
		return std::vector<float>{1};
	}
	std::vector<float> rst;
	float sigma = (float)radius / 3.0;
	float sigmaDouble = 2 * sigma * sigma;
	float sum = 0;

	for (int i = 0; i < radius + 1; i++) {
		for (int j = 0; j < radius + 1; j++) {
			float gaussWeight = (i * i + j * j > radius * radius) ? 0.0f : exp(-(i * i + j * j) / sigmaDouble);
			if (i == 0 && j == 0) {
				sum += gaussWeight;
			} else if (i == 0 || j == 0) {
				sum += gaussWeight * 2;
			} else {
				sum += gaussWeight * 4;
			}
			rst.push_back(gaussWeight);
		}
	}
	for (int i = 0; i < radius + 1; i++) {
		for (int j = 0; j < radius + 1; j++) {
			rst[i * (radius + 1) + j] /= sum;
		}
	}
	return rst;
}

#endif 