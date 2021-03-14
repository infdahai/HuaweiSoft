
#include "source.h"

#define STANDSTD_H

/*
inline bool cmpCore(const Server &a, const Server &b)
{
  if (a.core != b.core)
    return a.core < b.core;
  return a.mem < b.mem;
}

inline bool cmpMem(const Server &a, const Server &b)
{
  if (a.mem != b.mem)
    return a.mem < b.mem;
  return a.core < b.core;
}
*/

const float magicNumber = 1.2; // 1.18 more exactly

inline bool cmpMagic(const Server &a, const Server &b)
{
  float i = a.mem + magicNumber * a.core;
  float j = a.mem + magicNumber * b.core;
  return i < j;
}

int main()
{
  // std::ios::sync_with_stdio(false);
  //  std::cin.tie(0);

#ifdef STANDSTD_H

  scanf("%d", &n);
  for (int i = 1; i <= n; i++)
  {
    sers[i].nameid = i;
    scanf(" (%20[a-z|A-Z|0-9], %d, %d, %d, %d)", sersps[i].name, &sers[i].core,
          &sers[i].mem, &sersps[i].costo, &sersps[i].costd);
    sersps[i].len = strlen(sersps[i].name);
  }
  scanf("%d", &m);
  for (int i = 1; i <= m; i++)
  {
    vms[i].nameid = i;
    scanf(" (%20[a-z|A-Z|0-9.], %d, %d, %d)", vmname[i], &vms[i].core,
          &vms[i].mem, &vms[i].type);
    vms[i].len = strlen(vmname[i]);
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
        reqs[tmpk].type = true;
        vms_alive[tmp_vmid] = VM_Alive(tmp_vmid, &vms[nameid]);
        reqs[tmpk].vma_ptr = &vms_alive[tmp_vmid];
      }
      else
      {
        scanf("%*c, %d)", &tmp_vmid);
        reqs[tmpk].type = false;
        reqs[tmpk].vma_ptr = &vms_alive[tmp_vmid];
      }
      tmpk++;
    }

    ReqDay[j].req_end_day = tmpk;
  }
#else

#endif

  auto req_ptr = reqs + 1;
  VM_Alive *tmp_vma_ptr;
  VM *tmp_vm_ptr;
  for (int i = 1; i <= t; i++)
  {
    int pos_d, pos_s;
    for (int j = 1; j <= ReqDay[i].req_times; j++)
    {
      tmp_vma_ptr = req_ptr->vma_ptr;
      auto &tmp_vm_ptr = *(tmp_vma_ptr->vm_ptr);
      if (req_ptr->type)
      {
        if (tmp_vm_ptr.type)
        {
          memd_day[i] += tmp_vm_ptr.mem;
          cored_day[i] += tmp_vm_ptr.core;
          if (memd_add_max[i] < memd_day[i])
          {
            memd_add_max[i] = memd_day[i];
            pos_d = j;
          }
          if (cored_add_max[i] < cored_day[i])
          // really need the judgement ??
          {
            cored_add_max[i] = cored_day[i];
            pos_d = j;
          }
        }
        else
        {
          int tmpa = (mems_day[i][1] - mems_day[i][0]) + (cores_day[i][1] - cores_day[i][0]) * magicNumber;
          if (tmpa > 0)
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
        if (tmp_vm_ptr.type)
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
    ReqDay[i].req_max_pos = max(pos_d, pos_s) - 1;
    globalMemD = max(globalMemD, memd_add_max[i]);
    globalMemS = max(globalMemS, max(mems_add_max[i][0], mems_add_max[i][1]));
    int tmpv = globalMemD / 2 + globalMemS;
    if (globalMemSum < tmpv)
    {
      globalMemSum = tmpv;
      t_max = i;
    }
    /*
    globalCoreD = max(globalCoreD, cored_add_max[i]);
    globalCoreS = max(globalCoreS, max(cores_add_max[i][0], cores_add_max[i][1]));
    globalCoreSum = max(globalCoreSum, globalCoreD / 2 + globalCoreS);
    */
  }

  sort(sers + 1, sers + 1 + n, cmpMagic);
  for (int i = 1; i <= n; i++)
  {
    sersps[sers[i].nameid].magicRank = i;
  }
  /*
  sort(sers + 1, sers + 1 + n, cmpCore);
  for (int i = 1; i <= n; i++)
  {
    sersps[sers[i].nameid].coreRank = i;
  }
  sort(sers + 1, sers + 1 + n, cmpMem);
  for (int i = 1; i <= n; i++)
  {
    sersps[sers[i].nameid].memRank = i;
  }
  */

  int MaxComputer = globalMemSum / sers[1].mem + 1;
  // int startComputer = MaxComputer * (t_max / (double)t);
  int final_day_mem = memd_add_max[t] + max(mems_add_max[t][0], mems_add_max[t][1]);
  int startComputer = final_day_mem / sers[1].mem + 1;
  // else locate in Mode num.

  for (int i = 1; i <= t; i++)
  {
    if (i == 1)
    {
      printf("(purchase, %d)\n", Q);
    }
  }

  return 0;
}
