//
// Created by Yaoshu Wang on 13/4/17.
//

#include "utility.h"

/*
 * The function is to generate all substrings that have exactly e errors
 * Assume sig.size() >> e
 */
void gen_e_variants(vector<string>& sig, int E, int e, int pos, string cur, vector<pair<string, int> >& e_variants) {
    if( (sig.size() - pos) < e) return;
    if(e <= 0) {
        e_variants.push_back(make_pair(cur, E));
        return;
    }
    // keep current character
    gen_e_variants(sig, E, e, pos + 1, cur + sig[pos], e_variants);
    // delete current character
    gen_e_variants(sig, E, e - 1, pos + 1, cur, e_variants);

}

// one partition of string
void insert_one_sig(vector<string>& sig, int error, vector<pair<string, int> >& e_variants) {
    if(error <= 0) return;
    for(int e = 0; e <= error; e++) {
        if(sig.size() < e) break;
        gen_e_variants(sig, e, e, 0, "", e_variants);
    }
}



