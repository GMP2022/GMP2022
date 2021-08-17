/*
 * @Descripttion:
 * @version:
 * @Author: Zhao Xi
 * @Date: 2021-04-16 18:47:27
 * @LastEditors: Wan Jingyi
 * @LastEditTime: 2021-04-18 15:34:31
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <chrono>
#include <string.h>
#include "alg.h"
#include "Preprocess.h"

//#include "vld.h"

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		std::cerr << "Usage: ./mips.exe datasetName project_path k c" << std::endl;
		return -1;
	}

	std::cout<<"argc="<<argc<<std::endl;

	std::string datasetName=argv[1];
	std::string projectPath=argv[2];
	int k = atoi(argv[3]);
	float c = atof(argv[4]);

	std::string sourceDataFileName(projectPath);
	std::string groundTruthFileName(projectPath);

#ifdef _WIN32
	while(projectPath[projectPath.size()-1]=='\\') projectPath=projectPath.erase(projectPath.length()-1);
	sourceDataFileName=projectPath+"\\data\\"+datasetName+".data";
	groundTruthFileName=projectPath+"\\groundtruth\\"+datasetName+".ben";
#else
	if(projectPath[projectPath.length()-1]=='/') projectPath=projectPath.erase(projectPath.length()-1);
	sourceDataFileName=projectPath+"/data/"+datasetName+".data";
	groundTruthFileName=projectPath+"/groundtruth/"+datasetName+".ben";
#endif

	//to be deleted
	std::cout << "sourceDataFileName=" << sourceDataFileName << std::endl;
	std::cout << "groundTruthFileName=" << groundTruthFileName << std::endl;
	std::cout << "k=" << k << " c=" << c << std::endl;

	float c0 = 2.0f;

	std::vector<int> ks = { 1,10,20,30,40,50,60,70,80,90,100 };
	ks = { 50 };
	float S0 = 0.75f;
	int M = 2;

	int m, L, K;
	float W = 0.05f;


	Preprocess prep(sourceDataFileName, groundTruthFileName);

	std::vector<resOutput> res;
	m = std::max(prep.data.N / 20000, 20);
	L = 0;
	K = 0;

	c = 0.8;
	setParam(0, datasetName, m, L, K, W);
	alshsForMips(res,0, datasetName, projectPath,prep, c, ks, L, K);

	saveAndShow(c, k, datasetName, projectPath, res);

	return 0;
}

