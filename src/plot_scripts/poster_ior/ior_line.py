from __future__ import division
import matplotlib.pyplot as plt
import numpy as np
import sys
import re



grep_pattern = re.compile(r'Summary of all tests:')

i=0
write = []
read = []
API=[]


write=[]
read=[]
flag=0

transferSize=0
fileSize=0
filePP=0
tasksPerNode=0
readMax=0.0
readMin=0.0
readMean=0.0
writeMax=0.0
writeMin=0.0
writeMean=0.0
NumberOfNodes=0
blockSize=0.0


write_nodes = []
read_nodes = []
read_bandwidth_max = []
write_bandwidth_max = []
read_bandwidth_min = []
write_bandwidth_min = []
read_bandwidth_mean = []


chartTitle=""


write_bandwidth_mean = []

colors = ['b','g','r','c','m','y','k']

#IOR_Summary = Enum('Operation','Max','Min','Mean','StdDev','Mean','TestNum','Tasks','tPN','reps','fPP','reord','reordoff','reordrand','seed', 'segcnt', 'blksiz', 'xsize', 'aggsize', 'API', 'RefNum')
class IOR_Summary:
    Operation=0
    Max=1
    Min=2
    Mean=3
    StdDev=4
    Mean=5
    TestNum=6
    Tasks=7
    tPN=8
    reps=9
    fPP=10
    reord=11
    reordoff=12
    reordrand=13
    seed=14
    segcnt=15
    blksiz=16
    xsize=17
    aggsize=18
    API=19
    RefNum=20


fig,axs = plt.subplots()
rects=[]
API = []


def processFile(file_contents):
    line_count=len(file_contents)
    flag=0
    for i in range(line_count):
        line = read_file[i]
        line.rstrip()
        line_check = re.search(grep_pattern, line)
        if(line_check):
            i=i+2
            write = read_file[i].split()
            i=i+1
            read = read_file[i].split()
            transferSize=float(read[IOR_Summary.xsize])/(1024*1024)
            fileSize=int(read[IOR_Summary.aggsize])

            fileSize= fileSize / (1024*1024*1024)
            blockSize= int(read[IOR_Summary.blksiz]) / (1024*1024*1024)
            filePP=int(read[IOR_Summary.fPP])
            tasksPerNode=int(read[IOR_Summary.tPN])

            if(flag==0):
                rAPI=read[IOR_Summary.API]
                rAPI=rAPI+" read "

                global chartTitle
                chartTitle ="FilePerProcess = %d\nFile Size = %dgB\nTransfer Size = %dmB" % (filePP, fileSize, transferSize)
                flag=1





            if("inf" in read[IOR_Summary.Max] or "nan" in read[IOR_Summary.Max] ):
                print "here "+read[IOR_Summary.Max]
                read[IOR_Summary.Max]= 0
            if("inf" in write[IOR_Summary.Max] or "nan" in write[IOR_Summary.Max]):
                print "here "+write[IOR_Summary.Max]
                write[IOR_Summary.Max]= 0

            readMax=float(read[IOR_Summary.Max])
            writeMax=float(write[IOR_Summary.Max])
            NumberOfNodes=int(read[IOR_Summary.Tasks])

            write_nodes.append(NumberOfNodes)
            read_nodes.append(NumberOfNodes)
            write_bandwidth_max.append(writeMax)
            read_bandwidth_max.append(readMax)

def plotFile(counter,site):

    one=[]
    two=[]
    basewidth=0.1
    init = counter -1;

    nodes_i=[]
    write_f = []
    read_f = []
    nodes_s = []


    color1 = colors[counter+init]
    color2 = colors[counter+init+1]

    nodes_s = [str(x) for x in read_nodes]
    write_s = [str(x) for x in write_bandwidth_max]
    read_s = [str(x) for x in read_bandwidth_max]

    nodes_i = [int(x) for x in write_nodes]
    write_f =  [float(x) for x in write_bandwidth_max]
    read_f = [float(x) for x in read_bandwidth_max]


    API.append(site+"_write")
    axs.plot(np.log2(nodes_i), np.array(write_f) / 1000, label=site+"_write",marker='+', linestyle='--',color=colors[counter])
    API.append(site+"_read")
    axs.plot(np.log2(nodes_i), np.array(read_f) / 1000, label=site+"_read",marker='+', linestyle='-',color=colors[counter])

    axs.set_xticks(np.log2(nodes_i))
    axs.set_xlim([-0.1,4.1])
    axs.set_xticklabels(nodes_s)
    # axs.tick_params(axis='x', which='major', pad=15)
    # for x,y,z in zip(np.log2(nodes_i),write_f,write_s):
    #     axs.text(x,y,z)
    # for x,y,z in zip(np.log2(nodes_i),read_f,read_s):
    #     axs.text(x,y,z)

### MAIN #####


width=0.1
counter=1


Title = "IOR - Strong Scaling - File Per Process"
site = ""
for IOR_file_i in sys.argv[1:]:




    if("site:" in IOR_file_i):
        site=IOR_file_i
        site=site.replace("site:","")
        continue


    write_bandwidth_max=[]
    read_bandwidth_max=[]
    read_nodes=[]
    write_nodes =[]
    read_nodes = []



    IOR_file = open(str(IOR_file_i))
    read_file = IOR_file.readlines()
    line_count=len(read_file)
    processFile(read_file)
    plotFile(counter,site)
    counter=counter+1


# add some text for labels, title and axes ticks
axs.set_ylabel('Max Bandwidth / GiB/s')
axs.set_xlabel('Number of Nodes')

font = {'family' : 'monospace',
        'color'  : 'black',
        'weight' : 'normal',
        'size'   : 12,
        }

# these are matplotlib.patch.Patch properties
props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)

# place a text box in upper left in axes coords
# axs.text(0.85, 0.95, chartTitle, transform=axs.transAxes, fontsize=14,
#         verticalalignment='top', bbox=props, fontdict=font)

# plt.text(0.5, 1.08, Title, horizontalalignment='center', family='monospace',fontsize=20,  transform = axs.transAxes)
# axs.legend(API, loc='best', bbox_to_anchor=(1, 0.5))
axs.legend(API, loc='upper left')
axs.set_ylim([0,12])
# axs.set_yscale("log", nonposx='clip')


# plt.show()
savepath = "IOR.eps"
fig.set_size_inches(10,5)
plt.savefig(savepath,bbox_inches='tight')
