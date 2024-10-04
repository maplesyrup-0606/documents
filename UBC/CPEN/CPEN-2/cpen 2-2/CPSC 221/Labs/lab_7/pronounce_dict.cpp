/**
 * @file pronounce_dict.cpp
 * Implementation of the PronounceDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "pronounce_dict.h"

#include <iterator>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>

using std::string;
using std::map;
using std::vector;
using std::ifstream;
using std::istream;
using std::istream_iterator;
using std::stringstream;

/**
 * Constructs a PronounceDict from a CMU pronunciation dictionary
 * file. See http://www.speech.cs.cmu.edu/cgi-bin/cmudict .
 * @param pronun_dict_filename Filename of the CMU pronunciation
 * dictionary.
 */
PronounceDict::PronounceDict(const string& pronun_dict_filename)
{
    ifstream pronun_dict_file(pronun_dict_filename);
    string line;
    if (pronun_dict_file.is_open()) {
        while (getline(pronun_dict_file, line)) {
            /* Used to break the line by whitespace. The CMU Dict does this for
             * separating words from their pronunciations. */
            stringstream line_ss(line);
            istream_iterator<string> line_begin(line_ss);
            istream_iterator<string> line_end;
            if (line[0] != '#' && *line_begin != ";;;") {
                /* Associate the word with the rest of the line
                 * (its pronunciation). */
                auto temp_itr = line_begin;
                dict[*temp_itr] = vector<string>(++line_begin, line_end);
            }
        }
    }
    /* If it's not open then... well... just don't do anything for the sake
     * of simplicity. */
}

/**
 * Constructs a PronounceDict from a CMU std::map mapping the word
 * to a vector of strings which represent the pronunciation.
 * @param pronun_dict Maps a string word to a vector of strings
 * representing its pronunciation.
 */
PronounceDict::PronounceDict(const map<string, vector<string>>& pronun_dict)
    : dict(pronun_dict)
{
    /* Nothing to see here. */
}

/**
 * Uses the dictionary to determine if the two words are homophones.
 * @param word1 First word to be tested.
 * @param word2 Second word to be tested.
 * @return true if the two words are homophones, false otherwise (or
 * one or both words weren't in the dictionary).
 * Note: The word keys in the dictionary are stored in uppercase.
 * Note: word1 and word2 should **not** be modified after the
 *       function ends.
 */
bool PronounceDict::homophones(const string& word1, const string& word2) const
{
    /* Your code goes here! */
    string copy_word1 = word1;
    string copy_word2 = word2;
    std::transform(copy_word1.begin(), copy_word1.end(), copy_word1.begin(), ::toupper);
    std::transform(copy_word2.begin(), copy_word2.end(), copy_word2.begin(), ::toupper);
    

    // std::cout << "Word1: " + word1 + " "  + "Word2: " + word2 << std::endl;
    auto pronoun_1 = dict.find(copy_word1);
    auto pronoun_2 = dict.find(copy_word2);

    if(pronoun_1 == dict.end() || pronoun_2 == dict.end()) {
        // std::cout << "CANT FIND!" << std::endl;
        return false;
    }
    else{
        if(pronoun_1->second == pronoun_2->second) {
            // std::cout << "TRUE" << std::endl;
            return true;
        }
        else{
            // std::cout << "FALSE" << std::endl;
            return false;
        }
    }

    return false;
}
