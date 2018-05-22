#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <iostream>
#include <ctime>
#include <cstdlib>
#define random(a,b) (rand()%(b-a)+a)
using namespace std;

map<vector<string>, vector<string> >chineseChain(int gram, string filename){

    map<vector<string>, vector<string> > wordChain;
    ifstream work;
    vector<string> wordVec;

    work.open(filename);
    string iteraion_;

    while(!work.eof()){
        work>>iteraion_;
        for(int i = 0; i < iteraion_.size() ; i+=3){
            string hanzi = iteraion_.substr(i,i+3);
            if(hanzi.size()>3) hanzi = hanzi.substr(0,3);
            wordVec.push_back(hanzi);
        }
    }

    for(int i = 0; i < wordVec.size() - gram + 1; i++){
        
        vector<string> prefix;
        for(int j = 0; j < gram-1; j++){
            prefix.push_back(wordVec[j+i]);
        }
        
        wordChain[prefix].push_back(wordVec[i+gram-1]);

    }

    return wordChain;
}


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
        
        wordChain[prefix].push_back(wordVec[i+gram-1]);

    }

    return wordChain;
}

void elemTrans(vector<string> from, vector<string> &to){
    for(int i = 0; i < from.size(); i++){
        to.push_back(from[i]);
    }
}

vector<string> geneText(int length, map<vector<string>, vector<string> > &work)
{
    srand(time(0));
    int begin = random(0, work.size());
    vector<string> text;
    text.push_back("...");
    vector<string> window; 
    
    map<vector<string>, vector<string> >::iterator ptr;
    ptr = work.begin();
    for(int i = 0; i < begin; i++){
        ptr++;
    }

    window = ptr->first;
    elemTrans(window, text);

    while(text.size() != length+1){

        if(work[window].size() == 0) break;
        srand(time(0));
        int randElem = random(0, work[window].size());
        string nextWord = work[window][randElem];
        text.push_back(nextWord);

        window.erase(window.begin());
        window.push_back(nextWord);
    
    }
    text.push_back("...");
    return text;

}

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
    cout<<"Chines file or English file? ('C' or 'E') ";
    char language;
    cin>>language;
    while(language != 'C' && language != 'E'){
        cout<<"Invalid input. Retry: ";
        cin>>language;
    }


    srand(time(0));
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
            if(language == 'E' || language == 'e'){
                work = markovChain(gram,filename);
                vector<string> text = geneText(length, work);
                printVec(text);
            }else if(language == 'C' || language =='c'){
                work = chineseChain(gram,filename);
                vector<string> text = geneText(length, work);
                printChinese(text);
            }
            
    }

    return 0;
}
