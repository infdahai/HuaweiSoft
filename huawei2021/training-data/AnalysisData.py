file1 = './training-1.txt'
file2 = './training-2.txt'

avg_ser_core = 0
avg_ser_mem = 0
avg_vm_core = 0
avg_vm_mem = 0

magicNumber: float = 0


class Server:
    name = ''
    core = 0
    mem = 0
    costo = 0
    costd = 0

    def __init__(self, tmpl: list):
        self.name = tmpl[0]
        self.core = int(tmpl[1])
        self.mem = int(tmpl[2])
        self.costo = int(tmpl[3])
        self.costd = int(tmpl[4])
        global avg_ser_core, avg_ser_mem
        avg_ser_core += self.core
        avg_ser_mem += self.mem


class VM:
    name = ''
    core = 0
    mem = 0
    type = 0

    def __init__(self, tmpl: list):
        self.name = tmpl[0]
        self.core = int(tmpl[1])
        self.mem = int(tmpl[2])
        self.type = int(tmpl[3])

        global avg_vm_core, avg_vm_mem
        avg_vm_core += self.core
        avg_vm_mem += self.mem


vm_list: [VM] = []
ser_list: [Server] = []

with open(file1, "r") as f1:
    #  r = f1.read()
    k = f1.read().splitlines()
    i = 1
    n = int(k[0])
    for strk in k[1:n + 1]:
        tmpk = strk[1:-1].split(',')
        ser_list.append(Server(tmpk))
        i += 1
    m = int(k[i])
    i += 1
    for strk in k[i:i + m]:
        tmpk = strk[1:-1].split(',')
        vm_list.append(VM(tmpk))
        i += 1

avg_ser_core /= n
avg_ser_mem /= n
avg_vm_mem /= m
avg_vm_core /= m
a = avg_ser_mem / avg_ser_core
b = avg_vm_mem / avg_vm_core
print(file1[2:] + ": ")
print("n: " + str(n) + " m: " + str(m))
print("avg_ser_core:" + str(avg_ser_core) + "\n")
print("avg_ser_mem:" + str(avg_ser_mem) + "\n")
print("avg_vm_core:" + str(avg_vm_core) + "\n")
print("avg_vm_mem:" + str(avg_vm_mem) + "\n")
print("mem/core 1: " + str(a) + " 2:" +
      str(b))
print("result: " + str((a - b) / b))
mg1: float = (n * a + m * b) / (n + m)
print("MG: " + str(mg1))
print("\t")

with open(file2, "r") as f2:
    k = f2.read().splitlines()
    i = 1
    n = int(k[0])
    for strk in k[1:n + 1]:
        tmpk = strk[1:-1].split(',')
        ser_list.append(Server(tmpk))
        i += 1
    m = int(k[i])
    i += 1
    for strk in k[i:i + m]:
        tmpk = strk[1:-1].split(',')
        vm_list.append(VM(tmpk))
        i += 1

avg_ser_core /= n
avg_ser_mem /= n
avg_vm_mem /= m
avg_vm_core /= m
a = avg_ser_mem / avg_ser_core
b = avg_vm_mem / avg_vm_core
print(file2[2:] + ": ")
print("n: " + str(n) + " m: " + str(m))
print("avg_ser_core:" + str(avg_ser_core) + "\n")
print("avg_ser_mem:" + str(avg_ser_mem) + "\n")
print("avg_vm_core:" + str(avg_vm_core) + "\n")
print("avg_vm_mem:" + str(avg_vm_mem) + "\n")
print("mem/core 1: " + str(a) + " 2:" +
      str(b))
print("result: " + str((a - b) / b))
mg2: float = (n * a + m * b) / (n + m)
print("MG: " + str(mg2))

print("\t")

magicNumber = sum([mg1, mg2])/2
print("MagicNumber: " + str(magicNumber))
