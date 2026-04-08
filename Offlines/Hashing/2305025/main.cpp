#include "HashTable.h"
#include <vector>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

int main() {
    const int NUM_WORDS = 10000;
    const int WORD_LENGTH = 10;
    const int SEARCH_COUNT = 1000;

    cout << "=== Hash Table Performance Evaluation ===" << endl;
    cout << "Generating " << NUM_WORDS << " unique " << WORD_LENGTH << "-letter words..." << endl;

    WordGenerator generator;
    vector<string> words;
    words.reserve(NUM_WORDS);
    for (int i = 0; i < NUM_WORDS; i++) {
        words.push_back(generator.generateWord(WORD_LENGTH));
    }
    cout << "Generated " << words.size() << " unique words" << endl << endl;

    vector<string> searchKeys = words;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(searchKeys.begin(), searchKeys.end(), g);
    searchKeys.resize(SEARCH_COUNT);




    vector<vector<pair<long long, double>>> results(3, vector<pair<long long, double>>(2));

    for (int methodIdx = 0; methodIdx < 3; methodIdx++) {
        CollisionMethod method = static_cast<CollisionMethod>(methodIdx);

        for (int hashType = 1; hashType <= 2; hashType++) {
            HashTable<int> table(method, hashType);


            for (int i = 0; i < NUM_WORDS; i++) {
                table.insert(words[i], i + 1);
            }

            long long collisions = table.getCollisions();

            table.resetStatistics();

            int foundCount = 0;
            for (const auto& key : searchKeys) {
                int val;
                if (table.search(key, val)) foundCount++;
            }

            results[methodIdx][hashType - 1] = { collisions, table.getAverageProbes() };

            cout << "Finished Method " << methodIdx << " Hash " << hashType << endl;
        }
    }


    cout << "\n _________________________________________________________________________________________________\n";
    cout << "|                    |                                     |                                     |\n";
    cout << "|                    |                Hash1                |                Hash2                |\n";
    cout << "|                    |_____________________________________|_____________________________________|\n";
    cout << "|                    | Number of Collisions | Average Hits | Number of Collisions | Average Hits |\n";
    cout << "|____________________|______________________|______________|______________________|______________|\n";

    vector<string> names = {"Chaining Method", "Double Hashing", "Custom Probing"};

    for(int i=0; i<3; i++) {
        cout << "| " << left << setw(19) << names[i] << "|";

        cout << right << setw(21) << results[i][0].first << " | "
             << left << setw(13) << fixed << setprecision(3) << results[i][0].second << "|";

        cout << right << setw(21) << results[i][1].first << " | "
             << left << setw(13) << fixed << setprecision(3) << results[i][1].second << "|\n";

        cout << "|____________________|______________________|______________|______________________|______________|\n";
    }

    cout << endl;
    return 0;
}
