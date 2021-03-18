
#include "source.h"

#define STANDSTD_H

inline bool cmpMagic(const Server &a, const Server &b)
{
  return a.CmptNum > b.CmptNum;
}
inline bool cmpVM(const VMA *a, const VMA *b)
{
  return a->magicVal > b->magicVal;
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
  char tmpa;
  int tmp_vmid;

  int tmpk = 1;
  for (int j = 1; j <= t; j++)
  {
    int a;
    scanf("%d", &a);
    ReqDay[j].req_start_day = tmpk;
    ReqDay[j].req_times = a;

    for (int i = 1; i <= a; i++)
    {
      scanf(" (%c", &tmpa);
      if (tmpa == 'a')
      {
        char tmps[max_name_len];
        scanf("%*c, %20[a-z|A-Z|0-9.], %d)", tmps, &tmp_vmid);
        int len = strlen(tmps);
        int nameid = vmHashTable[string(tmps, len)];
        VMA *tmpp = new VMA(tmp_vmid, &vms[nameid]);
        tmpp->addReqID = i;
        vms_alive_hash[tmp_vmid] = tmpp;
        reqs[tmpk].type = true;
        reqs[tmpk].vma_ptr = tmpp;
        reqs[tmpk].type = tmpp->type;
      }
      else
      {
        scanf("%*c, %d)", &tmp_vmid);
        reqs[tmpk].type = false;
        VMA *tmpp = vms_alive_hash[tmp_vmid];
        reqs[tmpk].vma_ptr = tmpp;
        reqs[tmpk].vmsd = vms_alive_hash[tmp_vmid]->type;
      }
      tmpk++;
    }

    ReqDay[j].req_end_day = tmpk;
  }
}

