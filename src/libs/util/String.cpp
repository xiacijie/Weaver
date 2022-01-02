#include "String.h"

using namespace std;

string util::join(pair<uint32_t, uint32_t> p, const string& delimiter) {
    stringstream ss;
    ss << p.first << delimiter << p.second;
    return ss.str();
}

string util::join(const set<uint32_t>& s, const string& delimiter) {
    stringstream ss;
    int index = 0;

    for (const auto& element : s) {
        ss << element;
        if (index != s.size()-1) {
            ss << delimiter;
        }
        index++;
    }
    return ss.str();
}

string util::join(const unordered_set<uint32_t>& s, const string& delimiter) {
    stringstream ss;
    int index = 0;

    for (const auto& element : s) {
        ss << element;
        if (index != s.size()-1) {
            ss << delimiter;
        }
        index++;
    }
    return ss.str();
}

string util::join(const vector<uint32_t>& v, const string& delimiter) {
    stringstream ss;
    int index = 0;

    for (const auto& element : v) {
        ss << element;
        if (index != v.size()-1) {
            ss << delimiter;
        }
        index++;
    }
    return ss.str();
}

string util::join(const vector<string>& v, const string& delimiter) {
    stringstream ss;
    int index = 0;

    for (const auto& element : v) {
        ss << element;
        if (index != v.size()-1) {
            ss << delimiter;
        }
        index++;
    }
    return ss.str();
}

vector<string> util::tokenize(const string &str, char delimiter) {
    size_t start;
    size_t end = 0;
    vector<string> out;

    while ((start = str.find_first_not_of(delimiter, end)) != std::string::npos)
    {
        end = str.find(delimiter, start);
        out.push_back(str.substr(start, end - start));
    }

    return out;
}
