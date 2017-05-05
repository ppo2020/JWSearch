//
// Created by Yaoshu Wang on 10/4/17.
//

#ifndef JWSEARCH_DATA_H
#define JWSEARCH_DATA_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Parameters.h"

using namespace std;

class Lexicon {
private:
    int cmp(string& s1, string& s2);
    int __sort__(int eid);
    void enum_all_alphabet();
    void read_lexicon(char* data_file);
    void gen_characters_one_string(string& entity, vector<string>& alphabets);
public:
    // char alphabets according to their scores
    unordered_map<string, uint>* score;

    const string suffix = "#";
    uint max_part_num;
    Lexicon(char* data_file);
    Lexicon(char *data_file, uint part_num, unordered_map<string, uint>* score,
            unordered_map<string, uint>* cluster_char, vector<string>* SC);

    vector<string> dataset;

    // minmal length of string in dataset
    uint str_min_length;

    // string into vector<char>
    vector<vector<string> > dataset_sig;

    // partition number for all entities in dataset
    vector<uint> part_num_all;

    // Generate splitting characters
    vector<string>* SC;

    // clusters
    unordered_map<string, uint>* cluster_char;

    // sort each entity of lexicon
    void sort_lexicon();

};

#endif //JWSEARCH_DATA_H
