#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

const double eps = 1e-8;
int dcmp(const double &x){ if (fabs(x) < eps) return 0; return x < 0 ? -1 : 1; }

#define DEFAULT_CACHE_SIZE 123
#define DEFAULT_ALGORITHM 0
#define VALID -1

typedef pair<int, string> _algorithm;

int cache_size, working_algorithm, hit, miss;
vector<_algorithm> algorithms;
vector<int> pages;
vector<int> cache;

void init();
void set_cache_size();
void set_algorithm();
void run_algorithm();
void read_pages();
void FIFO_algorithm();
void Min_algorithm();
void LRU_algorithm();
void Clock_algorithm();
void Second_chance_algortithm();
void print_status();

int main(int argc, char *argv[])
{
    init();
    set_cache_size(); 
    set_algorithm();
    read_pages();
    run_algorithm();
    return 0;
}


void init()
{
    algorithms.clear(); pages.clear(); cache.clear();
    cache_size = DEFAULT_CACHE_SIZE;
    string s = "FIFO";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM, s));
    s = "LRU";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 1, s));
    s = "Min";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 2, s));
    s = "Clock";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 3, s));
    s = "Second_chance";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 4, s));
    working_algorithm = 0;
}


void set_cache_size()
{
    int x;
    cin >> x;
    if (x <= 0)
    {
        cerr << "Invalid cache size" << endl;
        return;
    }
    cache_size = x;
}


void set_algorithm()
{
    unsigned int id;
    cin >> id;
    working_algorithm = id;
    if (id >= algorithms.size())
    {
        cerr << "Algorithm does not exists" << endl;
        return;
    }
}


void read_pages()
{
    pages.clear();
    int page_size;
    cin >> page_size;  
    int page;
    for(int i = 0; i < page_size; ++i)
    {
        cin >> page;
        pages.push_back(page);
    }
}


void FIFO_algorithm()
{
    hit = miss = 0;
    for(int i = 0; i < pages.size(); ++i)
    {
        vector<int>::iterator iter=find(cache.begin(), cache.end(), pages[i]);  
        if(iter != cache.end())
        {   //cache hit 
            ++hit;
            continue;
        }
        else
        {   //cache miss
            ++miss;
            cache.push_back(pages[i]);
            if(cache.size() > cache_size) cache.erase(cache.begin());
        } 
    }
}


void LRU_algorithm()
{
    hit = miss = 0;
    int full = 0, cache_index = 0;
    for(int i = 0; i < pages.size(); ++i)
    {
        vector<int>::iterator iter=find(cache.begin(), cache.end(), pages[i]);  
        if(iter != cache.end())
        {   //cache hit 
            ++hit;
            iter = cache.erase(remove(cache.begin(), cache.end(), pages[i]), cache.end());
            cache.push_back(pages[i]);
        }
        else
        {   //cache miss
            ++miss;
            cache.push_back(pages[i]);
            if(cache.size() > cache_size ) cache.erase(cache.begin());
        }
    }
}

void Min_algorithm()
{
    hit = miss = 0;
    map<int, vector<int>> record;
    vector<int>::iterator iter; //iterator for vector
    map<int, vector<int>>::iterator iter2; //iterator for map
    for(int q = 0; q < pages.size(); ++q)
    {
        iter2 = record.find(pages[q]);
        if(iter2 == record.end()) 
        {
            vector<int> page_pos;
            page_pos.push_back(q);
            record.insert(make_pair(pages[q], page_pos));
        }
        else
        {
            record[pages[q]].push_back(q);
        }
    }
    for(int i = 0; i < pages.size(); ++i)
    {
        iter = find(cache.begin(), cache.end(), pages[i]);  
        if(!record[pages[i]].empty()) record[pages[i]].erase(record[pages[i]].begin());
        if(iter != cache.end())
        {   //cache hit 
            ++hit;
        }
        else
        {   //cache miss
            ++miss;
            if(cache.size() < cache_size)
            {   //not full, add the cache
                cache.push_back(pages[i]);
            }
            else
            {   
                //remove the one with samllest number of use 
                int index_record = 0, max_pos = 0;
                for(int j = 0; j < cache_size; ++j)
                {
                    if(!record[cache[j]].empty() && record[cache[j]][0] > max_pos)
                    {
                        max_pos = record[cache[j]][0];
                        index_record = j;
                    }
                    else if(record[cache[j]].empty())
                    {
                        index_record = j;
                        break;
                    }
                }
                cache[index_record] = pages[i];
            }
        }
    }
}


void Clock_algorithm()
{
    hit = miss = 0;
    int cache_index = 0, clock_pos = 0;
    int flag[cache_size];
    for(int k = 0; k < cache_size; ++k) flag[k] = 0; //initialize the flags for clock algorithm
    for(int i = 0; i < pages.size(); ++i)
    {
        vector<int>::iterator iter = find(cache.begin(), cache.end(), pages[i]);  
        if(iter != cache.end())
        {   //cache hit 
            ++hit;
            flag[(int)(iter-cache.begin())] = 1;
            continue;
        }
        else
        {   //cache miss
            ++miss;
            if(cache.size() < cache_size)
            {   //not full, add to cache
                flag[cache.size()] = 1;
                cache.push_back(pages[i]);
            }
            else
            {   //clock_pos records position of the pointer which simulates the clock behaviour
                for(; clock_pos < cache_size; ++clock_pos, clock_pos %= cache_size)
                {
                    if(flag[clock_pos] == 0)
                    {
                        cache[clock_pos++] = pages[i];
                        break;
                    }
                    else
                        flag[clock_pos] = 0; 
                }
            }
        }
    }
}

void Second_chance_algortithm()
{
    hit = miss = 0;
    for(int i = 0; i < pages.size(); ++i)
    {
        vector<int>::iterator iter=find(cache.begin(), cache.end(), pages[i]);  
        if(iter != cache.end())
        {   //cache hit 
            ++hit;
            if((int)(iter - cache.begin()) > cache_size/2)
            {
                iter = cache.erase(remove(cache.begin(), cache.end(), pages[i]), cache.end());
                cache.insert(cache.begin(), pages[i]);
            }
        }
        else
        {   //cache miss
            ++miss;
            cache.insert(cache.begin(), pages[i]); //FIFO
            if(cache.size() > cache_size) cache.pop_back(); //LRU
        }
    }
}

void print_status()
{
    cout << "Hit ratio = " << fixed <<setprecision(2) << (double)hit * 100 / (miss + hit) << "%" << endl;
}

void run_algorithm()
{
    switch (working_algorithm)
    {
        case 0: FIFO_algorithm(); break;
        case 1: LRU_algorithm(); break;
        case 2: Min_algorithm(); break;
        case 3: Clock_algorithm(); break;
        case 4: Second_chance_algortithm(); break;
        default: break;
    }
    print_status();
}
