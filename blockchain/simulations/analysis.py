import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.ticker import FuncFormatter

def fixed_format(x, pos):
    return f'{x:.4f}'

###############################
##### SCENARIO ONE ############
###############################

###### Computational Cost #####
comp = pd.read_csv("okadai/oneTAS/results/ComputationCost.csv", header=None, names=["time", "module"])
block_comp = comp[comp["module"] == "Block"]
veh_comp = comp[comp["module"] == "RA"]
TGS_comp = comp[comp["module"] == "TGS"]

# Block chain
fig, ax = plt.subplots()
ax.plot(block_comp["time"] / 1000, label="Blockchain")
ax.set_xlabel("Operations Count")
ax.set_ylabel("Computational Cost (s)")
ax.set_title("Computational Cost Blockchain: Scenario 1")
ax.legend()
ax.grid()
plt.show()

# Vehicle
fig, ax = plt.subplots()
ax.plot(veh_comp["time"], label="Vehicle")
ax.set_xlabel("Operations Count")
ax.set_ylabel("Computational Cost (ms)")
ax.set_title("Computational Cost Vehicle: Scenario 1")
ax.legend()
ax.grid()
plt.show()

# TAS
fig, ax = plt.subplots()
ax.plot(TGS_comp["time"], label="TAS")
ax.set_xlabel("Operations Count")
ax.set_ylabel("Computational Cost (ms)")
ax.set_title("Computational Cost TAS: Scenario 1")
ax.legend()
ax.grid()
plt.show()


##### Authentication delay #####
delay = pd.read_csv("okadai/oneTAS/results/delay.csv", header=None, names=["delay", "hdelay"])

fig, ax = plt.subplots()
ax.plot(delay["delay"] * 1000)
ax.yaxis.set_major_formatter(FuncFormatter(fixed_format))
ax.set_xlabel('Vehicles')
ax.set_ylabel('Delay (ms)')
ax.set_title('Authentication Delay Scenario 1')
ax.grid(True) 
plt.show()


##### Handover delay #####
fig, ax = plt.subplots()
ax.plot(delay["hdelay"] * 1000)
ax.yaxis.set_major_formatter(FuncFormatter(fixed_format))
ax.set_xlabel('Vehicles')
ax.set_ylabel('Delay (ms)')
ax.set_title('Handover Delay Scenario 1')
ax.grid(True) 
plt.show()

##### end to end delay  #####
end = pd.read_csv("okadai/oneTAS/results/end2end.csv", header=None, names=["delay", "time"])

fig, ax = plt.subplots()
ax.plot(end["time"], end["delay"] * 1000)
ax.set_xlabel('Time (s)')
ax.set_ylabel("Delay (ms)")
ax.set_title("End to End Delay Scenario 1")
ax.grid(True)
plt.show()


##### throughput #####
tput = pd.read_csv("okadai/oneTAS/results/throughput.csv", header=None, names=["tput", "time", "module"])
veh_tput = tput[tput["module"] == "Vehicle"]
as_tput = tput[tput["module"] == "AS"]
tgs_tput = tput[tput["module"] == "TGS"]

fig, ax = plt.subplots()
ax.plot(veh_tput["time"], veh_tput["tput"], label="Vehicle")
ax.plot(as_tput["time"], as_tput["tput"], label="AS")
ax.plot(tgs_tput["time"], tgs_tput["tput"], label="TGS")
ax.set_xlabel("Time (s)")
ax.set_ylabel("Throughput (bps)")
ax.set_title("Throughput Scenario 1")
ax.grid(True)
ax.legend()
plt.show()


##### packet count #####
pckt = pd.read_csv("okadai/oneTAS/results/packetCount.csv", header=None, names=["sent", "rcvd", "module"])
veh_pkt = pckt[pckt["module"] == "Vehicle"]
as_pkt = pckt[pckt["module"] == "AS"]
tgs_pkt = pckt[pckt["module"] == "TGS"]

modules = ["Vehicle", "AS", "TGS"]
sent = [veh_pkt["sent"].iloc[-1], as_pkt["sent"].iloc[-1], tgs_pkt["sent"].iloc[-1]]
rcvd = [veh_pkt["rcvd"].iloc[-1], as_pkt["rcvd"].iloc[-1], tgs_pkt["rcvd"].iloc[-1]]

bar_width = 0.35
index = np.arange(len(modules))

fig, ax = plt.subplots()
rects1 = ax.bar(index - bar_width/2, sent, bar_width, label='Sent')
rects2 = ax.bar(index + bar_width/2, rcvd, bar_width, label='Received')

ax.set_xlabel('Module')
ax.set_ylabel('Packet count')
ax.set_title('Packet Rates Scenario 1')
ax.set_xticks(index)
ax.set_xticklabels(modules)
ax.legend()

