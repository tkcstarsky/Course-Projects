#ifndef HF_H_INCLUDED
#define HF_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// 字符频度的存储结点
typedef struct {
	unsigned char huf_char;			 	// 无符号字符
	unsigned long weight;		     	// 每种字符的出现频度
} CountNode;

// 哈夫曼树结点
typedef struct {
	unsigned char huf_char;				// 无符号字符
	unsigned long weight;				// 每种字符出现频度
	char *code;							// 得到哈夫曼编码（长度不固定）
	int parent,left,right;				// 父结点和左右结点
} HuffNode, *HuffmanTree;

void Select(HuffNode *HT, unsigned int n, int &s1, int &s2);
void CreateTree(HuffNode *HT, unsigned int char_kinds, unsigned int node_num);
void HufCode(HuffNode *HT, unsigned n,int node_num);
bool Compress(const char *ifname);
bool Decompress(const char *ifname);


#endif // HF_H_INCLUDED

