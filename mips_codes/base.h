#pragma once
#include "StructType.h"
#include "Preprocess.h"
#include <cmath>
#include <assert.h>
#include <vector>
#include <queue>

class HashBase
{
public:
	int N;
	int dim;
	// Number of hash functions
	int S;
	//#L Tables; 
	int L;
	// Dimension of the hash table
	int K;

	int M;

	float** hashval;
	float** dataExpend;
	HashParam hashpar;
public:
	void SetHash();
	void GetHash(Preprocess& prep);
	void reset(Preprocess& prep);
	virtual void GenerateDataExpend(Preprocess& prep) = 0;
	virtual void GetTables(Preprocess& prep) = 0;
};

class QueryBase
{
protected:
	// the parameter "c" in "c-ANN"
	float c;

	float* query_point;
	// the hash value of query point
	float* hashval;
	//
	float** mydata;
	int dim;

public:
	// k-NN
	int k;
	//Indice of query point in dataset. Be equal to -1 if the query point isn't in the dataset.
	int flag;
	//
	float norm;
	//
	int cost = 0;
	//
	float time_total = 0;
	// query result:<indice of ANN,distance of ANN>
	std::vector<Res> res;

	virtual void cal_hash(HashBase& hash, Preprocess& prep)=0;
};