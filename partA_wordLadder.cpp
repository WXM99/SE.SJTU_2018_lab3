//  This is a programm that finds a path connecting two words,
//  and the path is made of words in dictionary with one letted changed each step
//  Solving procedure:
//      Step#1. Read the file and store valid word in a Map{english};
//      Step#2. Put word1's neighbors in the stack;
//      Step#3. Put the stack above in a queue;
//      Step#4. try to connect in each word in neighbors,
//              if can't try neighbor's neighbor and delet the previous one;
//      Step#5. If Step#4 faild, the queue will be empty. That is no path for w1 and w2;
//      Step#6. If w1 and w2 are in diffirent length,
//              neighbors of the short one enlarge one letter or shrink in a time to reach the same lengh;
//      Step#7. Word1 and 2 don't need to be in dictionary, but their neighbors do.
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


void lower(string &word){//transform to lower case
    int jump = int('a') - int('A');
    for(int i = 0; i<word.size(); i++){
        if(int(word[i])<=int('Z') && int(word[i])>=int('A')){
            word[i] += jump;
        }
    }
}

//store all valid word in a map
map<string, int> dict(string filename){
    map<string, int> vocabulary;
    ifstream file,file1,file2,file3;
    file.open(filename);
    string token;
    while(!file.eof()){
        file>>token;
        vocabulary[token] = 2;
    }
    return vocabulary;
}

//see if a given word is in the dictionary
bool isWord(const string& word, const map<string, int>& english){
    if(english.count(word)) return true;
    else return false;
}

//thoes words who differ the given one only one letter and in dictionaty
vector<string> neibs(string word, const map<string, int>& english){
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

//neighbors that a letter longer and in dictionary
vector<string> enlargeWord(const string &word, const map<string, int>& english){
    vector<string> nbr;
    for(int i = 0 ;i < word.size()+1 ;i++){
        for(int j = int('a'); j <= int('z');j++){
            string mid = word.substr(0,i) + char(j) + word.substr(i);
            if(isWord(mid,english) && mid != word){nbr.push_back(mid);}
        }
    }
    return nbr;
}

vector<string> shrinkWord(const string &word, const map<string, int>& english){
    vector<string> nbr;
    for(int i = 0 ;i < word.size() ;i++){
        string mid = word.substr(0,i) + word.substr(i+1);
        if(isWord(mid,english)){ 
            nbr.push_back(mid);
        }
    }
    return nbr;
}

//all possible neighbors summing up above 3 kinds
vector<string> allNeibs(const string &word, const map<string, int>& english){
    vector<string> nbr1 = shrinkWord(word, english);
    vector<string> nbr2 = enlargeWord(word, english);
    vector<string> nbr3 = neibs(word, english);
    vector<string> all;
    for(string i : nbr3){
        all.push_back(i);
    }
    for(string i : nbr2){
        all.push_back(i);
    }
    for(string i : nbr1){
        all.push_back(i);
    }
    return all;
}
vector<string> neibUp(const string &word, const map<string, int>& english){
    vector<string> nbr2 = enlargeWord(word, english);
    vector<string> nbr3 = neibs(word, english);
    vector<string> all;
    for(string i : nbr3){
        all.push_back(i);
    }
    for(string i : nbr2){
        all.push_back(i);
    }
    return all;
}
vector<string> neibDown(const string &word, const map<string, int>& english){
    vector<string> nbr1 = shrinkWord(word, english);
    vector<string> nbr3 = neibs(word, english);
    vector<string> all;
    for(string i : nbr3){
        all.push_back(i);
    }
    for(string i : nbr1){
        all.push_back(i);
    }
    return all;
}

//make a stack empty(for the storage is a static stack)
void clearSt(stack<string>& st){
    while(!st.empty()){
        st.pop();
    }
}

//print the words in a stack form top to buttom
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

//store the path
stack<string> result;
//search the path between same-length words with the method in Step#4
bool validReach(const string& w1,const string& w2, map<string, int> &english){
    if(!isWord(w2,english)){
        vector<string> neib = neibs(w2, english);
        if(neib.empty()) {   
            return false;
        }
        else{
            for(int i = 0;i<neib.size();i++){
                if(validReach(w1,neib[i],english)){ 
                    result.push(w2);
                    return true;
                    }
            } 
            return false;
        }
    }
    
    vector<string> repWords;
    queue<stack<string> > total;
    stack<string> start;
    start.push(w1);
    total.push(start);
    while(!total.empty()){
        stack<string> headSt = total.front();
        total.pop();
        vector<string> neib ;
        
        if(w1.size() > w2.size()){
            neib = neibDown(headSt.top(),english);
        }else if (w1.size() < w2.size()){
            neib = neibUp(headSt.top(),english);
        }else{
            neib = neibs(headSt.top(),english);
        }
        
        for(auto i: neib){
            if(english[i] != 1){
                if(i == w2) {
                    headSt.push(i);
                    result = headSt;
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
    return false;
}

int main(){
    cout<<"Welcome!"<<endl;
    
    string filename;
    cout<<"Dictionary filename? ";
    getline(cin,filename);
    ifstream test;
    test.open(filename);
    while(!test.is_open()){
        cerr<<filename<<" isn't found. Please try again: ";
        getline(cin,filename);
        test.open(filename);
    }
    test.clear();


    map<string, int> english = dict(filename);
    cout<<"word#1 and word#2 can be outsied "<<filename<<" and differ in length."<<endl;
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
        if(w1 != w2){
            if(!isWord(w1,english))  cerr<<"(Warning: "<<w1<<" is not included in dictionary)"<<endl;
            if(!isWord(w2,english))  cerr<<"(Warning: "<<w2<<" is not included in dictionary)"<<endl;
            if(w1.size() != w2.size()) cerr<<"(Warning: diffirent in length)"<<endl; 
            validReach(w1,w2,english);
            if(!result.empty()){
                stack<string> res = result;
                printSt(res);
                cout<<endl;
                clearSt(result);
            }else cout<<"no path"<<endl;
        }else{
            cout<<"word#1 and word#2 are the same. Retry: "<<endl;
        }
    }
    cout<<"Have a nice day!"<<endl;
    return 0;
}
