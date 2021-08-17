#include "gmp_alsh.h"
#include "Preprocess.h"
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

#define MAXSIZE 200000
#define pi 3.141592653
#define CANDIDATES 100
#define MINFLOAT -3.40282e+038

using namespace gmp_alsh;

Hash::Hash(Preprocess& prep_, Parameter& param_,
	const std::string& file, Partition& part_, const std::string& funtable)
	
{
	N = param_.N;
	dim = param_.dim;
	L = param_.L;
	K = param_.K;
	S = param_.S;
	M = 1;
	parti = &part_;
	load_funtable(funtable);
	reset(prep_);
}

void Hash::load_funtable(const std::string& file)
{
	std::ifstream is(file.c_str(), std::ios::binary);
	float header1[4] = { 0 };
	int header2[3] = { 0 };
	assert(sizeof header1 == 4 * 4);
	is.read((char*)header1, sizeof(header1));
	assert(sizeof header2 == 3 * 4);
	is.read((char*)header2, sizeof(header2));
	assert(header2[1] != 0);

	rows = header2[1];
	cols = header2[2];

	tmin = header1[0];
	tstep = (header1[1] - header1[0]) / rows;
	smin = header1[2];
	sstep = (header1[3] - header1[2]) / cols;

	float* array_ = new float[(size_t)header2[2] * header2[1]];
	is.read((char*)&array_[0], sizeof(float) * header2[2] * header2[1]);


	is.close();

	phi = new float* [(size_t)rows];
	for (int i = 0; i < rows; ++i) {
		phi[i] = new float[(size_t)cols];
		for (int j = 0; j < cols; ++j) {
			phi[i][j] = array_[j * rows + i];
		}
	}
	delete[] array_;
}

void Hash::GenerateDataExpend(Preprocess& prep)
{
	dataExpend = new float*[N];
	std::mt19937 rng(int(std::time(0)));
	std::uniform_real_distribution<float> ur(-1, 1);
	int count = 0;
	for (int j = 0; j < N; j++)
	{
		assert(parti->MaxLen[parti->chunks[j]] >= prep.SquareLen[j]);
		dataExpend[j] = new float[1];
		dataExpend[j][0] = sqrt(parti->MaxLen[parti->chunks[j]] - prep.SquareLen[j]);
		if (ur(rng) > 0) {
			dataExpend[j][0] *= -1;
			++count;
		}
		
	}
	std::cout << "RXT ratio: " << (double)count / N << std::endl;
}

void Hash::GetTables(Preprocess& prep)
{
	int i, j, k;

	int num_bucket = 1 << K;

	myIndexes = new std::vector<int> * *[parti->num_chunk];
	for (j = 0; j < parti->num_chunk; ++j) {
		myIndexes[j] = new std::vector<int> * [L];
		for (i = 0; i < L; ++i) {
			myIndexes[j][i] = new std::vector<int>[num_bucket];
		}
	}

	for (j = 0; j < L; j++) {
		for (i = 0; i < N; i++) {
			int start = j * K;
			int key = 0;
			for (k = 0; k < K; k++) {
				key = key << 1;
				if (this->hashval[i][ start + k] > 0) {
					++key;
				}
			}
			myIndexes[(size_t)parti->chunks[i]][j][key].push_back(i);
		}
	}
}

Hash::~Hash()
{
	printf("Destroy Index!\n");
	for (int j = 0; j < parti->num_chunk; ++j) {
		for (int i = 0; i < L; ++i) {
			for (int l = 0; l < (1 << K); ++l) {
				std::vector<int>().swap(myIndexes[j][i][l]);
			}
			delete[] myIndexes[j][i];
		}
		delete[] myIndexes[j];
	}
	delete[] myIndexes;

	clear_2d_array(hashval, N);
	clear_2d_array(hashpar.rndAs1, S);
	clear_2d_array(hashpar.rndAs2, S);
	clear_2d_array(phi, rows);
}

Query::Query(int id, float c_, int k_, Hash& hash, Preprocess& prep)
{
	flag = id;
	c = c_;
	k = k_;

	mydata = prep.data.val;
	dim = prep.data.dim;

	lsh::timer timer;

	timer.restart();
	cal_hash(hash, prep);
	time_hash = timer.elapsed();

	timer.restart();
	sift(hash, prep);
	time_sift = timer.elapsed();

	time_total = time_hash + time_sift;
}

