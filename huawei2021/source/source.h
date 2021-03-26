#pragma GCC optimize(3,"Ofast","inline")

#ifndef SOURCE_H
#define SOURCE_H

#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <iterator>
#include <deque>
using namespace std;


//#define CHRONO_TIMEH
#ifdef CHRONO_TIMEH
#include <chrono>
using namespace chrono;
#endif

// const int intSize = sizeof(int);
struct VM_Alive;
struct Server_Alive;

using ull = unsigned long long;
using VMA = VM_Alive;
using SA = Server_Alive;

const int maxn = 105;
const int maxm = 1000 + 5;
const int maxr = 2E5 + 5;
const int max_name_len = 21;
const int  magicNum = 1; // 1.18 more exactly
const int migDiv = 200;
const float shuffleFact = 0.2;
const int max_hash = 10e4+5;
const float tfactor = 2;

int thash=0;
int n; // [1..100]
int m; // [1..1000]
// int tt;
int t; // [1..1000]
int t_maxd, t_maxs;
// int r[maxm]; // sum(r[..]) <= 10e5
int idSerCur;
int aliveVM;
int migBound;
int shuffleBound;
float M1Ser;

int mems_day[maxm][2];
int cores_day[maxm][2];
int mems_add_max[maxm][2], cores_add_max[maxm][2];

int memd_day[maxm];
int cored_day[maxm];
int memd_add_max[maxm], cored_add_max[maxm];

int globalMemD, globalCoreD;
int globalMemS, globalCoreS;
int globalMemMD, globalMemMS;
int globalCoreMD, globalCoreMS;
int maxCompD, maxCompS;

struct Server
{
    int nameid=-1;
    int core=0; //1024
    int mem=0;  // 1024
    int CmptNum=0;
} sers[maxn],serms[maxn],sercs[maxn];

struct ServerSplit
{
    char name[max_name_len] = { '\0' };
    int len=0;
    int costo=0; // 5*10e5
    int costd=0; // 5000
    int magicRank=0;
    // int coreRank;
    // int memRank;
} sersps[maxn];

struct Server_Alive
{
    int server_id=-1;
    Server* server_ptr=nullptr;
    int rankid=-1;
    int core_remain[2];
    int mem_remain[2];

    int sd=0; // 0 : single, 1: dual

    bool used;
    int num;
    //    std::vector<VM_Alive *> vma_arr;
    Server_Alive(int serid, int raid, int sd_)
    {
        server_id = serid;
        rankid = raid;
        server_ptr = &sers[raid];
        core_remain[0] = sers[raid].core / 2;
        core_remain[1] = core_remain[0];
        mem_remain[0] = sers[raid].mem / 2;
        mem_remain[1] = mem_remain[0];
        sd = sd_;
        used = false;
        num = 0;
    }
    void update(int coren, int memn, int cas)
    {
        if (cas < 2)
        {
            core_remain[cas] -= coren;
            mem_remain[cas] -= memn;
        }
        else
        {
            core_remain[0] -= coren;
            core_remain[1] = core_remain[0];
            mem_remain[0] -= memn;
            mem_remain[1] = mem_remain[0];
        }
        num++;
        used = true;
    }
    void del(int coreq, int memq, int cas)
    {
        if (cas < 2)
        {
            core_remain[cas] += coreq;
            mem_remain[cas] += memq;
        }
        else
        {
            core_remain[0] += coreq;
            core_remain[1] = core_remain[0];
            mem_remain[0] += memq;
            mem_remain[1] = mem_remain[0];
        }
        num--;
        if (num == 0)
            used = false;
        if (num < 0)
            cerr << "Error del" << endl;
    }
};

struct Cmp_SerAD
{
    bool operator()(const SA* lhs, const SA* rhs) const
    {
        if (lhs->rankid != rhs->rankid)
            return lhs->rankid > rhs->rankid;
        if (lhs->used)
        {
            if (rhs->used) {
                int lcore = lhs->core_remain[0];
                int lmem = lhs->mem_remain[0];
                int rcore = rhs->core_remain[0];
                int rmem = rhs->mem_remain[0];
                return 0 < (rmem - lmem) + magicNum * (rcore - lcore);
            }
            else
            {
                return true;
            }
        }
        else {
            if (rhs->used) {
                return false;
            }
            else {
                return lhs->server_id < rhs->server_id;
            }
        }

    }
};

