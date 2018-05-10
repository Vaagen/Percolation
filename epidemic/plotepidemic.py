import matplotlib
matplotlib.use('TKAgg')


import numpy as np
import matplotlib.pyplot as plt
import os, sys

import re
import imp

font = {'family' : 'DejaVu Sans',
        'weight' : 'bold',
        'size'   : 22}

matplotlib.rc('font', **font)

def pause(message=''):
    print(message)
    programPause = input("Press <ENTER> to continue...")

def tryint(s):
    '''
        To sort filenames,
        taken from https://stackoverflow.com/questions/4623446/
                how-do-you-sort-files-numerically?utm_medium=organic&utm_
                source=google_rich_qa&utm_campaign=google_rich_qa
    '''
    try:
        return int(s)
    except:
        return s

def alphanum_key(s):
    '''
        To sort filenames,
        taken from https://stackoverflow.com/questions/4623446/
                how-do-you-sort-files-numerically?utm_medium=organic&utm_
                source=google_rich_qa&utm_campaign=google_rich_qa
        Turn a string into a list of string and number chunks.
        "z23a" -> ["z", 23, "a"]
    '''
    return [ tryint(c) for c in re.split('([0-9]+)', s) ]

def getParameters(filename):
    line = re.split('\(|\)',filenames[i])
    return (float(line[1]),float(line[3]),float(line[5]),float(line[7]),float(line[9]))

def getVectors(filename):
    data = np.loadtxt(filename,dtype='float')
    return data

if __name__ == '__main__':

    path='output/'
    # Get filenames
    filenames = os.listdir(path)
    filenames = sorted(filenames, key=alphanum_key)

    fig1 = plt.figure(1)
    ax1 = fig1.subplots(1,1)

    for i in range(0,len(filenames)):
        (p, q, lam, init, N)=getParameters(filenames[i])
        # Choose what to plot here
        if (q==0):
            data = getVectors(path + filenames[i])
            label = r'$p=$' + str(p) + r'$, q=$' + str(q) + r'$, \lambda=$' + str(lam) #+ r'$, L=$' + str(N)
            ax1.semilogy(data[:,0],data[:,1],label=label)

    # Shrink current axis by 20%
    box = ax1.get_position()
    ax1.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    # Put a legend to the right of the current axis
    ax1.legend(loc='center left', bbox_to_anchor=(1, 0.5))
    ax1.set_xlabel('Timesteps')
    ax1.set_ylabel('Fraction')

    # plt.show()
    plt.show(block=False)
    pause('Showing plot.')
