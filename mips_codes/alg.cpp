#include "alg.h"
#include "base.h"
#include "gmp_alsh.h"

#include "basis.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <cstring>
#include <chrono>

void alshsForMips(std::vector<resOutput>& res_all, int algName, std::string dataSetName, Preprocess& prep, float c_, std::vector<int> ks, int L_, int K_,int m_, int M_, float S0, float W)
{
	enum algs { GMP_ALSH, H2_ALSH, Simple_LSH, Range_LSH, Sign_ALSH, RPT, L2_ALSH, LSCAN };
	resOutput res;
	float c0 = 2.0f;

	std::string file2 = "Undefined";
	HashBase* myslsh = NULL;
	Parameter* param = NULL;
	Partition* parti = NULL;

	std::cout << std::endl;
	switch (algName) {
		case GMP_ALSH: {
			std::cout << "Using GMP for " << dataSetName << std::endl;
			param  = new Parameter(prep, L_, K_, 1);
			//parti = new Partition(prep.data.N / 4096 + 1, prep);
			parti = new Partition(c_, prep);
			myslsh = new gmp_alsh::Hash(prep, (Parameter&)*param, file2, (Partition&)*parti, "MyfunctionXTheta.data");
			break;
		}
		default: {
			param = new Parameter(prep, L_, K_, 1);
			break;
		}
	}

	for (auto& k_ : ks) {
		lsh::timer timer;
		std::cout << std::endl << "RUNNING QUERY ...(k=" << k_ << ")" << std::endl;

		int Qnum = 200;
		lsh::progress_display pd(Qnum);
		Performance perform;

		QueryBase* query = NULL;
		for (int j = 0; j < Qnum; j++)
		{
			switch (algName) {
			case GMP_ALSH: {
				using namespace gmp_alsh;
				query = new Query(j, c_, k_, (Hash&)*myslsh, prep);
				perform.update(query, prep);
				delete (gmp_alsh::Query*)query;
				break;
			}
			}
			++pd;
		}

		float mean_time = (float)perform.time_total / perform.num;
		std::cout << "AVG QUERY TIME:    " << mean_time * 1000 << "ms." << std::endl << std::endl;
		std::cout << "AVG RECALL:        " << ((float)perform.NN_num) / (perform.num * k_) << std::endl;
		std::cout << "AVG RATIO:         " << ((float)perform.ratio) / (perform.res_num) << std::endl;
		std::cout << "AVG COST:          " << ((float)perform.cost) / (perform.num * prep.data.N) << std::endl;


		time_t now = time(0);
		tm* ltm =localtime(&now);
		//tm* ltm = new tm[1];
		

		switch (algName) {
		case GMP_ALSH: {
			res.algName = "GMP";
			break;
		}
		}
			
		std::string query_result = ("result\\") + res.algName + ("_result.csv");
		std::ofstream os(query_result, std::ios_base::app);
		os.seekp(0, std::ios_base::end); 
		int tmp = (int)os.tellp();
		if (tmp == 0) {
			os << "Dataset,c,k,S,RATIO,RECALL,AVG_TIME,COST,DATE" << std::endl;
		}
		std::string dataset = dataSetName;
		//dataset.erase(dataset.end() - 5, dataset.end());
		os << dataset << ',' << c_ << ',' << k_ << ',' << param->S << ','
			<< ((float)perform.ratio) / (perform.res_num) << ','
			<< ((float)perform.NN_num) / (perform.num * k_) << ','
			<< mean_time * 1000 << ','
			<< ((float)perform.cost) / (perform.num * prep.data.N) << ','
			<< ltm->tm_mon + 1 << '-' << ltm->tm_mday << ' ' << ltm->tm_hour << ':' << ltm->tm_min
			<< std::endl;
		os.close();

		res.k = k_;
		res.L = param->L;
		res.K = param->K;
		res.time = mean_time * 1000;
		res.recall = ((float)perform.NN_num) / (perform.num * k_);
		res.ratio = ((float)perform.ratio) / (perform.res_num);
		res.cost = ((float)perform.cost) / (perform.num * prep.data.N);
		//delete[] ltm;

		res_all.push_back(res);
	}

	switch (algName) {
	case GMP_ALSH: {
		delete (gmp_alsh::Hash*)myslsh;
		break;
	}
	default:
		break;
	}
	delete param;
	delete parti;
	//return res;

}

