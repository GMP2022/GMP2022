/*
 * @Descripttion: 
 * @version: 
 * @Author: Wan Jingyi
 * @Date: 2021-04-16 18:47:27
 * @LastEditors: Wan Jingyi
 * @LastEditTime: 2021-04-17 10:13:58
 */
#pragma once
#include <string>
#include "Preprocess.h"
#include "performance.h"
struct resOutput
{
	std::string algName;
	int k;
	int L;
	int K;
	float time;
	float recall;
	float ratio;
	float cost;
};

//algName=:
//0:GMP_ALSH
//1:H2_ALSH
//2:Simple_LSH
//3:Range_LSH
//4:Sign_ALSH
//5:RPT
//6:L2_ALSH
void alshsForMips(std::vector<resOutput>& res_all, int algName, std::string dataSetName, Preprocess& prep, float c_, std::vector<int> ks, int L_ = 0, int K_ = 0, int m_ = 0, int M = 2, float S0 = 0.85f, float W = 2.0f);
void alshsForMips(std::vector<resOutput>& res_all, int algName, std::string dataSetName, std::string projectPath, Preprocess& prep, float c_, std::vector<int> ks, int L_ = 0, int K_ = 0, int m_ = 0, int M = 2, float S0 = 0.85f, float W = 2.0f);
void generateExpeData_knn();
void generateExpeData_rt();
//
void saveAndShow(float c, int k, std::string& dataset, std::vector<resOutput>& res);
void saveAndShow(float c, int k, std::string& dataset, std::string& projectPath,std::vector<resOutput>& res);

//alg=:
//0:GMP_ALSH
//1:H2_ALSH
//2:Simple_LSH
//3:Range_LSH
//4:Sign_ALSH
//5:RPT
//6:L2_ALSH
void setParam(int alg, std::string dataset, int& m, int& L, int& K, float& W);

void expe_k();
