///////////////////////////////////////////////////////////////////////////////
// words.cpp
//
// Computing the frequency distribution of a list of words, skipping stopwords.
//
// Students: you do not need to modify this file.
///////////////////////////////////////////////////////////////////////////////

// instructor's includes, delete from starter code
#include <algorithm>
#include <cctype>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string.h>
#include <iostream>
#include "words.hpp"






namespace words {

    word_vector read_words(const std::string& path) {
        word_vector result;
        std::ifstream file(path);
        if (!file) {
            throw io_exception(path);
        }
        for (std::string word; file >> word;) {
            result.emplace_back(std::move(word));
        }
        file.close();
        if (file.bad()) {
            throw io_exception(path);
        }
        return result;
    }

    std::string lowercase(const std::string& str) {
        auto result(str);
        std::for_each(result.begin(), result.end(),
                      [](char& c) { c = tolower(c); });
        return result;
    }

    /////////////////////////////////////////////////////////////////////////////
    // START OF CODE TO MODIFY
    /////////////////////////////////////////////////////////////////////////////

    struct sortFrequency
    {
        inline bool operator() (const word_frequency& word1, const word_frequency& word2)
        {
            return (word1.count() > word2.count());
        }
    };

    frequency_distribution
    most_frequent(
            const frequency_distribution& dist,
            size_t k
    ) noexcept {
        frequency_distribution res;
        for(int i=0;i<dist.size();i++)
        {
            if(res.size()<k)
            {
                res.push_back(dist[i]);
            }
            else
            {
                int pos=0;
                int min=res[0].count();
                for(int j=0;j<res.size();j++)
                {
                    if(res[j].count()<min)
                    {
                        pos=j;
                        min=res[j].count();
                    }
                }
                if(dist[i].count()>min)
                {
                    res[pos]=dist[i];
                }
            }
        }
        std::sort(res.begin(), res.end(), sortFrequency());
        return res;
    }

    frequency_distribution
    vector_word_counter::count(
            const word_vector& stop_words,
            const word_vector& document
    ) const noexcept {
        std::vector<std::string> stop;
        for(int i=0;i<stop_words.size();i++)
            stop.push_back(stop_words[i]);

        std::vector<std::string> words;
        std::vector<int> counts;
        for(int i=0;i<document.size();i++)
        {
            std::string str=lowercase(document[i]);
            int flg=0;
            for(int j=0;j<stop.size();j++)
            {
                if(str==stop[j])
                {
                    flg=1;
                    break;
                }
            }
            if(flg==1)
                continue;
            else
            {
                int flg1=0;
                for(int k=0;k<words.size();k++)
                {
                    if(str==words[k])
                    {
                        flg1=1;
                        counts[k]=counts[k]+1;
                        //if(counts[k]>1)
                        //  std::cout<<words[k];
                        break;
                    }
                }
                if(flg1==0)
                {
                    words.push_back(str);
                    counts.push_back(1);
                }
            }
        }
        frequency_distribution res;
        for(int i=0;i<words.size();i++)
        {
            word_frequency cur=word_frequency(words[i],counts[i]);
            res.push_back(cur);
        }
        // TODO: Write a definition for this function that actually
        // works. That will include deleting the current return statement,
        // which is just a placeholder to allow the code to compile. When
        // you are done, delete this TODO comment.
        return res;
        //return frequency_distribution();
    }