void alshsForMips(std::vector<resOutput>& res_all, int algName, std::string dataSetName, std::string projectPath,Preprocess& prep, float c_, std::vector<int> ks, int L_, int K_,int m_, int M_, float S0, float W)
{
	enum algs { GMP_ALSH, H2_ALSH, Simple_LSH, Range_LSH, Sign_ALSH, RPT, L2_ALSH, LSCAN };
	resOutput res;
	float c0 = 2.0f;
	

	std::string file2 = "Undefined";
	HashBase* myslsh = NULL;
	Parameter* param = NULL;
	Partition* parti = NULL;

#ifdef _WIN32
	std::string myFuntable=projectPath+"\\func\\MyfunctionXTheta.data";
#else
	std::string myFuntable=projectPath+"/func/MyfunctionXTheta.data";
#endif

	std::cout << std::endl;
	switch (algName) {
		case GMP_ALSH: {
			std::cout << "Using GMP for " << dataSetName << std::endl;
			param  = new Parameter(prep, L_, K_, 1);
			//parti = new Partition(prep.data.N / 4096 + 1, prep);
			parti = new Partition(c_, prep);
			myslsh = new gmp_alsh::Hash(prep, (Parameter&)*param, file2, (Partition&)*parti, myFuntable);
			break;
		}
		default: {
			param = new Parameter(prep, L_, K_, 1);
			break;
		}
	}

	for (auto& k_ : ks) {
		lsh::timer timer;
		std::cout << std::endl << "RUNNING QUERY ...(k=" << k_ << ")" << std::endl;

		int Qnum = 200;
		lsh::progress_display pd(Qnum);
		Performance perform;

		QueryBase* query = NULL;
		for (int j = 0; j < Qnum; j++)
		{
			switch (algName) {
			case GMP_ALSH: {
				using namespace gmp_alsh;
				query = new Query(j, c_, k_, (Hash&)*myslsh, prep);
				perform.update(query, prep);
				delete (gmp_alsh::Query*)query;
				break;
			}
			}
			++pd;
		}

		float mean_time = (float)perform.time_total / perform.num;
		std::cout << "AVG QUERY TIME:    " << mean_time * 1000 << "ms." << std::endl << std::endl;
		std::cout << "AVG RECALL:        " << ((float)perform.NN_num) / (perform.num * k_) << std::endl;
		std::cout << "AVG RATIO:         " << ((float)perform.ratio) / (perform.res_num) << std::endl;
		std::cout << "AVG COST:          " << ((float)perform.cost) / (perform.num * prep.data.N) << std::endl;


		time_t now = time(0);
		tm* ltm =localtime(&now);
		//tm* ltm = new tm[1];
		

		switch (algName) {
		case GMP_ALSH: {
			res.algName = "GMP";
			break;
		}
		}
		
#ifdef _WIN32
		std::string query_result = projectPath+("\\result\\") + res.algName + ("_result.csv");
#else
		std::string query_result = projectPath+("/result/") + res.algName + ("_result.csv");
#endif
		std::ofstream os(query_result, std::ios_base::app);
		os.seekp(0, std::ios_base::end);
		int tmp = (int)os.tellp();
		if (tmp == 0) {
			os << "Dataset,c,k,S,RATIO,RECALL,AVG_TIME,COST,DATE" << std::endl;
		}
		std::string dataset = dataSetName;
		os << dataset << ',' << c_ << ',' << k_ << ',' << param->S << ','
			<< ((float)perform.ratio) / (perform.res_num) << ','
			<< ((float)perform.NN_num) / (perform.num * k_) << ','
			<< mean_time * 1000 << ','
			<< ((float)perform.cost) / (perform.num * prep.data.N) << ','
			<< ltm->tm_mon + 1 << '-' << ltm->tm_mday << ' ' << ltm->tm_hour << ':' << ltm->tm_min
			<< std::endl;
		os.close();

		res.k = k_;
		res.L = param->L;
		res.K = param->K;
		res.time = mean_time * 1000;
		res.recall = ((float)perform.NN_num) / (perform.num * k_);
		res.ratio = ((float)perform.ratio) / (perform.res_num);
		res.cost = ((float)perform.cost) / (perform.num * prep.data.N);
		//delete[] ltm;

		res_all.push_back(res);
	}

	switch (algName) {
	case GMP_ALSH: {
		delete (gmp_alsh::Hash*)myslsh;
		break;
	}
	default:
		break;
	}
	delete param;
	delete parti;
	//return res;

}

