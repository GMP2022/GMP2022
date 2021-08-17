#include "base.h"
#include "basis.h"
#include <fstream>
#include <assert.h>
#include <random>
#include <iostream>
#include <fstream>
#include <map>
#include <ctime>
#include <sstream>
#include <numeric>
#include <iomanip>

void HashBase::reset(Preprocess& prep)
{
	std::cout << std::endl << "START HASHING..." << std::endl << std::endl;
	lsh::timer timer;

	std::cout << "SETTING HASH PARAMETER..." << std::endl;
	timer.restart();
	SetHash();
	std::cout << "SETTING TIME: " << timer.elapsed() << "s." << std::endl << std::endl;


	std::cout << "COMPUTING HASH..." << std::endl;
	timer.restart();
	GenerateDataExpend(prep);
	GetHash(prep);
	std::cout << "COMPUTING TIME: " << timer.elapsed() << "s." << std::endl << std::endl;

	std::cout << "BUILDING INDEX..." << std::endl;
	std::cout << "THERE ARE " << L << " " << K << "-D HASH TABLES." << std::endl;
	timer.restart();
	GetTables(prep);
	std::cout << "BUILDING TIME: " << timer.elapsed() << "s." << std::endl << std::endl;
}

void HashBase::SetHash()
{
	hashpar.rndAs1 = new float* [S];
	hashpar.rndAs2 = new float* [S];


	for (int i = 0; i < S; i++) {
		hashpar.rndAs1[i] = new float[dim];
		hashpar.rndAs2[i] = new float[M];
	}

	std::mt19937 rng(int(std::time(0)));
	std::normal_distribution<float> nd;//nd是一个norm随机数生成器，mu=0，sigma=1
	for (int j = 0; j < S; j++)
	{
		for (int i = 0; i < dim; i++)
		{
			hashpar.rndAs1[j][i] = (nd(rng));
		}
		for (int i = 0; i < M; i++)
		{
			hashpar.rndAs2[j][i] = (nd(rng));
		}
	}
}

void HashBase::GetHash(Preprocess& prep)
{
	hashval = new float* [N];
	for (int j = 0; j < N; j++) {
		hashval[j] = new float[S];
		for (int i = 0; i < S; i++) {
			hashval[j][i] = cal_inner_product(prep.data.val[j], hashpar.rndAs1[i], dim) + cal_inner_product(dataExpend[j], hashpar.rndAs2[i], M);
		}
	}
	clear_2d_array(dataExpend, N);
}