//HEADER FILE TO DEFINE THE CLASS NFA AND ITS MEMBER FUNCTIONS.

class NFAState {
public :
	int no_of_transitions;
	char transition_on[2];
	int transition_to[2];
};

class NFA {
public :
	int no_of_states;
	NFAState state[20];

	void initialize();
	void displayNFA();
};

void NFA :: initialize() {
	no_of_states = 0;
	for(int i=0;i<20;i++) state[i].no_of_transitions = 0;
}

void NFA :: displayNFA() {
	cout << "\n\t\t\tNFA STATE TABLE\n";
	cout << "\n\tSTATE\t\ta\t\tb\t\tî";
	cout << "\n\t\b-----------------------------------------------------";
	for(int i=0;i<no_of_states;i++) {
		cout << "\n";
		if(i==0) cout << "  ->";
		if(i==no_of_states-1) cout << "  Ÿ";
		cout << "\t  q" << i;

		if(state[i].no_of_transitions==1) {
			if(state[i].transition_on[0]=='a')
				cout << "\t\tq" << state[i].transition_to[0];
			else cout << "\t\t-";
			if(state[i].transition_on[0]=='b')
				cout << "\t\tq" << state[i].transition_to[0];
			else cout << "\t\t-";
			if(state[i].transition_on[0]=='î')
				cout << "\t\tq" << state[i].transition_to[0];
			else cout << "\t\t-";
		}
		else if(state[i].no_of_transitions==2) {
			cout << "\t\t-\t\t-\t\tq" << state[i].transition_to[0];
			cout << ", q" << state[i].transition_to[1];
		}
		else cout << "\t\t-\t\t-\t\t-";
	}
}