plt.show()


###############################
##### SCENARIO TWO ############
###############################

###### Computational Cost #####
comp = pd.read_csv("okayama/twoTAS/results/ComputationCost.csv", header=None, names=["time", "module"])
block_comp = comp[comp["module"] == "Block"]
veh_comp = comp[comp["module"] == "RA"]
TGS_comp = comp[comp["module"] == "TGS"]

# Block chain
fig, ax = plt.subplots()
ax.plot(block_comp["time"] / 1000, label="Blockchain")
ax.set_xlabel("Operations Count")
ax.set_ylabel("Computational Cost (s)")
ax.set_title("Computational Cost Blockchain: Scenario 2")
ax.legend()
ax.grid()
plt.show()

# Vehicle
fig, ax = plt.subplots()
ax.plot(veh_comp["time"], label="Vehicle")
ax.set_xlabel("Operations Count")
ax.set_ylabel("Computational Cost (ms)")
ax.set_title("Computational Cost Vehicle: Scenario 2")
ax.legend()
ax.grid()
plt.show()

# TAS
fig, ax = plt.subplots()
ax.plot(TGS_comp["time"], label="TAS")
ax.set_xlabel("Operations Count")
ax.set_ylabel("Computational Cost (ms)")
ax.set_title("Computational Cost TAS: Scenario 2")
ax.legend()
ax.grid()
plt.show()


##### Authentication delay #####
delay = pd.read_csv("okayama/twoTAS/results/delay.csv", header=None, names=["delay", "hdelay"])

fig, ax = plt.subplots()
ax.plot(delay["delay"] * 1000)
ax.yaxis.set_major_formatter(FuncFormatter(fixed_format))
ax.set_xlabel('Vehicles')
ax.set_ylabel('Delay (ms)')
ax.set_title('Authentication Delay Scenario 2')
ax.grid(True) 
plt.show()


##### Handover delay #####
fig, ax = plt.subplots()
ax.plot(delay["hdelay"] * 1000)
ax.yaxis.set_major_formatter(FuncFormatter(fixed_format))
ax.set_xlabel('Vehicles')
ax.set_ylabel('Delay (ms)')
ax.set_title('Handover Delay Scenario 2')
ax.grid(True) 
plt.show()


##### end to end delay  #####
end = pd.read_csv("okayama/twoTAS/results/end2end.csv", header=None, names=["delay", "time"])

fig, ax = plt.subplots()
ax.plot(end["time"], end["delay"] * 1000)
ax.set_xlabel('Time (s)')
ax.set_ylabel("Delay (ms)")
ax.set_title("End to End Delay Scenario 2")
ax.grid(True)
plt.show()


##### throughput #####
tput = pd.read_csv("okayama/twoTAS/results/throughput.csv", header=None, names=["tput", "time", "module"])
veh_tput = tput[tput["module"] == "Vehicle"]
as_tput = tput[tput["module"] == "AS"]
tgs_tput = tput[tput["module"] == "TGS"]

fig, ax = plt.subplots()
ax.plot(veh_tput["time"], veh_tput["tput"], label="Vehicle")
ax.plot(as_tput["time"], as_tput["tput"], label="AS")
ax.plot(tgs_tput["time"], tgs_tput["tput"], label="TGS")
ax.set_xlabel("Time (s)")
ax.set_ylabel("Throughput (bps)")
ax.set_title("Throughput Scenario 2")
ax.grid(True)
ax.legend()
plt.show()


##### packet count #####
pckt = pd.read_csv("okayama/oneTAS/results/packetCount.csv", header=None, names=["sent", "rcvd", "module"])
veh_pkt = pckt[pckt["module"] == "Vehicle"]
as_pkt = pckt[pckt["module"] == "AS"]
tgs_pkt = pckt[pckt["module"] == "TGS"]

modules = ["Vehicle", "AS", "TGS"]
sent = [veh_pkt["sent"].iloc[-1], as_pkt["sent"].iloc[-1], tgs_pkt["sent"].iloc[-1]]
rcvd = [veh_pkt["rcvd"].iloc[-1], as_pkt["rcvd"].iloc[-1], tgs_pkt["rcvd"].iloc[-1]]

bar_width = 0.35
index = np.arange(len(modules))

fig, ax = plt.subplots()
rects1 = ax.bar(index - bar_width/2, sent, bar_width, label='Sent')
rects2 = ax.bar(index + bar_width/2, rcvd, bar_width, label='Received')

ax.set_xlabel('Module')
ax.set_ylabel('Packet count')
ax.set_title('Packet Rates Scenario 1')
ax.set_xticks(index)
ax.set_xticklabels(modules)
ax.legend()

plt.show()
