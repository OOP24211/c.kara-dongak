#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <locale>
#include <algorithm>
#include <map>

using namespace std;

string toLower(const string& str) {
    string result = str;
    for (char& c : result) {
        unsigned char uc = (unsigned char)c;

        if (uc >= 'A' && uc <= 'Z') {
            c = tolower(c);
        }

        else if (uc >= 192 && uc <= 223) {
            c = static_cast<char>(uc + 32);
        }

        else if (uc == 168) {
            c = static_cast<char>(184);
        }
    }
    return result;
}

string cleanWord(const string& word) {
    string cleaned;
    for (unsigned char c : word) {
        if (isalnum(c) || (c >= 128 && c <= 255)) {
            cleaned += c;
        }
    }
    return cleaned;
}

pair<vector<string>, string> processBuffer(const string& buffer, const string& leftover) {
    vector<string> words;
    string currentWord = leftover;
    string newLeftover;

    for (char c : buffer) {
        if (isspace(c) || ispunct(c)) {
            if (!currentWord.empty()) {
                string cleaned = cleanWord(currentWord);
                if (!cleaned.empty()) {
                    words.push_back(cleaned);
                }
                currentWord.clear();
            }
        }
        else {
            currentWord += c;
        }
    }
    newLeftover = currentWord;
    return {words, newLeftover};
}

int main() {
    setlocale(LC_ALL, "Russian");

    ifstream input_file("input.txt", ios::binary);
    ofstream output_file("output.csv");

    output_file << "Слово,Частота,Частота(%)" << endl;

    if (!input_file.is_open()) {
        cerr << "The file cannot be opened" << endl;
        return 1;
    }

    cout << "The file has been opened successfully" << endl;

    const int BUFFER_SIZE = 1024;
    vector<char> buffer(BUFFER_SIZE);
    string leftover;
    vector<string> allWords;

    int bufferCount = 0;
    while (input_file) {
        input_file.read(buffer.data(), BUFFER_SIZE);
        streamsize bytesRead = input_file.gcount();

        if (bytesRead > 0) {
            string chunk(buffer.data(), bytesRead);
            bufferCount++;

            auto [words, newLeftover] = processBuffer(chunk, leftover);

            allWords.insert(allWords.end(), words.begin(), words.end());
            leftover = newLeftover;
        }
    }
    input_file.close();


    if (!leftover.empty()) {
        string cleaned = cleanWord(leftover);
        if (!cleaned.empty()) {
            allWords.push_back(cleaned);
        }
    }

    map<string, int> wordCount;
    for (const auto& word : allWords) {
        string lowerWord = toLower(word);
        wordCount[lowerWord]++;
    }

    vector<pair<string, int>> sortedWords(wordCount.begin(), wordCount.end());
    sort(sortedWords.begin(), sortedWords.end(), [](const auto& a, auto& b) {
        return a.second > b.second;
    });


    for (const auto& [word, freq] : sortedWords) {
        double percentage = (freq * 100.0) / allWords.size();
        output_file << word << "," << freq << "," << percentage << "%" << endl;
    }

    output_file.close();

    return 0;
}