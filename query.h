//
// Created by Yaoshu Wang on 13/4/17.
//

#ifndef JWSEARCH_QUERY_H
#define JWSEARCH_QUERY_H

#include <vector>
#include <string>
#include <unordered_map>

#include "Parameters.h"

using namespace std;

class Query;

class QueryVec {
public:

    vector<Query*> query_vec;
    // constructive function
    QueryVec(char* filename, unordered_map<string, uint>* score);
    QueryVec(char* query_file, unordered_map<string, uint>* score,
             unordered_map<string, uint>* cluster_char, uint max_part_num);

    uint query_num;
};


class Query {
private:
    int cmp(string& s1, string& s2);
    int __sort__();
    const string suffix = "#";
public:
    string query;
    vector<string> query_sig;

    // partitions
    vector<vector<string> > query_sig_pars;

    // score of each character
    unordered_map<string, uint>* score;

    // variants
    vector<pair<string, int> > e_variants;

    Query(string& entity, unordered_map<string, uint>* score);
    Query(string& entity, unordered_map<string, uint>* score,
          unordered_map<string, uint>* cluster_char, uint max_part_num);

    void gen_one_query_sig();
    void sort_quer_vec();
    void gen_one_string_partitions(unordered_map<string, uint>* cluster_char, uint max_part_num);

    void gen_one_string_variants(vector<int>& alloerrors);
    void naive_allo_errors(int T_min, vector<int>& alloerrors);
};

#endif //JWSEARCH_QUERY_H
