
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

const int intSize = sizeof(int);
struct VM_Alive;
struct Server_Alive;

using ull = unsigned long long;
using VMA = VM_Alive;
using SA = Server_Alive;

const int maxn = 105;
const int maxm = 1000 + 5;
const int maxr = 10e4 + 5;
const int max_name_len = 21;
const float magicNum = 1.2; // 1.18 more exactly
const int migDiv = 200;
const float shuffleFact = 0.2;

int n; // [1..100]
int m; // [1..1000]
int t; // [1..1000]
int t_maxd, t_maxs;
// int r[maxm]; // sum(r[..]) <= 10e5
int idSerCur;
int aliveVM;
int migBound;
int shuffleBound;
int startPos = 0;
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
int maxCompD, maxCompS;

struct Server
{
    int nameid;
    int core; //1024
    int mem;  // 1024
    float CmptNum;
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
    int rankid;
    struct
    {
        int core_remain;
        int mem_remain;
    } hole[2];
    int sd; // 0 : single, 1: dual

    bool used;
    //    std::vector<VM_Alive *> vma_arr;
    Server_Alive(int serid, int raid, int sd_)
    {
        server_id = serid;
        rankid = raid;
        server_ptr = &sers[raid]; //！
        hole[0] = hole[1] = {server_ptr->core / 2,
                             server_ptr->mem / 2};
        sd = sd_;
        used = false;
    }
    void update(int coren, int memn, int cas)
    {
        if (cas < 2)
        {
            hole[cas].core_remain -= coren;
            hole[cas].mem_remain -= memn;
        }
        else
        {
            hole[1].core_remain = (hole[0].core_remain -= coren);
            hole[1].mem_remain = (hole[0].mem_remain -= memn);
        }
    }
    void del(int coreq, int memq, int cas)
    {
        if (cas < 2)
        {
            hole[cas].core_remain += coreq;
            hole[cas].core_remain += memq;
        }
        else
        {
            hole[1].core_remain = (hole[0].core_remain += coreq);
            hole[1].mem_remain = (hole[0].mem_remain += memq);
        }
    }
};

struct Cmp_SerAD
{
    bool operator()(const SA *lhs, const SA *rhs)
    {
        //    if (lhs->rankid != rhs->rankid)
        //       return lhs->rankid < rhs->rankid;
        auto &lh = lhs->hole[0];
        int lcore = lh.core_remain;
        int lmem = lh.mem_remain;
        auto &rh = rhs->hole[0];
        int rcore = rh.core_remain;
        int rmem = rh.mem_remain;
        return 0 < (rmem - lmem) + magicNum * (rcore - lcore);
    }
};

struct Cmp_SerAS
{
    bool operator()(const SA *lhs_p, const SA *rhs_p)
    {
        //      if (lhs_p->rankid != rhs_p->rankid)
        //          return lhs_p->rankid < rhs_p->rankid;
        auto &lh0 = lhs_p->hole[0];
        auto &lh1 = lhs_p->hole[1];
        int lcore = lh0.core_remain + lh1.core_remain;
        int lmem = lh0.mem_remain + lh1.mem_remain;
        auto &rh0 = rhs_p->hole[0];
        auto &rh1 = rhs_p->hole[1];
        int rcore = rh0.core_remain + rh1.core_remain;
        int rmem = rh0.mem_remain + rh1.mem_remain;
        return 0 < (rmem - lmem) + magicNum * (rcore - lcore);
    }
};

struct VM
{
    int nameid;
    // string name; // len <= 20
    int len;
    int core;
    int mem;
    int type; // 0: single, 1: dual
    float magicVal;
} vms[maxm];

struct VM_Alive
{
    int vmid;
    VM *vm_ptr;
    int type; // 0s1d
    int ser_id = -1;
    int pos; // 0 : A, 1 : B
    float magicVal;
    int addReqID;
    int delReqID;
    VM_Alive(int vmid1, VM *vm_ptr1)
    {
        vmid = vmid1;
        vm_ptr = vm_ptr1;
        type = vm_ptr1->type;
        magicVal = vm_ptr1->magicVal;
    }
};

struct Cmp_VM_Alive
{
    bool operator()(const VMA *lhs, const VMA *rhs)
    {
        return lhs->magicVal > rhs->magicVal;
    }
};

char vmname[maxn][max_name_len];
unordered_map<string, int> vmHashTable;

unordered_map<int, VMA *> vms_alive_hash;
map<VMA *, int, Cmp_VM_Alive> vms_alive_rbd;
map<VMA *, int, Cmp_VM_Alive> vms_alive_rbs;

map<SA *, int, Cmp_SerAD> sers_alive_rbd;
map<SA *, int, Cmp_SerAS> sers_alive_rbs;
unordered_map<int, SA *> sers_alive_hash;

struct Req
{
    bool type; // true:add, false: del
    int vmsd;  // 0s1d
    VMA *vma_ptr;
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
    vector<string> migStr;
    vector<int, tuple<int, int, int>> MigPool;
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
    unordered_map<int, int> ServerPool;
    map<int, tuple<int, int>> DeployPool;
    void printDeploy()
    {
        printf("(purchase, %d)\n", TypeQ);
        for (int i = 1; i <= TypeQ; i++)
        {
            // string ser_name;
        }
        printf("(migration, 0)\n");
        for (auto &elem : DeployPool)
        {
            auto &el = elem.second;
            int serID = get<0>(el);
            int posID = get<1>(el);
            if (posID < 0)
            {
                printf("(%d)\n", serID);
            }
            else
            {
                printf("(%d, %d)\n", serID, posID);
            }
        }
    }
} PurStr;

#endif
