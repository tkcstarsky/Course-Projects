#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>

struct nextnode;

typedef struct nodetype{
	long long label;
	int flag; //0: never visit; 1: in the queue; 2: visited
	nextnode *index;
	nextnode *indextail;
}node;

struct nextnode{
	node *link;
	nextnode *next;
};

nextnode *tail; //tail of queue

void insert_index(node *a, node *b){
	nextnode *r;
	r=(nextnode*)malloc(sizeof(nextnode));
	r->link=b;
	r->next=NULL;
	if(a->index==NULL){
		a->index=r;
		a->indextail=r;
	}
	else{
		a->indextail->next=r;
		a->indextail=a->indextail->next;
	}
}

void insert(nextnode *list, nextnode *p){
	nextnode *q=(nextnode*)malloc(sizeof(nextnode));
	q->link=p->link;
	q->next=NULL;
	tail->next=q;
	tail=tail->next;
}

nextnode* delet(nextnode *list){
	nextnode *p=list;
	list=list->next;
	free(p);
	return(list);
}

nextnode* bfs(nextnode *list){
	struct nextnode *p=list->next->link->index;
	list->next->link->flag=2;
	while(p!=NULL){
		if(p->link->flag==0){
			p->link->flag=1;
			insert(list,p);
			//printf("%lld -> %lld\n",list->next->link->label,p->link->label);
		}
		p=p->next;
	}
	list=delet(list);
	return(list);
}

main(){
	int start,input,bfst,bfst0;
	long long i,j,x,y,n=0,m=0,counter=0;
	node *a,*p,*q;
	nextnode *list; //queue
	char name[20];
	a=(node*)malloc(20000000*sizeof(node));
	//568770233
	for(i=1;i<20000000;i++){
		a[i].label=-1;
		a[i].flag=0;
		a[i].index=NULL;
		a[i].indextail=NULL;
	}
//initialize

	scanf("%s",name);
	
	start=clock();
	FILE *fp=fopen(name,"r");
	while(fscanf(fp, "%lld%lld", &x, &y) != EOF){
		//counter++;
		i=x%11316811;
		while(a[i].label!=-1 && a[i].label!=x) i=(i+1)%20000000;
		a[i].label=x;
		p=&a[i];
		j=y%11316811;
		while(a[j].label!=-1 && a[j].label!=y) j=(j+1)%20000000;
		a[j].label=y;
		q=&a[j];
		insert_index(p,q); //put a[y] into a[x]'s lndex*/
		/*if(counter%1000000==0){
			system("cls");
			printf("%lld\n",counter);
		}*/
	}
	input=clock();
	printf("input time: %.3f s\n",(double)(input-start)/1000);
//input

	printf("search from: ");
	scanf("%lld",&m);
	j=m%11316811;
	while(a[j].label!=m)
		if(a[j].label==-1){
			printf("this node does not exist!\n");
			break;
		}
		else j=(j+1)%20000000;
	bfst0=clock();
	list=(nextnode*)malloc(sizeof(nextnode));
	list->next=(nextnode*)malloc(sizeof(nextnode));
	list->next->link=&a[j];
	list->next->next=NULL;
	tail=list->next;
	while(list->next!=NULL)
		list=bfs(list);
	bfst=clock();
	printf("bfs time: %.3f s\n",(double)(bfst-bfst0)/1000);
	fclose(fp);
}









