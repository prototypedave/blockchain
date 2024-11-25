import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.ticker import FuncFormatter

def fixed_format(x, pos):
    return f'{x:.4f}'

"""
    Plots
    @method single_plot :- plots individual plots of each attributes
    @method plot_tput :- plots throughput line charts
    @method plot_bar_graph:- plots a bar graph for packet counts
"""
def single_plot(df, label, xlabel, ylabel, title, attr, attr2="", format=False, cost=True, dir=""):
    fig, ax = plt.subplots()
    if not cost:
        if attr2 != "":
            ax.plot(df[attr2], df[attr] * 1000, label=label)
        else:
            ax.plot(df[attr] * 1000, label=label)
    else:
        if attr2 != "":
            ax.plot(df[attr2], df[attr] / 1000, label=label)
        else:
            ax.plot(df[attr] / 1000, label=label)
    if format:
        ax.yaxis.set_major_formatter(FuncFormatter(fixed_format))
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    ax.legend()
    ax.grid()
    plt.savefig(dir+"/"+title+".png")
    plt.close()


def plot_tput(df, label, title, dir):
    fig, ax = plt.subplots()
    ax.plot(df["time"], df["tput"], label=label)
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Throughput (bps)")
    ax.set_title(title)
    ax.grid(True)
    ax.legend()
    plt.savefig(dir+"/"+title+".png")
    plt.close()


def plot_bar_graph(vehicle, tas, rsu, title, dir):
    modules = ["Vehicle", "TAS", "RSU"]
    sent = [vehicle["sent"].iloc[-1], tas["sent"].iloc[-1], rsu["sent"].iloc[-1]]
    rcvd = [vehicle["rcvd"].iloc[-1], tas["rcvd"].iloc[-1], rsu["rcvd"].iloc[-1]]
    bar_width = 0.35
    index = np.arange(len(modules))

    fig, ax = plt.subplots()
    rects1 = ax.bar(index - bar_width/2, sent, bar_width, label='Sent')
    rects2 = ax.bar(index + bar_width/2, rcvd, bar_width, label='Received')

    ax.set_xlabel('Module')
    ax.set_ylabel('Packet count')
    ax.set_title(title)
    ax.set_xticks(index)
    ax.set_xticklabels(modules)
    ax.legend()
    plt.savefig(dir+"/"+title+".png")
    plt.close()


"""
    Read csv files
    @file ComputationCost:- Results for delay in generating authentication messages
    @file Delay:- Results for authentication delay
    @file ende2end:- Results for end to end delay
    @file packetCount:- packets transmitted in the simulation
"""
OkadaiCompDelay = pd.read_csv("okadai_one_TAS/results/ComputationCost.csv", header=None, names=["time", "module"])
OkayamaOneCompDelay = pd.read_csv("okayama_one_TAS/results/ComputationCost.csv", header=None, names=["time", "module"])
OkayamaTwoCompDelay = pd.read_csv("okayama_two_TAS/results/ComputationCost.csv", header=None, names=["time", "module"])

OkadaiDelay = pd.read_csv("okadai_one_TAS/results/delay.csv", header=None, names=["delay", "hdelay"])
OkayamaOneDelay = pd.read_csv("okayama_one_TAS/results/delay.csv", header=None, names=["delay", "hdelay"])
OkayamaTwoDelay = pd.read_csv("okayama_two_TAS/results/delay.csv", header=None, names=["delay", "hdelay"])

OkadaiEnd2End = pd.read_csv("okadai_one_TAS/results/end2end.csv", header=None, names=["delay", "time"])
OkayamaOneEnd2End = pd.read_csv("okayama_one_TAS/results/end2end.csv", header=None, names=["delay", "time"])
OkayamaTwoEnd2End = pd.read_csv("okayama_two_TAS/results/end2end.csv", header=None, names=["delay", "time"])