inline void getGlobalVal()
{
  auto req_ptr = reqs + 1;
  VMA *tmp_vma_ptr;
  VM *tmp_vm_ptr;
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
      auto &tmp_vm_ptr = *(tmp_vma_ptr->vm_ptr);
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

inline int findLocD(VMA *vma, ProcessStr &proStr)
{
  int core_need = vma->vm_ptr->core / 2;
  int mem_need = vma->vm_ptr->mem / 2;
  for (auto s : sers_alive_rbd)
  {
    SA *sa = s.first;
    int serid = s.second;
    if (core_need <= sa->hole[0].core_remain && mem_need <= sa->hole[0].mem_remain)
    {
      sa->update(core_need, mem_need, 2);
      int res = sa->server_id;
      vma->ser_id = res;
      proStr.DeployPool[vma->addReqID] = {res, -1};
      return res;
    }
  }
  return -1;
}
inline tuple<int, int> findLocS(VMA *vma, ProcessStr &proStr)
{
  // return {server_id, pos}
  int core_need = vma->vm_ptr->core;
  int mem_need = vma->vm_ptr->mem;
  for (auto s : sers_alive_rbs)
  {
    SA *sa = s.first;
    int serid = s.second;
    for (int i = 0; i < 2; i++)
    {
      if (core_need <= sa->hole[i].core_remain && mem_need <= sa->hole[i].mem_remain)
      {
        sa->update(core_need, mem_need, i);
        int res = sa->server_id;
        vma->ser_id = res;
        vma->pos = i;
        proStr.DeployPool[vma->addReqID] = {res, i};
        return {res, i};
      }
    }
  }

  return {-1, 0};
}

inline int binarySerch(float magicNeed, int coren, int memn, int cas)
{
  // return rank_id
  int l = 1, h = n;
  int res;
  if (magicNeed < sers[n].CmptNum)
  {
    h = n;
  }
  else
  {
    while (l < h)
    {
      int mid = (l + h) >> 1;
      if (sers[mid].CmptNum > magicNeed)
      {
        l = mid + 1;
      }
      else
      {
        h = mid;
      }
    }
  }
  if (cas)
  {
    for (int j = h; j >= 1; j--)
    {
      if (sers[j].core >= coren && sers[j].mem >= memn)
      {
        return j;
      }
    }
  }
  else
  {
    for (int j = h; j >= 1; j--)
    {
      if ((sers[j].core >> 1) >= coren && (sers[j].mem >> 1) >= memn)
      {
        return j;
      }
    }
  }

  return 1;
}

inline int buyComp(vector<VMA *> &adList, int loc, int cas, ProcessStr &prostr)
// 0s1d
{
  float magicNeed;
  int coren, memn;
  if (cas)
  {
    for (int i = loc; i <= adList.size(); i++)
    {
      magicNeed += adList[i]->magicVal;
      coren += adList[i]->vm_ptr->core;
      memn += adList[i]->vm_ptr->mem;
    }
  }
  else
  {
    int core1, core2, mem1, mem2;
    for (int i = loc; i <= adList.size(); i++)
    {
      magicNeed += adList[i]->magicVal;
      if (core1 < core2)
        core1 += adList[i]->vm_ptr->core;
      else
        core2 += adList[i]->vm_ptr->core;
      if (mem1 < mem2)
        mem1 += adList[i]->vm_ptr->mem;
      else
        mem2 += adList[i]->vm_ptr->mem;
    }
    coren = max(core1, core2);
    memn = max(mem1, mem2);
  }

  int M1Num = magicNeed / M1Ser;
  if (M1Num)
  {
    prostr.ServerPool[1] += M1Num;
    for (int i = 1; i <= M1Num; i++)
    {
      SA *tmpp = new Server_Alive(idSerCur, 1, 1);
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
    prostr.ServerPool[raid]++;
    SA *tmpp = new Server_Alive(idSerCur, raid, 1);
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
  prostr.TypeQ++;

  return 0;
}

inline void DelVMA(VMA *elem)
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
  //   vms_alive_hash.erase(elem->vmid);
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
#ifdef STANDSTD_H
  ReadStdin();
#else
  freopen("../training-data/training-1.txt", "r", stdin);
  freopen("../training-data/out.txt", "w", stdout);
#endif
  getGlobalVal();

  sort(sers + 1, sers + 1 + n, cmpMagic);
  M1Ser = sers[1].CmptNum;
  for (int i = 1; i <= n; i++)
  {
    sersps[sers[i].nameid].magicRank = i;
  }

  maxCompD = globalMemMD / sers[1].mem;
  maxCompS = globalMemMS * 2 / sers[1].mem;

  for (idSerCur = 0; idSerCur < maxCompD; idSerCur++)
  {
    SA *tmp_sera = new Server_Alive(idSerCur, 1, 1);
    sers_alive_hash[idSerCur] = tmp_sera;
    sers_alive_rbd[tmp_sera] = idSerCur;
  }
  for (int i = 0; i < maxCompS; i++, idSerCur++)
  {
    SA *tmp_sera = new Server_Alive(idSerCur, 1, 0);
    sers_alive_hash[idSerCur] = tmp_sera;
    sers_alive_rbs[tmp_sera] = idSerCur;
  }
  PurStr.TypeQ = 1;
  PurStr.ServerPool[1] = idSerCur;

  auto rq_ptr = &ReqDay[1];
  Req *reqs_ptr;
  for (int i = 1; i <= t; i++)
  {
    ProcessStr proStr;
    int start_id = rq_ptr->req_start_day;
    int end_id = rq_ptr->req_end_day;
    int max_id = rq_ptr->req_max_pos;
    // shuffleBound = aliveVM * shuffleFact;

    deque<VMA *> DelListS, DelListD;
    vector<VMA *> AddListD, AddListS;
    reqs_ptr = &reqs[start_id];
    int j = 1;
    int indD = 0;
    int indS = 0;
    for (; j <= max_id; j++)
    {
      VMA *tmpp = reqs_ptr->vma_ptr;
      int vmsd = reqs_ptr->vmsd;
      if (reqs_ptr->type)
      {
        if (vmsd)
        {
          indD++;
          vms_alive_rbd[tmpp] = tmpp->vmid;
          AddListD.emplace_back(tmpp);
        }
        else
        {
          indS++;
          vms_alive_rbs[tmpp] = tmpp->vmid;
          AddListS.emplace_back(tmpp);
        }
      }
      else
      {
        if (vmsd)
        {
          indD++;
          tmpp->delReqID = indD;
          DelListD.emplace_back(tmpp);
        }
        else
        {
          indS++;
          tmpp->delReqID = indS;
          DelListS.emplace_back(tmpp);
        }
      }
      reqs_ptr++;
    }
    sort(AddListD.begin(), AddListD.end(), cmpVM);
    sort(AddListS.begin(), AddListS.end(), cmpVM);

    int indd = 0, AllOps = 0;
    while (indd < AddListD.size())
    {
      VMA *pvma = AddListD[indd];
      if (findLocD(pvma, proStr) >= 0)
      {
        indd++, AllOps++;
      }
      else
      {
        if (!DelListD.empty() && DelListD[0]->delReqID <= AllOps + 1)
        {
          DelVMA(DelListD[0]);
          AllOps++;
          DelListD.pop_front();
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
      VMA *pvma = AddListS[inds];
      auto res = findLocS(pvma, proStr);
      int serId = get<0>(res);
      int pos = get<1>(res);
      if (serId >= 0)
      {
        inds++;
        AllOps++;
      }
      else
      {
        if (!DelListS.empty() && DelListS[0]->delReqID <= AllOps + 1)
        {
          DelVMA(DelListS[0]);
          AllOps++;
          DelListS.pop_front();
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

    proStr.TypeQ = proStr.ServerPool.size();
    for (; j < end_id; j++)
    {
      bool type = reqs_ptr->type;
      int vmsd = reqs_ptr->vmsd;
      VMA *pvma = reqs_ptr->vma_ptr;
      if (type)
      {
        if (vmsd)
        {
          findLocD(pvma, proStr);
        }
        else
        {
          findLocS(pvma, proStr);
        }
      }
      else
      {
        DelVMA(pvma);
      }
      reqs_ptr++;
    }
    proStr.printDeploy();
  }

#ifdef CHRONO_TIMEH
  auto end = system_clock::now();
  auto duration = duration_cast<microseconds>(end - start);
  cout << "花费了"
       << double(duration.count()) * microseconds::period::num / microseconds::period::den
       << "秒" << endl;
#endif

  return 0;
}