void saveAndShow(float c, int k, std::string& dataset, std::vector<resOutput>& res)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);
	std::ofstream os("result\\Running_result_final.txt", std::ios_base::app);
	os.seekp(0, std::ios_base::end);

	std::stringstream ss;

	ss << "*****************************************************************************************************************\n"
		<< "The result of GMP for " << dataset << " is as follow: c=" << c << ", k=" << k
		<< "\n"
		<< "*****************************************************************************************************************\n";

	ss << std::setw(12) << "algName"
		<< std::setw(12) << "L"
		<< std::setw(12) << "K"
		<< std::setw(12) << "Time"
		<< std::setw(12) << "Recall"
		<< std::setw(12) << "Ratio"
		<< std::setw(12) << "Cost"
		<< std::endl
		<< std::endl;
	for (int i = 0; i < res.size(); ++i) {
		if (res[i].k == 50) {
			ss << std::setw(12) << res[i].algName
				<< std::setw(12) << res[i].L
				<< std::setw(12) << res[i].K
				<< std::setw(12) << res[i].time
				<< std::setw(12) << res[i].recall
				<< std::setw(12) << res[i].ratio
				<< std::setw(12) << res[i].cost
				<< std::endl;
		}
	}
	ss << "\n*****************************************************************************************************************\n"
		<< "                                                                                    "
		<< ltm->tm_mon + 1 << '-' << ltm->tm_mday << ' ' << ltm->tm_hour << ':' << ltm->tm_min
		<< "\n*****************************************************************************************************************\n\n\n";

	std::cout << ss.str();
	os << ss.str();
	os.close();
	//delete[] ltm;
}

void saveAndShow(float c, int k, std::string& dataset, std::string& projectPath,std::vector<resOutput>& res)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);

#ifdef _WIN32
	std::string outputFileName=projectPath+"\\result\\Running_result_final.txt";
#else
	std::string outputFileName=projectPath+"/result/Running_result_final.txt";
#endif

	std::ofstream os(outputFileName, std::ios_base::app);
	os.seekp(0, std::ios_base::end); 

	std::stringstream ss;

	ss << "*****************************************************************************************************************\n"
		<< "The result of GMP for " << dataset << " is as follow: c=" << c << ", k=" << k
		<< "\n"
		<< "*****************************************************************************************************************\n";

	ss << std::setw(12) << "algName"
		<< std::setw(12) << "L"
		<< std::setw(12) << "K"
		<< std::setw(12) << "Time"
		<< std::setw(12) << "Recall"
		<< std::setw(12) << "Ratio"
		<< std::setw(12) << "Cost"
		<< std::endl
		<< std::endl;
	for (int i = 0; i < res.size(); ++i) {
		if (res[i].k == 50) {
			ss << std::setw(12) << res[i].algName
				<< std::setw(12) << res[i].L
				<< std::setw(12) << res[i].K
				<< std::setw(12) << res[i].time
				<< std::setw(12) << res[i].recall
				<< std::setw(12) << res[i].ratio
				<< std::setw(12) << res[i].cost
				<< std::endl;
		}
	}
	ss << "\n*****************************************************************************************************************\n"
		<< "                                                                                    "
		<< ltm->tm_mon + 1 << '-' << ltm->tm_mday << ' ' << ltm->tm_hour << ':' << ltm->tm_min
		<< "\n*****************************************************************************************************************\n\n\n";

	std::cout << ss.str();
	os << ss.str();
	os.close();
	//delete[] ltm;
}

