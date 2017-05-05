//
// Created by Yaoshu Wang on 13/4/17.
//


#include "search.h"
#include <iostream>

using namespace std;


bool if_early_termination(uint len, uint e, uint k, uint T_min_) {
    return (len - e * k) >= T_min_;
}

bool length_filter(uint qlen, uint slen, double tau) {
    double r = slen * 1.0 / qlen;
    return r >= (3 * tau - 1.5) && r <= 1.0/(3 * tau - 1.5);
}

bool skip_element_1(uint slen, uint qlen, uint e, uint k, double tau) {
    return (slen - e * k) >= (3 * tau - 0.5) * qlen * slen * 1.0 / (qlen + slen);
}

bool skip_element_2(uint slen, uint qlen, uint e, uint len, double tau) {
    return true;
}

uint __max__(uint a, uint b) {
    return a > b ? a : b;
}

void jw_search(Query* query, Index* index, vector<uint>& dup_cand, vector<uint>& cand, double tau) {
    unordered_map<string, vector<uint64_t> >::iterator it;
    // generate all variants
    vector<int> alloerrors;
    uint qlen = query->query.size();
    uint len = __max__(index->lexicon->str_min_length, ceil(3 * tau - 1.5) * qlen);
    uint T_min = ceil((3 * tau - 0.5) * qlen * len * 1.0 / (qlen + len));
    query->naive_allo_errors(T_min, alloerrors);
    query->gen_one_string_variants(alloerrors);
    for(uint i = 0; i < query->e_variants.size(); i++) {
        it = index->variant_index.find(query->e_variants[i].first);
        if(it == index->variant_index.end()) {
            continue;
        }
        vector<uint64_t>* list = & it->second;
        // sequentially scan list
        // get the minimal length of the list
        uint len_ = __max__(index->variant_min.find(query->e_variants[i].first)->second,
                                ceil(3 * tau - 1.5) * qlen);
        uint T_min_ = ceil((3 * tau - 0.5) * qlen * len_ * 1.0 / (qlen + len_));
        for(uint j = 0; j < list->size(); j++) {
            uint sid = (uint)(list->at(j) >> 8);
            uint slen = index->lexicon->dataset[sid].size();
            uint e = (uint)(list->at(j) & 0x0FF);
            uint k = index->lexicon->part_num_all[sid];
            // Early termination
            if(! if_early_termination(slen, e, k, T_min_)) break;

            // utilize strategies
            if(length_filter(qlen, slen, tau) && skip_element_1(slen, qlen, e, k, tau))
                dup_cand.push_back((uint)(list->at(j) >> 8));
        }
    }
    remove_dup_cand(dup_cand, cand);
}


// remove duplicate candidate
void remove_dup_cand(vector<uint>& dup_cand, vector<uint>& cand) {
    unordered_set<uint> dup;
    unordered_set<uint>::iterator it;
    for(uint i = 0; i < dup_cand.size(); i++) {
        it = dup.find(dup_cand[i]);
        if(it == dup.end()) {
            cand.push_back(dup_cand[i]);
            dup.insert(dup_cand[i]);
        }
    }
}