OkadaiPckt = pd.read_csv("okadai_one_TAS/results/packetCount.csv", header=None, names=["sent", "rcvd", "module"])
OkayamaOnePckt = pd.read_csv("okayama_one_TAS/results/packetCount.csv", header=None, names=["sent", "rcvd", "module"])
OkayamaTwoPckt = pd.read_csv("okayama_two_TAS/results/packetCount.csv", header=None, names=["sent", "rcvd", "module"])

OkadaiTput = pd.read_csv("okadai_one_TAS/results/throughput.csv", header=None, names=["tput", "time", "module"])
OkayamaOneTput = pd.read_csv("okayama_one_TAS/results/throughput.csv", header=None, names=["tput", "time", "module"])
OkayamaTwoTput = pd.read_csv("okayama_two_TAS/results/throughput.csv", header=None, names=["tput", "time", "module"])


"""
    Split dataset to given modules
    @df Block:-     computation cost for block chain
    @df Vehicle:-   vehicle computation cost
    @df RA:-        registration cost
    @df TAS:-       server computation cost
    @df RSU:-       RSU computation cost
"""
OkadaiCompDelay_Block = OkadaiCompDelay[OkadaiCompDelay["module"] == "Block"]
OkadaiCompDelay_Vehicle = OkadaiCompDelay[OkadaiCompDelay["module"] == "vehicle"]
OkadaiCompDelay_RA = OkadaiCompDelay[OkadaiCompDelay["module"] == "RA"]
OkadaiCompDelay_TAS = OkadaiCompDelay[OkadaiCompDelay["module"] == "TAS"]
OkadaiCompDelay_RSU = OkadaiCompDelay[OkadaiCompDelay["module"] == "RSU"]

OkayamaOneCompDelay_Block = OkayamaOneCompDelay[OkayamaOneCompDelay["module"] == "Block"]
OkayamaOneCompDelay_Vehicle = OkayamaOneCompDelay[OkayamaOneCompDelay["module"] == "vehicle"]
OkayamaOneCompDelay_RA = OkayamaOneCompDelay[OkayamaOneCompDelay["module"] == "RA"]
OkayamaOneCompDelay_TAS = OkayamaOneCompDelay[OkayamaOneCompDelay["module"] == "TAS"]
OkayamaOneCompDelay_RSU = OkayamaOneCompDelay[OkayamaOneCompDelay["module"] == "RSU"]

OkayamaTwoCompDelay_Block = OkayamaTwoCompDelay[OkayamaTwoCompDelay["module"] == "Block"]
OkayamaTwoCompDelay_Vehicle = OkayamaTwoCompDelay[OkayamaTwoCompDelay["module"] == "vehicle"]
OkayamaTwoCompDelay_RA = OkayamaTwoCompDelay[OkayamaTwoCompDelay["module"] == "RA"]
OkayamaTwoCompDelay_TAS = OkayamaTwoCompDelay[OkayamaTwoCompDelay["module"] == "TAS"]
OkayamaTwoCompDelay_RSU = OkayamaTwoCompDelay[OkayamaTwoCompDelay["module"] == "RSU"]

OkadaiPckt_RSU =OkadaiPckt[OkadaiPckt["module"] == "RSU"]
OkadaiPckt_Vehicle =OkadaiPckt[OkadaiPckt["module"] == "Vehicle"]
OkadaiPckt_TAS =OkadaiPckt[OkadaiPckt["module"] == "TAS"]

OkayamaOnePckt_RSU =OkayamaOnePckt[OkayamaOnePckt["module"] == "RSU"]
OkayamaOnePckt_Vehicle =OkayamaOnePckt[OkayamaOnePckt["module"] == "Vehicle"]
OkayamaOnePckt_TAS =OkayamaOnePckt[OkayamaOnePckt["module"] == "TAS"]

