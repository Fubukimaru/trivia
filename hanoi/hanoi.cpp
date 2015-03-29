#include <iostream>
#include <stack>          // std::stack
#include <vector>
#include <unistd.h>

using namespace std;

int MAX_N = 9;
int SLEEP_MS = 0; //0ms by default

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

void waitNextFrame(){
	usleep(SLEEP_MS*1000);
}

void printStacks(vector<stack<int> > towers, bool wait = true) {
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
	//Wait for next "frame"
	if (wait) waitNextFrame();
}

bool passConstraints(vector<stack<int> > &t, int o, int d) {
	if (t[o].empty()) return false;
	if (!t[d].empty() and t[o].top() > t[d].top()) return false;	
	return true;
}

bool move(vector<stack<int> > &t, int o, int d, bool wait=true) {
	if (passConstraints(t,o,d)) {
		t[d].push(t[o].top());
		t[o].pop();
		printStacks(t,wait);
		return true;
	}
	return false;
}


void resolveRecursive(vector<stack<int> > &t, int s, int d, int aux, int N) {
	if (N == 1) move(t,s,d);
	else {
		resolveRecursive(t, s, aux, d, N-1);
		move(t,s,d);
		resolveRecursive(t,aux,d,s,N-1);
	}
}

void resolveRecursiveStart(vector<stack<int> > t) {
	if (t.size() != 3) cout << "Algorithm not applicable for the number of towers used." << endl;
	else {
		cout << "Starting recursive solution" << endl;
		resolveRecursive(t,0,2,1,MAX_N);
	}
}

void resolveSimple(vector<stack<int> > t) {	
	if (t.size() != 3) cout << "Algorithm not applicable for the number of towers used." << endl;
	else {
		int a, b, c;
		a = 0; b = 1; c = 2;
		//2^n-1 moves
		//For evens
		if (MAX_N%2 == 0) {
			cout << "EVENS CASE!" << endl;
			while(t[c].size() != MAX_N) {     
				//make the legal move between pegs A and B
				move(t,a,b);
				//make the legal move between pegs A and C
				move(t,a,c);
				//make the legal move between pegs B and C
				move(t,b,c);
			}
		}
		else {
		//For odds
			cout << "ODDS CASE!" << endl;
			while(t[c].size() != MAX_N) {				
				//make the legal move between pegs A and C
				move(t,a,c);
				//make the legal move between pegs A and B
				move(t,a,b);
				//make the legal move between pegs C and B
				move(t,c,b);
			}

		}
	}
}


void resolvePivot(vector<stack<int> > t) {	
	int nTowers=t.size();
	if (nTowers != 3) {
		cout << "Algorithm not applicable for the number of towers used." << endl;
		return;
	}
	int smallPosition=0; //position of the small piece
	while(t[nTowers-1].size() != MAX_N) {  
		//Move small to the left
		int smNext = smallPosition-1;
		if (smNext < 0) smNext = nTowers-1;

		move(t,smallPosition,smNext); //move to the left

		//Find the only legal move that doesn't include the small one
		int other = smNext-1;
		if (other < 0) other = nTowers-1;
		//Small piece is in smNext, so we will use other and smallPosition (which is the old position)
		if (!move(t,smallPosition, other)) move(t, other, smallPosition); //If it's not old small -> other, try inverse.		

		smallPosition=smNext;
	}
}

void playHanoi(vector<stack<int> > t) {
	int o,d;
	int nTowers = t.size();
	int destination = nTowers-1; //Last tower is the destination.
	printStacks(t,false);
	while (t[destination].size()!=MAX_N) {
		cin >> o >> d;
		--o; --d;
		if (o < nTowers and d < nTowers and 0 <= o and 0 <= d) {
			if (!move(t,o,d,false)) cout << "Input doesn't fulfill constraints" << endl;
			//printStacks(t,false); //Print with no delay
		}
		else cout << "Wrong input" << endl;	
	}

}


inline void selectSpeed() {
	int speed;
	cout << "Select speed: (1) Normal, (2) Fast or (any other key) FUCKING FAST" << endl;
	cin >> speed;
	switch(speed) {
		case 1: //Normal (1s)
			SLEEP_MS = 1000;
			break;
		case 2: //Fast (50ms)
			SLEEP_MS = 50;
			break;
		default: //Otherwise... FUCKING FAST
			break;
	}
}

int main(){
	int N_TOWERS=3;
	vector<stack<int> > towers(N_TOWERS);
	for (int i = MAX_N; i > 0; --i) {
		towers[0].push(i);
	}

	cout << "Select mode: (1) Play, (2) Recursive resolution, (3) Simple iterative resolution (small as pivot). Other keys -> quit" << endl;
	int mode;
	cin >> mode;
	switch(mode) {
		case 1:
			playHanoi(towers);
			cout << ":) ";
			break;
		case 2:
			selectSpeed();
			resolveRecursiveStart(towers);
			cout << ":) ";
			break;
		case 3:
			selectSpeed();
			resolvePivot(towers);
			cout << ":) ";
			break;
		default:
			cout << ":( ";	
	}
	cout << "Bye!" << endl;	
}


