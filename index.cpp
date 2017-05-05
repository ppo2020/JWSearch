//
// Created by Yaoshu Wang on 10/4/17.
//

#include <cmath>
#include <iostream>

#include "index.h"
#include "utility.h"

using namespace std;

/*
// * The function is to generate all substrings that have exactly e errors
// * Assume sig.size() >> e
// */
//void gen_e_variants(vector<string>& sig, int e, int pos, string cur, vector<string>& e_variants) {
//    if( (sig.size() - pos) < e) return;
//    if(e <= 0) {
//        e_variants.push_back(cur);
//        return;
//    }
//    // keep current character
//    gen_e_variants(sig, e, pos + 1, cur + sig[pos], e_variants);
//    // delete current character
//    gen_e_variants(sig, e - 1, pos + 1, cur, e_variants);
//
//}
//
//
//void insert_one_sig(vector<string>& sig, int error, vector<string>& e_variants) {
//    if(error <= 0) return;
//    vector<string> e_variants;
//    for(int e = 0; e <= error; e++) {
//        if(sig.size() < e) break;
//        gen_e_variants(sig, e, 0, "", e_variants);
//    }
//}

/*
 * The function is to partition a string into set of substrings according to SC, and generate all
 * e-variants
 */
void Index::gen_string_sig(vector<string>* sig_entity, vector<pair<string, int> >& e_variants) {
    uint script = 0;
    unordered_map<string, uint>::iterator it;
    uint part_num = 0;
    for(uint i = 0; i < this->lexicon->max_part_num; i++) {
        if(script >= sig_entity->size()) break;
        it = this->lexicon->cluster_char->find(sig_entity->at(script));
        if(it->second != i)
            continue;
        vector<string> cur_sig;
        while(script < sig_entity->size() && this->lexicon->cluster_char->find(sig_entity->at(script))->second == i) {
            cur_sig.push_back(sig_entity->at(script));
            script ++;
        }
        // :com
        //for(int i = 0; i < cur_sig.size(); i++) cout<<cur_sig[i]<<"  ";
        //cout<<endl;
        // << com
        part_num++;
        int error = ceil(this->tau_rate * cur_sig.size());
        insert_one_sig(cur_sig, error, e_variants);
    }
    // record the partition number for each entity
    this->lexicon->part_num_all.push_back(part_num);
}


void Index::construct_index() {
    for(uint i = 0; i < this->lexicon->dataset_sig.size(); i++) {
        vector<string>* sig = &(this->lexicon->dataset_sig[i]);
        // generate all e-variants for sig
        vector<pair<string, int> > e_variants;
        this->gen_string_sig(sig, e_variants);
        //for(uint i = 0; i < e_variants.size(); i++) cout<<e_variants[i].first<<" ";
        //cout<<endl;

        // insert e-variants into inverted index
        unordered_map<string, vector<uint64_t> >::iterator it;
        for(uint j = 0; j < e_variants.size(); j++) {
            it = this->variant_index.find(e_variants[j].first);
            uint64_t elem = (uint64_t)(i << 8) || (uint64_t)(e_variants[j].second);
            if(it == this->variant_index.end()) {
                vector<uint64_t> vlist(1, elem);
                this->variant_index.insert({e_variants[j].first, vlist});
            } else {
                it->second.push_back(elem);
            }
        }
    }
    this->fill_all_lists_min();
    this->sort_lists();
}

void Index::fill_all_lists_min() {
    unordered_map<string, vector<uint64_t> >::iterator it;
    for(it = this->variant_index.begin(); it != this->variant_index.end(); it++) {
        vector<uint64_t >* list = &(it->second);
        uint min_l = this->lexicon->dataset[(uint)(list->at(0) >> 8)].size();
        for(uint i = 1; i < list->size(); i++) {
            uint len = this->lexicon->dataset[(uint)list->at(i) >> 8].size();
            if(min_l > len)
                min_l = len;
        }
        this->variant_min.insert({it->first, min_l});
    }

}

bool Index::cmp(uint64_t a, uint64_t b) {
    uint slen_1 = this->lexicon->dataset[a >> 8].size();
    uint e_1 = (uint)(a & 0x0FF);
    uint k_1 = this->lexicon->part_num_all[a >> 8];

    uint slen_2 = this->lexicon->dataset[b >> 8].size();
    uint e_2 = (uint)(b & 0x0FF);
    uint k_2 = this->lexicon->part_num_all[b >> 8];

    return ((int)(slen_1) - (int)(e_1 * k_1)) >= ((int)(slen_2) - (int)(e_2 * k_2));
}


int Index::__sort__(vector<uint64_t>* sig) {
    uint len = sig->size();
    int i, j;
    uint64_t temp;
    for(i = 1; i < len; i++) {
        temp = sig->at(i);
        j = i - 1;
        for(; j >= 0 && this->cmp(sig->at(j), temp); j--) {
            sig->at(j + 1) = sig->at(j);
        }
        sig->at(j + 1) = temp;
    }
}

void Index::sort_lists(){
    unordered_map<string, vector<uint64_t> >::iterator it;
    for(it = this->variant_index.begin(); it != this->variant_index.end(); it++) {
        //vector<uint64_t >* list = &(it->second);
        //sort(list->begin(), list->end(), this->cmp);
        //sort(it->second.begin(), it->second.end(), cmp_);
        this->__sort__(&(it->second));
    }
}