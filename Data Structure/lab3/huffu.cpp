#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// 统计字符频度的临时结点

typedef struct {
	unsigned char huf_char;			// 以8bits为单元的无符号字符
	unsigned long weight;		// 每类字符出现频度
} TmpNode;

// 哈夫曼树结点
typedef struct {
	unsigned char huf_char;				// 以8bits为单元的无符号字符
	unsigned long weight;			// 每类字符出现频度
	char *code;						// 字符对应的哈夫曼编码（动态分配存储空间）
	int parent, lchild, rchild;		// 双亲和左右孩子
} HTNode, *HuffmanTree;

// 选择最小和次小的两个结点，建立哈夫曼树调用
void Select(HTNode *HT, unsigned int n, int &s1, int &s2)
{
	// 找最小
	unsigned int i;
	unsigned long min = ULONG_MAX;
	for(i = 0; i < n; ++i)           
		if(HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			s1 = i;
		}
		HT[s1].parent=1;   // 标记此结点找次小
	// 找次小
	min=ULONG_MAX;
	for(i = 0; i < n; ++i)            
		if(HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			s2 = i;
		}
} 

// 建立哈夫曼树
void CreateTree(HTNode *HT, unsigned int char_kinds, unsigned int node_num)
{
	unsigned int i;
	int s1, s2;
	for(i = char_kinds; i < node_num; ++i)  
	{ 
		Select(HT, i, s1, s2);		// 选择最小的两个结点,生成新的节点
		HT[s1].parent = HT[s2].parent = i; 
		HT[i].lchild = s1; 
		HT[i].rchild = s2; 
		HT[i].weight = HT[s1].weight + HT[s2].weight; 
	} 
}
/*
// 生成哈夫曼编码
void HufCode(HTNode *HT, unsigned n,int node_num)
{
	unsigned int i;
	int cur, next, start;
	char *cd = (char *)malloc(256*sizeof(char));		// 暂存编码，最多256个叶子，编码长度不超多255
	cd[256-1] = '\0'; 
	for(i = 0; i < n; ++i) 
	{
		start = 256-1;	// 编码临时空间索引初始化
		// 从叶子向根反向遍历求编码
		for(cur = i, next = HT[i].parent; next != 0; cur = next, next = HT[next].parent)  
			if(HT[next].lchild == cur) 
				cd[--start] = '0';
			else 
				cd[--start] = '1';
		HT[i].code = (char *)malloc((256-start)*sizeof(char));			// 为第i个字符编码动态分配存储空间 
		strcpy(HT[i].code, &cd[start]);     // 正向保存编码到树结点相应域中
	} 
	free(cd);		// 释放编码临时空间
}*/
void HufCode(HTNode *HT, unsigned n,int node_num)
{
	unsigned int i;
	int cur, next, start;
	char *cd = (char *)malloc(node_num*sizeof(char));		// 暂存编码，最多256个叶子，编码长度不超多255
	cd[node_num-1] = '\0'; 
	for(i = 0; i < n; ++i) 
	{
		start = node_num-1;	// 编码临时空间索引初始化
		// 从叶子向根反向遍历求编码
		for(cur = i, next = HT[i].parent; next != 0; cur = next, next = HT[next].parent)  
			if(HT[next].lchild == cur) 
				cd[--start] = '0';
			else 
				cd[--start] = '1';
		HT[i].code = (char *)malloc((node_num-start)*sizeof(char));			// 为第i个字符编码动态分配存储空间 
		strcpy(HT[i].code, &cd[start]);     // 正向保存编码到树结点相应域中
	} 
	free(cd);		// 释放编码临时空间
}

