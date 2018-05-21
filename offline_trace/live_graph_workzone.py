# -*- coding: utf-8 -*-
"""
Created on Thu Jul 20 16:29:03 2017

@author: iljoo
"""

import matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import random

style.use('fivethirtyeight')

matplotlib.rc('font', size=10)
matplotlib.rc('axes', titlesize=10)

DEADLINE = 50
X_MIN = 0
Y_MAX = 60

fig = plt.figure(figsize=(5,2))

ax4 = fig.add_subplot(3,3,4) #lane_change
ax7 = fig.add_subplot(3,3,7) #workzone
ax5 = fig.add_subplot(3,3,5) #high
ax1 = fig.add_subplot(3,3,1) #dnn
ax6 = fig.add_subplot(3,3,6) #high_partition
ax8 = fig.add_subplot(3,3,8) #low
ax9 = fig.add_subplot(3,3,9) #low_partition

#start_time = 6479

def animate(i):
    try:
        lane_change_graph = open('lanechange_trace','r').read()
        workzone_graph = open('workzone_trace','r').read()
        high_graph = open('high_trace','r').read()
        low_graph = open('low_trace','r').read()
        dnn_graph = open('dnn_trace','r').read()
        high_partition_graph = open('high_partition_trace','r').read()
        low_partition_graph = open('low_partition_trace','r').read()

	start_time = int(lane_change_graph.split('\n')[0].split(' ')[0])

    except IOError:
        print ''
            
    else:        
        lines = lane_change_graph.split('\n')
        xs = []
        ys = []
	end_time = start_time + i
	current_time = start_time
	current_max = 0
        for line in lines:
            if len(line) > 1:
                x, y = line.split(' ')
		x = int(x)
		y = float(y)
		if x > end_time:
			break
		if x > current_time:
			if current_max > 0:
				xs.append(current_time)
				ys.append(current_max)
			current_time = x
			current_max = y
		else:
			current_max = max(current_max, y)

        ax4.clear()
        ax4.set_xlabel('time(s)')
        ax4.set_ylabel('response time (ms)')
        ax4.set_ylim([0, 80]) 
	ax4.set_xlim([start_time, end_time])
	ax4.set_title("lane_change", fontsize=15)
        ax4.axhline(y=33, color='r', linestyle='-')
        ax4.plot(xs, ys)

        lines = workzone_graph.split('\n')
        xs = []
        ys = []
	end_time = start_time + i
	current_time = start_time
	current_max = 0
        for line in lines:
            if len(line) > 1:
                x, y = line.split(' ')
		x = int(x)
		y = float(y)
		if x > end_time:
			break
		if x > current_time:
			if current_max > 0:
				xs.append(current_time)
				ys.append(current_max)
			current_time = x
			current_max = y
		else:
			current_max = max(current_max, y)

        ax7.clear()
        ax7.set_xlabel('time(s)')
        ax7.set_ylabel('response time (ms)')
        ax7.set_ylim([0, 80]) 
	ax7.set_xlim([start_time, end_time])
	ax7.set_title("workzone", fontsize=15)
        ax7.axhline(y=DEADLINE, color='r', linestyle='-')
        ax7.plot(xs, ys)


        lines = high_graph.split('\n')
        xs = []
        ys = []
	end_time = start_time + i
	current_time = start_time
	current_max = 0
        for line in lines:
            if len(line) > 1:
                x, y = line.split(' ')
		x = int(x)
		y = float(y)
		if x > end_time:
			break
		if x > current_time:
			if current_max > 0:
				xs.append(current_time)
				ys.append(current_max)
			current_time = x
			current_max = y
		else:
			current_max = max(current_max, y)

        ax5.clear()
        ax5.set_xlabel('time(s)')
        ax5.set_ylabel('response time (ms)')
        #ax5.set_ylim([0, 3000]) 
	ax5.set_xlim([start_time, end_time])
	ax5.set_title("cuda_kernel", fontsize=15)
        ax5.axhline(y=600, color='r', linestyle='-')
        ax5.plot(xs, ys)

        lines = low_graph.split('\n')
        xs = []
        ys = []
	end_time = start_time + i
	current_time = start_time
	current_max = 0
        for line in lines:
            if len(line) > 1:
                x, y = line.split(' ')
		x = int(x)
		y = float(y)
		if x > end_time:
			break
		if x > current_time:
			if current_max > 0:
				xs.append(current_time)
				ys.append(current_max)
			current_time = x
			current_max = y
		else:
			current_max = max(current_max, y)

        ax8.clear()
        ax8.set_xlabel('time(s)')
        ax8.set_ylabel('response time (ms)')
        #ax8.set_ylim([0, 3000]) 
	ax8.set_xlim([start_time, end_time])
	ax8.set_title("cuda_kernel", fontsize=15)
        ax8.axhline(y=600, color='r', linestyle='-')
        ax8.plot(xs, ys)

        lines = dnn_graph.split('\n')
        xs = []
        ys = []
	end_time = start_time + i
	current_time = start_time
	current_max = 0
        for line in lines:
            if len(line) > 1:
                x, y = line.split(' ')
		x = int(x)
		y = float(y)
		if x > end_time:
			break
		if x > current_time:
			if current_max > 0:
				xs.append(current_time)
				ys.append(current_max)
			current_time = x
			current_max = y
		else:
			current_max = max(current_max, y)

        ax1.clear()
        ax1.set_xlabel('time(s)')
        ax1.set_ylabel('response time (ms)')
        ax1.set_ylim([0, 200]) 
	ax1.set_xlim([start_time, end_time])
	ax1.set_title("dnn", fontsize=15)
        ax1.axhline(y=100, color='r', linestyle='-')
        ax1.plot(xs, ys)
    
        lines = high_partition_graph.split('\n')
        xs = []
        ys = []
	end_time = start_time + i
	current_time = start_time
	current_max = 0
        for line in lines:
            if len(line) > 1:
                x, y = line.split(' ')
		x = int(x)
		y = float(y)
		if x > end_time:
			break
		if x > current_time:
			if current_max > 0:
				xs.append(current_time)
				ys.append(current_max)
			current_time = x
			current_max = y
		else:
			current_max = max(current_max, y)

        ax6.clear()
        ax6.set_xlabel('time(s)')
        ax6.set_ylabel('response time (ms)')
        ax6.set_ylim([0, 110]) 
	ax6.set_xlim([start_time, end_time])
	ax6.set_title("partition", fontsize=15)
        #ax6.axhline(y=DEADLINE, color='r', linestyle='-')
        ax6.plot(xs, ys)

        lines = low_partition_graph.split('\n')
        xs = []
        ys = []
	end_time = start_time + i
	current_time = start_time
	current_max = 0
        for line in lines:
            if len(line) > 1:
                x, y = line.split(' ')
		x = int(x)
		y = float(y)
		if x > end_time:
			break
		if x > current_time:
			if current_max > 0:
				xs.append(current_time)
				ys.append(current_max)
			current_time = x
			current_max = y
		else:
			current_max = max(current_max, y)

        ax9.clear()
        ax9.set_xlabel('time(s)')
        ax9.set_ylabel('response time (ms)')
        ax9.set_ylim([0, 110]) 
	ax9.set_xlim([start_time, end_time])
	ax9.set_title("partition", fontsize=15)
        #ax9.axhline(y=DEADLINE, color='r', linestyle='-')
        ax9.plot(xs, ys)
ani = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()
