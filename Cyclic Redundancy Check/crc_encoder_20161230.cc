#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#define sz(x) (int)x.size()
using namespace std;

int d_sz;
string gen;

string to_binary(int c) {
    string res = "";
    for (int i = 7; i >= 0; i--) {
        if ((1 << i) & c) res += '1';
        else res += '0';
    }
    return res;
}
string find_mod(string s) {
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
    return s.substr(sz(s) - sz(gen)+1, sz(gen) - 1);
}
string to_codeword(string s) {
    string tmp = s;
    for (int i = 1; i < sz(gen); i++) tmp += '0';
    string mod = find_mod(tmp);
    return s + mod;
}

int main(int argc, char* argv[]) {
    if (argc != 5) return cout << "usage: ./crc_encoder input_file output_file generator dataword_size", 0;
    ifstream ifp(argv[1]);
    ofstream ofp(argv[2]);
    if (!ifp.is_open()) return cout << "input file open error.", 0;
    if (!ofp.is_open()) return cout << "output file open error.", 0;
    if (!(strlen(argv[4]) == 1 && (argv[4][0] == '4' || argv[4][0] == '8'))) return cout << "dataword size must be 4 or 8.", 0;
    d_sz = argv[4][0] - '0';
    gen = argv[3];
    char c;
    string ret = "";
    while (ifp.get(c)) {
        string d_word = to_binary(c);
        if (d_sz == 4) {
            ret += to_codeword(d_word.substr(0, 4));
            ret += to_codeword(d_word.substr(4, 4));
        }
        else {
            ret += to_codeword(d_word);
        }
    }
    int len = sz(ret) % 8;
    if (len != 0) len = 8 - len;
    string pad = "";
    for (int i = 7; i >= 0; i--) {
        if ((1 << i) & len) pad += '1';
        else pad += '0';
    }
    for (int i = 1; i <= len; i++) pad += '0';
    ret = pad + ret;
    string tmp = "";
    for (int i = 0; i < sz(ret); i += 8) {
        char ch = 0;
        for (int j = 0; j < 8; j++) {
            if (ret[i + j] == '1') ch += (1 << (7 - j));
        }
        tmp += (char)ch;
    }
    ofp.write(tmp.c_str(), sz(tmp));

    ifp.close();
    ofp.close();
}
