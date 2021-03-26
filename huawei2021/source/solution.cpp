
#include "source.h"
// #define _CRT_SECURE_NO_WARNINGS
#define STANDSTD_H
#define addLINE cout<<__LINE__<<endl;

inline bool cmpMagic(const Server& a, const Server& b)
{
    return a.CmptNum > b.CmptNum;
}
inline bool cmpMem(const Server& a, const Server& b) {
    if (a.mem != b.mem)
        return a.mem > b.mem;
    else
        return a.core > b.core;
}
inline bool cmpCore(const Server& a, const Server& b) {
    if (a.core != b.core)
        return a.core > b.core;
    else
        return a.mem > b.mem;
}
inline bool cmpVM(const VMA* lhs, const VMA* rhs)
{
    if (lhs->vm_ptr->nameid != rhs->vm_ptr->nameid) {
        if (lhs->addReqID < rhs->addReqID)
        {   
            if (lhs->vm_ptr-> magicVal <= rhs->vm_ptr->magicVal) {
                if (lhs->vm_ptr-> core <= rhs->vm_ptr->core && lhs->vm_ptr->mem <= rhs->vm_ptr->mem) {
                    return false;
                }
            }    
                return true;
        }
        else
        {
            if (lhs->vm_ptr->magicVal >= rhs->vm_ptr->magicVal) {
                if (lhs->vm_ptr->core >= rhs->vm_ptr->core && lhs->vm_ptr->mem >= rhs->vm_ptr->mem) {
                    return true;
                }
            }
                return false;
        }
   
    }
    else
    {
        return lhs->addReqID  < rhs->addReqID ;
    }
}

inline void ReadStdin()
{
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
    {
        sers[i].nameid = i;
        scanf(" (%20[a-z|A-Z|0-9], %d, %d, %d, %d)", sersps[i].name, &sers[i].core,
            &sers[i].mem, &sersps[i].costo, &sersps[i].costd);
        sersps[i].len = strlen(sersps[i].name);
        sers[i].CmptNum = sers[i].core * magicNum + sers[i].mem;
    }
    scanf("%d", &m);
    for (int i = 1; i <= m; i++)
    {
        vms[i].nameid = i;
        scanf(" (%20[a-z|A-Z|0-9.], %d, %d, %d)", vmname[i], &vms[i].core,
            &vms[i].mem, &vms[i].type);
        vms[i].len = strlen(vmname[i]);
        vms[i].magicVal = magicNum * vms[i].core + vms[i].mem;
        vmHashTable[string(vmname[i], vms[i].len)] = i;
    }
    scanf("%d", &t);
    int tmpk = 1;
    int size_vms_hash = 0;
    for (int j = 1; j <= t; j++)
    {
        int a;
        scanf("%d", &a);
        ReqDay[j].req_start_day = tmpk;
        ReqDay[j].req_times = a;

        for (int i = 1; i <= a; i++)
        {
            char tmpa;
            scanf(" (%c", &tmpa);
            int tmp_vmid;
            if (tmpa == 'a')
            {
                char tmps[max_name_len] = { '\0' };
                scanf("dd, %20[a-z|A-Z|0-9.], %d)", tmps, &tmp_vmid);
                int len = strlen(tmps);
                int nameid = vmHashTable[string(tmps, len)];
                reqs[tmpk].type = true;
                reqs[tmpk].vmid = tmp_vmid;

                reqs[tmpk].vmsd = vms[nameid].type;
                    VMA* tmpp = new VM_Alive(tmp_vmid, &vms[nameid]);
                    tmpp->addReqID = i;
                    vms_alive_hash[tmp_vmid] = tmpp;
                    reqs[tmpk].vma_ptr = tmpp;
                    thash = j;
                    size_vms_hash++; 
              
            }
            else
            {
                scanf("el, %d)", &tmp_vmid);
                reqs[tmpk].type = false;
                if (size_vms_hash < max_hash) {
                    VMA* tmpp = vms_alive_hash[tmp_vmid];
                    reqs[tmpk].vma_ptr = tmpp;
                    reqs[tmpk].vmsd = tmpp->type;
                    reqs[tmpk].vmid = tmp_vmid;
                }
                else {
                    reqs[tmpk].vmid = tmp_vmid;
                }
            }
            tmpk++;
        }
        ReqDay[j].req_end_day = tmpk;
    }
    int tmpn;
    while (scanf("%d",&tmpn)!=EOF)
    {
        continue;
    }
    fflush(stdin);
    return;
}


