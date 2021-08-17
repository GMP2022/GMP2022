#pragma once

#ifndef   MY_H_FILE       //���û�ж��������  
#define   MY_H_FILE       //���������  
#include <set>
#include <algorithm>
#include "Preprocess.h"
#include <iterator>
#include <iostream>
#include "base.h"

class Performance
{
public:
	//cost
	int cost;
	//
	std::vector<int> costs;
	// times of query
	int num;
	//
	float time_total;
	//
	float time_hash;
	//
	float time_sift;
	//
	float time_verify;
	//number of exact NN
	int NN_num;
	//number of results
	int res_num;
	//
	float ratio;
public:
	Performance();
	//ccccc
	void update(QueryBase* query, Preprocess& prep);
	~Performance();
};

#endif 

