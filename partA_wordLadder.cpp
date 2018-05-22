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
//              neighbors of the short one enlarge one letter in a time to reach the same lengh;
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

//print the words in a stack in reverse order
void printReSt(const stack<string> &pass_){
    stack<string> pass = pass_;
    vector<string> path;
    while(!pass.empty()){
        path.push_back(pass.top());
        pass.pop();
    }
    cout<<"A ladder form "<<path[path.size()-1]<<" to "
        <<path[0]<<": "<<endl;
    for(int i = path.size()-1; i >= 0; i--){
        cout<<path[i]<<" ";
    }
    cout<<endl;
}


stack<string> result;//store the path
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
        vector<string> neib = neibs(headSt.top(),english);
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

//make a stack empty(for the storage is a static stack)
void clearSt(stack<string>& st){
    while(!st.empty()){
        st.pop();
    }
}

stack<string> container;//Record the growing_longer path
bool geneReach(const string& w1,const string& w2, map<string, int> &english){
    if(w1.size() == w2.size() ){
        if(!isWord(w1,english)) cerr<<"(Warning: "<<w1<<" is not in \"dictionary.txt\".)"<<endl;
        if(!isWord(w2,english)) cerr<<"(Warning: "<<w2<<" is not in \"dictionary.txt\".)"<<endl;
        return validReach(w1, w2, english);
    }else{
        if(w1.size() > w2.size()){
            if(!isWord(w1,english)) cerr<<"(Warning: "<<w1<<" is not in \"dictionary.txt\".)"<<endl;
            if(!isWord(w2,english)) cerr<<"(Warning: "<<w2<<" is not in \"dictionary.txt\".)"<<endl;
    
            cerr<<"(Warning: different length)"<<endl;
            stack<string> myPath;
            myPath.push(w2);
            container.push(w2);//a path recording w2 -> enlarged w2
            while(!myPath.empty()){//iterate every possible word

                while(myPath.top().size() != w1.size()){//fill up
                    vector<string> larger = enlargeWord(myPath.top(), english);
                    if(container.top().size() == myPath.top().size()){//container keeps the latest word
                        container.pop();
                    }
                    container.push(myPath.top());
                    myPath.pop();
                    if(!larger.empty()){
                        for(string i: larger){
                            myPath.push(i);

                        }
                    }
                    if(myPath.empty()){
                        clearSt(container);
                        return false;
                    } 
                }  

                if(validReach(w1,myPath.top(),english)){
                    while(!container.empty()){
                        result.push(container.top());
                        container.pop();
                    };
                    return true;
                }
                else myPath.pop();
            }
            clearSt(container);
            return false;
        }

        else{
            return geneReach(w2,w1,english);//w1 swich the position with w2
        }   
    }
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
        if(geneReach(w1, w2 ,english)){
            if(w1.size() < w2.size()) printReSt(result);
            else printSt(result);
            clearSt(result);
        }else{
            clearSt(result);
            cout<<"no path";
        }
        cout<<endl;
    }
    return 0;
}



