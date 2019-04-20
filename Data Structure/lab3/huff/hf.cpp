#include "hf.h"
// 选择最小和次小的两个结点，建立哈夫曼树 
void Select(HuffNode *HT, unsigned int n, int &s1, int &s2)
{
	// 找最小 
	unsigned int i;
	unsigned long min = ULONG_MAX;		//将min赋值为无限大
	for(i = 0; i < n; ++i)           
		if(HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			s1 = i;
		}
		HT[s1].parent=1;   				// 使用parent进行标记
		
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
void CreateTree(HuffNode *HT, unsigned int char_kinds, unsigned int node_num)
{
	unsigned int i;
	int s1, s2;
	for(i = char_kinds; i < node_num; ++i)  
	{ 
		Select(HT, i, s1, s2);								//合并最小的两个结点加入结点数组中
		HT[s1].parent = HT[s2].parent = i; 
		HT[i].left = s1; 
		HT[i].right = s2; 
		HT[i].weight = HT[s1].weight + HT[s2].weight; 
	} 
}

// 生成哈夫曼编码
void HufCode(HuffNode *HT, unsigned n,int node_num)
{
	unsigned int i;
	int cur, next, start;
	char *buf = (char *)malloc(node_num*sizeof(char));		//创建一个缓存用以暂存编码（根据文件中字符种类动态分配）
	buf[node_num-1] = '\0'; 								//结尾加上字符串结束符
	for(i = 0; i < n; ++i) 
	{														// 从叶子向根反向遍历求编码，反向保存编码到buf中
		start = node_num-1;	
		for(cur = i, next = HT[i].parent; next != 0; cur = next, next = HT[next].parent)  
			if(HT[next].left == cur) 
				buf[--start] = '0';							//左结点编码0
			else 
				buf[--start] = '1';							//右结点编码0
		HT[i].code = (char *)malloc((node_num-start)*sizeof(char));			// 为huf编码动态分配存储空间 
		strcpy(HT[i].code, &buf[start]);     				// 正向保存
	} 
	free(buf);		
}

// 压缩
bool Compress(const char *ifname)
{
    char *ofname;
	CountNode node_temp;
	HuffmanTree huf_tree;

	unsigned int i, j;				//循环变量
	unsigned int char_kinds;		//字符种类
	unsigned char char_buf;			//暂存8bits字符
	unsigned long file_len = 0;		//文件长度
	FILE *infile, *outfile;			//输入输出文件
	unsigned int node_num;			//节点数
	char code_buf[256] = "\0";		//待存编码缓冲区
	unsigned int code_len;			//临时huf编码长度

    //生出输出文件名
    bool flag = false;							//用于判断是否有路径
    short PathSize = strlen(ifname);			//路径总长度
    for(short i = PathSize-1; i >=0 ; i--)		//对文件路径进行拆分
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
    strcat(ofname, ".huff");


	//统计不同字符的频数
	CountNode *tmp_nodes =(CountNode *)malloc(256*sizeof(CountNode));		//生成tmp_nodes[256]

	//初始化结点
	for(i = 0; i < 256; ++i)
	{
		tmp_nodes[i].weight = 0;
		tmp_nodes[i].huf_char = (unsigned char)i;					//当前节点内存放当前字符的ascll码
	}

    //读取文件并统计字符权重	
	infile = fopen(ifname, "rb");
	if (infile == NULL)												//判断输入文件是否存在
		return false;												//不存在则解压失败
	fread((char *)&char_buf, sizeof(unsigned char), 1, infile);	  	//读入一个字符
	while(!feof(infile))
	{
		tmp_nodes[char_buf].weight++;								//对应字符的权重+1
		file_len++;													//文件总长度+1
		fread((char *)&char_buf, sizeof(unsigned char), 1, infile); 
	}
	fclose(infile);

	// 对按权数排序并删去权重为零的
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

	//对特殊情况的处理（字符种类太少）
	if(char_kinds==0)
	{
		printf("error:It's a empty file!");
		return false;
	}
    if(char_kinds<=20)
        printf("tips:the file is very small,the result coundl not be ideal!");
	if (char_kinds == 1)
	{
        printf("the file is too small");
		outfile = fopen(ofname, "wb");													//打开压缩后将生成的文件
		fwrite((char *)&char_kinds, sizeof(unsigned int), 1, outfile);					//写入字符种类
		fwrite((char *)&tmp_nodes[0].huf_char, sizeof(unsigned char), 1, outfile);		//写入唯一的字符
		fwrite((char *)&tmp_nodes[0].weight, sizeof(unsigned long), 1, outfile);		//写入字符频度，也就是文件长度
		free(tmp_nodes);
		fclose(outfile);
	}
	else
	{	//生成huffuman树及编码
		node_num = 2 * char_kinds - 1;									//哈夫曼树结点数 
		huf_tree = (HuffNode *)malloc(node_num*sizeof(HuffNode));		//为huffunman树分配空间

		//将频数统计的结点数据放入huffuman树阶段中
		for(i = 0; i < char_kinds; ++i) 
		{ 
			huf_tree[i].huf_char = tmp_nodes[i].huf_char; 
			huf_tree[i].weight = tmp_nodes[i].weight;
			huf_tree[i].parent = 0; 
		}	
		free(tmp_nodes); 												//释放统计结点

		// 初始化其余huffuman树结点
		for(; i < node_num; ++i) 
			huf_tree[i].parent = 0; 

		CreateTree(huf_tree, char_kinds, node_num);						//创建哈夫曼树
        HufCode(huf_tree, char_kinds,node_num);							//生成哈夫曼编码
		//完成生成哈夫曼树及编码


		//向压缩文件中写入huffuman编码
		outfile = fopen(ofname, "wb");									
		fwrite((char *)&char_kinds, sizeof(unsigned int), 1, outfile);	//写入总计字符种类数
		for(i = 0; i < char_kinds; ++i)
		{
			fwrite((char *)&huf_tree[i].huf_char, sizeof(unsigned char), 1, outfile);	//写入字符
			fwrite((char *)&huf_tree[i].weight, sizeof(unsigned long), 1, outfile);		//写入字符对应权重
		}

		//写入文件长度
		fwrite((char *)&file_len, sizeof(unsigned long), 1, outfile);		//写入文件长度

		//开始压缩(写入转换后字的字符编码)
		infile = fopen(ifname, "rb");										//以二进制形式打开待压缩的文件
		fread((char *)&char_buf, sizeof(unsigned char), 1, infile);     	//每次读取1字符(8位)
		while(!feof(infile))
		{
			// 匹配字符对应编码
			for(i = 0; i < char_kinds; ++i)
				if(char_buf == huf_tree[i].huf_char)
					strcat(code_buf, huf_tree[i].code);

			//将8位字符串存入一个char变量中
			while(strlen(code_buf) >= 8)			
			{
				char_buf = '\0';					//清空字符暂存空间，改为暂存字符对应编码
				for(i = 0; i < 8; ++i)
				{
					char_buf <<= 1;					//左移一位，为下一个bit腾出位置
					if(code_buf[i] == '1')			//将字符串的1变为char变量中一个位的1
						char_buf |= 1;				//置最低位1
				}
				fwrite((char *)&char_buf, sizeof(unsigned char), 1, outfile);		//将1字节(8位)存入文件
				strcpy(code_buf, code_buf+8);		//编码buf去除前八位
			}
			fread((char *)&char_buf, sizeof(unsigned char), 1, infile);     
		}

		// 处理最后不足8bits编码
		code_len = strlen(code_buf);
		if(code_len > 0)
		{
			char_buf = '\0';								
			for(i = 0; i < code_len; ++i)
			{
				char_buf <<= 1;		
				if(code_buf[i] == '1')
					char_buf |= 1;					
			}
			char_buf <<= 8-code_len;       			//将编码字段从尾部移到字节的高位(空位补0)
			fwrite((char *)&char_buf, sizeof(unsigned char), 1, outfile);       	//存入最后一个字节
			//完成对原文件的huffuman编码
		}

		//关闭文件
		fclose(infile);
		fclose(outfile);

		//回收内存
		for(i = 0; i < char_kinds; ++i)
			free(huf_tree[i].code);
		free(huf_tree);
	}

	//压缩成功
	return true;
}
//compress



// 解压
bool Decompress(const char *ifname)
{
	unsigned int i;
	unsigned long file_len;
	FILE *infile, *outfile;
	unsigned int node_num;
	HuffmanTree huf_tree;
	unsigned long writen_len = 0;	//控制文件写入长度
	unsigned int char_kinds;		//存储字符种类
	unsigned char code_temp;		//暂存8bits编码
	unsigned int root;				//根

	//打开解压文件
	infile = fopen(ifname, "rb");// 以二进制方式打开压缩文件
	if (infile == NULL)// 判断输入文件是否存在
		return false;

	//获取解压后的文件名
    int len = strlen(ifname);
    char *ofname = new char[len-4];
    for(int i = 0; i < len-5; i++)	//复制输入的文件名，去掉最后两位后缀
        ofname[i] = ifname[i];
    ofname[len-5] = '\0';

	// 读取压缩文件前端的字符及对应编码，用于重建哈夫曼树
	fread((char *)&char_kinds, sizeof(unsigned int), 1, infile);		//读取字符种类数

	if (char_kinds == 1)
	{
		fread((char *)&code_temp, sizeof(unsigned char), 1, infile);	//读取唯一的字符
		fread((char *)&file_len, sizeof(unsigned long), 1, infile);		//读取文件长度
		outfile = fopen(ofname, "wb");									//打开压缩后将生成的文件
		while (file_len--)
			fwrite((char *)&code_temp, sizeof(unsigned char), 1, outfile);	
		fclose(infile);
		fclose(outfile);
	}
	else
	{
		node_num = 2 * char_kinds - 1;									//根据字符种类数，计算建立哈夫曼树所需结点数 
		huf_tree = (HuffNode *)malloc(node_num*sizeof(HuffNode));		//动态分配哈夫曼树结点空间

		// 读取字符及对应权重，存入哈夫曼树节点
		for(i = 0; i < char_kinds; ++i)     
		{
			fread((char *)&huf_tree[i].huf_char, sizeof(unsigned char), 1, infile);	//读入字符
			fread((char *)&huf_tree[i].weight, sizeof(unsigned long), 1, infile);	//读入字符对应权重
			huf_tree[i].parent = 0;
		}
		//初始化
		for(; i < node_num; ++i) 
			huf_tree[i].parent = 0;

		//重建哈夫曼树
		CreateTree(huf_tree,char_kinds,node_num);	

		//读取文件长度和编码
		fread((char *)&file_len, sizeof(unsigned long), 1, infile);		//读入文件长度
		outfile = fopen(ofname, "wb");									//打开压缩后将生成的文件
		root = node_num-1;												//最后一个节点即为根节点

		//开始解压缩
		while(1)
		{
			fread((char *)&code_temp, sizeof(unsigned char), 1, infile);	//读取一个字符长度的编码

			//对该编码进行转码还原成ascll码
			for(i = 0; i < 8; ++i)
			{
				//由根向下直至叶节点正向匹配编码对应字符
				if(code_temp & 128)											//检测code_temp首位的值
					root = huf_tree[root].right;							//递归生成新的根
				else
					root = huf_tree[root].left;

				if(root < char_kinds)										//根已经到达叶处
				{
					fwrite((char *)&huf_tree[root].huf_char, sizeof(unsigned char), 1, outfile);
					++writen_len;
					if (writen_len == file_len) break;						//该文件已转码完成
					root = node_num-1;										//复位根
				}
				code_temp <<= 1;// 将编码缓存的下一位移到最高位
			}
			if (writen_len == file_len) break;//解压缩已完成
		}

		// 关闭文件
		fclose(infile);
		fclose(outfile);

		// 释放内存
		free(huf_tree);
	}

	//解压成功
	return true;
}



