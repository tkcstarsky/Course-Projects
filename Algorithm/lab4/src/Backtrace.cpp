#include<stdio.h>
#include<cstdlib>
#include<time.h>
#include<algorithm>

#define N 100
#define MAX 10000000

int c[N], s[N], T[N];//c[]记录最优解,s[]记录临时，x[]记录时间,T[]代表任务时间
int x[N] = { 0 };
int best = MAX, count;
int n, k;
int getmax(int a[], int num)
{
	int max = a[1];
	for (int i = 1; i <= num; i++) {
		if (a[i] > max)
			max = a[i];
	}
	return max;
}
int getmin(int a[], int num)
{
	int min = a[1];
	int min_index = 1;
	for (int i = 1; i <= num; i++) {
		if (a[i] < min) {
			min = a[i];
			min_index = i;
		}
	}
	return min_index;
}
int getfz(int a[], int num)
{
	int i = 1;
	for (i = 1; i <= num && a[i]; i++)
		;
	return i;
}
void Backtrace(int t)
{
	int i;
	if (t > n)
	{
		int temp = getmax(x, k);
		count = temp;
		if (count < best)
		{
			for (i = 1; i <= n; i++)
				c[i] = s[i];
			best = count;
		}
	}
	else
	{
		int fz = getfz(x, k);
		if (fz == k + 1)
		{
			fz = k;
		}
		for (i = 1; i <= fz; i++)
		{
			s[t] = i;
			x[i] += T[t];
			if (x[i] < best) {
				Backtrace(t + 1);
			}
			x[i] -= T[t];
		}
	}
}
int main()
{
	printf("Input numbers of tasks and machines:\n");
	scanf("%d%d", &n, &k);
	int i;
	printf("Input each task's time:\n");
	for (i = 1; i <= n; i++)
		scanf("%d", &T[i]);

	clock_t begin,end;
	double toltime;

	begin=clock();

	for (i = 1; i <= n; i++)
	{
		int min = getmin(x, k);
		x[min] = x[min] + T[i];
	}
	best = getmax(x, k);
	best++;
	for (i = 1; i <= k; i++)
	{
		x[i] = 0;
	}
	Backtrace(1);

	end=clock();
    toltime=(double)(end-begin)/CLOCKS_PER_SEC;
	printf("Running time is:%.8fs\n",toltime);

	printf("The tasks are assigned as:\n");

	for (i = 1; i <= n; i++)
		printf("Task %d is assigned to Machine %d.\n", i, c[i]);
	printf("The shortest time is %d.\n", best);
}