inline void getGlobalVal()
{
    auto req_ptr = reqs + 1;
    VMA* tmp_vma_ptr;
   // int t_bound = min((int)(t * tfactor), thash - 1);
    for (int i = 1; i <= t; i++)
    {
        memd_day[i] += memd_day[i - 1];
        cored_day[i] += cored_day[i - 1];
        for (int k = 0; k < 2; k++)
        {
            mems_day[i][k] += mems_day[i - 1][k];
            cores_day[i][k] += cores_day[i - 1][k];
        }
        int pos_d = 0, pos_s = 0;
        for (int j = 1; j <= ReqDay[i].req_times; j++)
        {
            tmp_vma_ptr = req_ptr->vma_ptr;
            VM& tmp_vm_ptr = *(tmp_vma_ptr->vm_ptr);
            if (req_ptr->type)
            {
                if (req_ptr->vmsd)
                {
                    memd_day[i] += tmp_vm_ptr.mem;
                    cored_day[i] += tmp_vm_ptr.core;
                    if (memd_add_max[i] < memd_day[i])
                    {
                        memd_add_max[i] = memd_day[i];
                        pos_d = j;
                    }
                    if (cored_add_max[i] < cored_day[i])
                    {
                        cored_add_max[i] = cored_day[i];
                        pos_d = j;
                    }
                }
                else
                {
                    int tmpf = (mems_day[i][1] - mems_day[i][0]) +
                        (cores_day[i][1] - cores_day[i][0]) * magicNum;
                    if (tmpf > 0)
                    {
                        tmp_vma_ptr->pos = 0;
                        mems_day[i][0] += tmp_vm_ptr.mem;
                        if (mems_add_max[i][0] < mems_day[i][0])
                        {
                            mems_add_max[i][0] = mems_day[i][0];
                            pos_s = j;
                        }
                        cores_day[i][0] += tmp_vm_ptr.core;
                        if (cores_add_max[i][0] < cores_day[i][0])
                        {
                            cores_add_max[i][0] = cores_day[i][0];
                            pos_s = j;
                        }
                    }
                    else
                    {
                        tmp_vma_ptr->pos = 1;
                        mems_day[i][1] += tmp_vm_ptr.mem;
                        if (mems_add_max[i][1] < mems_day[i][1])
                        {
                            mems_add_max[i][1] = mems_day[i][1];
                            pos_s = j;
                        }
                        cores_day[i][1] += tmp_vm_ptr.core;
                        if (cores_add_max[i][1] < cores_day[i][1])
                        {
                            cores_add_max[i][1] = cores_day[i][1];
                            pos_s = j;
                        }
                    }
                }
            }
            else
            {
                if (req_ptr->vmsd)
                {
                    memd_day[i] -= tmp_vm_ptr.mem;
                    cored_day[i] -= tmp_vm_ptr.core;
                }
                else
                {
                    int tmp_pos = tmp_vma_ptr->pos;
                    mems_day[i][tmp_pos] -= tmp_vm_ptr.mem;
                    cores_day[i][tmp_pos] -= tmp_vm_ptr.core;
                }
            }
            req_ptr++;
        }
        ReqDay[i].req_max_pos = max(pos_d, pos_s);
        globalMemD = max(globalMemD, memd_add_max[i]);
        globalMemS = max(globalMemS, max(mems_add_max[i][0], mems_add_max[i][1]));
        if (globalMemMD < globalMemD)
        {
            globalMemMD = globalMemD;
            t_maxd = i;
        }
        if (globalMemMS < globalMemS)
        {
            globalMemMS = globalMemS;
            t_maxs = i;
        }
    }
}

inline int findLocD(VMA* vma, ProcessStr& proStr)
{
    int core_need = vma->vm_ptr->core / 2;
    int mem_need = vma->vm_ptr->mem / 2;
    for (auto s : sers_alive_rbd)
    {
        SA* sa = s.first;
        int serid = s.second;
        if (core_need <= sa->core_remain[0] && mem_need <= sa->mem_remain[0])
        {
            sa->update(core_need, mem_need, 2);
            vma->ser_id = serid;
            proStr.DeployPool[vma->addReqID] = { serid, -1 };
            return serid;
        }
    }
    return -1;
}
inline tuple<int, int> findLocS(VMA* vma, ProcessStr& proStr)
{
    // return {server_id, pos}
    int core_need = vma->vm_ptr->core;
    int mem_need = vma->vm_ptr->mem;
    for (auto s : sers_alive_rbs)
    {
        SA* sa = s.first;
        int serid = s.second;
        for (int i = 0; i < 2; i++)
        {
            if (core_need <= sa->core_remain[i] && mem_need <= sa->mem_remain[i])
            {
                sa->update(core_need, mem_need, i);
                vma->ser_id = serid;
                vma->pos = i;
                proStr.DeployPool[vma->addReqID] = { serid, i };
                return { serid, i };
            }
        }
    }

    return { -1, 0 };
}

