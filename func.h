//
// Created by Yaoshu Wang on 13/4/17.
//

#ifndef JWSEARCH_FUNC_H
#define JWSEARCH_FUNC_H

#include <iostream>
#include <vector>
#include <string>

#include "data.h"
#include "index.h"
#include "search.h"
#include "query.h"
#include "verify.h"

using namespace std;

void jw_pigeonvariance(char* data_file, char* query_file, char* index_file, uint part_num,
                        unordered_map<string, uint>* score, unordered_map<string, uint>* cluster_char, vector<string>* SC, double tau);

#endif //JWSEARCH_FUNC_H
