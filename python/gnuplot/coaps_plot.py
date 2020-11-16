#!/usr/bin/python

from console_loggin import *
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import time
import os

#0 view h248 statis current coaps
#1 Current Coaps Statistics Value:
#2 (Elapsed time: 0:11:52)
#3 ===================================================
#4                                                         Max     Min     Avg
#5   Context Attempts per Second                    :       59       0      45
#6     Accept Context Attempts per Second           :       59       0      45
#7       Successful Context Completion per Second   :        0       0       0
#8       Unsuccessful Context Completion per Second :        0       0       0
#9     Reject Context Attempts per Second           :        0       0       0
#10   ADD Commands per Second                        :      117       0      90
#11   H.248 Messages per Second                      :       87       0      67
#12   H.248 Commands per Second                      :      146       0     112
#13   CmSock layer messages per Second               :       87       0      67
#14   SCoC: H.248 Messages per Context               :        0       0       0
#15   SCoC: H.248 Commands per Context               :        0       0       0
#16   H248.11 Events per Second                      :        0       0       0
#17   Dropped Messages per Second                    :        0       0       0
#18   --------------------------------------------------------------------------
#19 sbc01media:ACT-SCM:1.11(r0)>=9:diag:main:vMG1#

def parse(output):
    lines = output.splitlines()
    if len(lines) != 20:
        print("ERRROR < 20")
        exit()

    aca_l = lines[6]
    rca_l = lines[9]
    add_l = lines[10]

    aca = int(aca_l.split()[-1])
    rca = int(rca_l.split()[-1])
    add = int(add_l.split()[-1])
    return (aca, rca, add) 

def main():
    num_of_seconds = 500

    acas = []
    rcas = []
    adds = []

    diag = Diag()
    try:
        output = diag.run_command("define vmg sc 1")
        print(output)
        output = diag.run_command("clear h248 statis current")
        print(output)
        output = diag.run_command("diag clear sysmon trace_values")
        print(output)
        output = diag.run_command("diag define sysmon trace_value h248_statis add 0 100")
        print(output)
        output = diag.run_command("diag define sysmon trace_value h248_statis aca 0 100")
        print(output)
        output = diag.run_command("diag define sysmon trace_value h248_statis rca 0 100")
        print(output)
        output = diag.run_command("diag define sysmon start h248_t h248_t 1")
        print(output)

        for i in range(num_of_seconds):
            print("Second: %d" % i)
            output = diag.run_command("view h248 statis current coaps")
            parsed = parse(output)
            acas.append(parsed[0])
            rcas.append(parsed[1])
            adds.append(parsed[2])
            print(parsed)
            time.sleep(1)

        output = diag.run_command("diag define sysmon end")
        print(output)

        x = range(num_of_seconds)
        fig = plt.figure()
        plt.plot(x, acas, label = "aca")
        plt.plot(x, rcas, label = "rca")
        plt.plot(x, adds, label = "add")
        plt.legend(loc='upper left')
        fig.savefig('plot.png')

       
    except Diag.CommandTimeoutException:
        print("timeout")   
    
    
if __name__ == '__main__': main()
