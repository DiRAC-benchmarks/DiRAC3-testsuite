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
grep_pattern_lattice = re.compile(r'Local Lattice Size')
#grep_pattern_lattice = re.compile('-x\s*\d+\s*-y\s*\d+\s*-z\s*\d+\s*-t\s*\d+')
grep_pattern_block = re.compile('-by\s*\d+\s*-bz\s*\d+')
grep_pattern_core = re.compile('-c\s*\d*')
grep_pattern_minct = re.compile('-minct\s*'+minct)
grep_pattern_1core = re.compile('-c 1 -sy')
level1={}
lattice={}
colors = ['b','g','r','c','m','y','k']

def plotFile():
    lattlist = lattice.keys()
    lattlist.sort()
    init = 0;
    for latt in lattlist:
        fig,axs = plt.subplots()
        block_labels = []
        print latt
        blocks=lattice[latt]
        blocklist = blocks.keys()
        blocklist.sort()
        for block in blocklist:
            cores_s=[]
            band_s=[]
            cores_i=[]
            band_f=[]

            print "\t%s" % block
            cores = blocks[block]

            corelist =  [int(x) for x in cores.keys()]
            corelist.sort()

            for core in corelist:
                flops = float(cores[str(core)])
                print('\t\t{} : {}'.format(core, flops))
                cores_s.append(str(core))
                band_s.append(str(flops))
                cores_i.append(int(core))
                band_f.append(float(flops))

            color1 = colors[init]
            color2 = colors[init+1]

            block_labels.append(block)
            axs.plot(cores_i, np.array(band_f), label=block,marker='+', linestyle='--')

            axs.set_xticks(cores_i)
            #axs.set_xlim([-0.1,4.1])
            axs.set_xticklabels(cores_s)


        axs.set_ylabel('GFLOPS')
        axs.set_xlabel('Cores')

        font = {'family' : 'monospace',
                'color'  : 'black',
                'weight' : 'normal',
                'size'   : 12,
                }

        # these are matplotlib.patch.Patch properties
        props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)
        chartTitle = latt

        #axs.text(0.85, 0.95, chartTitle, transform=axs.transAxes, fontsize=14,verticalalignment='top', bbox=props, fontdict=font)
        Title = "QPhiX - Lattice Size:"+chartTitle+" - COSMIC"
        plt.text(0.5, 1.08, Title, horizontalalignment='center', family='monospace',fontsize=20,  transform = axs.transAxes)
        axs.legend(block_labels, loc='best', bbox_to_anchor=(1, 0.5))

        #axs.legend(block_labels, loc='upper left')
        # axs.set_ylim([0,12])
        # axs.set_yscale("log", nonposx='clip')
        savepath = "/home/sid/Work/cosmic_charts/"+chartTitle+".png"
        plt.savefig(savepath,bbox_inches='tight')
        #plt.show()



def processFile(file_contents):
    flag=0
    minctCheck_flag = 0
    command = ""
    flop = ""
    debug = 0
    for i in range(line_count):
        line = read_file[i]
        line.rstrip()
        line_check = re.search(grep_pattern_command, line)
        if(line_check):
            #print "got command"+line
            minctCheck = grep_pattern_minct.search(line)
            if minctCheck is None:
                minctCheck_flag=0
            else:
                minctCheck_flag=1

            if(minctCheck_flag is 1):
                flag = 1
                command = line
                oneCoreCheck = grep_pattern_1core.search(line)
                next
        flop_check = re.search(grep_pattern_gflops,line)

        if(flop_check and flag == 1 ):

            flop = line
            flag = 0
            level1[command]=flop;

            next



    for k in level1.keys():
        k.rstrip('\n')
        #print k
        match_lattice = grep_pattern_lattice.findall(k)
        lattice_dim_string = ""
        block_string = ""
        core = ""
        GFLOPS = ""
        if(match_lattice):
            blocks={}
            strlattice_size = match_lattice[0]
            lattice_dim = strlattice_size.split()
            lattice_dim_string = lattice_dim[1]+"."+lattice_dim[3]+"."+lattice_dim[5]+"."+lattice_dim[7]
            if lattice_dim_string in lattice:
                blocks=lattice[lattice_dim_string]
            else:
                blocks={}
                lattice[lattice_dim_string]=blocks

            match_block = grep_pattern_block.findall(k)
            if(match_block):
                strBlock = match_block[0]
                arrblocks = strBlock.split()
                block_string = arrblocks[1]+"."+arrblocks[3]

            match_core = grep_pattern_core.findall(k)
            if (match_core):
                strCore = match_core[0]
                coreArray = strCore.split()
                core = coreArray[1]


            strGFLOPS = level1[k]
            arrGFLOPS = strGFLOPS.split()

            fGflops= float(arrGFLOPS[1])

            if block_string in blocks:
                cores = blocks[block_string]
                cores[core] = fGflops

            else:
                cores={}
                cores[core]=fGflops
                blocks[block_string]=cores

            # print "Lattice:"+lattice_dim_string+"\n"
            # print "Block Size:"+block_string+"\n"
            # print "Core:"+core+"\n"
            # print "GFlops:%f"% fGflops





for IOR_file_i in sys.argv[1:]:
    IOR_file = open(str(IOR_file_i))
    read_file = IOR_file.readlines()
    line_count=len(read_file)
    processFile(read_file)
    plotFile()
