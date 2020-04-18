//816022438

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string.h>
using namespace std;

struct Node{
	char data;
	Node *next;
};

struct Stack{
	Node *top;
};

struct Queue{
	Node *front;
	Node *rear;
};

struct NodeDouble{
	double data;
	NodeDouble *next;
};

struct StackDouble{
	NodeDouble *top;
};

NodeDouble *createNodeDouble(double n){
	NodeDouble *newNode=(NodeDouble*)malloc(sizeof(NodeDouble));
	newNode->data=n;
	newNode->next=NULL;
	return newNode;
}

NodeDouble *insertAtHeadDouble(NodeDouble *top, double n){
	NodeDouble *newNode=createNodeDouble(n);
	newNode->next=top;
	top=newNode;
	
	return top;
}

NodeDouble *deleteAtHeadDouble(NodeDouble *top){
	NodeDouble *curr=top;
	if(top==NULL)
		return NULL;
	top=top->next;
	free(curr);
	return top;
}

bool isEmptyDouble(StackDouble *s){
	return(s->top==NULL);
}

Node *createNode(char ch){
	Node *newNode= (Node *)malloc(sizeof(Node));
	newNode->data=ch;
	newNode->next=NULL;
	
	return newNode;
}

Node *insertAtHead(Node *top, char ch){
	Node *newNode=createNode(ch);
	newNode->next=top;
	top=newNode;
	
	return top;
}

Node *deleteAtHead(Node *top){
	Node *curr=top;
	if(top==NULL)
		return NULL;
	top=top->next;
	free(curr);
	return top;
}

StackDouble *initStackDouble(){
	StackDouble *s=(StackDouble *)malloc(sizeof(StackDouble));
	s->top=NULL;
	return s;
}

void pushDouble(StackDouble *s,double n){
	s->top=insertAtHeadDouble(s->top,n);
}

double popDouble(StackDouble *s){
	int num;
	if(isEmptyDouble(s)){
		cout<<"Stack is empty."<<endl;
		return -999;
	}
	num=s->top->data;
	s->top=deleteAtHeadDouble(s->top);
	return num;
}

double peekDouble(StackDouble *s){
	double num;
	NodeDouble *temp;
	if(isEmptyDouble(s))
		return 0;
	temp=s->top;
	num=temp->data;
	return num;
}

Stack *initStack();
Queue *initQueue();
bool isEmptyS(Stack *s);
bool isEmptyQ(Queue *q);
void push(Stack *s, char ch);
char pop(Stack *s);
char peek(Stack *s);
void enqueue(Queue *q, char ch);
char dequeue(Queue *q);
bool isBalanced(string infix);
bool isOperator(char ch);
int prec(char ch);
void toPostfix(string infix, Queue *postfix, char post[]);
double evalPostfix(Queue *postfix);
double toDigit(char ch);
double evalOperation(double num1, double num2, char op);

int main(){
	string infix;
	char post[20];
	Queue *postfix=initQueue();
	ifstream in;
	ofstream out;
	out.open("output.txt");
	in.open("input.txt");
	if(!in.is_open()){
		cout<<"File could not be opened.";
		return 0;
	}
	double result=0.0;
	getline(in, infix);
	while(infix!="$"){
		cout<<infix<<endl;

		if(isBalanced(infix)){
			out<<"Infix Expression: "<<infix<<endl;
			toPostfix(infix,postfix,post);
			out<<"Outfix Expression: "<<post<<endl;
			cout<<endl;
			result=evalPostfix(postfix);
			cout<<"Answer= "<<result<<endl<<endl;
			out<<"Answer= "<<result<<endl<<endls;
		}
		else out<<"Error: Unbalanced paranthesis."<<endl<<endl;
		getline(in,infix);
	}
	
	in.close();
	out.close();
	return 0;
}

Stack *initStack(){
	Stack *s=(Stack *)malloc(sizeof(Stack));
	s->top=NULL;
	return s;
}

bool isEmptyS(Stack *s){
	return (s->top==NULL);
}

void push(Stack *s, char ch){
	s->top=insertAtHead(s->top,ch);
}

char pop(Stack *s){
	char ch;
	if(isEmptyS(s)){
		cout<<"Stack is empty."<<endl;
		return 0;
	}
	ch=s->top->data;
	s->top=deleteAtHead(s->top);
	return ch;
}

