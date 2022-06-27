//link state
#include<iostream>
#include<algorithm>
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<queue>
using namespace std;
using pii = pair<int, int>;

int adj[105][105];
int table[105][105][3]; //0 : next, 1 : dist, 2 : parent
int n;
void dijkstra() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			table[i][j][1] = table[i][j][2] = -999;
		}
	}
	for (int i = 0; i < n; i++) {
		priority_queue<pair<int, int>> pq;
		pq.push({ 0, -i });
		table[i][i][0] = i;
		table[i][i][1] = 0;
		table[i][i][2] = i;
		while (!pq.empty()) {
			int d = -pq.top().first;
			int u = -pq.top().second;
			pq.pop();
			if (d > table[i][u][1]) continue;
			for (int v = 0; v < n; v++) {
				if(v == u) continue;
				int c = adj[u][v];
				if (c == -999) continue;
				if (table[i][v][1] == -999 || table[i][v][1] > d + c || (table[i][v][1] == d + c && table[i][v][2] > u)) {
					table[i][v][1] = d + c;
					table[i][v][2] = u;
					pq.push({ -table[i][v][1], -v });
				}
			}
		}
	}
}
void find_next() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j || table[i][j][1] == -999) continue;
			int u = j;
			while (table[i][u][2] != i) u = table[i][u][2];
			table[i][j][0] = u;
		}
	}
}
int main(int argc, char* argv[]) {
	if (argc != 4) return cout << "usage: linkstate topologyfile messagesfile changesfile", 0;
	ifstream top(argv[1]);
	ifstream mes(argv[2]);
	ifstream cha(argv[3]);	
	if (!top.is_open() || !mes.is_open() || !cha.is_open()) return cout << "Error: open input file.", 0;
	ofstream ofp("output_ls.txt");
	top >> n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if(i != j) adj[i][j] = -999;
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
		dijkstra();
		find_next();
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
		 	if(table[u][v][1] == -999){
				ofp<<"infinite hops unreachable ";
			}
			else{	
				ofp<< table[u][v][1] << " hops ";
				ofp << u <<' ';
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
	cout << "Complete. Output file written to output_ls.txt.";
}
