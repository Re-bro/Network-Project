#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#define sz(x) (int)x.size()

using namespace std;
int d_sz;
string gen;
bool flag;
string mod(string s) {
    string res = s.substr(0, sz(s) - sz(gen) + 1);
    string tmp = s;
    flag = true;
    for (int i = 0; i <= sz(s) - sz(gen); i++) {
        if (s[i] == gen[0]) {
            for (int j = 1; j < sz(gen); j++) {
                if (gen[j] == '1') {
                    if (s[i + j] == '1') s[i + j] = '0';
                    else s[i + j] = '1';
                }
            }
        }
    }
    for (int i = sz(s) - sz(gen) + 1; i < sz(s); i++) {
        if (s[i] == '1') {
		    flag = false;
	    }
    }
    return res;
}

int main(int argc, char* argv[]) {
    if (argc != 6) return cout << "usage: ./crc_decoder input_file output_file result_file generator dataword_size", 0;
    ifstream ifp(argv[1]);
    ofstream ofp1(argv[2]);
    ofstream ofp2(argv[3]);

    if (!ifp.is_open()) return cout << "input file open error.", 0;
    if (!ofp1.is_open()) return cout << "output file open error.", 0;
    if (!ofp2.is_open()) return cout << "result file open error.", 0;
    if (!(strlen(argv[5]) == 1 && (argv[5][0] == '4' || argv[5][0] == '8'))) return cout << "dataword size must be 4 or 8.", 0;

    char c;
    ifp.get(c);
    int p = (int)c;
    string in = "";
    while (ifp.get(c)) {
	    for(int i = 7; i>=0; i--){
		    if(c & (1<<i)) in += '1';
		    else in += '0';
	    }
    }
    d_sz = argv[5][0] - '0';
    gen = argv[4];
    int cnt = 0;
    int err = 0;
    string res = "";
    string tmp = "";
    for (int i = p; i < sz(in); i += sz(gen) + d_sz - 1) {
        string c_word = "";
        cnt++;
        for (int j = 0; j < sz(gen) + d_sz - 1; j++) c_word += in[i + j];
        tmp += mod(c_word);
	    if(sz(tmp) == 8){
		    char ch = 0;
            for (int i = 0; i < 8; i++) {
                if (tmp[i] == '1') ch += (1 << (7 - i));
            }
		    res += (char)ch;
		    tmp = "";
	    }
        if (!flag) err++;
    }
    ofp1.write(res.c_str(), sz(res));
    res = "";
    while (err >= 0) {
        res = (char)((err % 10) + '0') + res;
        err /= 10;
	    if(err == 0) break;
    }
    res = ' ' + res;
    while (cnt >= 0) {
        res = (char)((cnt % 10) + '0') + res;
        cnt /= 10;
	    if(cnt == 0) break;
    }
    ofp2.write(res.c_str(), sz(res));
}