    frequency_distribution
    list_word_counter::count(
            const word_vector& stop_words,
            const word_vector& document
    ) const noexcept {
        std::list<std::string> stop;
        for(int i=0;i<stop_words.size();i++)
            stop.push_back(stop_words[i]);

        std::list<word_frequency> words;
        for(int i=0;i<document.size();i++)
        {
            std::string str=lowercase(document[i]);
            int flg=0;
            std::list<std::string>::iterator it;
            for(it=stop.begin();it!=stop.end();++it)
            {
                //std::cout<<*it;
                if(str==*it)
                {
                    flg=1;
                    break;
                }
            }
            if(flg==1)
                continue;
            else
            {
                int flg1=0;
                std::list<word_frequency>::iterator it;
                for(it=words.begin();it!=words.end();++it)
                {
                    word_frequency cur=*it;
                    if(str==cur.word())
                    {
                        //std::cout<<str;
                        cur.set_count(cur.count()+1);
                        *it=cur;
                        flg1=1;
                        break;
                    }
                }
                if(flg1==0)
                {
                    word_frequency next=word_frequency(str,1);
                    words.push_back(next);
                }
            }
        }
        frequency_distribution res;
        std::list<word_frequency>::iterator it;
        for(it=words.begin();it!=words.end();++it)
        {
            word_frequency cur=*it;
            res.push_back(cur);
        }

        return res;
    }

    frequency_distribution
    hash_word_counter::count(
            const word_vector& stop_words,
            const word_vector& document
    ) const noexcept {
        std::unordered_map<std::string,int> stop;
        std::unordered_map<std::string,int> words;
        for(int i=0;i<stop_words.size();i++)
            stop[stop_words[i]]=1;
        for(int i=0;i<document.size();i++)
        {
            std::string str=lowercase(document[i]);
            if(stop.find(str)==stop.end())
            {
                if(words.find(str)!=words.end())
                    words[str]=words[str]+1;
                else
                    words[str]=1;
            }
        }
        frequency_distribution res;
        std::unordered_map<std::string,int>::iterator it;
        for(it=words.begin();it!=words.end();it++)
        {
            word_frequency cur=word_frequency(it->first,it->second);
            res.push_back(cur);
        }


        // TODO: Write a definition for this function that actually
        // works. That will include deleting the current return statement,
        // which is just a placeholder to allow the code to compile. When
        // you are done, delete this TODO comment.
        return res;
    }

    struct Node{
        std::string word;
        int count;
        Node* left;
        Node* right;
    };


    void search(Node*& root,std::string key,Node**& _result){

        if(root==NULL){
            _result=NULL;
            return;
        }

        if(key<root->word){
            search(root->left,key,_result);
        }
        else if(key==root->word){
            _result=&root;
            return;
        }
        else{
            search(root->right,key,_result);
        }
    }



    void insert(Node*& root,std::string word1){
        if(root==NULL){
            root=new Node;
            root->left=NULL;
            root->right=NULL;
            root->word=word1;
            root->count=1;
            return;
        }

        if(word1>root->word){
            insert(root->right,word1);
        }
        else if(word1==root->word){
            root->count ++;
            return;
        }
        else{
            insert(root->left,word1);
        }
    }


    void copy(Node* root, frequency_distribution &res)
    {
        if(root==NULL)
            return;
        word_frequency cur=word_frequency(root->word,root->count);
        res.push_back(cur);
        std::cout<<cur.count();
        copy(root->left,res);
        copy(root->right,res);
    }



    frequency_distribution
    tree_word_counter::count(
            const word_vector& stop_words,
            const word_vector& document
    ) const noexcept {
        Node* root_stop=new Node;
        root_stop->left=NULL;
        root_stop->right=NULL;
        root_stop->word=stop_words[0];
        root_stop->count=1;
        for(int i=1;i<stop_words.size();i++)
            insert(root_stop,stop_words[i]);
        Node* root_words= NULL;
//        root_words->left=NULL;
//        root_words->right=NULL;
//        root_words->word=document[0];
//        root_words->count=1;
        for(int i=0;i<document.size();i++)
        {
            std::string str=lowercase(document[i]);
            Node** _result;
            search(root_stop,str, _result);
            if(_result!=NULL)
                continue;
            insert(root_words, str);
//            search(root_words,str,_result);
//            if(_result==0)
//                insert(root_words,str,1);
//            else
//            {
//                //std::cout<<(*_result)->count;
//                (*_result)->count=(*_result)->count+1;
//            }

        }
        frequency_distribution res;
        copy(root_words,res);
        return res;
    }

    /////////////////////////////////////////////////////////////////////////////
    // END OF CODE TO MODIFY
    /////////////////////////////////////////////////////////////////////////////

}
