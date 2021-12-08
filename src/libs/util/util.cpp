#include "util.h"

using namespace std;

string join(pair<uint32_t, uint32_t> p, const string& delimiter) {
    stringstream ss;
    ss << p.first << delimiter << p.second;
    return ss.str();
}

string join(const set<uint32_t>& s, const string& delimiter) {
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

string join(const unordered_set<uint32_t>& s, const string& delimiter) {
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

string join(const vector<uint32_t>& v, const string& delimiter) {
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

string join(const vector<string>& v, const string& delimiter) {
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