struct Cmp_SerAS
{
    bool operator()(const SA* lhs, const SA* rhs) const
    {
        if (lhs->rankid != rhs->rankid)
            return lhs->rankid > rhs->rankid;
        if (lhs->used)
        {
            if (rhs->used) {
                int lcore = lhs->core_remain[0] + lhs->core_remain[1];
                int lmem = lhs->mem_remain[0] + lhs->mem_remain[1];
                int rcore = rhs->core_remain[0] + rhs->core_remain[1];
                int rmem = rhs->mem_remain[0] + rhs->mem_remain[1];
                return 0 < (rmem - lmem) + magicNum * (rcore - lcore);
            }
            else
            {
                return true;
            }
        }
        else {
            if (rhs->used) {
                return false;
            }
            else {
                return lhs->server_id < rhs->server_id;
            }
        }
    }
};

struct VM
{
    int nameid=-1;
    // string name; // len <= 20
    int len=0;
    int core=0;
    int mem=0;
    int type=-1; // 0: single, 1: dual
    int magicVal=0;
} vms[maxm];


struct VM_Alive
{
    int vmid;
    VM* vm_ptr;
    int type ; // 0s1d
    int ser_id;
    int pos; // 0 : A, 1 : B
    int addReqID;
    int delReqID;
    VM_Alive(int vmid1, VM* vm_ptr1) {
        vmid = vmid1;
        vm_ptr = vm_ptr1;
        type = vm_ptr1->type;
        ser_id = -1;
        pos = -1; // 0 : A, 1 : B
        addReqID = -1;
        delReqID = -1;
    }
 
};


struct Cmp_VM_Alive
{
    bool operator()  (VMA const* lhs, VMA const* rhs) const
    {   
        if (lhs->vm_ptr->nameid != rhs->vm_ptr->nameid) {
            return lhs->vm_ptr->magicVal > rhs->vm_ptr->magicVal;
        }
        else
        {
            return lhs->vmid < rhs->vmid;
        }
        
    }
};

char vmname[maxm][max_name_len];
unordered_map<string, int> vmHashTable;

unordered_map<int, VMA*> vms_alive_hash;
map<VMA*, int, Cmp_VM_Alive> vms_alive_rbd;
map<VMA*, int, Cmp_VM_Alive> vms_alive_rbs;

map<SA*, int, Cmp_SerAD> sers_alive_rbd;
map<SA*, int, Cmp_SerAS> sers_alive_rbs;
unordered_map<int, SA*> sers_alive_hash;

struct Req
{
    bool type; // true:add, false: del
    int vmsd=-1;  // 0s1d
    VMA* vma_ptr=nullptr;
    int vmid=-1;
    int nameid = -1;
    int addReqid = -1;
} reqs[maxr];
//(add,vm_name,vm_id) or (del, vm_id)

struct _ReqDay
{
    int req_start_day;
    int req_times;
    int req_end_day;
    int req_max_pos;
    //  int req_pos_core;
    //  int req_pos_mem;
} ReqDay[maxm];

struct MigStr
{
    /*******
 *    MigPool
 *   first: reqID
 *  second: vmID
 *  third:  serID
 *  fourth: enum{ Dual: -1, Single: [A=0, B=1]}
 *
 * *******/
    int MigW;
    map<int, tuple<int, int, int>> MigPool;
};

struct ProcessStr
{
    int TypeQ;

    /*******
 *  ServerPool
 *  first: Rankid -- Servername
 *  second: Num
 *
 * DeployPool
 * first: reqID
 * second: serverID
 * third: enum{ Dual: -1, Single: [A=0, B=1]}
 *
*******/
    vector<tuple<int, int>> ServerPool;
    map<int, tuple<int, int>> DeployPool;
    void printDeploy()
    {
        TypeQ = ServerPool.size();
        printf("(purchase, %d)\n", TypeQ);
        for (int i = 1; i <= TypeQ; i++)
        {
            int raid = get<0>(ServerPool[i - 1]);
            int num = get<1>(ServerPool[i - 1]);
            printf("(%s, %d)\n", sersps[sers[raid].nameid].name,
                num);
        }
        printf("(migration, 0)\n");
        for (auto& elem : DeployPool)
        {
            auto& el = elem.second;
            int serID = get<0>(el);
            int posID = get<1>(el);
            if (posID < 0)
            {
                printf("(%d)\n", serID);
            }
            else
            {
                if (posID == 0) {
                    printf("(%d, A)\n", serID);
                }
                else {
                    printf("(%d, B)\n", serID);
                }
            }
        }
        DeployPool.clear(); 
        ServerPool.clear();
    }
};

#endif
