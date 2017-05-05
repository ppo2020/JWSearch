//
// Created by Yaoshu Wang on 10/4/17.
//

#include <fstream>

#include "data.h"
using namespace std;


void Lexicon::read_lexicon(char* data_file) {
    ifstream file(data_file);
    string line;
    while(getline(file, line)) {
        this->dataset.push_back(line);
        if(this->str_min_length > line.size())
            this->str_min_length = line.size();
    }
    file.close();
}


// constructive function
Lexicon::Lexicon(char* data_file) {
    this->read_lexicon(data_file);
}


Lexicon::Lexicon(char *data_file, uint part_num, unordered_map<string, uint>* score,
                    unordered_map<string, uint>* cluster_char, vector<string>* SC) {
    this->max_part_num = part_num;
    // Load entities of lexicon
    this->str_min_length = 1000000000;
    this->read_lexicon(data_file);
    this->score = score;
    this->cluster_char = cluster_char;
    this->SC = SC;
    // generate character vectors (entity_sig) of lexicon
    this->enum_all_alphabet();
    // sort string in lexicon
    this->sort_lexicon();

}


// Get character distribution of one string
void Lexicon::gen_characters_one_string(string& entity, vector<string>& alphabets) {
    unordered_map<string, int> dup;
    unordered_map<string, int>::iterator it;
    for(int i = 0; i < entity.size(); i++) {
        string c(1, entity[i]);
        it = dup.find(c);
        if(it == dup.end()) {
            dup.insert({c, 0});
        } else {
            it->second ++;
        }
    }
    // Scan all characters
    for(it = dup.begin(); it != dup.end(); it++) {
        alphabets.push_back(it->first);
        for(int i = 1; i <= it->second; i++) {
            alphabets.push_back(it->first + this->suffix + to_string(i));
        }
    }

}

// enumerate all alphebats from lexicon
void Lexicon::enum_all_alphabet() {

    unordered_map<string, uint>::iterator it;
    for(int i = 0; i < this->dataset.size(); i++) {
        vector<string> alphabets;
        this->gen_characters_one_string(this->dataset[i], alphabets);
        this->dataset_sig.push_back(alphabets);
        /*
        // scan alphabets and insert them into hash table
        for(int j = 0; j < alphabets.size(); j++) {
            it = charfreq.find(alphabets[j]);
            if(it == charfreq.end()) charfreq.insert({alphabets[i], 1});
            else it->second++;
        }
        */
    }
}


int Lexicon::cmp(string& s1, string& s2) {
    return this->score->find(s1)->second < this->score->find(s2)->second;
}

int Lexicon::__sort__(int eid) {
    vector<string>* sig = &(this->dataset_sig[eid]);
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

void Lexicon::sort_lexicon() {
    for(int i = 0; i < this->dataset_sig.size(); i++) {
        //sort(this->dataset_sig[i].begin(), this->dataset_sig[i].end(), this->cmp);
        this->__sort__(i);
    }
}