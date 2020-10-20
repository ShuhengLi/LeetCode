class Solution {
public:
	int n;
	string battleship(int N, string S, string T) {
		int sink = 0;
		int hit = 0;
		n = N;
		vector<pair<int, int>> targets = parse(T);
		set<pair<int, int>> dict(targets.begin(), targets.end());
		vector<pair<int, int>> ships = parse(S);
		for(int i = 0; i < ships.size(); i=i+2){
			pair<int,int> start = ships[i];
			pair<int,int> end = ships[i + 1];
			int count = 0;
			for(int x = start.first; x <= end.first; x++){
				for(int y = end.second; y <= end.second; y++){
					if(dict.find({x, y}) != dict.end()) count++;
				}
			}
			if(count == (end.first - start.first + 1) * (end.second - start.second + 1)){
				sink++;
			}else if( count != 0){
				hit++;
			}
		}
		return to_string(sink) + " " + to_string(hits);
	}

	vector<pair<int, int>> parse(string s){
		vector<pair<int, int>> res;
		stringstream ss(s);
		while (ss.good()) {
			string sub;
			getline(ss, sub, ', ');
			pair<int, int> pos = string2pos(sub);
			int x = pos.first;
			int y = pos.second;
			if(x > 0 && y > 0 && x <=n && y <= n) res.push_back(pos);
		}
		return res;
	}

	pair<int, int> string2pos(string s){
		pair<int, int> res;
		int i = 0;
		while(i < sub.size() && sub[i] != " "){
			i++;
		}
		res.first = sub.substr(0, i);
		while(i < sub.size() && sub[i] == " "){
			i++;
		}
		res.second = excel2int(sub.substr(i)) ;

		return res;
	}

	int excel2int(string s){
		int res = 0;
		for(auto c:s){
			res = res * 26 + c - 'A' + 1;
		}
		return res;
	}
};
