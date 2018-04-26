#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

#define PROCESS_NAME_LEN 32 //进程名最大长度
#define MIN_SLICE 10 //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024  //总内存大小
#define DEFAULT_MEM_START 0  //内存开始分配时的起始地址

typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
bool flag = 0; //当内存以及被分配了之后，不允许更改总内存大小的flag
static int pid = 0;
int choice = 0; //默认选择第一个分配算法
My_algo algo[123];

struct free_block{	//空闲数据块
	int size;
	int start_addr;
	struct free_block *next;
};

struct allocated_block{ //已分配的数据块
	int pid;
	int size;
	int start_addr;
	char process_name[PROCESS_NAME_LEN];
	int *data;
	struct allocated_block *next;
};

free_block *free_block_head; //空闲数据块首指针
allocated_block *allocated_block_head = NULL; //分配块首指针

allocated_block *find_process(int id); //寻找pid为id的分配块
free_block *init_free_block(int mem_size); //空闲块初始化
void display_menu(); //显示选项菜单
void set_mem_size(); //设置内存大小
int allocate_mem(allocated_block *ab); //为制定块分配内存
void rearrange(); // 对块进行重新分配
int create_new_process(); //创建新的进程
int free_mem(allocated_block *ab); //释放分配块
void swap(int *p, int *q); //交换地址
int dispose(allocated_block *ab); //释放分配块结构体
void display_mem_usage(); //显示内存情况
void kill_process(); //杀死对应进程并释放其空间与结构体
void Usemy_algo(int id); //使用对应的分配算法
int BestFit(allocated_block *ab, int size); //BestFit 算法
int FirstFit(allocated_block *ab, int size); //FirstFit 算法
int WorstFit(allocated_block *ab, int size); //WorstFit 算法
void reverse_free_block(); //逆序空闲块
void merge_free_block(); //合并连续空闲块
void sort_free_block(); //排序空闲内存块


//主函数
int main()
{
	int op;
	pid = 0;
	free_block_head = init_free_block(mem_size); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
	for(;;)
	{
		sleep(1);
		display_menu();
		fflush(stdin);
		cin>>op;
		if(cin.fail() || (cin.good() && cin.get() == '.'))
		{
			cin.clear();
			cin.sync();  
			cin.ignore(1024, '\n');
			continue;
		}
		switch (op)
		{
			case 1:{ set_mem_size(); break; }
			case 2:
			{
				cout<<"Choose algoirthm(0: BestFit(default), 1: FirstFit, 2:WorstFit): ";
				int id = 0;
				cin>>id;
				if(cin.fail()|| (cin.good() && cin.get() == '.'))
				{	//处理非法输入
					cin.clear();
					cin.sync();  
					cin.ignore(1024, '\n');
					puts("Please input 0 or 1 or 2");
					sleep(1);
					break;
				}
				Usemy_algo(id);
				break; 
			}
			case 3:{ create_new_process(); sort_free_block(); break; }
			case 4:{ kill_process(); break; }
			case 5:{ display_mem_usage(); break; }
			case 233:{ puts("bye...."); sleep(1); return 0; }
			defaut: break;
		}
	}
	return 0;
}

allocated_block *find_process(int id)
{ //循环遍历分配块链表，寻找pid=id的进程所对应的块
	allocated_block* p = NULL;
	if(allocated_block_head)
	{
		p = allocated_block_head;
		while(p && p->pid != id)
		{
			p = p->next;
		}
		if(p)
		{
			puts("Process found");
		}
		else
		{
			puts("No process");
			return NULL;
		}		
		return p;
	}
	else
	{
		puts("No process");
		return NULL;
	}
}

free_block *init_free_block(int mem_size)
{ //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
	free_block *p;
	p = (free_block *)malloc(sizeof(free_block));
	if (p == NULL){
		puts("No memory left");
		return NULL;
	}
	p->size = mem_size;
	p->start_addr = DEFAULT_MEM_START;
	p->next = NULL;
	return p;
}

void display_menu()
{
	puts("\n\n******************menu*******************");
	printf("1) Set memory size (default = %d)\n", DEFAULT_MEM_SIZE);
	printf("2) Set memory allocation algorithm\n");
	printf("3) Create a new process\n");
	printf("4) Kill a process\n");
	printf("5) Display memory usage\n");
	printf("233) Exit\n");
}

