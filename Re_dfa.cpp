//PROGRAM TO CONVERT A GIVEN REGULAR EXPRESSION TO DFA (DETERMINISTIC
//FINITE AUTOMATA).

#include <iostream.h>
#include <process.h>
#include <conio.h>
#include "mystack.h"
#include "nfa.h"
#include "dfa.h"

//NOTE :- 'i', 'j' & 'k', wherever used, are used as loop counters.

void infixToPostfix(char input[], char output[]);
NFA makeNFA(char re[]);
DFA makeDFA(NFA nfa);
void findImportantStates(NFA nfa, int imp_states[]);

void main() {
	char infix_re[50], postfix_re[50];     //regular expression
	int important_states[20];

	clrscr();
	cout << "\n\n\n\t\tCONVERSION OF REGULAR EXPRESSION TO DFA\n";
	cout << "\nEnter the regular expression : ";
	cin >> infix_re;
	infixToPostfix(infix_re, postfix_re);
	cout << "\nRegular expression in postfix form : " << postfix_re;
	getch();
	clrscr();
	NFA nfa = makeNFA(postfix_re);
	nfa.displayNFA();
	getch();
	clrscr();
	DFA dfa = makeDFA(nfa);
	dfa.displayDFA();
	getch();
	for(int i=0;i<20;i++) important_states[i] = -1;
	findImportantStates(nfa, important_states);
	dfa.reduceDFA(important_states, nfa.no_of_states-1);
	cout << "\n\n\t\t\t    REDUCED";
	dfa.displayDFA();
	getch();

}

void infixToPostfix(char input[], char output[]) {
	MyStack <char> stack(50);
	int i, j=0;
	char c;

	for(i=0;input[i]!=0;i++) {
		switch(input[i]) {
		case 'a' :
		case 'b' :
			output[j++] = input[i];
			break;

		case '(' :
			stack.myPush(input[i]);
			break;

		case ')' :
			while((c=stack.myPop()) != '(') output[j++] = c;
			break;

		case '+' :
			if((stack.stackEmpty())||(stack.peep()=='('))
				stack.myPush('+');
			else {
				while((!stack.stackEmpty())&&(stack.peep()!='('))
					output[j++] = stack.myPop();
				stack.myPush(input[i]);
			}
			break;
		case '.' :
			if((stack.stackEmpty())||(stack.peep()=='(')||(stack.peep()=='+')) stack.myPush('.');
			else {
				while((!stack.stackEmpty())&&(stack.peep()!='(')&&(stack.peep()!='+'))
					output[j++] = stack.myPop();
				stack.myPush(input[i]);
			}
			break;
		case '*' :
			if((stack.stackEmpty())||(stack.peep()!='*')) stack.myPush('*');
			else {
				while((!stack.stackEmpty())&&(stack.peep()=='*'))
					output[j++] = stack.myPop();
				stack.myPush(input[i]);
			}
			break;

		default :
			cout << "\n\nInvalid Expression";
			getch();
			exit(0);
		}
	}
	while(!stack.stackEmpty()) output[j++] = stack.myPop();
	output[j++] = 0;
}

