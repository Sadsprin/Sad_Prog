import sys
import re
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from functools import reduce
from os import listdir

mod = sys.modules[__name__]
key_value_list_df = {}
payload = []
rePayload = re.compile(r'[0-9]+')
reWal = re.compile(r'nowal')
key_value_list = listdir('./key-value')

for payloads in key_value_list:
    if payloads.endswith('.csv'):
        key_value_df = '{}_df_'.format(payloads.split('.')[0])
        setattr(mod, key_value_df , pd.read_csv('./key-value/{}'.format(payloads)))
        key_value_list_df[key_value_df] = locals()[key_value_df]

for idx, i in enumerate(key_value_list_df.keys()):
    index_wal = reWal.findall(i)
    index_wal_r = 'nowal' if len(index_wal) != 0 else 'onwal'
    payload.clear()
    currList = key_value_list_df[i]
    for j in currList.loc[:,"key-value"]:
        result = reduce(lambda acc, cur : acc + cur,list(map(int, rePayload.findall(j))))
        payload.append(result)
    currList["payload_"] = payload
    latencyBybyte = currList.values[:,1] / currList.values[:,4]
    latencyBybyte = np.round(latencyBybyte.astype('float64'),4)
    currList["latencyBybyte_{}".format(index_wal_r)] = latencyBybyte

graphOptions = {
    'kind' : 'line',
    'style' : ['k-','r-'],
    'xlabel' : 'key_size(byte)',
    'ylabel' : 'throughput(MB/s)',
    'fontsize' : 13,
    'figsize' : (5,5),
}


accordingTokeyOrValue = {
    'ticks' :np.array([16, 100, 500, 1000, 1250, 2000, 3000, 4000, 4096, 5000]),
    'on_wal_keysize' : np.array([6.76, 25.64, 103.38, 199.78, 237.8, 343.2, 432.8, 313.96, 316.82, 316.82]),
    'no_wal_keysize' : np.array([10.2, 43.14, 187.92, 373.04, 457.68, 604.34, 593.58, 370.38, 375.2,375.2]),
    'on_wal_valuesize' : np.array([12.26, 44.18, 167.82, 298.26,343.48,444.8,516.2,514.8,np.nan, 569.98]),
    'no_wal_valuesize' : np.array([34.82, 121.84, 374.06 , 465, 505.54, 558.8 , 620.14, 601.14, np.nan, 600.68]),
}

ticks = [16, 100, 500, 750, 1000, 1250, 1500, 2000]

fillseq_value_size_mb = {
    'on_wal_fillseq_value_size' : np.array([12.26, 44.18, 167.82, 298.26,343.48,444.8,516.2,514.8]),
    'no_wal_fillseq_value_size' : np.array([34.82, 121.84, 374.06 , 465, 505.54, 558.8 , 620.14, 601.14]),
}

overhead = {
    'key_ticks' : ticks,
    'overhead_fillseq_value_size' : fillseq_value_size_mb['no_wal_fillseq_value_size'] - fillseq_value_size_mb['on_wal_fillseq_value_size'],
}

index = ran_nowal_payload_df_.values[:,[4]]
ax = ran_nowal_payload_df_.set_index('key-value').iloc[:,[4]].plot(kind="area")
k = ran_onwal_payload_df_.set_index('key-value').iloc[:,[4]].plot(kind="bar", figsize=(8,8), ax=ax, color=['peru', 'red'])

overhead_df = pd.DataFrame(data=overhead).set_index('key_ticks')

keyOrValue = pd.DataFrame(data=accordingTokeyOrValue).set_index('ticks')

keyOrValue.iloc[:,:2].plot(**graphOptions, title="WAL according to key_size")
keyOrValue.iloc[:,2:4].plot(**graphOptions, title="WAL according to value_size")
overhead_df.iloc[:,:1].plot(**graphOptions, title= "WAL overhead according to value_size")

plt.show()

