import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys
import re
from os import listdir
from functools import reduce


option = {
    'kind' : 'line',
    'style' : ['k-','r-'],

    'xlabel' : 'value_size(byte)',
    'ylabel' : 'throughput(MB/s)',
    'fontsize' : 13,
    'figsize' : (5,5),
    'grid' : 'true'
}

kblock = {
    'ticks' : [16, 32, 64,128, 256, 512, 1024, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 16384],
    'seq_4kb' : [3.8, 6.2, 21.7, 35, 61, 91, 137, 171.9, 198.7, 204, 220, 225, 227, 232, 229.1],
    'seq_8kb' : [9.7, 5.9, 21, 35.1, 59.1, 83.7, 130.7, 162.7, 180.2, 196, 214.6, 220.2, 234.1, 234.1, 250.2],
    'seq_16kb' : [9.8, 6, 20.7, 35, 60, 97.8, 144, 182, 194.3, 196.3, 230.5, 240, 249.8, 236, 263.9],
    'seq_32kb' : [9.3, 12.5, 20.3, 33.7, 58.7, 89.1, 127.6, 175.7, 196.9, 208.4, 216.1, 232.8, 223.3, 218, 230.4],
    'seq_64kb' : [8.6, 11.9, 19, 32, 53, 84, 124.2, 157.5, 182.3, 190.8, 218.4, 220.2, 236.2, 231.9, 234],
    'ran_4kb' : [5.3,  7.5, 12.6, 22, 35.4, 57, 73.4, 94, 96.3, 82.9, 90.6, 84.8, 87.9, 82.6, 58.2],
    'ran_8kb' : [5, 7.2, 12.3, 21, 33.7, 55, 72.4, 96.6, 98.6, 82.3, 87.9, 82.6, 90.8, 81.3, 57.3],
    'ran_16kb' : [5.2, 7.8, 13.1, 23.1, 38.1, 60, 74, 102.4, 92, 83, 89.7, 82.8, 93.8, 84.4, 58.5],
    'ran_32kb' : [4.9, 7.2, 11.9, 19.9, 34.7, 57.8, 71.8, 93.9, 95.4, 81.2, 86, 81.4, 85.5, 79.1, 49.6 ],
    'ran_64kb' : [5.1, 7.3, 12.2, 21.5, 34.1, 54.1, 71.9 ,98.8, 97.8, 79.1, 82.2, 77.3, 88.1, 80.5, 54.9],
}

mod = sys.modules[__name__]
rePayload = re.compile(r'[0-9]+')
payload = []

ispadding_list = listdir('./kblocksize')
for ispadding in ispadding_list:
    if ispadding.endswith('.csv'):
        setattr(mod, '__{}_'.format(ispadding.split('.')[0]), pd.read_csv('./kblocksize/{}'.format(ispadding)))

ispadding_dict = {
    "__ran_onwal_payload_onpadding_32k_" : __ran_onwal_payload_onpadding_32k_,
    "__ran_onwal_payload_onpadding_16k_" : __ran_onwal_payload_onpadding_16k_,
    "__ran_onwal_payload_onpadding_8k_" : __ran_onwal_payload_onpadding_8k_,
}

ran_on_32 = ispadding_dict["__ran_onwal_payload_onpadding_32k_"].set_index("key-value")
ran_on_16 = ispadding_dict["__ran_onwal_payload_onpadding_16k_"].set_index("key-value")
ran_on_8 = ispadding_dict["__ran_onwal_payload_onpadding_8k_"].set_index("key-value")

for i in ispadding_dict.keys():
    payload.clear()
    currList = ispadding_dict[i]
    for j in currList.loc[:,"key-value"]:
        result = reduce(lambda acc, cur : acc + cur, list(map(int, rePayload.findall(j))))
        payload.append(result)
    currList["payload_"] = payload
print(ispadding_dict["__ran_onwal_payload_onpadding_8k_"])

latency_padding_exist_kblocksize = {
    "key-value" : ispadding_dict["__ran_onwal_payload_onpadding_8k_"].values[:,0],
    "payload" : ispadding_dict["__ran_onwal_payload_onpadding_8k_"].values[:,4],
    "ran_on_latency_32" : ran_on_32.values[:,0]/ 10,
    "ran_on_latency_16" : ran_on_16.values[:,0]/ 10, 
    "ran_on_latency_8" : ran_on_8.values[:,0]/ 10,
}

latency_ispadding_df = pd.DataFrame(data=latency_padding_exist_kblocksize).set_index("key-value")


kblock_0726_df = pd.DataFrame(data=kblock).set_index('ticks')

kblock_seq = kblock_0726_df.iloc[:,:5].plot(**option, title="latency_padding_exist_kblocksize when seq")
kblock_ran = kblock_0726_df.iloc[:,5:].plot(**option, title="latency_padding_exist_kblocksize when ran")

option = {
    'kind' : 'bar',
    'figsize' : (15, 15),
    'xlabel' : "key-value pair",
    'ylabel' : "latency"
}

padding_kBlockSize = latency_ispadding_df.sort_values(by="payload").loc[:,['ran_on_latency_8','ran_on_latency_16','ran_on_latency_32']].plot(**option, title="latency according to kBlocksize when padding exists")# ran


plt.show()