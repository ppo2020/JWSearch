//
// Created by Yaoshu Wang on 13/4/17.
//

#include "func.h"


void jw_pigeonvariance(char* data_file, char* query_file, char* index_file, uint part_num,
                        unordered_map<string, uint>* score, unordered_map<string, uint>* cluster_char, vector<string>* SC, double tau) {
    // read lexicon
    Lexicon* lexicon = new Lexicon(data_file, part_num, score, cluster_char, SC);
    // construct index
    double tau_rate = 0.8;
    Index* index = new Index(lexicon, tau_rate);
    index->construct_index();

    // read queries
    QueryVec* queryVec = new QueryVec(query_file, score, cluster_char, part_num);

    // make query processing
    vector<uint> dup_cand, cand, results;
    for(uint qid = 0; qid < queryVec->query_num; qid++) {
        cout<<"Q: "<<queryVec->query_vec[qid]->query<<endl;
        dup_cand.clear();
        cand.clear();
        jw_search(queryVec->query_vec[qid], index, dup_cand, cand, tau);

        // verify
        for(uint i = 0; i < cand.size(); i++) {
            double t = jaro_winkler_distance(queryVec->query_vec[qid]->query.c_str(),
                                    lexicon->dataset[cand[i]].c_str());
            if(t >= tau)
                results.push_back(cand[i]);
        }
        fprintf(stderr, "avgcnd: %d avgrst: %d\n", cand.size(), results.size());
    }
}