void set_mem_size()
{ //更改最大内存大小
	if(flag)
	{	//内存未被分配
		puts("Memory size cannot be changed");
		return;
	}
	cout<<"Please input memory size: ";
	cin>>mem_size;
	if(cin.fail() || (cin.good() && cin.get() == '.'))
	{
		cin.clear();
		cin.sync();
		cin.ignore(1024, '\n');
		puts("Invalid input");
		return;
	}
	else
	{
		if(mem_size <= 0)
	    {
	    	puts("Memory size should be positive.");
	    	mem_size = DEFAULT_MEM_SIZE;
	    	return;
	    }
	    if(free_block_head)
	    {
	    	free_block_head->size = mem_size;
	    	cout<<"Memory size changed to "<<mem_size<<endl;
	    }
	}
	return;
}

int allocate_mem(allocated_block *ab)
{ //为块分配内存，真正的操作系统会在这里进行置换等操作
	int size = 0;
	cout<<"input memory size: ";
	cin>>size;
	if(cin.fail()|| (cin.good() && cin.get() == '.') || size<=0)
	{
		cin.clear();
		cin.sync();  
		cin.ignore(1024, '\n');
		return -1;
	}
	if(free_block_head==NULL)
	{
		return 0;
	}
	else if(choice == 0)
	{	//BestFit
		return BestFit(ab, size);
	}
	else if(choice == 1)
	{   //FirstFit
		return FirstFit(ab, size);
	}
	else if(choice == 2)
	{	//WorstFit
		return WorstFit(ab, size);
	}
	return 1;
}

int create_new_process()
{ //创建新进程
	allocated_block *new_process = NULL;
	int result = allocate_mem(new_process);
	if(result == 0)
	{	
		puts("No enough memory");
		return 0;
	}
	else if(result == 1)
	{
		puts("Allocate successfully");
		if(!flag) flag = 1; //锁定内存更改
		return 1;
	}
	else if(result == -1)
	{
		puts("Invalid input");
		return 0;
	}
}

void swap(int *p, int *q)
{
	int tmp = *p;
	*p = *q;
	*q = tmp;
	return;
}

void rearrange()
{ //将块按照地址大小进行排序
	free_block *tmp, *tmpx;
	puts("Rearrange begins...");
	puts("Rearrange by address...");
	tmp = free_block_head;
	while(tmp != NULL)
	{
		tmpx = tmp->next;
		while (tmpx != NULL)
		{
			if (tmpx->start_addr < tmp->start_addr)
			{
				swap(&tmp->start_addr, &tmpx->start_addr);
				swap(&tmp->size, &tmpx->size);
			}
			tmpx = tmpx->next;
		}
		tmp = tmp->next;
	}
	usleep(500);
	puts("Rearrange Done.");
}


int free_mem(allocated_block *ab)
{ //释放某一块的内存
	if(ab!=NULL)
	{ 	//检测有效指针
		free_block *free_p = NULL;
		free_p = (free_block *)malloc(sizeof(free_block));
		free_p->size = ab->size;
		free_p->start_addr = ab->start_addr;
		free_p->next = NULL;
		if(free_block_head == NULL)
		{
			free_block_head = free_p;
		}
		else
		{
			free_block *p = free_block_head;
			while(p->next!=NULL) p = p->next;
			p->next = free_p;
		}
		merge_free_block();
		puts("Free memory successfully");
		return 1;
	}
	else
	{
		puts("Unable to free memory");
		return 0;
	}
}

int dispose(allocated_block *fab)
{ //释放结构体所占的内存
	allocated_block *pre, *ab;
	if (fab == allocated_block_head)
	{
		allocated_block_head = allocated_block_head->next;
		free(fab);
		return 1;
	}
	pre = allocated_block_head;
	ab = allocated_block_head->next;
	while (ab != fab){ pre = ab; ab = ab->next;}
	pre->next = ab->next;
	free(ab);
	return 2;
}