OkayamaTwoPckt_RSU =OkayamaTwoPckt[OkayamaTwoPckt["module"] == "RSU"]
OkayamaTwoPckt_Vehicle =OkayamaTwoPckt[OkayamaTwoPckt["module"] == "Vehicle"]
OkayamaTwoPckt_TAS =OkayamaTwoPckt[OkayamaTwoPckt["module"] == "TAS"]

OkadaiTput_RSU =OkadaiTput[OkadaiTput["module"] == "RSU"]
OkadaiTput_Vehicle =OkadaiTput[OkadaiTput["module"] == "Vehicle"]
OkadaiTput_TAS =OkadaiTput[OkadaiTput["module"] == "TAS"]

OkayamaOneTput_RSU =OkayamaOneTput[OkayamaOneTput["module"] == "RSU"]
OkayamaOneTput_Vehicle =OkayamaOneTput[OkayamaOneTput["module"] == "Vehicle"]
OkayamaOneTput_TAS =OkayamaOneTput[OkayamaOneTput["module"] == "TAS"]

OkayamaTwoTput_RSU =OkayamaTwoTput[OkayamaTwoTput["module"] == "RSU"]
OkayamaTwoTput_Vehicle =OkayamaTwoTput[OkayamaTwoTput["module"] == "Vehicle"]
OkayamaTwoTput_TAS =OkayamaTwoTput[OkayamaTwoTput["module"] == "TAS"]


# computational cost plots
single_plot(OkadaiCompDelay_Block, label="Blockchain-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost Blockchain: Okadai", attr="time", dir="computational_cost")
single_plot(OkayamaOneCompDelay_Block, label="Blockchain-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost Blockchain: Okayama 1-TAS", attr="time", dir="computational_cost")
single_plot(OkayamaTwoCompDelay_Block, label="Blockchain-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost Blockchain: Okayama 2-TAS", attr="time", dir="computational_cost")

single_plot(OkadaiCompDelay_Vehicle, label="Vehicle-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost Vehicle: Okadai", attr="time", dir="computational_cost")
single_plot(OkayamaOneCompDelay_Vehicle, label="Vehicle-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost Vehicle: Okayama 1-TAS", attr="time", dir="computational_cost")
single_plot(OkayamaTwoCompDelay_Vehicle, label="Vehicle-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost Vehicle: Okayama 2-TAS", attr="time", dir="computational_cost")

single_plot(OkadaiCompDelay_RA, label="Vehicle-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost Vehicle: Okadai", attr="time", dir="computational_cost")
single_plot(OkayamaOneCompDelay_RA, label="Vehicle-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost Vehicle: Okayama 1-TAS", attr="time", dir="computational_cost")
single_plot(OkayamaTwoCompDelay_RA, label="Vehicle-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost Vehicle: Okayama 2-TAS", attr="time", dir="computational_cost")

single_plot(OkadaiCompDelay_TAS, label="TAS-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost TAS: Okadai", attr="time", dir="computational_cost")
single_plot(OkayamaOneCompDelay_TAS, label="TAS-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost TAS: Okayama 1-TAS", attr="time", dir="computational_cost")
single_plot(OkayamaTwoCompDelay_TAS, label="TAS-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost TAS: Okayama 2-TAS", attr="time", dir="computational_cost")

single_plot(OkadaiCompDelay_RSU, label="RSU-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost RSU: Okadai", attr="time", dir="computational_cost")
single_plot(OkayamaOneCompDelay_RSU, label="RSU-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost RSU: Okayama 1-TAS", attr="time", dir="computational_cost")
single_plot(OkayamaTwoCompDelay_RSU, label="RSU-computational-cost", xlabel="Operations Count", ylabel="Computational Cost (s)", title="Computational Cost RSU: Okayama 2-TAS", attr="time", dir="computational_cost")