NFA makeNFA(char re[]) {
	MyStack <NFA> stack(20);
	NFA nfa, *nfa1, *nfa2;
	int i;		//counter

	for(int j=0;re[j]!=0;j++) switch(re[j]) {
	case 'a' :
	case 'b' :
		nfa.initialize();
		nfa.no_of_states = 2;
		nfa.state[0].no_of_transitions = 1;
		nfa.state[0].transition_on[0] = re[j];
		nfa.state[0].transition_to[0] = 1;
		stack.myPush(nfa);
		break;

	case '*' :
		nfa.initialize();
		nfa1 = &(stack.myPop());
		nfa.no_of_states = (*nfa1).no_of_states + 2;
		nfa.state[0].no_of_transitions = 2;
		nfa.state[0].transition_on[0] = 'î';
		nfa.state[0].transition_to[0] = 1;
		nfa.state[0].transition_on[1] = 'î';
		nfa.state[0].transition_to[1] = nfa.no_of_states - 1;
		for(i=0;i<(*nfa1).no_of_states;i++) {
			nfa.state[i+1].no_of_transitions = (*nfa1).state[i].no_of_transitions;
			nfa.state[i+1].transition_on[0] = (*nfa1).state[i].transition_on[0];
			nfa.state[i+1].transition_to[0] = (*nfa1).state[i].transition_to[0] + 1;
			nfa.state[i+1].transition_on[1] = (*nfa1).state[i].transition_on[1];
			nfa.state[i+1].transition_to[1] = (*nfa1).state[i].transition_to[1] + 1;
		}
		nfa.state[i].no_of_transitions = 2;
		nfa.state[i].transition_on[0] = 'î';
		nfa.state[i].transition_to[0] = 1;
		nfa.state[i].transition_on[1] = 'î';
		nfa.state[i].transition_to[1] = (*nfa1).no_of_states + 1;
		stack.myPush(nfa);
		break;

	case '.' :
		nfa.initialize();
		nfa2 = &(stack.myPop());
		nfa1 = &(stack.myPop());
		nfa.no_of_states = (*nfa1).no_of_states + (*nfa2).no_of_states;
		for(i=0;i<(*nfa1).no_of_states;i++) {
			nfa.state[i].no_of_transitions = (*nfa1).state[i].no_of_transitions;
			nfa.state[i].transition_on[0] = (*nfa1).state[i].transition_on[0];
			nfa.state[i].transition_to[0] = (*nfa1).state[i].transition_to[0];
			nfa.state[i].transition_on[1] = (*nfa1).state[i].transition_on[1];
			nfa.state[i].transition_to[1] = (*nfa1).state[i].transition_to[1];
		}
		nfa.state[(*nfa1).no_of_states-1].no_of_transitions = 1;
		nfa.state[(*nfa1).no_of_states-1].transition_on[0] = 'î';
		nfa.state[(*nfa1).no_of_states-1].transition_to[0] = (*nfa1).no_of_states;
		for(i=(*nfa1).no_of_states;i<(*nfa1).no_of_states+(*nfa2).no_of_states;i++) {
			nfa.state[i].no_of_transitions = (*nfa2).state[i-(*nfa1).no_of_states].no_of_transitions;
			nfa.state[i].transition_on[0] = (*nfa2).state[i-(*nfa1).no_of_states].transition_on[0];
			nfa.state[i].transition_to[0] = (*nfa2).state[i-(*nfa1).no_of_states].transition_to[0] + (*nfa1).no_of_states;
			nfa.state[i].transition_on[1] = (*nfa2).state[i-(*nfa1).no_of_states].transition_on[1];
			nfa.state[i].transition_to[1] = (*nfa2).state[i-(*nfa1).no_of_states].transition_to[1] + (*nfa1).no_of_states;
		}
		stack.myPush(nfa);
		break;

	case '+' :
		nfa.initialize();
		nfa2 = &(stack.myPop());
		nfa1 = &(stack.myPop());
		nfa.no_of_states = (*nfa1).no_of_states + (*nfa2).no_of_states + 2;
		nfa.state[0].no_of_transitions = 2;
		nfa.state[0].transition_on[0] = 'î';
		nfa.state[0].transition_to[0] = 1;
		nfa.state[0].transition_on[1] = 'î';
		nfa.state[0].transition_to[1] = (*nfa1).no_of_states + 1;
		for(i=0;i<(*nfa1).no_of_states;i++) {
			nfa.state[i+1].no_of_transitions = (*nfa1).state[i].no_of_transitions;
			nfa.state[i+1].transition_on[0] = (*nfa1).state[i].transition_on[0];
			nfa.state[i+1].transition_to[0] = (*nfa1).state[i].transition_to[0] + 1;
			nfa.state[i+1].transition_on[1] = (*nfa1).state[i].transition_on[1];
			nfa.state[i+1].transition_to[1] = (*nfa1).state[i].transition_to[1] + 1;
		}
		nfa.state[i].no_of_transitions = 1;
		nfa.state[i].transition_on[0] = 'î';
		nfa.state[i].transition_to[0] = nfa.no_of_states - 1;
		for(i=(*nfa1).no_of_states+1;i<(*nfa1).no_of_states+(*nfa2).no_of_states+1;i++) {
			nfa.state[i].no_of_transitions = (*nfa2).state[i-(*nfa1).no_of_states-1].no_of_transitions;
			nfa.state[i].transition_on[0] = (*nfa2).state[i-(*nfa1).no_of_states-1].transition_on[0];
			nfa.state[i].transition_to[0] = (*nfa2).state[i-(*nfa1).no_of_states-1].transition_to[0] + (*nfa1).no_of_states + 1;
			nfa.state[i].transition_on[1] = (*nfa2).state[i-(*nfa1).no_of_states-1].transition_on[1];
			nfa.state[i].transition_to[1] = (*nfa2).state[i-(*nfa1).no_of_states-1].transition_to[1] + (*nfa1).no_of_states + 1;
		}
		nfa.state[i-1].no_of_transitions = 1;
		nfa.state[i-1].transition_on[0] = 'î';
		nfa.state[i-1].transition_to[0] = nfa.no_of_states - 1;
		stack.myPush(nfa);
		break;
	}
	nfa1 = &(stack.myPop());
	return (*nfa1);
}

