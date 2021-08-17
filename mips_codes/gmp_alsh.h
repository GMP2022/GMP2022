#pragma once
#include "StructType.h"
#include "Preprocess.h"
#include "base.h"
#include <cmath>
#include <assert.h>
#include <vector>
#include <queue>

namespace gmp_alsh
{
	class Hash:public HashBase
	{		
	public:

		//float** hashval;
		Partition* parti;
		//HashParam hashpar;
		std::vector<int>*** myIndexes;

		float tmin;
		float tstep;
		float smin;
		float sstep;
		int rows;
		int cols;
		float** phi;

		void load_funtable(const std::string& file);
	public:
		Hash(Preprocess& prep_, Parameter& param_, const std::string& file, Partition& part_, const std::string& funtable);
		void GenerateDataExpend(Preprocess& prep);
		void GetTables(Preprocess& prep);
		bool IsBuilt(const std::string& file);
		~Hash();
	};

	struct hash_pair
	{
		float val;
		int bias;

		bool operator < (const hash_pair& rhs) const {
			return val < rhs.val;
		}
	};

	struct indice_pair
	{
		int key;
		int end;
		int table_id;
		float score;
		bool operator < (const indice_pair& rhs) const {
			return score > rhs.score;
		}
	};

	class Query:public QueryBase
	{
	private:
		//which chunk is accessed
		int chunks;

		std::vector<std::vector<hash_pair>> weigh;
		std::vector<float> total_score;

		float** mydata;
		int dim;

		float inp_LB;
		// Set of points sifted
		//std::vector<int> candidate;

		std::vector<int> keys;

		void shift(indice_pair& ip0, indice_pair& res);
		void expand(indice_pair& ip0, indice_pair& res);

		std::priority_queue<indice_pair> global_min;
		indice_pair* ProbingSequence;
		int SequenceLen;

		float varphi(float x, float theta, Hash& hash);
	public:
		// Indice of query point in dataset. Be equal to -1 if the query point isn't in the dataset.
		//int flag;
		//cost of each partition
		std::vector<int> costs;
		//
		float time_hash = 0;
		//
		float time_sift = 0;

		float time_verify = 0;

		void cal_hash(HashBase& hash, Preprocess& prep);
		void sift(Hash& hash, Preprocess& prep);
		void knn(std::priority_queue<Res>& res_PQ,
			Hash& hash, Preprocess& prep,
			std::vector<int>** table,
			std::vector<bool>& flag, int& num_res);
	public:
		Query(int id, float c_, int k_, Hash& hash, Preprocess& prep);

		~Query();
	};
}


