from __future__ import division

import matplotlib.pyplot as plt
import numpy as np
import sys
import re

grep_pattern_sqnorm_per=re.compile('spinor_field_sqnorm: Average FLOP/s per process:.*')
grep_pattern_sqnorm_avg=re.compile('spinor_field_sqnorm\: Average total FLOP\/s\:.*')
grep_pattern_dirac_per=re.compile('Dphi: Average FLOP/s per process:.*')
grep_pattern_dirac_avg=re.compile('Dphi: Average total FLOP/s:.*')


width=0.1
counter=1

comms_dirac_avg=[]
comms_dirac_per=[]
comms_spinor_avg=[]
comms_spinor_per=[]

balance_dirac_avg=[]
balance_dirac_per=[]
balance_spinor_avg=[]
balance_spinor_per=[]

compute_dirac_avg=[]
compute_dirac_per=[]
compute_spinor_avg=[]
compute_spinor_per=[]


#add more core counts if needed
core_count=[8,16,32,64,128,256]
test_type=["comms","compute","balance"]
out_dir="/home/sid/Work/DiRAC3-testsuite/src/plot_scripts/BSMBench/output/"
Title = "BSM Bench"
site = ""

colors = ['b','g','r','c','m','y','k']
rects=[]
API = []


def processFile(file_contents,benchtype,count):
    print "bench -"+benchtype+" - count -"+ str(count)
    line_count=len(file_contents)
    flag=0

    dirac_avg=[]
    dirac_per=[]
    spinor_per=[]
    spinor_avg=[]

    if(benchtype == "comms"):
        dirac_avg=comms_dirac_avg
        dirac_per=comms_dirac_per
        spinor_avg=comms_spinor_avg
        spinor_per=comms_spinor_per

    elif(benchtype == "balance"):
        dirac_avg=balance_dirac_avg
        dirac_per=balance_dirac_per
        spinor_avg=balance_spinor_avg
        spinor_per=balance_spinor_per

    elif(benchtype == "compute"):
        dirac_avg=compute_dirac_avg
        dirac_per=compute_dirac_per
        spinor_avg=compute_spinor_avg
        spinor_per=compute_spinor_per

    for i in range(line_count):
        line = file_contents[i]
        line.rstrip()

        lattice_dim_string = ""
        block_string = ""
        core = ""
        GFLOPS = ""


        match_sqnorm_per = grep_pattern_sqnorm_per.search(line)
        match_sqnorm_avg = grep_pattern_sqnorm_avg.search(line)
        match_dirac_per = grep_pattern_dirac_per.search(line)
        match_dirac_avg = grep_pattern_dirac_avg.search(line)
        tmp=line.split(':')

        if(match_sqnorm_per):
            val = float(tmp[2])
            print "square norm per node - " + benchtype + " - %f"%val
            spinor_per.append(val)
        elif(match_sqnorm_avg):
            val = float(tmp[2])
            print "square norm average - " + benchtype + " - %f"%val
            spinor_avg.append(val)
        elif(match_dirac_avg):
            val = float(tmp[2])
            print "dirac average - " + benchtype + " - %f"%val
            dirac_avg.append(val)
        elif(match_dirac_per):
            val = float(tmp[2])
            print "dirac per node  - " + benchtype + " - %f"%val
            dirac_per.append(val)


def plotFile():


    cores_s=[]
    cores_s = [str(x) for x in core_count]
    fig,axs = plt.subplots()

    block_labels=[]
    # block="comms dirac"
    # block_labels.append(block)
    # axs.plot(np.log2(core_count), np.array(comms_dirac_per), label=block,marker='*', linestyle='-')
    # block="comms spinor"
    # block_labels.append(block)
    # axs.plot(np.log2(core_count), np.array(comms_spinor_per), label=block,marker='o', linestyle=':')
    #
    # block="balance dirac"
    # block_labels.append(block)
    # axs.plot(np.log2(core_count), np.array(balance_dirac_per), label=block,marker='x', linestyle='-')
    # block="balance spinor"
    # block_labels.append(block)
    # axs.plot(np.log2(core_count), np.array(balance_spinor_per), label=block,marker='^', linestyle=':')
    #
    # block="compute dirac"
    # block_labels.append(block)
    # axs.plot(np.log2(core_count), np.array(compute_dirac_per), label=block,marker='H', linestyle='-')
    # block="compute spinor"
    # block_labels.append(block)
    # axs.plot(np.log2(core_count), np.array(compute_spinor_per), label=block,marker='8', linestyle=':')

    block="comms dirac"
    block_labels.append(block)
    axs.plot(np.log2(core_count), np.array(comms_dirac_avg), label=block,marker='*', linestyle='-')

    block="comms spinor"
    block_labels.append(block)
    axs.plot(np.log2(core_count), np.array(comms_spinor_avg), label=block,marker='^', linestyle=':')


    block="compute dirac"
    block_labels.append(block)
    axs.plot(np.log2(core_count), np.array(compute_dirac_avg), label=block,marker='o', linestyle=':')

    block="compute spinor"
    block_labels.append(block)
    axs.plot(np.log2(core_count), np.array(compute_spinor_avg), label=block,marker='H', linestyle='-')

    block="balance dirac"
    block_labels.append(block)
    axs.plot(np.log2(core_count), np.array(balance_dirac_avg), label=block,marker='x', linestyle='-')

    block="balance spinor"
    block_labels.append(block)
    axs.plot(np.log2(core_count), np.array(balance_spinor_avg), label=block,marker='8', linestyle=':')

    axs.set_xticks(np.log2(core_count))
    axs.set_xticklabels(cores_s)

    axs.set_ylabel('FLOPS*e11 - Total')
    axs.set_xlabel('MPI Procs')

    font = {
            'family' : 'monospace',
            'color'  : 'black',
            'weight' : 'normal',
            'size'   : 12,
            }

    #these are matplotlib.patch.Patch properties
    props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)

    #axs.text(0.85, 0.95, chartTitle, transform=axs.transAxes, fontsize=14,verticalalignment='top', bbox=props, fontdict=font)
    Title = "BSM Bench - COSMA"
    chartTitle = Title
    plt.text(0.5, 1.08, Title, horizontalalignment='center', family='monospace',fontsize=20,  transform = axs.transAxes)
    axs.legend(block_labels, loc='best', bbox_to_anchor=(1, 0.5))
    axs.set_xlim([2,9])
    #savepath = "/home/sid/Work/cosmic_charts/"+chartTitle+".png"
    plt.show()

#### MAIN #####

for i in core_count:
    for file_type in test_type:
        BSM_file_path = out_dir+file_type+str(i)+".out"
        BSM_file = open(str(BSM_file_path))
        BSM_read_file = BSM_file.readlines()
        line_count=len(BSM_read_file)

        processFile(BSM_read_file,file_type,i)
        counter=counter+1

plotFile()
