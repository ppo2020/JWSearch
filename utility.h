//
// Created by Yaoshu Wang on 13/4/17.
//

#ifndef JWSEARCH_UTILITY_H
#define JWSEARCH_UTILITY_H


#include <vector>
#include <string>

using namespace std;


void gen_e_variants(vector<string>& sig, int e, int pos, string cur, vector<string>& e_variants);

void insert_one_sig(vector<string>& sig, int error, vector<pair<string, int> >& e_variants);

#endif //JWSEARCH_UTILITY_H
