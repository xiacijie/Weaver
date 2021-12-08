#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <vector>
#include <unordered_set>

using namespace std;

string join(pair<uint32_t, uint32_t> p, const string& delimiter);
string join(const set<uint32_t>& s, const string& delimiter);
string join(const unordered_set<uint32_t>& s, const string& delimiter);
string join(const vector<uint32_t>& v, const string& delimiter);
string join(const vector<string>& v, const string& delimiter);