void display_mem_usage()
{
	free_block *fb = free_block_head;
	allocated_block *ab = allocated_block_head;
	puts("*********************Free Memory*********************");
	printf("%20s %20s\n", "start_addr", "size");
	int cnt = 0;
	while (fb != NULL)
	{
		cnt++;
		printf("%20d %20d\n", fb->start_addr, fb->size);
		fb = fb->next;
	}
	if (!cnt) puts("No Free Memory");
	else printf("Totaly %d free blocks\n", cnt);
	puts("");
	puts("*******************Used Memory*********************");
	printf("%10s %20s %10s %20s\n", "PID", "ProcessName", "start_addr", "size");
	cnt = 0;
	while (ab != NULL)
	{
		cnt++;
		printf("%10d %20s %10d %20d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
		ab = ab->next;
	}
	if (!cnt) puts("No allocated block");
	else printf("Totally %d allocated blocks\n", cnt);
	return;
}

void kill_process()
{ //杀死某个进程
	allocated_block *ab;
	int pid;
	cout<<"Please input the pid of Killed process: ";
	cin>>pid;
	if(cin.fail()||(cin.good() && cin.get() == '.'))
	{
		cin.clear();
		cin.sync();  
		cin.ignore(1024, '\n');
		puts("Invalid input");
		return;
	}
	ab = find_process(pid);
	if (ab != NULL)
	{
		free_mem(ab);
		dispose(ab);
	}
}


void Usemy_algo(int id)
{//使用对应的分配算法
	algo[0] = make_pair(0, "BestFit");
	algo[1] = make_pair(1, "FirstFit");
	algo[2] = make_pair(2, "WorstFit");
	if(id == 0 || id == 1 || id == 2)
	{
		cout<<"Set "<<algo[id].second<<endl;
		if(choice!=id && id!=1)
		{
			reverse_free_block();
		}
		choice = id;
	}
	else
	{
		puts("Invalid input");
		return;
	}
}


int BestFit(allocated_block *ab, int size)
{	//BestFit 算法
	free_block *p = free_block_head;
	free_block *pre = NULL;
	free_block *next = p->next;
	free_block *record = NULL;
	while(p!=NULL)
	{
		if(size <= p->size)
		{
			record = p;
			break;
		}
		else if(next!=NULL)
		{
			pre = p;
			p = p->next;
			next = p->next;
		}	
	}
	if(record!=NULL)
	{
		int allocated_start_addr = p->start_addr;
		int allocated_mem_size = 0;
		if(p->size - size > MIN_SLICE)
		{
			p->size -= size;
			p->start_addr += size;
			allocated_mem_size = size;
		}
		else if(p->size - size <= MIN_SLICE)
		{
			if(p == free_block_head)
			{
				free_block_head = p->next;
			}
			else
			{
				free_block *pre = free_block_head;
				while(pre->next!=p) pre = pre->next;
				pre->next = p->next;
			}
			allocated_mem_size = p->size;
			free(p);
			p = NULL;
		}
		ab = (allocated_block *)malloc(sizeof(allocated_block));		
		ab->size = allocated_mem_size;
		ab->start_addr = allocated_start_addr;
		ab->pid = pid++;
		strcpy(ab->process_name, "Hello_OS");
		ab->data = NULL;
		ab->next = NULL;
		if(allocated_block_head == NULL)
		{
			allocated_block_head = ab;
		}
		else
		{
			allocated_block *alloc = allocated_block_head;
			while(alloc->next!=NULL) alloc = alloc->next;
			alloc->next = ab;
		}
		return 1;	
	}
	else
	{
		return 0;
	}
}


int FirstFit(allocated_block *ab, int size)
{	//FirstFit 算法
	free_block *p = free_block_head;
	free_block *pre = NULL;
	free_block *next = p->next;
	while(p!=NULL)
	{
		if(size <= p->size)
		{
			int allocated_start_addr = p->start_addr;
			int allocated_mem_size = 0;
			if(p->size - size > MIN_SLICE)
			{
				p->size -= size;
				p->start_addr += size;
				allocated_mem_size = size;
			}
			else if(p->size - size <= MIN_SLICE)
			{
				if(p == free_block_head)
				{
					free_block_head = p->next;
				}
				else
				{
					free_block *pre = free_block_head;
					while(pre->next!=p) pre = pre->next;
					pre->next = p->next;
				}
				allocated_mem_size = p->size;
				free(p);
				p == NULL;
			}
			ab = (allocated_block *)malloc(sizeof(allocated_block));
			ab->size = allocated_mem_size;
			ab->start_addr = allocated_start_addr;
			ab->pid = pid++;
			strcpy(ab->process_name, "Hello_world");
			ab->data = NULL;
			ab->next = NULL;
			if(allocated_block_head == NULL)
			{
				allocated_block_head = ab;
			}
			else
			{
				allocated_block *alloc = allocated_block_head;
				while(alloc->next!=NULL) alloc = alloc->next;
				alloc->next = ab;
			}
			return 1;
		}
		else if(next!=NULL)
		{
			pre = p;
			p = p->next;
			next = p->next;
		}
		else
		{
			return 0;
		}
	}
}

int WorstFit(allocated_block *ab, int size)
{	//WorstFit 算法
	free_block *p = free_block_head;
	free_block *next = p->next;
	free_block *record = NULL;
	if(size > p->size)
	{
		return 0;
	}
	else
	{
		int allocated_start_addr = p->start_addr;
		int allocated_mem_size = 0;
		if(p->size - size > MIN_SLICE)
		{
			p->size -= size;
			p->start_addr += size;
			allocated_mem_size = size;
		}
		else if(p->size - size <= MIN_SLICE)
		{
			free_block_head = p->next;
			allocated_mem_size = p->size;
			free(p);
			p == NULL;
		}
		ab = (allocated_block *)malloc(sizeof(allocated_block));		
		ab->size = allocated_mem_size;
		ab->start_addr = allocated_start_addr;
		ab->pid = pid++;
		strcpy(ab->process_name, "Hello_OS");
		ab->data = NULL;
		ab->next = NULL;
		if(allocated_block_head == NULL)
		{
			allocated_block_head = ab;
		}
		else
		{
			allocated_block *alloc = allocated_block_head;
			while(alloc->next!=NULL) alloc = alloc->next;
			alloc->next = ab;
		}
		return 1;	
	}
}


void reverse_free_block()
{	//逆序空闲块
	if(free_block_head)
	{
		free_block *pre = NULL;
		free_block *next = NULL;
		while(free_block_head)
		{
			next = free_block_head->next;
			free_block_head->next = pre;
			pre = free_block_head;
			free_block_head = next;
		}
		free_block_head = pre;
		return;
	}
	else
	{
		return;
	}
}

void merge_free_block()
{	//合并连续的内存块
	rearrange();
	puts("Merge free memory...");
	if(free_block_head==NULL || free_block_head->next==NULL)
	{
		return;
	}
	else
	{	
		free_block *pre = NULL;
		free_block *cur = free_block_head;
		free_block *next = cur->next;
		while(next!=NULL)
		{
			if(cur->start_addr+cur->size == next->start_addr)
			{
				free_block *merged = (free_block *)malloc(sizeof(free_block));
				merged->size = cur->size+next->size;
				merged->start_addr = cur->start_addr;
				merged->next = next->next;
				if(cur==free_block_head)
				{
					free_block_head = merged;
				}
				else
				{
					pre = free_block_head;
					while(pre->next != cur) pre = pre->next;
					pre->next = merged;
				}
				free(cur);
				free(next);
				cur = free_block_head;
				next = cur->next;
				continue;
			}
			pre = cur;
			cur = cur->next;
			next = next->next;
		}
	}
	puts("Merge done");
}


void sort_free_block()
{	//排序空闲快
	puts("Sort free block...");
	free_block *tmp_a = free_block_head;
	free_block *tmp_b = NULL;
	if(choice == 0)
	{
		while(tmp_a != NULL)
		{
			tmp_b = tmp_a->next;
			while (tmp_b != NULL)
			{
				if (tmp_b->size < tmp_a->size)
				{
					swap(&tmp_a->start_addr, &tmp_b->start_addr);
					swap(&tmp_a->size, &tmp_b->size);
				}
				tmp_b = tmp_b->next;
			}
			tmp_a = tmp_a->next;
		}
		puts("Sort done");
	}
	else if(choice == 2)
	{
		while(tmp_a != NULL)
		{
			tmp_b = tmp_a->next;
			while (tmp_b != NULL)
			{
				if (tmp_b->size > tmp_a->size)
				{
					swap(&tmp_a->start_addr, &tmp_b->start_addr);
					swap(&tmp_a->size, &tmp_b->size);
				}
				tmp_b = tmp_b->next;
			}
			tmp_a = tmp_a->next;
		}
	}
	else
	{
		return;
	}
}