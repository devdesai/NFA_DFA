//HEADER FILE DEFINING THE CLASS STACK AND ITS MEMBER FUNCTIONS.

template <class T> class MyStack {
	T *array;
	int top, max;
public :
	MyStack();              //overloaded constructor
	MyStack(int i);         //overloaded constructor with parameter
	void myPush(T data);
	T myPop();
	T peep();
	int stackFull();
	int stackEmpty();
};

template <class T> MyStack<T> :: MyStack() {
	array = new T[1];
	top = 0;
	max = 1;
}

template <class T> MyStack<T> :: MyStack(int i) {
	array = new T[i];
	top = 0;
	max = i;
}

template <class T> void MyStack<T> :: myPush(T data) {
	array[top++] = data;
}

template <class T> T MyStack<T> :: myPop() {
	return array[--top];
}

template <class T> T MyStack<T> :: peep() {
	return array[top-1];
}

template <class T> int MyStack<T> :: stackFull() {
	if(top == max) return 1;
	return 0;
}

template <class T> int MyStack<T> :: stackEmpty() {
	if(!top) return 1;
	return 0;
}