char peek(Stack *s){
	char ch;
	Node *temp;
	if(isEmptyS(s))
		return 0;
	temp=s->top;
	ch=temp->data;
	return ch;
}

Queue *initQueue(){
	Queue *q=(Queue *)malloc(sizeof(Queue));
	q->front=NULL;
	q->rear=NULL;
	return q;
}

bool isEmptyQ(Queue *q){
	return(q->front==NULL);
}

void enqueue(Queue *q,char ch){
	Node *newNode=createNode(ch);
	if(isEmptyQ(q)){
		q->front=newNode;
		q->rear=newNode;
		return;
	}
	q->rear->next=newNode;
	q->rear=newNode;
	
}

char dequeue(Queue *q){
	Node *temp=q->front;
	if(isEmptyQ(q)){
		cout<<"Queue is empty.";
		return 0;
	}
	q->front=q->front->next;
	if(q->front==NULL)
		q->rear=NULL;
	char ch=temp->data;
	free(temp);
	return ch;
}

bool isBalanced(string infix){
	Stack *s=initStack();
	int i=0;
	char ch;
	while(infix[i]!='\0'){
		if(infix[i]=='(')
			push(s,infix[i]);
		if(infix[i]==')'){
			if(isEmptyS(s)){
				cout<<"Error: More right-hand paranthesis than left"<<endl;
				return false;
			}
			ch=pop(s);
		}
		
		i++;
	}
	if(!isEmptyS(s)){
		cout<<"Error: More left-hand paranthesis than right"<<endl;
		return false;
	}
	return true;
}

void toPostfix(string infix, Queue *postfix,char post[]){
	Stack *operators=initStack();
	int length=infix.length();
	int i=0;
	int j=0;
	char ch, op;
	while(i<length){
		ch=infix[i];
		if(ch>='0' && ch<='9'){
			cout<<ch<<" ";
			enqueue(postfix,ch);
			post[j]=ch;
			j++;
		}
		else if(ch=='('){
			push(operators,ch);
		}
		else if(isOperator(ch)){
			if(isEmptyS(operators) || prec(ch)>=prec(peek(operators)))
				push(operators,ch);
			else{
				while(!isEmptyS(operators) && prec(ch)<prec(peek(operators))){
					op=pop(operators);
					cout<<op<<" ";
					enqueue(postfix,op);
					post[j]=op;
					j++;
				}
				push(operators,ch);
			}
		}
		else if(ch==')'){
			while(!isEmptyS(operators) && peek(operators)!='('){
				op=pop(operators);
				cout<<op<<" ";
				enqueue(postfix,op);
				post[j]=op;
				j++;
			}	
			pop(operators);
		}
		i++;
	}
	while(!isEmptyS(operators)){
		op=pop(operators);
		cout<<op<<" ";
		enqueue(postfix,op);
		post[j]=op;
		j++;
	}
	post[j]='\0';
}

bool isOperator(char ch){
	if(ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='^')
		return true;
	return false;
}

int prec(char ch){
	if(ch=='^')
		return 3;
	if(ch=='*' || ch=='/')
		return 2;
	if(ch=='+' || ch=='-' )
		return 1;
	if(ch=='(')
		return 0;
}

double toDigit(char ch){
	double num=ch-'0';
	num*=1.0;
	return num;
}

double evalPostfix(Queue *postfix){
	StackDouble *operands=initStackDouble();
	char op;
	double toStack=0.0, result=0.0;
	double num1=0.0,num2=0.0,num3=0.0;
	while(!isEmptyQ(postfix)){
		op=dequeue(postfix);
		if(!isOperator(op)){
			toStack=toDigit(op);
			pushDouble(operands,toStack);
		}
		else{
			num2=popDouble(operands);
			num1=popDouble(operands);
			num3=evalOperation(num1,num2,op);
			pushDouble(operands,num3);
		}
	}
	result=popDouble(operands);
}

double evalOperation(double num1, double num2, char op){
	double result;
	if(op=='+')
		result=num1+num2;
	else if(op=='-')
		result=num1-num2;
	else if(op=='*')
		result=num1*num2;
	else if(op=='/')
		result=num1/num2;
	else if(op=='^'){
		result=1;
		for(int i=0;i<num2;i++)
			result*=num1;
	}
	return result;
}