void generateExpeData_knn()
{
	std::string dataset = "yahoo_large";
	std::string* argvStr = new std::string[4];
	argvStr[1] = (dataset + ".data");
	argvStr[2] = (dataset + ".index");
	argvStr[3] = (dataset + ".ben");

	float c = 0.5f;
	float c0 = 2.0;
	int k = 50;
	std::vector<int> ks = { 1,10,20,30,40,50,60,70,80,90,100 };
	float S0 = 0.85f;
	int M = 2;

	int m, L, K;
	float W = 0.05f;

	std::string pos = "E:\\";
	std::string data_fold = pos + ("Dataset_for_c\\"), index_fold = pos + ("Dataset_for_c\\MIPS\\ALSHS\\");
	std::string data_fold1 = data_fold, data_fold2 = data_fold + ("MIPS\\ALSHS\\");
	Preprocess prep(data_fold1 + (argvStr[1]), data_fold2 + (argvStr[3]));

	std::vector<resOutput> res;
	m = std::max(prep.data.N / 20000, 20);
	L = 5;
	K = 2;

	setParam(0, dataset, m, L, K, W);
	alshsForMips(res, 0, argvStr[1], prep, c, ks, L, K);

	//c = 0.8;
	setParam(1, dataset, m, L, K, W);
	alshsForMips(res,1, argvStr[1], prep, c, ks);

	setParam(2, dataset, m, L, K, W);
	alshsForMips(res,2, argvStr[1], prep, c, ks, L, K);

	setParam(3, dataset, m, L, K, W);
	alshsForMips(res,3, argvStr[1], prep, c, ks, L, K, m);

	setParam(4, dataset, m, L, K, W);
	alshsForMips(res,4, argvStr[1], prep, c, ks, L, K, m, M, S0);

	setParam(5, dataset, m, L, K, W);
	alshsForMips(res,5, argvStr[1], prep, c, ks, L, K);

	setParam(6, dataset, m, L, K, W);
	alshsForMips(res,6, argvStr[1], prep, c, ks, L, K, m, M, S0, W);

	alshsForMips(res, 7, argvStr[1], prep, c, ks, -1, -1);

	saveAndShow(c, k, dataset, res);
	std::string query_result = "E:\\TexStudio\\MIPS\\illustration_of_papers_V2-master\\data\\0716\\knn\\" + dataset + "_knn.csv";
	std::ofstream os(query_result, std::ios_base::out);
	for (int i = 0; i < ks.size(); ++i) {
		for (int j = 0; j < 8; ++j) {
			int id = j * ks.size() + i;
			if (res[id].k != ks[i]) {
				std::cout << "Error!\n";
			}
			os << res[id].ratio << ',' << res[id].recall << ',' << res[id].time << ',';
		}
		os << std::endl;
	}
	os.close();


	delete[]argvStr;
	return ;
}

void make_Ls(std::vector<int>& Ls, int L) {
	Ls.clear();
	int n = 15;

}

void generateExpeData_rt()
{
	std::string dataset = "yahoo_large";
	std::string* argvStr = new std::string[4];
	argvStr[1] = (dataset + ".data");
	argvStr[2] = (dataset + ".index");
	argvStr[3] = (dataset + ".ben");

	float c = 0.5f;
	float c0 = 2.0;
	int k = 50;
	std::vector<int> ks = { 50 };
	float S0 = 0.75f;
	int M = 2;

	int m, L, K;
	float W = 0.05f;

	std::string pos = "E:\\";
	std::string data_fold = pos + ("Dataset_for_c\\"), index_fold = pos + ("Dataset_for_c\\MIPS\\ALSHS\\");
	std::string data_fold1 = data_fold, data_fold2 = data_fold + ("MIPS\\ALSHS\\");
	Preprocess prep(data_fold1 + (argvStr[1]), data_fold2 + (argvStr[3]));

	std::vector<resOutput> res;
	m = std::max(prep.data.N / 20000, 20);
	L = 5;
	K = 2;
	std::vector<int> Ls;

	setParam(0, dataset, m, L, K, W);
	Ls = { 1,2,3,4,5,6,7,8,9,10,12,14,16,18,20 };
	std::vector<float> cs0 = { 0.2,0.5,0.8,0.85,0.9 };
	//for (int L_ : Ls) {
	//	alshsForMips(res, 0, argvStr[1], prep, 0.5f, ks, L_, K);
	//	
	//}
	for (int i = 0; i < 150;++i) {
		alshsForMips(res, 0, argvStr[1], prep, 0.5f, ks, 5, K);

	}

	alshsForMips(res, 0, argvStr[1], prep, 0.5f, ks, 1, K);

	delete[]argvStr;
	return;
}