// 压缩
//int compress(char *ifname, char *ofname)
int compress(char *ifname)
{
    char *ofname;
	unsigned int i, j;
	unsigned int char_kinds;// 字符种类
	unsigned char char_temp;// 暂存8bits字符
	unsigned long file_len = 0;//文件长度
	FILE *infile, *outfile;//输入输出文件
	TmpNode node_temp;
	unsigned int node_num;//节点数
	HuffmanTree huf_tree;
	char code_buf[256] = "\0";// 待存编码缓冲区
	unsigned int code_len;

    //生出输出文件
    bool flag = false;
    short PathSize = strlen(ifname);
    for(short i = PathSize-1; i >=0 ; i--)
    {
        if(ifname[i] == '\\')
        {
            ofname = new char[PathSize-i+3];
            for(short j = 0; i < PathSize; i++, j++)
               ofname[j] = ifname[i+1];
            flag = true;
            break;
        }
    }

    if(flag == false)
    {
        ofname = new char[PathSize+4];
        strcpy(ofname, ifname);
    }
    strcat(ofname, ".hf");



	TmpNode *tmp_nodes =(TmpNode *)malloc(256*sizeof(TmpNode));		//统计频率

	// 初始化暂存结点
	for(i = 0; i < 256; ++i)
	{
		tmp_nodes[i].weight = 0;
		tmp_nodes[i].huf_char = (unsigned char)i;// 数组的256个下标与256种字符对应
	}

	
	infile = fopen(ifname, "rb");
	if (infile == NULL)// 判断输入文件是否存在
		return -1;

    //读取文件，得到字符权重
	fread((char *)&char_temp, sizeof(unsigned char), 1, infile);// 读入一个字符
	while(!feof(infile))
	{
		++tmp_nodes[char_temp].weight;	// 统计下标对应字符的权重，利用数组的随机访问快速统计字符频度
		++file_len;
		fread((char *)&char_temp, sizeof(unsigned char), 1, infile);// 读入一个字符
	}
	fclose(infile);

	// 排序并删去权重为零的
	for(i = 0; i < 256-1; ++i)           
		for(j = i+1; j < 256; ++j)
			if(tmp_nodes[i].weight < tmp_nodes[j].weight)
			{
				node_temp = tmp_nodes[i];
				tmp_nodes[i] = tmp_nodes[j];
				tmp_nodes[j] = node_temp;
			}

	// 统计实际的字符种类（出现次数不为0）
	for(i = 0; i < 256; ++i)
		if(tmp_nodes[i].weight == 0) 
			break;
	char_kinds = i;
    if(char_kinds<=20)
        printf("tips:the file is very small,the result coundl not be ideal!");
	if (char_kinds == 1)
	{
        printf("the file is too small");
		outfile = fopen(ofname, "wb");					// 打开压缩后将生成的文件
		fwrite((char *)&char_kinds, sizeof(unsigned int), 1, outfile);		// 写入字符种类
		fwrite((char *)&tmp_nodes[0].huf_char, sizeof(unsigned char), 1, outfile);		// 写入唯一的字符
		fwrite((char *)&tmp_nodes[0].weight, sizeof(unsigned long), 1, outfile);		// 写入字符频度，也就是文件长度
		free(tmp_nodes);
		fclose(outfile);
	}
	else
	{
		node_num = 2 * char_kinds - 1;		// 建立哈夫曼树所需结点数 
		huf_tree = (HTNode *)malloc(node_num*sizeof(HTNode));	

		// 初始化前char_kinds个结点
		for(i = 0; i < char_kinds; ++i) 
		{ 
			// 将暂存结点的字符和频度拷贝到树结点
			huf_tree[i].huf_char = tmp_nodes[i].huf_char; 
			huf_tree[i].weight = tmp_nodes[i].weight;
			huf_tree[i].parent = 0; 
		}	
		free(tmp_nodes); // 释放字符频度统计的暂存区

		// 初始化后node_num-char_kins个结点
		for(; i < node_num; ++i) 
			huf_tree[i].parent = 0; 

		CreateTree(huf_tree, char_kinds, node_num);	// 创建哈夫曼树，已优化
        HufCode(huf_tree, char_kinds,node_num);	
		// 生成哈夫曼编码

		// 写入字符和相应权重，供解压时重建哈夫曼树
		outfile = fopen(ofname, "wb");					// 打开压缩后将生成的文件
		fwrite((char *)&char_kinds, sizeof(unsigned int), 1, outfile);		// 写入字符种类
		for(i = 0; i < char_kinds; ++i)
		{
			fwrite((char *)&huf_tree[i].huf_char, sizeof(unsigned char), 1, outfile);			// 写入字符
			fwrite((char *)&huf_tree[i].weight, sizeof(unsigned long), 1, outfile);		// 写入字符对应权重
		}

		// 紧接着字符和权重信息后面写入文件长度和字符编码
		fwrite((char *)&file_len, sizeof(unsigned long), 1, outfile);		// 写入文件长度
		infile = fopen(ifname, "rb");		// 以二进制形式打开待压缩的文件
		fread((char *)&char_temp, sizeof(unsigned char), 1, infile);     // 每次读取8bits
		while(!feof(infile))
		{
			// 匹配字符对应编码
			for(i = 0; i < char_kinds; ++i)
				if(char_temp == huf_tree[i].huf_char)
					strcat(code_buf, huf_tree[i].code);

			while(strlen(code_buf) >= 8)
			{
				char_temp = '\0';		// 清空字符暂存空间，改为暂存字符对应编码
				for(i = 0; i < 8; ++i)
				{
					char_temp <<= 1;		// 左移一位，为下一个bit腾出位置
					if(code_buf[i] == '1')
						char_temp |= 1;		// 置最低位1
				}
				fwrite((char *)&char_temp, sizeof(unsigned char), 1, outfile);		// 将字节对应编码存入文件
				strcpy(code_buf, code_buf+8);		// 编码缓存去除已处理的前八位
			}
			fread((char *)&char_temp, sizeof(unsigned char), 1, infile);     // 每次读取8bits
		}
		// 处理最后不足8bits编码
		code_len = strlen(code_buf);
		if(code_len > 0)
		{
			char_temp = '\0';//清零		
			for(i = 0; i < code_len; ++i)
			{
				char_temp <<= 1;		
				if(code_buf[i] == '1')
					char_temp |= 1;//最高位置1
			}
			char_temp <<= 8-code_len;       // 将编码字段从尾部移到字节的高位
			fwrite((char *)&char_temp, sizeof(unsigned char), 1, outfile);       // 存入最后一个字节
		}

		// 关闭文件
		fclose(infile);
		fclose(outfile);

		// 释放内存
		for(i = 0; i < char_kinds; ++i)
			free(huf_tree[i].code);
		free(huf_tree);
	}
}
//compress



