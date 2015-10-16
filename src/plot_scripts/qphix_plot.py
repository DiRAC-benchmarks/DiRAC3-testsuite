from __future__ import division
import matplotlib.pyplot as plt
import numpy as np
import sys
import re

#configuration that we are interested in
minct="2"
#############################################




grep_pattern_command = re.compile(r'time_dslash_noqdp -x')
grep_pattern_gflops = re.compile(r'GFLOPS total')
grep_pattern_lattice = re.compile('-x\s*\d+\s*-y\s*\d+\s*-z\s*\d+\s*-t\s*\d+')
grep_pattern_block = re.compile('-by\s*\d\s*-bz\s*\d')
grep_pattern_core = re.compile('-c\s*\d')
grep_pattern_minct = re.compile('-minct\s*'+minct)
level1 = {}

def processFile(file_contents):
    flag=0
    minctCheck_flag = 0
    command = ""
    flop = ""
    for i in range(line_count):
        line = read_file[i]
        line.rstrip()
        line_check = re.search(grep_pattern_command, line)
        if(line_check):
            minctCheck = grep_pattern_minct.search(line)
            if minctCheck is None:
                minctCheck_flag=0
            else:
                minctCheck_flag=1

            if(minctCheck_flag is 1):
                flag = 1
                command = line
                next
        flop_check = re.search(grep_pattern_gflops,line)
        if(flop_check and flag == 1 ):
            flop = line
            flag = 0
            level1[command]=flop;
            #print "command: %s \n Flops: %s\n" % (command,flop)
            next


    lattice={}
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

    lattlist = lattice.keys()
    lattlist.sort()

    for latt in lattlist:
        print latt
        blocks=lattice[latt]

        blocklist = blocks.keys()
        blocklist.sort()
        for block in blocklist:
            print "\t%s" % block
            cores = blocks[block]
            for core, flops in sorted(cores.iteritems(), key=lambda (k,v): (v,k)):
                print('\t\t{} : {}'.format(core, flops))


for IOR_file_i in sys.argv[1:]:
    IOR_file = open(str(IOR_file_i))
    read_file = IOR_file.readlines()
    line_count=len(read_file)
    processFile(read_file)
