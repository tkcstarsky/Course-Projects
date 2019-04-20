
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <map>
#include <queue>
#include <time.h>
#include <omp.h>
#include <windows.h>
#include <Psapi.h>
#include <bitset>

#pragma comment(lib,"Psapi.lib")
using namespace std;

typedef vector<int> table;



/*struct node{
	int value;
	node *next;
};

struct table{
	node *head=NULL;
	node *tail=NULL;
};*/


void savelist(map<int, table> &p, char *name) {
	FILE *fp;
	clock_t t;

	if ((fp = fopen(name, "r")) == NULL) {
		printf("file cannot open\n");
	}
	long a, b;
	char word;
	t = clock();
	do {
		fscanf(fp, "%ld%c%ld", &a, &word, &b);
		map<int, table>::iterator i;
		/*for(i=p.begin();i!=p.end();i++){
			//printf("%d\n",i->head->value);
			if(i->head->value==a){
				break;
			}
		}*/
		i = p.find(a);
		if (i == p.end()) {
			table new1;
			//node *node2=(node*)malloc(sizeof(node));
			//node *node1=(node*)malloc(sizeof(node));
			//node1->value=a;node1->next=node2;
			//node2->value=b;node2->next=NULL;
			//new1.head=node1;
			//new1.tail=node2;
			new1.push_back(a);
			new1.push_back(b);
			p[a] = new1;
		}
		else {
			//node *node1=(node*)malloc(sizeof(node));
			//node1->value=b;
			//node1->next=NULL;
			//i->second.tail->next=node1;
			//i->second.tail=node1;
			i->second.push_back(b);
			//          }
			  //    }
		}

	} while (fgetc(fp) != EOF);
	t = clock() - t;
	printf("Read file time cost:%fs\n", ((float)t) / CLOCKS_PER_SEC);
	fclose(fp);
}
#define RAND_MAX 10000000
void randomlist(map<int, table> &p, int num) {
	long a, b;
	char word;
	clock_t t;
	t = clock();
	srand(time(0));
	do {
		
		a = rand();
		b = rand();
		map<int, table>::iterator i;
		i = p.find(a);
		if (i == p.end()) {
			table new1;
			//node *node2=(node*)malloc(sizeof(node));
			//node *node1=(node*)malloc(sizeof(node));
			//node1->value=a;node1->next=node2;
			//node2->value=b;node2->next=NULL;
			//new1.head=node1;
			//new1.tail=node2;
			new1.push_back(a);
			new1.push_back(b);
			p[a] = new1;
		}
		else {
			//node *node1=(node*)malloc(sizeof(node));
			//node1->value=b;
			//node1->next=NULL;
			//i->second.tail->next=node1;
			//i->second.tail=node1;
			i->second.push_back(b);
			//          }
			//    }
		}

	} while (--num);
	t = clock() - t;
	printf("Read file time cost:%fs\n", ((float)t) / CLOCKS_PER_SEC);
}

bitset<1200000000> visted;
void searchlist(map<int, table> &save) {
	clock_t t;
	int num = 0, block = 0;

	visted.reset();

	queue<int> searchlist;
	map<int, table>::iterator i, k;
	vector<int>::iterator j;
	t = clock();
	for (i = save.begin(); i != save.end(); i++) {
		if (visted[i->second[0]]==0) {
			visted.set(i->second[0]);
			num++;
			block++;
			for (j = i->second.begin() + 1; j != i->second.end(); j++) {
				searchlist.push(*j);
			}

			/* while(p&&p->next!=NULL){
				 if(visted.find(p->value)==visted.end())
					 searchlist.push(p);
				 p=p->next;
			 }
			 while(searchlist.size()){
				 node* p=searchlist.front();
				 //map<int,int>::iterator i;
				 //if((i=visted.find(p->value))==visted.end()){
				 //    i->second =1;
				 //}
				 if(p->next)
					 searchlist.push(p->next);
				 searchlist.pop();
				 free(p);
			 }*/
			while (searchlist.size()) {
				if (visted[searchlist.front()]==0) {
					num++;
					visted.set(searchlist.front());
					k = save.find(searchlist.front());
					if (k != save.end() && k->second.size() > 1 )
						for (j = k->second.begin() + 1; j != k->second.end(); j++) {
							if (visted[*j]==0) {
								searchlist.push(*j);
							}
						}
				}
				/*if(visted[searchlist.front()]==1){
					visted[searchlist.front()]=2;
					k=save.find(searchlist.front());
					if(k->second.size()>1&&k!=save.end())
						for(j=k->second.begin()+1;j!=k->second.end();j++){
							if(visted.find(*j)==visted.end()) {
								searchlist.push(*j);
								visted[*j]==1;
							}
						}
				}*/
				searchlist.pop();
			}
		}

	}
	
	t = clock() - t;
	printf("BFS time cost:%fs\n", ((float)t) / CLOCKS_PER_SEC);
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	float memory = pmc.WorkingSetSize;
	if (memory < 1024)
		printf("Total memory cost: %.2lfB\n", memory);
	else {
		memory /= 1024.0;
		if (memory < 1024)
			printf("Total memory cost: %.2lfKB\n", memory);
		else {
			memory /= 1024.0;
			if (memory < 1024)
				printf("Total memory cost: %.2lfMB\n", memory);
			else {
				memory /= 1024.0;
				if (memory < 1024)
					printf("Total memory cost: %.2lfGB\n", memory);
			}
		}
	}
	printf("Total node num:%d, total block num:%d\n", num, block);

}

int main() {
	map<int, table> save;
	clock_t t;

	int size = 5;
	while (size < 500000) {
		printf("\n============\nTesting size %d...\n", size*2);
		randomlist(save, size);
		searchlist(save);
		save.clear();
		size *= 10;
	}
	printf("\n============\nTesting file small...\n");
	char sma[] = "twitter_small.txt";
	char lar[] = "twitter_large.txt";
	savelist(save, sma);
	searchlist(save);
	save.clear();
	printf("\n============\nTesting file large...\n");
	savelist(save, lar);
	searchlist(save);
	save.clear();
	getchar();

	//savelist(save,"test.txt");
	//long a,b;
	//findlist(a,save);
	//printf("end,%d",save.size());


	return 0;
}
