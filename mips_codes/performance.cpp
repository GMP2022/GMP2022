#include "performance.h"

Performance::Performance()
{
	cost = 0;
	num = 0;
	time_total = 0;
	time_hash = 0;
	time_sift = 0;
	time_verify = 0;
	NN_num = 0;
	res_num = 0;
	ratio = 0;
};

void Performance::update(QueryBase* query, Preprocess& prep)
{
	num++;
	cost += query->cost;
	time_total += query->time_total;

	int num0 = query->res.size();
	if (num0 > query->k)
		num0 = query->k;
	res_num += num0;

	std::set<int> set1, set2;
	std::vector<int> set_intersection;
	set_intersection.clear();
	set1.clear();
	set2.clear();

	for (int j = 0; j < num0; j++)
	{
		float rate = query->res[j].inp / prep.benchmark.innerproduct[query->flag][j];

		//if (rate > 1.1 || rate < 0.001)
		//{
		//	std::cout << "An abnormol ratio appears in:" << query->flag << ',' << j << ','
		//		<< query->res[j].inp << ',' << prep.benchmark.innerproduct[query->flag][j] <<
		//		std::endl;
		//	system("pause");
		//}
		ratio += rate;

		set1.insert(query->res[j].id);
		set2.insert((int)prep.benchmark.indice[query->flag][j]);
	}
	std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),
		std::back_inserter(set_intersection));

	NN_num += set_intersection.size();
}

Performance::~Performance()
{

}
  
