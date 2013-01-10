//HEADER FILE TO DEFINE THE CLASS DFA AND ITS MEMBER FUNCTIONS.

#define ON_A 0
#define ON_B 1

class DFAState {
public :
	int final_state;
	int transition[2];
	int no_of_reachables;
	int reachables[10];
};

class DFA {
public :
	int no_of_states;
	DFAState state[20];

	void findReachables(NFA nfa, int i);
	void initialize();
	void rs_union(DFA temp);
	int isRepeatedState();
	void sortReachables();
	void markFinalState(int final_state);
	void displayDFA();
	void reduceDFA(int imp_states[], int final_state);
	int sameImportantStates(int x, int y, int imp_states[]);
	int sameFinalState(int x, int y, int final_state);
};

void DFA :: initialize() {
	no_of_states = 0;
	for(int i=0;i<20;i++) state[i].no_of_reachables = state[i].final_state = 0;
}

void DFA :: findReachables(NFA nfa, int i) {     //recursive function
	state[no_of_states].reachables[(state[no_of_states].no_of_reachables)++] = i;

	if(nfa.state[i].no_of_transitions==1) {
		if(nfa.state[i].transition_on[0]=='î')
			findReachables(nfa, nfa.state[i].transition_to[0]);
	}
	else if(nfa.state[i].no_of_transitions==2) {
		findReachables(nfa, nfa.state[i].transition_to[0]);
		findReachables(nfa, nfa.state[i].transition_to[1]);
	}
}

void DFA :: rs_union(DFA temp) {
	for(int i=0;i<temp.state[0].no_of_reachables;i++) {
		int flag = 0;
		for(int j=0;j<state[no_of_states].no_of_reachables;j++)
			if(temp.state[0].reachables[i]==state[no_of_states].reachables[j])
				flag = 1;
		if(!flag) {
			state[no_of_states].reachables[j] = temp.state[0].reachables[i];
			(state[no_of_states].no_of_reachables)++;
		}
	}
	sortReachables();
}

int DFA :: isRepeatedState() {
	for(int i=no_of_states-1;i>=0;i--) {
		if(state[i].no_of_reachables==state[no_of_states].no_of_reachables) {
			for(int j=0;j<state[i].no_of_reachables;j++) {
				if(state[no_of_states].reachables[j]!=state[i].reachables[j])
					break;
			}
			if(j==state[i].no_of_reachables) return i;
		}
	}
	return -1;
}

void DFA :: sortReachables() {           //Bubble Sort
	int i, j, temp, flag=0;

	for(j=0;j<(state[no_of_states].no_of_reachables-1);j++) {
		for(i=0;i<(state[no_of_states].no_of_reachables-j-1);i++) {
			if(state[no_of_states].reachables[i]>state[no_of_states].reachables[i+1]) {
				temp = state[no_of_states].reachables[i];
				state[no_of_states].reachables[i] = state[no_of_states].reachables[i+1];
				state[no_of_states].reachables[i+1] = temp;
				flag = 1;
			}
		}
		if(!flag) break;
		flag = 0;
	}
}

void DFA :: displayDFA() {
	cout << "\n\t\t\tDFA STATE TABLE\n";
	cout << "\n\tSTATE\t\ta\t\tb";
	cout << "\n\t\b-------------------------------------------";
	for(int i=0;i<no_of_states;i++) {
		cout << "\n";
		if(i==0) cout << "  ->";
		if(state[i].final_state) cout << "  Ÿ";
		cout << "\t  q" << i;

		if(state[i].transition[ON_A]==-1) cout << "\t\tí";
		else cout << "\t\tq" << state[i].transition[ON_A];
		if(state[i].transition[ON_B]==-1) cout << "\t\tí";
		else cout << "\t\tq" << state[i].transition[ON_B];
	}
}

void DFA :: reduceDFA(int imp_states[], int final_state) {
	int i, j, k, flag=1;

	while(flag) {
		flag = 0;
		for(i=1;i<no_of_states;i++) {
			for(j=i-1;j>=0;j--) if(sameImportantStates(i, j, imp_states)&&sameFinalState(i, j, final_state)) {
				flag = 1;
				break;
			}
			if(j!=-1) break;
		}
		if(flag) {
			for(k=i;k<no_of_states-1;k++) state[k] = state[k+1];
			no_of_states--;
			for(k=0;k<no_of_states;k++) {
				if(state[k].transition[ON_A]==i)
					state[k].transition[ON_A]=j;
				else if(state[k].transition[ON_A]>i)
					state[k].transition[ON_A]--;

				if(state[k].transition[ON_B]==i)
					state[k].transition[ON_B]=j;
				else if(state[k].transition[ON_B]>i)
					state[k].transition[ON_B]--;
			}
		}
	}
}

int DFA :: sameImportantStates(int x, int y, int imp_states[]) {
	int i, j, flag_x, flag_y;

	for(i=0;imp_states[i]!=-1;i++) {
		flag_x = flag_y = 0;
		for(j=0;j<state[x].no_of_reachables;j++) if(imp_states[i]==state[x].reachables[j]) {
			flag_x = 1;
			break;
		}
		for(j=0;j<state[y].no_of_reachables;j++) if(imp_states[i]==state[y].reachables[j]) {
			flag_y = 1;
			break;
		}
		if(flag_x != flag_y) return 0;
	}
	return 1;
}

void DFA :: markFinalState(int fin_state) {
	if(state[no_of_states].reachables[(state[no_of_states].no_of_reachables)-1]==fin_state)
		state[no_of_states].final_state = 1;
}

int DFA :: sameFinalState(int x, int y, int final_state) {
	int flag_x=0, flag_y=0;

	if(state[x].reachables[(state[x].no_of_reachables)-1]==final_state) flag_x = 1;
	if(state[y].reachables[(state[y].no_of_reachables)-1]==final_state) flag_y = 1;
	if(flag_x != flag_y) return 0;
	return 1;
}