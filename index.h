//
// Created by Yaoshu Wang on 10/4/17.
//

#ifndef JWSEARCH_INDEX_H
#define JWSEARCH_INDEX_H

#include <vector>
#include <string>
#include <iostream>

#include "data.h"

using std::vector;
using std::string;

class Index {
private:
    bool cmp(uint64_t a, uint64_t b);
    int __sort__(vector<uint64_t>* sig);
    void fill_all_lists_min();
    void sort_lists();
public:
    Lexicon* lexicon;
    // error rate
    double tau_rate;

    Index(Lexicon* lexicon,double tau_rate) {
        this->lexicon = lexicon;
        this->tau_rate = tau_rate;
    }

    // inverted index:  elem uint64_t -> entity_id << 8 || error
    unordered_map<string, vector<uint64_t> > variant_index;
    // statistic the minimal length of each list
    unordered_map<string, uint> variant_min;

    // construct index
    void gen_string_sig(vector<string>* sig_entity, vector<pair<string, int> >& e_variants);
    void construct_index();
};


#endif //JWSEARCH_INDEX_H
