#include <iostream>
#include <stack>          // std::stack
#include <vector>

using namespace std;

int MAX_N = 9;

void printEmptyRow() {
	for(int i = 0; i < 2*MAX_N; ++i) {
		cout << " ";
	}
}

void printStackLine(stack<int> &p) {

	//cout << "Current Top  " << p.top() << endl;
	int currTop = p.top();
		
	for(int i = 0; i < MAX_N-currTop; ++i) {
		cout << " ";
	}
	for(int i = 0; i < 2*currTop; ++i) {
                cout << "#";  
        }
	for(int i = 0; i < MAX_N-currTop; ++i) {
                cout << " ";  
        }
        p.pop();
}

void printStacks(vector<stack<int> > towers) {
	vector<int> nEmptyRows(towers.size(),0);
	for (int j = 0; j < towers.size(); ++j) {
		nEmptyRows[j] = MAX_N - towers[j].size();	
	}	

	for (int round = 0; round < MAX_N; ++round) {
		for (int j = 0; j < towers.size(); ++j) {
			//cout << "round " << round << endl;
			if (0 < nEmptyRows[j]) {
				printEmptyRow();
				--nEmptyRows[j];
			}
			else printStackLine(towers[j]);
		
			cout << " ";
		}		
		cout << endl;
        }

	//Floor
	for (int j = 0; j < towers.size(); ++j) {
		for (int i = 0; i < 2*MAX_N; ++i) cout << "_";
		cout << " ";
	}
	cout << endl;
}

bool passConstraints(vector<stack<int> > &t, int o, int d) {
	if (t[o].empty()) return false;
	if (!t[d].empty()){
		if (t[o].top() > t[d].top()) return false;
	}
	return true;
}

int main(){
	int N_TOWERS=3;
	vector<stack<int> > towers(N_TOWERS);
	for (int i = MAX_N; i > 0; --i) {
		towers[0].push(i);
	}
	printStacks(towers);
	//towers[1].push(towers[0].top());
	//towers[0].pop();
	//printStacks(towers);
	int o,d;
	while (cin >> o >> d) {
		--o; --d;
		if (o < towers.size() and d < towers.size() and 0 <= o and 0 <= d) {
			if (passConstraints(towers,o,d)) {
				towers[d].push(towers[o].top());
				towers[o].pop();
			}
			else cout << "Input doesn't fulfill constraints" << endl;
			printStacks(towers);
		}
		else cout << "Wrong input" << endl;
	
	}
}

