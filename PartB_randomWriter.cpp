//  This is a programm that generates a short text with a given length and a txt file.
//  Solving procedure:
//      Step#1. Read the file and store each word (or Chinese character) in a vector;
//      Step#2. Creat the Markov Chain Map using the big vector above;
//      Step#3. Creat a vector as a storage for the newly-generated words;
//      Step#4. Randomly pick a key (N-1 words) in Map.
//              Put it and its value (in Map) in vector above;
//      Step#5. The key turns to the top N-1 words in storage vector. 
//              Put back its key in the vector;
//      Step#6. Repeat step#5 untill the storage vector is filled up to a given size.
//      Step#7. Print the storage vector.
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <iostream>
#include <ctime>
#include <cstdlib>
#define random(a,b) (rand()%(b-a)+a)  //return a random int in [a,b)
using namespace std;


//generate a Chinese charactor Markov chain map
map<vector<string>, vector<string> >chineseChain(int gram, string filename){
    
    map<vector<string>, vector<string> > wordChain;
    ifstream work;
    vector<string> wordVec;

    work.open(filename);
    string iteraion_;//literally it is a sentence in Chinese.

    while(!work.eof()){
        work>>iteraion_;
        for(int i = 0; i < iteraion_.size() ; i+=3){//here the sentence can be spilited
            string hanzi = iteraion_.substr(i,i+3);
            //Chinese charactor (hanzi) count for 3 char in a string.
            if(hanzi.size()>3) hanzi = hanzi.substr(0,3);
            wordVec.push_back(hanzi);
        }
    }

    //generate the map using wordVec
    for(int i = 0; i < wordVec.size() - gram + 1; i++){
        
        vector<string> prefix;
        for(int j = 0; j < gram-1; j++){
            prefix.push_back(wordVec[j+i]);
        }
        
        wordChain[prefix].push_back(wordVec[i+gram-1]);

    }

    return wordChain;
}

void lower(string &word){
    int jump = int('a') - int('A');
    for(int i = 0; i<word.size(); i++){
        if(int(word[i])<=int('Z') && int(word[i])>=int('A')){
            word[i] += jump;
        }
    }
}

bool isUpper(char a){
    if(int(a) <= int('X') && int(a) >= int('A')) return true;
    else return false;
}

bool isMark(char a){
    switch(a){
        case'!':
        case'.':
        case'?':
        case';':
        case')':
        case']':return true;
        default: return false;
    }
}

//generate an English charactor Markov chain map
vector<vector<string> > head,tail;
map<vector<string>, vector<string> >markovChain(int gram, string filename){

    map<vector<string>, vector<string> > wordChain;
    ifstream work;
    vector<string> wordVec;

    work.open(filename);
    string iteraion;

    while(!work.eof()){
        work>>iteraion;
        wordVec.push_back(iteraion);
    }

    for(int i = 0; i < wordVec.size() - gram + 1; i++){
        
        vector<string> prefix;
        for(int j = 0; j < gram-1; j++){
            prefix.push_back(wordVec[j+i]);
        }
        if(isUpper(prefix[0][0])) head.push_back(prefix);
        if(isMark(prefix[prefix.size()-1][prefix[prefix.size()-1].size()-1])){
            tail.push_back(prefix);
        }
        wordChain[prefix].push_back(wordVec[i+gram-1]);

    }

    return wordChain;
}

//transport word in "from" to "to" without changing from.
void elemTrans(vector<string> from, vector<string> &to){
    for(int i = 0; i < from.size(); i++){
        to.push_back(from[i]);
    }
}

//generate the randomly-writen text
vector<string> geneText(int length, map<vector<string>, vector<string> > &work)
{
    srand(time(0));
    vector<string> text;
    vector<string> window; 
    int begin;
    
    begin = random(0, head.size());//random position in map to begin

    window = head[begin];
    elemTrans(window, text);
    srand(time(0));
    while(text.size() != length+1){
        if(work[window].size() == 0) break;
        int randElem = random(0, work[window].size());
        string nextWord = work[window][randElem];
        text.push_back(nextWord);
        
        //window slide down
        window.erase(window.begin());
        window.push_back(nextWord);
    
    }
    if(isMark(text[text.size()-1][0])) return text;
    else{
        int size = tail[0].size();
        for(int i = 0; i < tail.size(); i++){
            for (int j = 0; j < size-1; j++){
                if( tail[i][j] == text[text.size()-size+1+j]){
                    text.push_back(tail[i][tail[i].size()-1]);
                    return text;
                }
            }
        }
        text.push_back(".");
        return text;
    }

}

vector<string> geneTextChinese(int length, map<vector<string>, vector<string> > &work)
{
    srand(time(0));
    vector<string> text;
    text.push_back("...");
    vector<string> window; 
    int begin;
    
    begin = random(0, work.size());//random position in map to begin
    map<vector<string>, vector<string> >::iterator ptr;
    ptr = work.begin();		
    for(int i = 0; i < begin; i++){		
        ptr++;		
    }
    window = ptr->first;
    elemTrans(window, text);
    srand(time(0));
    while(text.size() != length+1){
        if(work[window].size() == 0) break;
        int randElem = random(0, work[window].size());
        string nextWord = work[window][randElem];
        text.push_back(nextWord);
        
        //window slide down
        window.erase(window.begin());
        window.push_back(nextWord);
    
    }
        
    text.push_back("...");
    return text;

}
//print words (charactor) in vector
void printVec(vector<string> vec){
    for(int i = 0; i< vec.size(); i++){
        cout<<vec[i]<<" ";
    }
    cout<<endl;
}

void printChinese(vector<string> vec){
    for(int i = 0; i< vec.size(); i++){
        cout<<vec[i];
    }
    cout<<endl;
}


int main(){

    cout<<"Chinese file or English file? ('C' or 'E') ";
    string language;
    cin>>language;
    while(language != "C" && language != "E"){
        cout<<"Invalid input. Retry: ";
        cin>>language;
    }

    string filename;
    cout<<"Input file? ";
    cin>>filename;
    ifstream test;
    test.open(filename);
    while(!test.is_open()){
        cerr<<filename<<" isn't found. Please try again: ";
        cin>>filename;
        test.open(filename);
    }

    int gram;
    cout<<"Value of N? ";
    cin>>gram;
    while(gram < 2){
        cerr<<"gram shouldn't be lower than 2. Retry: ";
        cin>>gram;
    }

    int length;
    cout<<"# of random words to generate (0 to quit)? ";
    cin>>length;
    if(length != 0){
            while(length < gram){
                cerr<<"length should't be lower than gram ("<<gram<<"). Retry: ";
                cin>>length;
            }
            map<vector<string>, vector<string> > work;
            if(language == "E"){
                work = markovChain(gram,filename);
                vector<string> text = geneText(length, work);
                cout<<"Here is a complete sentence: "<<endl;
                printVec(text);
            }else{
                work = chineseChain(gram,filename);
                vector<string> text = geneTextChinese(length, work);
                printChinese(text);
            }
            
    }

    return 0;
}
