#ifndef SRC_SV_SORT_H_
#define SRC_SV_SORT_H_

#include <iostream>
#include <string.h>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>

#include "events.h"

using namespace std;


typedef struct{
	string chrname, chrname2, altseq, info;
	int64_t startPos, endPos, startPos2:56, valid_flag:8, endPos2:56, sv_type:8;
	int32_t sv_len;
}SV_item;


SV_item *constructSVItem(string &line);
vector<SV_item*> loadDataBED(string &filename);
bool isComma(string &seq);
bool isSeq(string &seq);
bool isBase(const char ch);
vector<string> getSVType(vector<string> &str_vec);
SV_item *allocateSVItem(string &chrname, size_t startPos, size_t endPos, string &chrname2, size_t startPos2, size_t endPos2, string &sv_type_str, size_t sv_len, string &altseq, string &line);
vector<SV_item*> loadDataVcf(string &filename);
void destroyData(vector<SV_item*> &sv_vec);
set<string> getChrnames(vector<SV_item*> &dataset);
vector<string> sortChrnames(set<string> &chrname_set);
bool IsSameChrname(string &chrname1, string &chrname2);
vector<SV_item*> getItemsByChr(string &chrname, vector<SV_item*> &dataset);
vector<vector<SV_item*>> constructSubsetByChrOp(vector<SV_item*> &sv_vec, vector<string> &chrname_vec);
vector<vector<SV_item*>> constructSubsetByChr(vector<SV_item*> &sv_vec);
bool sortFunSameChr(const SV_item *item1, const SV_item *item2);
void sortSVitem(vector<vector<SV_item*>> &subsets);
void sortSubset(vector<SV_item*> &sv_vec);
void rmDupSVitem(vector<vector<SV_item*>> &subsets, double size_ratio_thres, double identity_thres);
void rmDupSVitemSubset(vector<SV_item*> &sv_vec, double size_ratio_thres, double identity_thres);


#endif /* SRC_SV_SORT_H_ */