//alg=:
//0:GMP_ALSH
//1:H2_ALSH
//2:Simple_LSH
//3:Range_LSH
//4:Sign_ALSH  
//5:RPT
//6:L2_ALSH
void setParam(int alg, std::string dataset, int& m, int& L, int& K, float& W)
{  
	enum algs { GMP_ALSH, H2_ALSH, Simple_LSH, Range_LSH, Sign_ALSH, RPT, L2_ALSH };
	if (dataset == "tiny") {
		switch (alg) {
		case GMP_ALSH: {
			L = 10;
			K = 12;
			break;
		}
		case H2_ALSH: {
			break;
		}
		case Simple_LSH: {
			L = 8;
			K = 13;
			break;
		}
		case Range_LSH: {
			m = 1000;
			L = 8;
			K = 12;
			break;
		}
		case Sign_ALSH: {
			L = 5;
			K = 12;
			break;
		}
		case RPT: {
			L = 5;
			K = 12;
			break;
		}
		case L2_ALSH: {
			L = 5;
			K = 8;
			W = 2.0f;
			break;
		}
		default:
			break;
		}

	}
	else if (dataset == "gist") {
		switch (alg)  {
		case GMP_ALSH: {
			L = 5;
			K = 12;
			break;
		}
		case H2_ALSH: {
			break;
		}
		case Simple_LSH: {
			L = 20;
			K = 14;
			break;
		}
		case Range_LSH: {
			m = 200;
			L = 20;
			K = 12;
			break;
		}
		case Sign_ALSH: {
			L = 50;
			K = 16;
			break;
		}
		case RPT: {
			L = 50;
			K = 16;
			break;
		}
		case L2_ALSH: {
			L = 20;
			K = 6;
			W = 1.0f;
			break;
		}
		default:
			break;
		}
		
	}
	else if (dataset == "NUS") {
		switch (alg) {
		case GMP_ALSH: {
			L = 5;
			K = 12;
			break;
		}
		case H2_ALSH: {
			break;
		}
		case Simple_LSH: {
			L = 20;
			K = 6;
			break;
		}
		case Range_LSH: {
			m = 50;
			L = 20;
			K = 6;
			break;
		}
		case Sign_ALSH: {
			L = 20;
			K = 6;
			break;
		}
		case RPT: {
			L = 20;
			K = 6;
			break;
		}
		case L2_ALSH: {
			L = 20;
			K = 5;
			W = 1.0f;
			break;
		}
		default:
			break;
		}
	}
	else if (dataset == "yahoo_large") {
		switch (alg) {
		case GMP_ALSH: {
			L = 5;
			K = 12;
			break;
		}
		case H2_ALSH: {
			break;
		}
		case Simple_LSH: {
			L = 50;
			K = 9;
			break;
		}
		case Range_LSH: {
			m = 40;
			L = 50;
			K = 5;
			break;
		}
		case Sign_ALSH: {
			L = 30;
			K = 8;
			break;
		}
		case RPT: {
			L = 20;
			K = 9;
			break;
		}
		case L2_ALSH: {
			L = 50;
			K = 6;
			W = 1.5f;
			break;
		}
		default:
			break;
		}
	}
	else if (dataset == "MovieLens") {
		switch (alg) {
		case GMP_ALSH: {
			L = 5;
			K = 12;
			break;
		}
		case H2_ALSH: {
			break;
		}
		case Simple_LSH: {
			L = 200;
			K = 7;
			break;
		}
		case Range_LSH: {
			m = 40;
			L = 50;
			K = 5;
			break;
		}
		case Sign_ALSH: {
			L = 50;
			K = 5;
			break;
		}
		case RPT: {
			L = 50;
			K = 5;
			break;
		}
		case L2_ALSH: {
			L = 50;
			K = 6;
			W = 2.0f;
			break;
		}
		default:
			break;
		}
	}
	else if (dataset == "trevi") {
		switch (alg) {
		case GMP_ALSH: {
			L = 5;
			K = 12;
			break;
		}
		case H2_ALSH: {
			break;
		}
		case Simple_LSH: {
			L = 10;
			K = 12;
			break;
		}
		case Range_LSH: {
			m = 40;
			L = 20;
			K = 6;
			break;
		}
		case Sign_ALSH: {
			L = 10;
			K = 12;
			break;
		}
		case RPT: {
			L = 10;
			K = 12;
			break;
		}
		case L2_ALSH: {
			L = 10;
			K = 5;
			W = 1.0f;
			break;
		}
		default:
			break;
		}
	}
	else if (dataset == "audio") {
		switch (alg) {
		case GMP_ALSH: {
			L = 7;
			K = 12;
			break;
		}
		case H2_ALSH: {
			break;
		}
		case Simple_LSH: {
			L = 25;
			K = 5;
			break;
		}
		case Range_LSH: {
			m = 40;
			L = 25;
			K = 5;
			break;
		}
		case Sign_ALSH: {
			L = 50;
			K = 6;
			break;
		}
		case RPT: {
			L = 30;
			K = 6;
			break;
		}
		case L2_ALSH: {
			L = 40;
			K = 5;
			W = 1.5f;
			break;
		}
		default:
			break;
		}
	}
	else if (dataset == "netflix") {
		switch (alg) {
		case GMP_ALSH: {
			L = 5;
			K = 12;
			break;
		}
		case H2_ALSH: {
			break;
		}
		case Simple_LSH: {
			L = 20;
			K = 6;
			break;
		}
		case Range_LSH: {
			m = 40;
			L = 30;
			K = 5;
			break;
		}
		case Sign_ALSH: {
			L = 20;
			K = 6;
			break;
		}
		case RPT: {
			L = 20;
			K = 6;
			break;
		}
		case L2_ALSH: {
			L = 20;
			K = 5;
			W = 2.0f;
			break;
		}
		default:
			break;
		}
	}
	else if (dataset == "mnist") {
		switch (alg) {
		case GMP_ALSH: {
			L = 5;
			K = 12;
			break;
		}
		case H2_ALSH: {
			break;
		}
		case Simple_LSH: {
			L = 20;
			K = 7;
			break;
		}
		case Range_LSH: {
			m = 40;
			L = 20;
			K = 8;
			break;
		}
		case Sign_ALSH: {
			L = 10;
			K = 8;
			break;
		}
		case RPT: {
			L = 10;
			K = 7;
			break;
		}
		case L2_ALSH: {
			L = 50;
			K = 5;
			W = 1.2f;
			break;
		}
		default:
			break;
		}
	}
	else if (dataset == "cifar") {
	switch (alg) {
	case GMP_ALSH: {
		L = 5;
		K = 9;
		break;
	}
	case H2_ALSH: {
		break;
	}
	case Simple_LSH: {
		L = 10;
		K = 9;
		break;
	}
	case Range_LSH: {
		m = 40;
		L = 10;
		K = 8;
		break;
	}
	case Sign_ALSH: {
		L = 10;
		K = 9;
		break;
	}
	case RPT: {
		L = 10;
		K = 9;
		break;
	}
	case L2_ALSH: {
		L = 20;
		K = 5;
		W = 1.0f;
		break;
	}
	default:
		break;
	}
	}
}