inline int binarySerch(float magicNeed, int coren, int memn, int cas,int arg=0)
{   
    Server* arr=sers;
    if (coren / memn > 2) {
        arg = 2;
    }
    else if (memn / coren > 2) {
        arg = 1;
    }
    switch (arg)
    {
      case  0:
        arr = sers;
        break;
      case   1:
        arr = serms;
        break;
      case  2:
        arr = sercs;
        break;
    default:
        break;
    }

    // return rank_id
    int l = 1, h = n;
    if (magicNeed < arr[n].CmptNum)
    {
        h = n;
    }
    else
    {
        while (l < h)
        {
            int mid = (l + h) >> 1;
            switch (arg)
            {
            case  0:
                if (arr[mid].CmptNum > magicNeed)
                {
                    l = mid + 1;
                }
                else
                {
                    h = mid;
                }
                break;
            case   1:
                if (cas) {
                    if (arr[mid].mem >= memn)
                    {
                        l = mid + 1;
                    }
                    else
                    {
                        h = mid;
                    }
                }
                else {
                    if (arr[mid].mem/2 >= memn)
                    {
                        l = mid + 1;
                    }
                    else
                    {
                        h = mid;
                    }
                }
             
                break;
            case  2:
                if (cas) {
                    if (arr[mid].core >= coren)
                    {
                        l = mid + 1;
                    }
                    else
                    {
                        h = mid;
                    }
                }
                else
                {
                    if (arr[mid].core / 2 >= coren) {
                        l = mid + 1;
                    }
                    else {
                        h = mid;
                    }
                }
               
                break;
            default:
                break;
            }
       
        }
    }
    if (cas)
    {   
        for (int j = h; j >= 1; j--)
        {
            if (arr[j].core >= coren && arr[j].mem >= memn)
            {
                return j;
            }
        }
    }
    else
    {
        for (int j = h; j >= 1; j--)
        {
            if ((arr[j].core / 2) >= coren && (arr[j].mem / 2) >= memn)
            {
                return j;
            }
        } 
    }
    return 1;
}

inline int buyComp(vector<VMA*>& adList, int loc, int cas, ProcessStr& prostr)
// 0s1d
{
    float magicNeed = 0;
    int coren = 0, memn = 0;
    if (cas)
    {
        for (int i = loc; i < adList.size(); i++)
        {   
            VM* vmptr = adList[i]->vm_ptr;
            magicNeed += vmptr->magicVal;
            coren += vmptr->core;
            memn += vmptr->mem;
        }
    }
    else
    {
        int core1 = 0, core2 = 0, mem1 = 0, mem2 = 0;
        float magic1 = 0, magic2 = 0;   
        for (int i = loc; i < adList.size(); i++)
        {
            VM* vmptr = adList[i]->vm_ptr;
            if (magic1 < magic2)
            {   
                magic1 += vmptr->magicVal;
                core1 += vmptr->core;
                mem1 += vmptr->mem;
            }
            else
            {
                magic2 += vmptr->magicVal;
                core2 += vmptr->core;
                mem2 += vmptr->mem;
            }
        }
        magicNeed = magic1 + magic2;
        coren = max(core1, core2);
        memn = max(mem1, mem2);

    }

    int M1Num = magicNeed / M1Ser;
    if (M1Num)
    {
        prostr.ServerPool.emplace_back(tuple<int, int>(1, M1Num));
        for (int i = 1; i <= M1Num; i++)
        {
            SA* tmpp = new Server_Alive(idSerCur, 1, cas);
            sers_alive_hash[idSerCur] = tmpp;
            if (cas)
            {
                sers_alive_rbd[tmpp] = idSerCur;
            }
            else
            {
                sers_alive_rbs[tmpp] = idSerCur;
            }
            idSerCur++;
        }
    }
    else
    {   
        int raid = binarySerch(magicNeed, coren, memn, cas);
        prostr.ServerPool.emplace_back(tuple<int, int>(raid, 1));
        SA* tmpp = new Server_Alive(idSerCur, raid, cas);
        sers_alive_hash[idSerCur] = tmpp;
        if (cas)
        {
            sers_alive_rbd[tmpp] = idSerCur;
        }
        else
        {
            sers_alive_rbs[tmpp] = idSerCur;
        }
        idSerCur++;
    }

    return 0;
}

