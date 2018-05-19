#include <bits/stdc++.h>
#include <unistd.h>


using namespace std;

vector<int> requests;
vector<int> movements;
int start, request_num, distance_sum, pre, min_num = 0, max_num;

void init(); //initialize the requests and read request from file
void print_result(char *algorithm_name); //print the sequence of the head movement
// Scheduling algorithms
void FCFS();
void SSTF();
void SCAN();
void CSCAN();
void LOOK();
void CLOOK();

int main(int argc, char *argv[])
{   
    init();
    FCFS();
    SSTF();
    SCAN();
    CSCAN();
    LOOK();
    CLOOK();
    return 0;
}

void init()
{
    requests.clear();
    cin >> start; //start of the track
    cin >> max_num; //max range of track
    cin >> request_num; //numbers of requests
    int request = 0;
    for(int i = 0; i < request_num; ++i)
    {
        cin >> request;
        requests.push_back(request);
    }
}

void print_result(char *algorithm_name)
{
    cout << algorithm_name << endl;
    cout << start;
    for(int i = 0; i < movements.size(); ++i)
    {
        cout << "->" << movements[i];
    }
    cout << endl << distance_sum << endl;
}

void FCFS()
{
    movements.clear();
    pre = start;
    distance_sum = 0;
    for(int i = 0; i < requests.size(); ++i)
    {
        movements.push_back(requests[i]);
        distance_sum += abs(pre - requests[i]);
        pre = requests[i];
    }
    char algorithm_name[5] = "FCFS";
    print_result(algorithm_name);
}

void SSTF()
{
    movements.clear();
    pre = start;
    distance_sum = 0;
    vector<int> left;
    vector<int> right;
    for (int i = 0; i < requests.size(); ++i)
    {
        if (requests[i] <= start)
            left.push_back(requests[i]);
        else
            right.push_back(requests[i]);
    }
    sort(left.begin(), left.end(), less<int>()); //sort in ascending order since our removement starts from back
    sort(right.begin(), right.end(), greater<int>());  //sort in descending order
    int left_dis = 0, right_dis = 0;
    while(!left.empty() || !right.empty())
    {
        if(!left.empty() && !right.empty())
        {
            left_dis = abs(pre - left.back());
            right_dis = abs(pre - right.back());
            if (left_dis <= right_dis)
            {
                movements.push_back(left.back());
                distance_sum += left_dis;
                pre = left.back();
                left.pop_back(); //remove the element from back
            }
            else
            {
                movements.push_back(right.back());
                distance_sum += right_dis;
                pre = right.back();
                right.pop_back();
            }
        }
        else if(!left.empty() && right.empty())
        {
            movements.push_back(left.back());
            distance_sum += abs(pre - left.back());
            pre = left.back();
            left.pop_back();
        }
        else
        {
            movements.push_back(right.back());
            distance_sum += abs(pre - right.back());
            pre = right.back();
            right.pop_back();
        }
    }
    char algorithm_name[5] = "SSTF";
    print_result(algorithm_name);
}

void SCAN()
{
    movements.clear();
    pre = start;
    distance_sum = 0;
    vector<int> left;
    vector<int> right;
    for (int i = 0; i < requests.size(); ++i)
    {
        if(requests[i] <= start)
            left.push_back(requests[i]);
        else
            right.push_back(requests[i]);
    }
    sort(left.begin(), left.end(), greater<int>()); //sort in descending order
    sort(right.begin(), right.end(), less<int>()); //sort in ascending order
    for(int j = 0; j < left.size(); ++j)
    {
        movements.push_back(left[j]);
        distance_sum += abs(pre - left[j]);
        pre = left[j];
    }
    movements.push_back(min_num);
    distance_sum += abs(pre - min_num);
    pre = min_num;
    for(int k = 0; k < right.size(); ++k)
    {
        movements.push_back(right[k]);
        distance_sum += abs(pre - right[k]);
        pre = right[k];
    }
    char algorithm_name[5] = "SCAN";
    print_result(algorithm_name);
}

void CSCAN()
{
    movements.clear();
    pre = start;
    distance_sum = 0;
    vector<int> left;
    vector<int> right;
    for (int i = 0; i < requests.size(); ++i)
    {
        if (requests[i] <= start)
            left.push_back(requests[i]);
        else
            right.push_back(requests[i]);
    }
    sort(left.begin(), left.end(), greater<int>());
    sort(right.begin(), right.end(), greater<int>());
    for (int j = 0; j < left.size(); ++j)
    {
        movements.push_back(left[j]);
        distance_sum += abs(pre - left[j]);
        pre = left[j];
    }
    movements.push_back(min_num);
    distance_sum += abs(pre - min_num);
    movements.push_back(max_num);
    pre = max_num;
    for (int k = 0; k < right.size(); ++k)
    {
        movements.push_back(right[k]);
        distance_sum += abs(pre - right[k]);
        pre = right[k];
    }
    char algorithm_name[7] = "C-SCAN";
    print_result(algorithm_name);
}

void LOOK()
{
    movements.clear();
    pre = start;
    distance_sum = 0;
    vector<int> left;
    vector<int> right;
    for (int i = 0; i < requests.size(); ++i)
    {
        if (requests[i] <= start)
            left.push_back(requests[i]);
        else
            right.push_back(requests[i]);
    }
    sort(left.begin(), left.end(), greater<int>());
    sort(right.begin(), right.end(), less<int>());
    for (int j = 0; j < left.size(); ++j)
    {
        movements.push_back(left[j]);
        distance_sum += abs(pre - left[j]);
        pre = left[j];
    }
    for (int k = 0; k < right.size(); ++k)
    {
        movements.push_back(right[k]);
        distance_sum += abs(pre - right[k]);
        pre = right[k];
    }
    char algorithm_name[5] = "LOOK";
    print_result(algorithm_name);
}

void CLOOK()
{
    movements.clear();
    pre = start;
    distance_sum = 0;
    vector<int> left;
    vector<int> right;
    for (int i = 0; i < requests.size(); ++i)
    {
        if (requests[i] <= start)
            left.push_back(requests[i]);
        else
            right.push_back(requests[i]);
    }
    sort(left.begin(), left.end(), greater<int>());
    sort(right.begin(), right.end(), greater<int>());
    for (int j = 0; j < left.size(); ++j)
    {
        movements.push_back(left[j]);
        distance_sum += abs(pre - left[j]);
        pre = left[j];
    }
    pre = right[0];
    movements.push_back(right[0]);
    for (int k = 1; k < right.size(); ++k)
    {
        movements.push_back(right[k]);
        distance_sum += abs(pre - right[k]);
        pre = right[k];
    }
    char algorithm_name[7] = "C-LOOK";
    print_result(algorithm_name);
}