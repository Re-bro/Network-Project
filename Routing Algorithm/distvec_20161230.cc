//distance vector
#include<iostream>
#include<algorithm>
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<queue>
using namespace std;
using pii = pair<int, int>;

int table[105][105][2]; //0 : next, 1 : dist
int adj[105][105];
int n;

void dv_func() {
	queue<int> q;
	for (int i = 0; i < n; i++) q.push(i);
	while (!q.empty()) {
		int u = q.front(); q.pop();
		for (int v = 0; v < n; v++) {
			if (v == u || adj[u][v] == -999) continue;
			bool updated = false;
			for (int i = 0; i < n; i++) {
				if (table[u][i][1] == -999) continue;
				int d = table[u][i][1] + adj[u][v];
				if (table[v][i][1] == -999 || table[v][i][1] > d || (table[v][i][1] == d && table[v][i][0] > u)) {
					table[v][i][1] = d;
					table[v][i][0] = u;
					updated = true;
				}
			}
			if (updated) q.push(v);
		}
	}
}
void init_table() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			table[i][j][1] = adj[i][j];
			if (adj[i][j] >= 0) {
				table[i][j][0] = j;
			}
		}
	}
}
int main(int argc, char* argv[]) {
	if (argc != 4) return cout << "usage: distvec topologyfile messagesfile changesfile", 0;
	ifstream top(argv[1]);
	ifstream mes(argv[2]);
	ifstream cha(argv[3]);
	if (!top.is_open() || !mes.is_open() || !cha.is_open()) return cout << "Error: open input file.", 0;
	ofstream ofp("output_dv.txt");
	top >> n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j) adj[i][j] = -999;
		}
	}
	int u, v, c;
	while(top >> u >> v >> c){
		adj[u][v] = adj[v][u] = c;
	}
	vector<pair<pii, string>> messages;
	while (mes >> u >> v) {
		string s; getline(mes, s);
		messages.push_back({ {u, v}, s });
	}

	vector<pair<pii, int>> changes;
	while (cha >> u >> v >> c) {
		changes.push_back({ {u, v}, c });
	}
	for (int i = 0; i <= changes.size(); i++) {
		init_table();
		dv_func();
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				if(table[j][k][1] == -999) continue;
				ofp << k << ' ' << table[j][k][0] << ' ' << table[j][k][1] << '\n';
			}
			ofp << '\n';
		}
		for (int j = 0; j < messages.size(); j++) {
			int u = messages[j].first.first;
			int v = messages[j].first.second;
			string s = messages[j].second;
			ofp << "from " << u << " to " << v << " cost ";
		        if(table[u][v][1] == -999) ofp<<"infinite hops unreachable ";
			else{	
				ofp<< table[u][v][1] << " hops ";
				ofp << u << ' ';
				while (table[u][v][0] != v) {
					ofp << table[u][v][0] << ' ';
					u = table[u][v][0];
				}
			}
			ofp << "message" << s << '\n';
		}
		ofp << '\n';

		if (i == changes.size()) break;
		int u = changes[i].first.first;
		int v = changes[i].first.second;
		int c = changes[i].second;
		adj[u][v] = adj[v][u] = c;
	}
	cout << "Complete. Output file written to output_dv.txt.";
}
