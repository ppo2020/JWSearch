//
// Created by Yaoshu Wang on 13/4/17.
//

#ifndef JWSEARCH_SEARCH_H
#define JWSEARCH_SEARCH_H

#include <vector>
#include <string>

#include "query.h"
#include "data.h"
#include "index.h"

void jw_search(Query* query, Index* index, vector<uint>& dup_cand, vector<uint>& cand, double tau);

void remove_dup_cand(vector<uint>& dup_cand, vector<uint>& cand);

#endif //JWSEARCH_SEARCH_H