void Query::cal_hash(HashBase& hash, Preprocess& prep)
{
	query_point = mydata[flag];
	norm = 0;
	for (int i = 0; i < dim; ++i) {
		norm += query_point[i] * query_point[i];
	}
	norm = sqrt(norm);

	hashval = new float[hash.S];
	for (int i = 0; i < hash.S; ++i) {
		hashval[i] = cal_inner_product(query_point, hash.hashpar.rndAs1[i], dim) / norm;
	}
	hash_pair hp0;
	this->weigh.resize(hash.L);
	total_score.resize(hash.L);
	for (int i = 0; i < hash.L; i++) {
		this->weigh[i].resize(hash.K);
		total_score[i] = 0;
		for (int j = 0; j < hash.K; j++) {
			hp0.val = abs(hashval[(size_t)(i * hash.K + j)]);

			hp0.bias = 1 << (hash.K - 1 - j);
			if (hashval[i * hash.K + j] > 0) {
				hp0.bias *= -1;
			}
			weigh[i][j] = hp0;
			total_score[i] += -hp0.val / 2;
		}
		sort(weigh[i].begin(), weigh[i].end());
	}
}

void Query::sift(Hash& hash, Preprocess& prep)
{
	lsh::timer timer;
	ProbingSequence = new indice_pair[(size_t)(hash.L * (size_t)(1 << hash.K) + 1)];
	SequenceLen = 0;

	indice_pair ip0, ip1;
	if (!global_min.empty()) {
		system("pause");
	}
	keys.resize(hash.L);
	for (int i = 0; i < hash.L; i++) {
		int key = 0;
		for (int j = 0; j < hash.K; j++) {
			key = key << 1;
			if (this->hashval[i * hash.K + j] > 0) {
				++key;
			}
		}
		keys[i] = key;
		ip0.key = key + weigh[i][0].bias;

		ip0.end = 0;
		ip0.table_id = i;
		ip0.score = weigh[i][0].val;
		global_min.push(ip0);
	}

	int UB = k + CANDIDATES;

	std::vector<bool> flag_(hash.N, false);
	std::priority_queue<Res> res_PQ;
	Res res_pair;

	int num_cand = 0;
	inp_LB = MINFLOAT;
	int num_res = 0;
	costs.resize(hash.parti->num_chunk);


	for (int t = hash.parti->num_chunk - 1; t >= 0; t--)
	{
		if (sqrt(hash.parti->MaxLen[t]) * norm < inp_LB/c) {
			break;
		}

		if (hash.parti->nums[t] < 4 * CANDIDATES) {
			int num_cand = hash.parti->EachParti[t].size();
			for (int j = 0; j < num_cand; j++)
			{
				res_pair.id = hash.parti->EachParti[t][j];

				res_pair.inp = 0;
				for (int i = 0; i < dim; ++i) {
					res_pair.inp += mydata[res_pair.id][i] * query_point[i];
				}

				res_PQ.push(res_pair);
				num_res++;
				costs[t]++;
				if (num_res > UB)
				{
					res_PQ.pop();
					num_res--;
				}
			}
			if (num_res == UB && inp_LB < res_PQ.top().inp)
				inp_LB = res_PQ.top().inp;
			continue;
		}
		chunks = t;
		//Hash* hash_ = &hash;

		knn(res_PQ, hash, prep, hash.myIndexes[t], flag_, num_res);
		if (num_res == UB && inp_LB < res_PQ.top().inp)
			inp_LB = res_PQ.top().inp;
	}

	res.clear();

	int len = res_PQ.size();
	res.resize(len);
	int rr = len - 1;
	while (rr >= 0)
	{
		res[rr] = res_PQ.top();
		res_PQ.pop();
		rr--;
	}
	for (int i = 0; i < hash.parti->num_chunk; i++)
	{
		cost += costs[i];
	}
	time_verify = timer.elapsed();
}

