from __future__ import division
import matplotlib.pyplot as plt
import numpy as np
import sys
import re

#configuration that we are interested in
minct="1"
#############################################
grep_pattern_command = re.compile(r'time_dslash_noqdp -a')
grep_pattern_gflops = re.compile(r'GFLOPS / node')
grep_pattern_lattice = re.compile('Local Lattice Size')
grep_pattern_block = re.compile('-by\s*\d+\s*-bz\s*\d+')
grep_pattern_core = re.compile('-c\s*\d*')
grep_pattern_minct = re.compile('-minct\s*'+minct)
grep_pattern_1core = re.compile('-c 1 -sy')
level1={}
lattice={}
colors = ['b','g','r','c','m','y','k']
fig,axs = plt.subplots()

def plotFile():
    lattlist = [ '16.4.4.4','16.4.4.8','16.4.8.8', '16.8.8.8', '16.8.8.16', '16.8.16.16', '16.16.16.16','16.16.16.32',  '16.16.32.32',  '16.32.32.32','32.32.32.32']
    numlatt = len(lattlist)
    init = 0;
    flops=[]
    for latt in lattlist:
        init = init+1
        flops.append(float(level1[latt]))

    ticklist = range(1,init+1)
    axs.plot(ticklist, flops, marker='o', linestyle='-')

    axs.set_xticks(ticklist)
    axs.set_xticklabels(lattlist)

    axs.set_ylabel('GFLOPS/node')
    axs.set_xlabel('Local Lattice dim')

    Title = "QPhiX - COSMA"
    plt.text(0.5, 1.08, Title, horizontalalignment='center', family='monospace',fontsize=20,  transform = axs.transAxes)
    plt.show()

def processFile(file_contents):
    flag=0
    minctCheck_flag = 0
    command = ""
    flop = ""
    debug = 0
    lattice = ""
    for i in range(line_count):
        line = read_file[i]
        line.rstrip()
        local_lattice = re.search(grep_pattern_lattice, line)
        if(local_lattice):
            tmp = line.split('=')
            tmp[1]=tmp[1].strip()
            tmp[1]=tmp[1].replace(' ','.')
            lattice = tmp[1]

        flop_check = re.search(grep_pattern_gflops,line)
        if(flop_check):
            arrGFLOPS = line.split()
            fGflops= float(arrGFLOPS[0])
            lattice.strip()
            lattice.replace(" ",".")
            level1[lattice]=fGflops



########## MAIN ###################
for IOR_file_i in sys.argv[1:]:
    IOR_file = open(str(IOR_file_i))
    read_file = IOR_file.readlines()
    line_count=len(read_file)
    processFile(read_file)
    plotFile()
