//
// Created by Yaoshu Wang on 13/4/17.
//

#include <fstream>
#include "query.h"
#include "utility.h"

using namespace std;


QueryVec::QueryVec(char* query_file, unordered_map<string, uint>* score) {
    ifstream file(query_file);
    string line;
    while(getline(file, line)) {
        this->query_vec.push_back(new Query(line, score));
    }
    this->query_num = this->query_vec.size();
}

QueryVec::QueryVec(char* query_file, unordered_map<string, uint>* score, unordered_map<string, uint>* cluster_char, uint max_part_num) {
    ifstream file(query_file);
    string line;
    while(getline(file, line)) {
        this->query_vec.push_back(new Query(line, score, cluster_char, max_part_num));
    }
    this->query_num = this->query_vec.size();
}


Query::Query(string& entity, unordered_map<string, uint>* score) {
    this->query = entity;
    this->score = score;

    // operation
    this->gen_one_query_sig();
    this->sort_quer_vec();
}


Query::Query(string& entity, unordered_map<string, uint>* score, unordered_map<string, uint>* cluster_char, uint max_part_num) {
    this->query = entity;
    this->score = score;

    // operation
    this->gen_one_query_sig();
    this->sort_quer_vec();
    this->gen_one_string_partitions(cluster_char, max_part_num);
}

int Query::cmp(string &s1, string &s2) {
    return this->score->at(s1) < this->score->at(s2);
}


void Query::gen_one_query_sig() {
    unordered_map<string, int> dup;
    unordered_map<string, int>::iterator it;
    for(uint i = 0; i < this->query.size(); i++) {
        string c(1, this->query[i]);
        it = dup.find(c);
        if(it == dup.end()) {
            dup.insert({c, 0});
        } else {
            it->second++;
        }
    }
    for(it = dup.begin(); it != dup.end(); it++) {
        query_sig.push_back(it->first);
        for(int i = 1; i <= it->second; i++) {
            this->query_sig.push_back(it->first + this->suffix + to_string(i));
        }
    }
}

int Query::__sort__() {
    vector<string>* sig = &(this->query_sig);
    uint len = sig->size();
    int i, j;
    string temp;
    for(i = 1; i < len; i++) {
        temp = sig->at(i);
        j = i - 1;
        for(; j >= 0 && this->cmp(sig->at(j), temp); j--) {
            sig->at(j + 1) = sig->at(j);
        }
        sig->at(j + 1) = temp;
    }
}

void Query::sort_quer_vec() {
    //sort(this->query_sig.begin(), this->query_sig.end(), this->cmp);
    this->__sort__();
}

// error allocation
void Query::naive_allo_errors(int T_min, vector<int>& alloerrors) {
    int error = this->query.size() - T_min;
    if(error < 0) return;
    for(uint i = 0; i < this->query_sig_pars.size(); i++) {
        alloerrors.push_back(floor(error * 1.0 * this->query_sig_pars[i].size() / this->query.size()));
    }
}


// Generate partitions
void Query::gen_one_string_partitions(unordered_map<string, uint>* cluster_char, uint max_part_num) {
    uint script = 0;
    unordered_map<string, uint>::iterator it;
    for(uint i = 0; i < max_part_num; i++) {
        if(script >= this->query_sig.size()) break;
        it = cluster_char->find(this->query_sig.at(script));
        if(it->second != i) continue;
        vector<string> query_par_sig;
        while(script < this->query_sig.size() &&
                cluster_char->find(this->query_sig.at(script))->second == i) {
            query_par_sig.push_back(this->query_sig.at(script));
            script++;
        }
        this->query_sig_pars.push_back(query_par_sig);
    }
}


// generate variants
void Query::gen_one_string_variants(vector<int>& alloerrors) {
    for(uint k = 0; k < this->query_sig_pars.size(); k++) {
        insert_one_sig(this->query_sig_pars[k], alloerrors[k], this->e_variants);
    }
}