void Query::knn(std::priority_queue<Res>& res_PQ,
	Hash& hash, Preprocess& prep,
	std::vector<int>** table,
	std::vector<bool>& flag_, int& num_res)
{

	Res res_pair;
	int UB = k + CANDIDATES;

	float ratio = c * (float)chunks / (hash.parti->num_chunk + 1);
	int MaxCost = (int)(ratio * hash.parti->nums[chunks]) * 1 + k;
	int cnt = 0;
	float inpK = -1.0f;
	float Max_inp = this->norm * sqrt(hash.parti->MaxLen[chunks]);

	for (int i = 0; i < hash.L; i++) {
		for (auto& x : table[i][keys[i]]) {
			if (flag_[x] == false)
			{
				res_pair.id = x;

				res_pair.inp = 0;
				for (int i = 0; i < dim; ++i) {
					res_pair.inp += mydata[x][i] * query_point[i];
				}

				res_PQ.push(res_pair);
				num_res++;
				cnt++;
				costs[chunks]++;
				if (num_res > this->k)
				{
					res_PQ.pop();
					num_res--;
					inpK = res_PQ.top().inp;
				}
				flag_[x] = true;
			}
		}
	}

	/*if (cnt >= MaxCost)
		return;*/

	float Max_score = sqrt(2.0f / pi);
	float coeff = (pi / Max_score);
	

	int probingNum = 0;

	indice_pair ip0, ip1;

	int len = hash.K;
	float reduced_score, est_inp;

	int MaxNum = hash.parti->nums[chunks]
		* hash.L
		;
	float beta = 1.0f;

	//cnt = 2 * MaxNum;
	while (cnt < (int)(beta*MaxNum)
		&& (probingNum < SequenceLen||(!global_min.empty()))
		)//bug2020.5.26: STL�����ȶ���û�б߽��⣬��Ѫ
	{
		if (probingNum < SequenceLen) {
			ip1 = ProbingSequence[probingNum];
			++probingNum;
		}
		else {
			//printf("Now>>>>>>>> %d,%d", probingNum, SequenceLen);
			ip1 = global_min.top();
			ProbingSequence[SequenceLen++] = ip1;
			++probingNum;

			global_min.pop();
			if (ip1.end < len - 1) {
				this->shift(ip1, ip0);
				global_min.push(ip0);

				this->expand(ip1, ip0);
				global_min.push(ip0);
			}
		}
		if (table[ip1.table_id][ip1.key].empty()) {
			continue;
		}
		else {
			for (auto& x : table[ip1.table_id][ip1.key]) {
				cnt++;
				if (flag_[x] == false)
				{
					res_pair.id = x;
					res_pair.inp = 0;
					for (int i = 0; i < dim; ++i) {
						res_pair.inp += mydata[x][i] * query_point[i];
					}

					res_PQ.push(res_pair);
					num_res++;
					//cnt++;
					costs[chunks]++;
					if (num_res > this->k)
					{
						res_PQ.pop();
						num_res--;
						inpK = res_PQ.top().inp;
					}
					flag_[x] = true;
				}
			}
		}
		if (inpK > 0) {
			beta = 2 * pow(1 - varphi(ip1.score, acos(inpK / (Max_inp)), hash), hash.L);
			float theta = acos(std::min(0.9999f, inpK / (c * Max_inp)));
			//std::cout << theta << std::endl;
			float pr = varphi(ip1.score, theta, hash);
			//if ((int)(2 * pr * (hash.parti.nums[chunks] - cnt)//* hash.L)< 1) break;
			if (pow(1 - pr, hash.L) < 0.05) break;
			if (cnt > 0.15 * chunks / (float)hash.parti->num_chunk * hash.parti->nums[chunks] + k) break;
		}
			

		
		//if ((int)(2 * (1 - pr) * (hash.parti.nums[chunks] - cnt)) < 1) break;

	}//endwhile
	//std::cout << chunks << "," << beta << "," << flag << "," << ip1.score << "," << acos(inpK / (Max_inp)) << "," << cnt << "," << (int)(beta * MaxNum) << std::endl;
}

void Query::shift(indice_pair& ip0, indice_pair& res)
{
	res = ip0;
	++res.end;
	res.key += weigh[res.table_id][res.end].bias - weigh[res.table_id][(size_t)res.end - 1].bias;
	res.score = ip0.score + weigh[res.table_id][res.end].val - weigh[res.table_id][(size_t)res.end - 1].val;
}

void Query::expand(indice_pair& ip0, indice_pair& res)
{
	res = ip0;
	++res.end;
	res.key += weigh[res.table_id][res.end].bias;
	res.score = ip0.score + weigh[res.table_id][res.end].val;
}

float Query::varphi(float x, float theta, Hash& hash)
{
	int c0 = (int)((x - hash.smin) / hash.sstep);
	int r0 = (int)((theta - hash.tmin) / hash.tstep);
	//std::cout << "Test rc:" << r0 << "," << c0 << std::endl;
	return hash.phi[std::min(r0, hash.rows - 1)][std::min(c0, hash.cols - 1)];
}

Query::~Query()
{
	delete[] ProbingSequence;
	delete[] hashval;
}