// 解压
int extract(char *ifname)
{
	unsigned int i;
	unsigned long file_len;
	unsigned long writen_len = 0;// 控制文件写入长度
	FILE *infile, *outfile;
	unsigned int char_kinds;// 存储字符种类
	unsigned int node_num;
	HuffmanTree huf_tree;
	unsigned char code_temp;// 暂存8bits编码
	unsigned int root;// 根

	infile = fopen(ifname, "rb");// 以二进制方式打开压缩文件
	if (infile == NULL)// 判断输入文件是否存在
		return -1;

    int len = strlen(ifname);
    char *ofname = new char[len-2];
    for(int i = 0; i < len-3; i++)
        ofname[i] = ifname[i];
        ofname[len-3] = '\0';//解压文件名


	// 读取压缩文件前端的字符及对应编码，用于重建哈夫曼树
	fread((char *)&char_kinds, sizeof(unsigned int), 1, infile);// 读取字符种类数
	if (char_kinds == 1)
	{
		fread((char *)&code_temp, sizeof(unsigned char), 1, infile);// 读取唯一的字符
		fread((char *)&file_len, sizeof(unsigned long), 1, infile);// 读取文件长度
		outfile = fopen(ofname, "wb");// 打开压缩后将生成的文件
		while (file_len--)
			fwrite((char *)&code_temp, sizeof(unsigned char), 1, outfile);	
		fclose(infile);
		fclose(outfile);
	}
	else
	{
		node_num = 2 * char_kinds - 1;// 根据字符种类数，计算建立哈夫曼树所需结点数 
		huf_tree = (HTNode *)malloc(node_num*sizeof(HTNode));// 动态分配哈夫曼树结点空间
		// 读取字符及对应权重，存入哈夫曼树节点
		for(i = 0; i < char_kinds; ++i)     
		{
			fread((char *)&huf_tree[i].huf_char, sizeof(unsigned char), 1, infile);// 读入字符
			fread((char *)&huf_tree[i].weight, sizeof(unsigned long), 1, infile);// 读入字符对应权重
			huf_tree[i].parent = 0;
		}
		// 初始化
		for(; i < node_num; ++i) 
			huf_tree[i].parent = 0;

		CreateTree(huf_tree, char_kinds, node_num);	// 重建哈夫曼树

		//读取文件长度和编码，进行解码
		fread((char *)&file_len, sizeof(unsigned long), 1, infile);	// 读入文件长度
		outfile = fopen(ofname, "wb");// 打开压缩后将生成的文件
		root = node_num-1;
		while(1)
		{
			fread((char *)&code_temp, sizeof(unsigned char), 1, infile);// 读取一个字符长度的编码

			// 处理读取的一个字符长度的编码（通常为8位）
			for(i = 0; i < 8; ++i)
			{
				// 由根向下直至叶节点正向匹配编码对应字符
				if(code_temp & 128)//检测code_temp首位的值
					root = huf_tree[root].rchild;
				else
					root = huf_tree[root].lchild;

				if(root < char_kinds)
				{
					fwrite((char *)&huf_tree[root].huf_char, sizeof(unsigned char), 1, outfile);
					++writen_len;
					if (writen_len == file_len) break;//控制文件长度
					root = node_num-1;//复位根
				}
				code_temp <<= 1;// 将编码缓存的下一位移到最高位
			}
			if (writen_len == file_len) break;// 控制文件长度
		}

		// 关闭文件
		fclose(infile);
		fclose(outfile);

		// 释放内存
		free(huf_tree);
	}
}//extract()

int main()
{
	while(1)
	{
		int opt, flag  = 0;		// 每次进入循环都要初始化flag为0
		char ifname[256], ofname[256];		// 保存输入输出文件名
		// 输入所选择操作类型的数字代号：1：压缩，2：解压，3：退出
		printf("Please input the number of operations:\n 1: compress\n 2: extract\n 3: quit\n");
		scanf("%d", &opt);
		if (opt == 3)
			break;
		else
		{
			printf("Please input the infile name: ");
            getchar();
			fflush(stdin);		// 清空标准输入流，防止干扰gets函数读取文件名
			gets(ifname);
			//printf("Please input the outfile name: ");
			//fflush(stdin);
			//gets(ofname);
		}
		switch(opt)
		{
		case 1: printf("Compressing……\n");
				flag = compress(ifname);//, ofname);	// 压缩，返回值用于判断是否文件名不存在
				break;		
		case 2: printf("Extracting……\n");
				flag = extract(ifname);//, ofname);		// 解压，返回值用于判断是否文件名不存在
				break;		
		}
		if (flag == -1)
			printf("Sorry, infile \"%s\" doesn't exist!\n", ifname);		// 如果标志为‘-1’则输入文件不存在
		else
			printf("Operation is done!\n");		// 操作完成
	}

	return 0;
}