void expe_k()
{
	std::string dataset = "mnist";
	std::string* argvStr = new std::string[4];
	argvStr[1] = (dataset + ".data");
	argvStr[2] = (dataset + ".index");
	argvStr[3] = (dataset + ".ben");

	float c = 0.8f;
	float c0 = 2.0;
	int k = 50;
	float S0 = 0.85f;
	int M = 2;

	int m, L, K;
	float W = 1.0f;

	std::cout << "Using ALSHS for " << argvStr[1] << std::endl;
	std::string pos = "E:\\";
	std::string data_fold = pos + ("Dataset_for_c\\"), index_fold = pos + ("Dataset_for_c\\MIPS\\ALSHS\\");
	std::string data_fold1 = data_fold, data_fold2 = data_fold + ("MIPS\\ALSHS\\");
	Preprocess prep(data_fold1 + (argvStr[1]), data_fold2 + (argvStr[3]));

	std::vector<resOutput> res;
	m = std::max(prep.data.N / 20000, 20);
	L = 5;
	K = 12;

	//setParam(0, dataset, m, L, K, W);
	//Alg1_mfalsh(argvStr[1], argvStr[2], argvStr[3], c, m, k, L, K, prep);

	//c = 0.8;
	//setParam(1, dataset, m, L, K, W);
	//Alg1_h2alsh(argvStr, c, c0, k, prep);

	//setParam(2, dataset, m, L, K, W);
	//Alg1_simplelsh(argvStr, c, k, L, K, prep);

	//setParam(3, dataset, m, L, K, W);
	//Alg1_rangelsh(argvStr[1], argvStr[2], argvStr[3], c, m, k, L, K, prep);

	//setParam(4, dataset, m, L, K, W);
	//Alg1_signalsh(argvStr[1], argvStr[2], argvStr[3], c, m, k, L, K, S0, prep);

	//setParam(5, dataset, m, L, K, W);
	//Alg1_rpt(argvStr[1], argvStr[2], argvStr[3], c, m, k, L, K, prep);

	//setParam(6, dataset, m, L, K, W);
	//Alg1_l2alsh(argvStr, c, m, k, L, K, S0, W, prep);

	//Alg1_lscan(argvStr[1], argvStr[2], argvStr[3], c, m, k, L, K, res);

	//saveAndShow(c, k, dataset, res);

	delete[]argvStr;
	return ;
}