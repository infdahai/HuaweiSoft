
#ifndef SOURCE_H
#define SOURCE_H

#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <string>

using namespace std;
using ull = unsigned long long;

const int maxn = 105;
const int maxm = 1000 + 5;
const int maxr = 10e5 + 5;
const int max_name_len = 21;
int n; // [1..100]
int m; // [1..1000]
int t; // [1..1000]
int t_max;
//int r[maxm]; // sum(r[..]) <= 10e5
int Q = 1;

int mems_day[maxm][2];
int cores_day[maxm][2];
int mems_add_max[maxm][2], cores_add_max[maxm][2];

int memd_day[maxm];
int cored_day[maxm];
int memd_add_max[maxm], cored_add_max[maxm];

int globalMemD, globalCoreD;
int globalMemS, globalCoreS;
int globalMemSum, globalCoreSum;

struct Migrate
{
    int vmid;
    int serid;
    int type; // 0 A 1 B
} migrate[maxm];

struct Answer
{
    int ser_nameid;
    int num;
};

struct Server
{
    int nameid;
    int core; //1024
    int mem;  // 1024
} sers[maxn];

struct ServerSplit
{
    char name[max_name_len];
    int len;
    int costo; // 5*10e5
    int costd; // 5000
    int magicRank;
    // int coreRank;
    // int memRank;
} sersps[maxn];

struct Server_Alive
{
    int server_id;
    Server *server_ptr;
    int nameid;
    struct
    {
        int core_remain;
        int mem_remain;
    } A, B;
};

char vmname[maxn][max_name_len];

struct VM
{
    int nameid;
    // string name; // len <= 20
    int len;
    int core;
    int mem;
    int type; // 0: single, 1: dual
} vms[maxm];

struct VM_Alive
{
    int vmid;
    VM *vm_ptr;
    int ser_id = -1;
    int pos; // 0 : A, 1 : B
    bool isAlive = true;
    VM_Alive(int vmid1, VM *vm_ptr1)
    {
        vmid1 = vmid1;
        vm_ptr = vm_ptr1;
    }
};

unordered_map<string, int> vmHashTable;
unordered_map<string, int> serHashTable;
unordered_map<int, VM_Alive> vms_alive;

struct Req
{
    bool type; // true:add, false: del
    VM_Alive *vma_ptr;
} reqs[maxr];
//(add,vm_name,vm_id) or (del, vm_id)

struct _ReqDay
{
    int req_start_day;
    int req_times;
    int req_end_day;
    int req_max_pos;

} ReqDay[maxm];

#endif
