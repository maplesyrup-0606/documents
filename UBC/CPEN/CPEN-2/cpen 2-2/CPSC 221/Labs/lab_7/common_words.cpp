/**
 * @file common_words.cpp
 * Implementation of the CommonWords class.
 *
 * @author Zach Widder
 * @date Fall 2014
 */

#include "common_words.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::endl;
using std::feof;

string remove_punct(const string& str)
{
    string ret;
    std::remove_copy_if(str.begin(), str.end(), std::back_inserter(ret),
                        [](int c) { return std::ispunct(c); });
    return ret;
}

CommonWords::CommonWords(const vector<string>& filenames)
{
    // initialize all member variables
    init_file_word_maps(filenames);
    init_common();
}

void CommonWords::init_file_word_maps(const vector<string>& filenames)
{
    // make the length of file_word_maps the same as the length of filenames
    file_word_maps.resize(filenames.size());

    // go through all files
    for (size_t i = 0; i < filenames.size(); i++) {
        // get the corresponding vector of words that represents the current file
        vector<string> words = file_to_vector(filenames[i]);
        // go through all the words in the file
        /* Your code goes here! */
        // std::map<string, unsigned int> current_map;

        // for( auto word : words){
        //     unsigned int count = 0;
        //     for( auto word_2 : words){
        //         if(word == word_2) count++;
        //     }
        //     if(current_map.find(word) == current_map.end()){
        //         current_map.insert(std::make_pair(word,count));
        //     }

        // }
        // file_word_maps.push_back(current_map);
        for(string& word: words){
            if(file_word_maps[i].find(word) == file_word_maps[i].end()) file_word_maps[i][word] = 0;
            file_word_maps[i][word]++;
        }
        
    }

    // file_word_maps.erase(std::remove_if(file_word_maps.begin(), file_word_maps.end(),
    //     [](const std::map<std::string, unsigned int>& m) { return m.empty(); }), file_word_maps.end());

    // for (const auto& map : file_word_maps) {
    //     for (const auto& pair : map) {
    //         std::cout << pair.first << ": " << pair.second << std::endl;
    //     }
    //     std::cout << std::endl;
    // }
}

void CommonWords::init_common()
{
    /* Your code goes here!  */
    
    // for (const auto& map : file_word_maps) {
    //     for(const auto& pair : map){
    //         // std::cout << "Current string " + pair.first << endl;
    //         unsigned int count = 0;
    //         for (const auto& map_2: file_word_maps){
    //             for(const auto& pair2 : map_2){
    //                 // std::cout << "Second string " + pair2.first << endl;
    //                 if(pair.first == pair2.first){
    //                     count++;
    //                 }
    //             }
    //             // std::cout << "count : " << endl; 
    //             // std::cout << count << endl;
    //             // std::cout << ""<< endl;
    //             if(common.find(pair.first) == common.end()){
    //                 common.insert(std::make_pair(pair.first,count));
    //             }
    //             if(common.find(pair.first) != common.end()){
    //                 if(common.find(pair.first)->second < count){
    //                     common[pair.first] = count;
    //                 }
    //             }
    //         }
    //     }
    // }
    for(auto& map : file_word_maps){
        for(auto& pair : map){
            if(common.find(pair.first) == common.end()) common[pair.first] = 0;
            common[pair.first]++;
        }
    }
    // std::cout << "common" << endl;
    // for (const auto& pair : common) {
    //         std::cout << pair.first << ": " << pair.second << std::endl;
    // }
    // std::cout << std::endl;
}

/**
 * @param n The number of times to word has to appear.
 * @return A vector of strings. The vector contains all words that appear
 * in each file >= n times.
 */
vector<string> CommonWords::get_common_words(unsigned int n) const
{
    vector<string> out;
    /* Your code goes here! */
    auto minim_map = file_word_maps[0];

    for(auto& m : file_word_maps){
        if(m.size() < minim_map.size()){
            minim_map = m;
        }
    }

    for(auto& kevindurant : minim_map){
        bool itTrue = true;

        for(auto& m : file_word_maps){
            auto it = m.find(kevindurant.first);
            if(it == m.end() || it->second < n){
                itTrue = false;
                break;
            }
        }

        if (itTrue) out.push_back(kevindurant.first);
    }

    return out;
}

/**
 * Takes a filename and transforms it to a vector of all words in that file.
 * @param filename The name of the file that will fill the vector
 */
vector<string> CommonWords::file_to_vector(const string& filename) const
{
    ifstream words(filename);
    vector<string> out;

    if (words.is_open()) {
        std::istream_iterator<string> word_iter(words);
        while (!words.eof()) {
            out.push_back(remove_punct(*word_iter));
            ++word_iter;
        }
    }
    return out;
}
