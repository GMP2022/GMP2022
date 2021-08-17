#pragma once
#include <numeric>
#include <float.h>
struct Data
{
	// Dimension of data
	int dim;
	// Number of data
	int N;
	// Data matrix
	float** val;
};

struct Ben
{
	int N;
	int num;
	int** indice;
	float** innerproduct;
};

struct HashParam
{
	// the value of a in S hash functions
	float** rndAs1;
	// the value of a in S hash functions
	float** rndAs2;
};

struct Res//the result of knns
{
	int id;
	float inp;
	bool operator < (const Res& rhs) const {
		return inp > rhs.inp;
	}
	Res(int id_, float inp_) :id(id_), inp(inp_) {}
	Res() :id(-1), inp(-FLT_MAX) {}
};