# Authentication delay
single_plot(OkadaiDelay, label="Authentication-delay", xlabel="Vehicles", ylabel='Delay (ms)', title='Authentication Delay Okadai', attr='delay', cost=False, dir="authentication_delay")
single_plot(OkadaiDelay, label="Handover-delay", xlabel="Vehicles", ylabel='Delay (ms)', title='Handover Delay Okadai', attr='hdelay', format=True, cost=False, dir="authentication_delay")

single_plot(OkayamaOneDelay, label="Authentication-delay", xlabel="Vehicles", ylabel='Delay (ms)', title='Authentication Delay Okayama 1-TAS', attr='delay', cost=False, dir="authentication_delay")
single_plot(OkayamaOneDelay, label="Handover-delay", xlabel="Vehicles", ylabel='Delay (ms)', title='Handover Delay Okayama 1-TAS', attr='hdelay', format=True, cost=False, dir="authentication_delay")

single_plot(OkayamaTwoDelay, label="Authentication-delay", xlabel="Vehicles", ylabel='Delay (ms)', title='Authentication Delay Okayama 2-TAS', attr='delay', cost=False, dir="authentication_delay")
single_plot(OkayamaTwoDelay, label="Handover-delay", xlabel="Vehicles", ylabel='Delay (ms)', title='Handover Delay Okayama 2-TAS', attr='hdelay', format=True, cost=False, dir="authentication_delay")

# End-to-End Delay
single_plot(OkadaiEnd2End, label="End2End-delay", xlabel="Time (s)", ylabel="Delay (ms)", title="End to End Delay Okadai", attr2="time", attr="delay", cost=False, dir="end_to_end_delay")
single_plot(OkayamaOneEnd2End, label="End2End-delay", xlabel="Time (s)", ylabel="Delay (ms)", title="End to End Delay Okayama 1-TAS", attr2="time", attr="delay", cost=False, dir="end_to_end_delay")
single_plot(OkayamaTwoEnd2End, label="End2End-delay", xlabel="Time (s)", ylabel="Delay (ms)", title="End to End Delay Okayama 2-TAS", attr2="time", attr="delay", cost=False, dir="end_to_end_delay")

# Throughput
plot_tput(OkadaiTput_RSU, label="RSU-throughput", title="Throughput RSU: Okadai", dir="throughput")
plot_tput(OkayamaOneTput_RSU, label="RSU-throughput", title="Throughput RSU: Okayama 1-TAS", dir="throughput")
plot_tput(OkayamaTwoTput_RSU, label="RSU-throughput", title="Throughput RSU: Okayama 2-TAS", dir="throughput")

plot_tput(OkadaiTput_Vehicle, label="Vehicle-throughput", title="Throughput Vehicle: Okadai", dir="throughput")
plot_tput(OkayamaOneTput_Vehicle, label="Vehicle-throughput", title="Throughput Vehicle: Okayama 1-TAS", dir="throughput")
plot_tput(OkayamaTwoTput_Vehicle, label="Vehicle-throughput", title="Throughput Vehicle: Okayama 2-TAS", dir="throughput")

plot_tput(OkadaiTput_TAS, label="TAS-throughput", title="Throughput TAS: Okadai", dir="throughput")
plot_tput(OkayamaOneTput_TAS, label="TAS-throughput", title="Throughput TAS: Okayama 1-TAS", dir="throughput")
plot_tput(OkayamaTwoTput_TAS, label="TAS-throughput", title="Throughput TAS: Okayama 2-TAS", dir="throughput")

# Packet count
plot_bar_graph(OkadaiPckt_Vehicle, OkadaiPckt_TAS, OkadaiPckt_RSU, "Packet Count Okadai", dir="packet_count")
plot_bar_graph(OkayamaOnePckt_Vehicle, OkayamaOnePckt_TAS, OkayamaOnePckt_RSU, "Packet Count Okayama 1-TAS", dir="packet_count")
plot_bar_graph(OkayamaTwoPckt_Vehicle, OkayamaTwoPckt_TAS, OkayamaTwoPckt_RSU, "Packet Count Okayama 2-TAS", dir="packet_count")
