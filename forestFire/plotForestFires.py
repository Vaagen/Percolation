import matplotlib.pyplot as plt
import glob
import numpy as np

def pause(message):
    print(message)
    programPause = raw_input("Press <ENTER> to continue...")


data=np.zeros((99,2,1))
p=np.linspace(0.01,0.99,99)

fig = plt.figure()
timeFig = plt.subplot(1,2,1)
deathFig = plt.subplot(1,2,2)

firstFile=True
for fileName in glob.glob('outputFiles/*'):
    newdata = np.zeros((99,2,1))
    numberOfTimesPincluded=np.zeros((99,2,1))
    with open(fileName) as f:
        next(f)
        for line in f:
            lineNums = [float(n) for n in line.strip().split()]
            pIndex = int(np.round(100*lineNums[0]-1))
            newdata[pIndex,0]+=lineNums[1]
            newdata[pIndex,1]+=lineNums[2]
            numberOfTimesPincluded[pIndex,:,0] += 1
        newdata = newdata[:,:,0]/numberOfTimesPincluded[:,:,0]
        newdata = newdata.reshape((99,2,1))
        if firstFile:
            data[:,:,0]=newdata[:,:,0]
            firstFile=False
        else:
            data = np.concatenate((data,newdata), axis=2)
    timeFig.plot(p,newdata[:,0,0])
    deathFig.plot(p,newdata[:,1,0])

plt.show(block=False)
pause('Showing plot.')