DFA makeDFA(NFA nfa) {
	MyStack <int> stack(20);
	DFA dfa, temp;
	int transition_on_a[10], transition_on_b[10];

	dfa.initialize();
	dfa.findReachables(nfa, 0);
	dfa.sortReachables();
	dfa.markFinalState(nfa.no_of_states-1);
	(dfa.no_of_states)++;
	stack.myPush(dfa.no_of_states);
	while(1) {
		if(stack.stackEmpty()) break;
		int original = stack.myPop();

		//consider symbol a
		int len=0;
		for(int i=0;i<dfa.state[original-1].no_of_reachables;i++)
			if(nfa.state[dfa.state[original-1].reachables[i]].transition_on[0] == 'a')
				transition_on_a[len++] = nfa.state[dfa.state[original-1].reachables[i]].transition_to[0];
		for(i=0;i<len;i++) {
			temp.initialize();
			temp.findReachables(nfa, transition_on_a[i]);
			temp.sortReachables();
			dfa.rs_union(temp);
		}
		if(dfa.isRepeatedState()==-1) {
			if(!len) dfa.state[original-1].transition[ON_A] = -1;
			else {
				dfa.markFinalState(nfa.no_of_states-1);
				(dfa.no_of_states)++;
				dfa.state[original-1].transition[ON_A] = dfa.no_of_states - 1;
				stack.myPush(dfa.no_of_states);
			}
		}
		else {
			dfa.state[original-1].transition[ON_A] = dfa.isRepeatedState();
			dfa.state[dfa.no_of_states].no_of_reachables = 0;
		}

		//consider symbol b
		len=0;
		for(i=0;i<dfa.state[original-1].no_of_reachables;i++)
			if(nfa.state[dfa.state[original-1].reachables[i]].transition_on[0] == 'b')
				transition_on_b[len++] = nfa.state[dfa.state[original-1].reachables[i]].transition_to[0];
		for(i=0;i<len;i++) {
			temp.initialize();
			temp.findReachables(nfa, transition_on_b[i]);
			temp.sortReachables();
			dfa.rs_union(temp);
		}
		if(dfa.isRepeatedState()==-1) {
			if(!len) dfa.state[original-1].transition[ON_B] = -1;
			else {
				dfa.markFinalState(nfa.no_of_states-1);
				(dfa.no_of_states)++;
				dfa.state[original-1].transition[ON_B] = dfa.no_of_states - 1;
				stack.myPush(dfa.no_of_states);
			}
		}
		else {
			dfa.state[original-1].transition[ON_B] = dfa.isRepeatedState();
			dfa.state[dfa.no_of_states].no_of_reachables = 0;
		}
	}
	return dfa;
}

void findImportantStates(NFA nfa, int imp_states[]) {
	int len=0;
	for(int i=0;i<nfa.no_of_states;i++)
		if((nfa.state[i].no_of_transitions==1)&&(nfa.state[i].transition_on[0]!='î'))
			imp_states[len++] = i;
}