#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <set>

int syllable_count(std::string str) {
    int syllable_count = 0;
    for (auto pos = str.begin(); pos != str.end(); ++pos) {
        if (*pos == '0' || *pos == '1' || *pos == '2') {
            syllable_count++;
        }
    }
    return syllable_count;
}

std::string final_pattern(std::string str) {
    char space = ' ';
    int stress_ind;
    int one_ind = str.rfind('1');
    int two_ind = str.rfind('2');
    if (one_ind == -1) {
        stress_ind = two_ind;
    } else if (two_ind == -1) {
        stress_ind = one_ind;
    } else {
        stress_ind = std::max(one_ind, two_ind);
    }
    std::string final_pattern = str.substr(str.substr(0, stress_ind + 1).rfind(space) + 1);
    return final_pattern;
}

int main(int argc, char** argv) {
    std::string query = argv[1];
    std::string sec_arg;
    std::unordered_map<std::string, int> q_prons;
    if (argv[2]) {
        sec_arg = argv[2];
    }
    std::transform(query.begin(), query.end(), query.begin(), ::tolower);
    std::unordered_map<std::string, std::map<std::string, int>> CMUmap;
    char space = ' ';
    std::ifstream CMU("/srv/datasets/cmudict/cmudict.dict");
    std::string line;
    std::string current_pattern;
    while (std::getline(CMU, line)) {
        int space_ind = line.find(space);
        std::string word = line.substr(0, space_ind);
        std::string pron = line.substr(space_ind + 1);
        if (word.substr(0, word.find('(')) == query) {
            current_pattern = final_pattern(pron);
            q_prons[final_pattern(pron)] = syllable_count(pron);
        }
        if (final_pattern(pron) == current_pattern)
            CMUmap[current_pattern][word] = syllable_count(pron);
    }
    std::set<std::string> words;
    for (auto const &[q_pron, q_count] : q_prons) {
        for (auto const &[word, count] : CMUmap[q_pron]) {
            std::string word_sf = word.substr(0, word.find('('));
            int sf_count = words.count(word_sf);
            if (sf_count == 0 && sec_arg == "-a") {
                words.insert(word_sf);
            } else if (sf_count == 0 && count == q_count) {
                words.insert(word_sf);
            }
        }
    }
    for (auto const &word : words)
        std::cout << word << '\n';
    return 0;
}