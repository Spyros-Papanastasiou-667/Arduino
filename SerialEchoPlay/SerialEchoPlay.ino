typedef char	item;
typedef struct node{
	item i;
	struct node * next;
}Node;

typedef struct queue{
	int items;
	Node * start;
}Queue;

Queue * Q;


void enqueue(Queue * Q,item i);
item dequeue(Queue * Q);

typedef Queue Stack;

Stack * S,* S2;
void push(Stack * S,item i);
item pop(Stack * S);

void setup() {
	// put your setup code here, to run once:

	// Initialize Serial
		Serial.begin(115200);

	// needed for Leonardo
		while(!Serial);
	//

	// initialize queue
		Q=new Queue;
		Q->items=0;
		Q->start=NULL;
	//
	//	initialize stack
		S=new Stack;
		S->items=0;
		S->start=NULL;
		// 2
		S2=new Stack;
		S2->items=0;
		S->start=NULL;
	//
}

char input=0;
int length=0,tmp=0;
long counter=0;
void loop() {
	counter++;
	// put your main code here, to run repeatedly:
	input=0;
	if(Serial.available()>0)
	{
		while(input!='\n' && input!='\r')
		{
			if(Serial.available()>0)
			{
				input=Serial.read();
				enqueue(Q,input);
//				Serial.print("Enqueued:: ");
//				Serial.println(input);
			}
		}
		length=Q->items;
		while(input=='\n' || input=='\r')
			input=Serial.read();
		while((input=dequeue(Q)))
		{
			if(input!='\n' && input!='\r')
			{
				push(S,input);
				Serial.print(input);
			}
		}
		/* 
		 * start <<---
		 * _
		 * --->>
		 *      _
		 * <<---
		 * ...
		 * */
		for(int i=0;i<4;i++)
		{
			while(input=pop(S))
			{
				push(S2,input);
				Serial.print("\b \b");
				delay(75);
			}
			while(input=pop(S2))
			{
				push(S,input);
				Serial.print(input);
				delay(75);
			}
		}
		while(pop(S));
		while(pop(S2));
		while(dequeue(Q));
		Serial.print("  :: ");
		Serial.print(millis());
		Serial.print(" counter is :: ");
		Serial.println(counter);
//		Serial.println("Hello!!");
	}
//	delay(1000);
}

void enqueue(Queue * Q,item i)
{
	if(Q)
	{
		if(Q->start==NULL)
		{
			Q->items=1;
			Q->start=new Node;
			Q->start->i=i;
			Q->start->next=NULL;
			return;
		}else
		{
			(Q->items)++;
			Node * tmpNode=Q->start;
			while(tmpNode->next)
			{
				tmpNode=tmpNode->next;
			}
			tmpNode->next=new Node;
			tmpNode=tmpNode->next;
			tmpNode->i=i;
			tmpNode->next=NULL;
			return;
		}
	}
}

item dequeue(Queue * Q)
{
	if(Q && Q->start)
	{
		Node * tmpNode=Q->start;
		item i=tmpNode->i;
		Q->start=tmpNode->next;
		(Q->items)--;
		delete tmpNode;
		return i;
	}else
	{
		return 0;
	}
}

/* 
 * Stack
 * */

void push(Stack * S,item i)
{
	if(S)
	{
		if(S->start==NULL)
		{
			S->items=1;
			S->start=new Node;
			S->start->i=i;
			S->start->next=NULL;
			return;
		}else
		{
			(S->items)++;
			Node * tmpNode=S->start;
			while(tmpNode->next)
				tmpNode=tmpNode->next;
			tmpNode->next=new Node;
			tmpNode=tmpNode->next;
			tmpNode->i=i;
			tmpNode->next=NULL;
			return;
		}
	}
}

item pop(Stack * S)
{
	if(S && S->start)
	{
		if(S->items >= 2)
		{
			Node * prev,* tmp;
			prev=tmp=S->start;
			while(tmp->next)
			{
				prev=tmp;
				tmp=tmp->next;
			}
			item i = tmp->i;
			delete tmp;
			prev->next=NULL;
			(S->items)--;
			return i;
		}else if(S->items == 1)
		{
			item i=S->start->i;
			delete S->start;
			S->start=NULL;
			(S->items)--;
			return i;
		}
	}
	else
		return 0;
}
