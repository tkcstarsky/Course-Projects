#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>

#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#define MAX_VERTEX_NUM 20
#define INFINITY 1000


typedef struct
{
    char vexs[MAX_VERTEX_NUM];
    int arc[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    int vexnum,arcnum;
}MGraph;

bool dijkstra(const char *ifname);
#endif // DIJKSTRA_H