inline void DelVMA(VMA* elem)
{
    int ser_id = elem->ser_id;
    int coreq = elem->vm_ptr->core;
    int memq = elem->vm_ptr->mem;

    if (elem->type)
    {
        sers_alive_hash[ser_id]->del(coreq / 2, memq / 2, 2);
        vms_alive_rbd.erase(elem);
    }
    else
    {
        sers_alive_hash[ser_id]->del(coreq, memq, elem->pos);
        vms_alive_rbs.erase(elem);
    }
 //  vms_alive_hash.erase(elem->vmid);
    delete elem;
}

inline void MigrateD()
{
    aliveVM = vms_alive_rbs.size() + vms_alive_rbd.size();
    int migCount = aliveVM / migDiv;
}

inline void MigrateS()
{
    aliveVM = vms_alive_rbs.size() + vms_alive_rbd.size();
    int migCount = aliveVM / migDiv;
}

int main()
{
    // std::ios::sync_with_stdio(false);
    //  std::cin.tie(0);
#ifdef CHRONO_TIMEH
    auto start = system_clock::now();
#endif
#ifndef STANDSTD_H
    FILE* stream1, * stream2;
    freopen_s(&stream1, "training-2.txt", "r", stdin);
    freopen_s(&stream2, "out.txt", "w", stdout);
#endif
   
    vmHashTable.reserve(1000);
    vms_alive_hash.reserve(max_hash);
    sers_alive_hash.reserve(1000);
  

    ReadStdin();
    getGlobalVal();

    memcpy(serms + 1, sers + 1, n * sizeof(Server));
    memcpy(sercs + 1, sers + 1, n * sizeof(Server));
    sort(sers + 1, sers + 1 + n, cmpMagic);
    sort(serms + 1, serms + 1 + n, cmpMem);
    sort(sercs + 1, sercs + 1 + n, cmpCore);

    M1Ser = sers[1].CmptNum;
    for (int i = 1; i <= n; i++)
    {
        sersps[sers[i].nameid].magicRank = i;
    }

    maxCompD = globalMemMD / sers[1].mem* tfactor;
    maxCompS = globalMemMS * 2 / sers[1].mem* tfactor;

   
    
    for (int i = 1; i <= t; i++)
    {
        ProcessStr proStr;
        
        if (i == 1)
        {
            for (idSerCur = 0; idSerCur < maxCompD; idSerCur++)
            {
                SA* tmp_sera = new Server_Alive(idSerCur, 1, 1);
                sers_alive_hash[idSerCur] = tmp_sera;
                sers_alive_rbd[tmp_sera] = idSerCur;
            }
            for (int i = 0; i < maxCompS; i++, idSerCur++)
            {
                SA* tmp_sera = new Server_Alive(idSerCur, 1, 0);
                sers_alive_hash[idSerCur] = tmp_sera;
                sers_alive_rbs[tmp_sera] = idSerCur;
            }
            if (idSerCur != 0) {
                proStr.ServerPool.emplace_back(tuple<int, int>(1, idSerCur));
            }
        }
        auto rq_ptr = &ReqDay[i];
        int start_id = rq_ptr->req_start_day;
        int end_id = rq_ptr->req_end_day;
        int max_id = rq_ptr->req_max_pos;
        if (start_id == end_id) {
            continue;
        }
        deque<VMA*> DelListS, DelListD;
        vector<VMA*> AddListD, AddListS;
        
        int indD = 0;
        int indS = 0;
        for (int j = 1; j <= max_id; j++)
        {   
            Req* reqs_ptr = &reqs[start_id+j-1];
            int vmid = reqs_ptr->vmid;
            VMA* tmpp;
            int vmsd;
            if (reqs_ptr->type)
            {   
                /*
                *     if (!reqs_ptr->vma_ptr) {
                    tmpp = new VM_Alive(vmid,&vms[reqs_ptr->nameid]);
                    vms_alive_hash[vmid] = tmpp;
                    reqs_ptr->vma_ptr = tmpp;
                    tmpp->addReqID = reqs_ptr->addReqid;
                }
                else {
                    tmpp = vms_alive_hash[vmid];
                }
                */
            
                tmpp = vms_alive_hash[vmid];
                vmsd = tmpp->type;
                if (vmsd)
                {
                    indD++;
                    vms_alive_rbd[tmpp] = vmid;
                    AddListD.push_back(tmpp);
                }
                else
                {
                    indS++;
                    vms_alive_rbs[tmpp] = vmid;
                    AddListS.push_back(tmpp);
                }
            }
            else
            {   
                if (!reqs_ptr->vma_ptr) {                    
                    tmpp = vms_alive_hash[vmid];
                    reqs_ptr->vma_ptr = tmpp;
                    reqs_ptr->vmsd = tmpp->type;
                }
                else {
                    tmpp = reqs_ptr->vma_ptr;
                }
                reqs_ptr->vmsd = tmpp->type;
                vmsd = tmpp->type;
                if (vmsd)
                {
                    indD++;
                    tmpp->delReqID = indD;
                    DelListD.push_back(tmpp);
                }
                else
                {
                    indS++;
                    tmpp->delReqID = indS;
                    DelListS.push_back(tmpp);
                }
            }
        }
        sort(AddListD.begin(), AddListD.end(), cmpVM);
        sort(AddListS.begin(), AddListS.end(), cmpVM);

        int indd = 0, AllOps = 0;
        while (indd < AddListD.size())
        {
            VMA* pvma = AddListD[indd];
            if (findLocD(pvma, proStr) >= 0)
            {
                indd++, AllOps++;
            }
            else
            {
                if (!DelListD.empty() && DelListD[0]->delReqID <= AllOps + 1)
                {
                    do
                    {
                        DelVMA(DelListD[0]);
                        AllOps++;
                        DelListD.pop_front();
                    } while (!DelListD.empty() && DelListD[0]->delReqID <= AllOps + 1);
                }
                else
                {
                    buyComp(AddListD, indd, 1, proStr);
                }
            }
        }
        int inds = 0;
        AllOps = 0;
        while (inds < AddListS.size())
        {  
            VMA* pvma = AddListS[inds];
            auto res = findLocS(pvma, proStr);
            int serId = get<0>(res);
            //  int pos = get<1>(res);
            if (serId >= 0)
            {
                inds++;
                AllOps++;
            }
            else
            {
                if (!DelListS.empty() && DelListS[0]->delReqID <= AllOps + 1)
                {
                    do
                    {
                        DelVMA(DelListS[0]);
                        AllOps++;
                        DelListS.pop_front();
                    } while (!DelListS.empty() && DelListS[0]->delReqID <= AllOps + 1);
                }
                else
                    buyComp(AddListS, inds, 0, proStr);
            }
        }
        for (auto elem : DelListD)
        {
            DelVMA(elem);
        }
        for (auto elem : DelListS)
        {   
            DelVMA(elem);
        }

       
        for (int j = start_id + max_id; j < end_id; j++)
        {   
            Req* reqs_ptr = &reqs[j];
            bool type = reqs_ptr->type;
            int vmsd = reqs_ptr->vmsd;
            VMA* pvma = reqs_ptr->vma_ptr;
       //     int vmid = pvma->vmid;      
     
            if (type)
            {   
                VM* vmptr = pvma->vm_ptr;
                if (vmsd)
                {
                    if (findLocD(pvma, proStr) <0) {                       
                        int raid = binarySerch(pvma->vm_ptr->magicVal, pvma->vm_ptr->core,
                            pvma->vm_ptr->mem,1);
                        proStr.ServerPool.emplace_back(tuple<int, int>(raid, 1));
                        SA* tmpp = new Server_Alive(idSerCur, raid, 1);
                        sers_alive_hash[idSerCur] = tmpp;
                        sers_alive_rbd[tmpp] = idSerCur;
                        idSerCur++;
                        findLocD(pvma, proStr);
                   }
                }
                else
                {
                    std::tuple<int,int> res =  findLocS(pvma, proStr);
                    if (get<0>(res) < 0) {
                        int raid = binarySerch(pvma->vm_ptr->magicVal, pvma->vm_ptr->core,
                            pvma->vm_ptr->mem, 0);
                        proStr.ServerPool.emplace_back(tuple<int, int>(raid, 1));
                        SA* tmpp = new Server_Alive(idSerCur, raid, 0);
                        sers_alive_hash[idSerCur] = tmpp;
                        sers_alive_rbs[tmpp] = idSerCur;
                        idSerCur++;
                        findLocS(pvma, proStr);
                    }
                }
            }
            else
            {
                DelVMA(pvma);
            }
        }
        DelListD.clear();
        DelListS.clear();
        AddListD.clear();
        AddListS.clear();
        proStr.printDeploy();
    }
    fflush(stdout);

#ifndef STANDSTD_H
    fclose(stdin);
    fclose(stdout);
#endif

#ifdef CHRONO_TIMEH
    auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "花费了"
        << double(duration.count()) * microseconds::period::num / microseconds::period::den
        << "秒" << endl;
#endif

    return 0;
}
