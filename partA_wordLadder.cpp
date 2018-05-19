#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
using namespace std;

void lower(string &word){
    int jump = int('a') - int('A');
    for(int i = 0; i<word.size(); i++){
        if(int(word[i])<=int('Z') && int(word[i])>=int('A')){
            word[i] += jump;
        }
    }
}

map<string, int> dict(){
    map<string, int> vocabulary;
    ifstream file,file1,file2,file3;
    file.open("dictionary.txt");
    string token;
    while(!file.eof()){
        file>>token;
        vocabulary[token] = 2;
    }
    return vocabulary;
}

bool isWord(const string& word, const map<string, int>& english){
    if(english.count(word)) return true;
    else return false;
}

vector<string> neibs(string word, map<string, int>& english){
    lower(word);
    vector<string> nbr;
    for(int i = 0 ;i < word.size();i++){
        for(int j = int('a'); j <= int('z');j++){
            string mid = word.substr(0,i) + char(j) + word.substr(i+1);
            if(isWord(mid,english) && mid != word){nbr.push_back(mid);}
        }
    }
    return nbr;
}

void printSt(const stack<string> &pass_){
    stack<string> pass = pass_;
    vector<string> path;
    while(!pass.empty()){
        path.push_back(pass.top());
        pass.pop();
    }
    cout<<"A ladder form "<<path[0]<<" to "
        <<path[path.size()-1]<<": "<<endl;
    for(string i : path){
        cout<<i<<" ";
    }
    cout<<endl;
}

bool wordReach(const string& w1,const string& w2, map<string, int> &english){
    if(!isWord(w1,english) || !isWord(w2,english) 
    || w1.size() != w2.size() ){
        cout<<w1<<" or "<<w2<<" is not found in dictionary. "<<endl;
        return false;
    }
    vector<string> repWords;
    queue<stack<string> > total;
    stack<string> start;
    start.push(w1);
    total.push(start);
    while(!total.empty()){
        stack<string> headSt = total.front();
        total.pop();
        vector<string> neib = neibs(headSt.top(),english);
        for(auto i: neib){
            if(english[i] != 1){
                if(i == w2) {
                    headSt.push(i);
                    printSt(headSt);
                    for(string j:repWords){
                        english[j] = 2;
                    }
                    return true;
                }else{
                    stack<string> copy = headSt;
                    copy.push(i);
                    total.push(copy);
                    english[i] = 1;
                    repWords.push_back(i);
                }
            }
        }
    }
    for(string j:repWords){
        english[j] = 2;
    }
    cout<<"No path to show."<<endl;
    return false;
}

int main(){
    map<string, int> english = dict();
    while(true){
        string w1;
        string w2;
        cout<<"word #1 (or Enter to quit ): ";
        getline(cin,w1);
        if(w1 == "") break;
        cout<<"word #2 (or Enter to quit ): ";
        getline(cin,w2);
        if(w2 == "") break;
        lower(w1);lower(w2);
        wordReach(w1, w2 ,english);
        cout<<endl;
    }
    return 0;
}

