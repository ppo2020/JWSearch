#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <unistd.h>

#include "func.h"

using namespace std;

void read_shuffle_data_info(char* L_file, char* SC_file, unordered_map<string, uint>& L,
                            unordered_map<string, uint>& cluster_char,vector<string>& SC) {
    ifstream file_L(L_file);
    string line;
    while(getline(file_L, line)) {
        string key = line.substr(0,line.find(';'));
        string value_ = line.substr(line.find(';')+1, line.size() - line.find(';') + 1);
        //cout<<key<<"  "<<value_<<endl;
        uint64_t value = (uint64_t)std::strtoull(value_.c_str(), NULL, 0);
        L.insert({key, value & 0x0FFFFFFFF});
        cluster_char.insert({key, value >> 32});
    }
    file_L.close();
    ifstream file_SC(SC_file);
    while(getline(file_SC, line)){
        SC.push_back(line);
    }
    file_SC.close();
}


int main(int argc, char* argv[]) {
    int opt;
    char* method_type;
    char* data_file;
    char* query_file;
    char* index_file;
    char* L_file;
    char* SC_file;
    double tau = 0.0;
    while((opt = getopt(argc, argv, "m:r:q:I:L:S:t:"))!= -1) {
        switch(opt) {
            case 'm':
                method_type = optarg;
                break;
            case 'r':
                data_file = optarg;
                break;
            case 'q':
                query_file = optarg;
                break;
            case 'I':
                index_file = optarg;
                break;
            case 'L':
                L_file = optarg;
                break;
            case 'S':
                SC_file = optarg;
                break;
            case 't':
                tau = atof(optarg);
                break;
            default:
                cerr<<"wrong option -----------------"<<endl;
                exit(EXIT_FAILURE);
        }
    }
    cout<<"All Set Up"<<endl;
    // set up shuffle strategy
    unordered_map<string, uint> L;
    vector<string> SC;
    unordered_map<string, uint> cluster_char;
    read_shuffle_data_info(L_file, SC_file, L, cluster_char, SC);
    cout<<"Start Search"<<endl;
    if(! strcmp(method_type, "even")) {
        cout<<"JW search ..."<<endl;
        jw_pigeonvariance(data_file, query_file, index_file, SC.size() + 1, &L, &cluster_char, &SC, tau);
    }
    return 0;
}

