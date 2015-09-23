from __future__ import division
import matplotlib.pyplot as plt
import numpy as np
import sys
import re





threads_pattern = re.compile(r'Number of Threads counted =')
copy_pattern = re.compile(r'Copy:')
add_pattern= re.compile(r'Add:')
scale_pattern=re.compile(r'Scale:')
triad_pattern=re.compile(r'Triad:')

i=0
numThreads = []
BW = []
colors = ['b','g','r','c','m','y','k']

fig,axs = plt.subplots()
rects=[]


def processFile(file_contents,operation):
    line_count=len(file_contents)
    flag=0
    for i in range(line_count):
        line = read_file[i]
        line.rstrip()

        if(operation=='copy'):

            line_check = re.search(threads_pattern, line)
            if(line_check):

                thread_split = line.split()
                threads = thread_split[5]
                numThreads.append(threads)
                continue

            line_check = re.search(copy_pattern, line)
            if(line_check):
                copy_split = line.split()
                temp = copy_split[1]
                BW.append(temp)
                continue


        if(operation=='add'):
            line_check = re.search(threads_pattern, line)
            if(line_check):

                thread_split = line.split()
                threads = thread_split[5]
                numThreads.append(threads)
                continue

            line_check = re.search(add_pattern, line)
            if(line_check):
                line_split = line.split()
                temp = line_split[1]
                BW.append(temp)
                continue


        if(operation=='scale'):
            line_check = re.search(threads_pattern, line)
            if(line_check):

                thread_split = line.split()
                threads = thread_split[5]
                numThreads.append(threads)
                continue

            line_check = re.search(scale_pattern, line)
            if(line_check):
                scale_split = line.split()
                temp = scale_split[1]
                BW.append(temp)
                continue



        if(operation=="triad"):

            line_check = re.search(threads_pattern, line)
            if(line_check):


                thread_split = line.split()
                threads = thread_split[5]
                numThreads.append(threads)
                continue

            line_check = re.search(triad_pattern, line)
            if(line_check):

                triad_split = line.split()
                temp = float(triad_split[1])
                BW.append(temp)
                continue


def plotFile(site):


    numThreads_i = [int(x) for x in numThreads]
    BW_S = [str(x) for x in BW]
    axs.plot(np.log2(numThreads_i), np.array(BW)/1000, label=site,marker='+', linestyle='-',color=colors[counter])

    axs.set_xticks(np.log2(numThreads_i))
    axs.set_xlim([-0.1,6.1])
    axs.set_xticklabels(numThreads)

    # axs.tick_params(axis='x', which='major', pad=15)

    # for x,y,z in zip(np.log2(numThreads_i),BW,BW_S):
    #     axs.text(x,y,z)

### MAIN #####

counter=1
site = ""
operation = "triad"
sites = []

#iterate through the files
#the command line is in this order
#site <file> site <file> ... site <file>
for stream_file in sys.argv[1:]:

    if("site" in stream_file):

        site=stream_file
        site=site.replace("site:","")
        sites.append(site)
        continue
    BW = []
    numThreads = []
    stream_out_file = open(str(stream_file))
    read_file = stream_out_file.readlines()
    line_count=len(stream_file)
    processFile(read_file,operation)
    counter=counter+1
    plotFile(site)

# add some text for labels, title and axes ticks

axs.set_ylabel('Max Bandwidth / GiB/s')
axs.set_xlabel('Number of OpenMP Threads')

font = {'family' : 'monospace',
        'color'  : 'black',
        'weight' : 'normal',
        'size'   : 12,
        }

Title = "STREAM " + operation

# these are matplotlib.patch.Patch properties
props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)

# place a text box in upper left in axes coords
chartTitle=""
# axs.text(0.05, 0.95, chartTitle, transform=axs.transAxes, fontsize=14,
#         verticalalignment='top', bbox=props, fontdict=font)
# plt.text(0.5, 1.08, Title, horizontalalignment='center', family='monospace',fontsize=20,  transform = axs.transAxes)

#plt.title(Title,fontsize=16,fami

# axs.legend(sites, loc='best', bbox_to_anchor=(1, 0.5),
#           fancybox=True, shadow=True)
axs.legend(sites, loc='upper left')
axs.set_xticks([0,1,2,3,4,5,6])
axs.set_xticklabels([1,2,4,8,16,32,64])
axs.set_ylim([0,100])

# plt.show()
savepath = "STREAM.eps"
fig.set_size_inches(5,5)
plt.savefig(savepath,bbox_inches='tight')
