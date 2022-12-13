import pandas as pd
from os import listdir
import re
import sys
from functools import reduce
import numpy as np
import matplotlib.pyplot as plt

mod = sys.modules[__name__]
rePayload = re.compile(r'[0-9]+')
payload = []
ispadding_list = listdir('./padding')
for ispadding in ispadding_list:
    if ispadding.endswith('.csv'):
        setattr(mod, '__{}_'.format(ispadding.split('.')[0]), pd.read_csv('./padding/{}'.format(ispadding)))

        
ispadding_dict = {
    "__seq_onwal_payload_nopadding_" : __seq_onwal_payload_nopadding_,
    "__seq_onwal_payload_onpadding_" : __seq_onwal_payload_onpadding_,
    "__ran_onwal_payload_nopadding_" : __ran_onwal_payload_nopadding_,
    "__ran_onwal_payload_onpadding_" : __ran_onwal_payload_onpadding_,
}

for i in ispadding_dict.keys():
    payload.clear()
    currList = ispadding_dict[i]
    for j in currList.loc[:,"key-value"]:
        result = reduce(lambda acc, cur : acc + cur, list(map(int, rePayload.findall(j))))
        payload.append(result)
    currList["payload_"] = payload

seq_no = ispadding_dict["__seq_onwal_payload_nopadding_"].set_index("key-value")
seq_on = ispadding_dict["__seq_onwal_payload_onpadding_"].set_index("key-value")
ran_no = ispadding_dict["__ran_onwal_payload_nopadding_"].set_index("key-value")
ran_on = ispadding_dict["__ran_onwal_payload_onpadding_"].set_index("key-value")

throughput_ispadding = {
    "key-value" : ispadding_dict["__seq_onwal_payload_nopadding_"].values[:,0],
    "payload" : ispadding_dict["__seq_onwal_payload_nopadding_"].values[:,4],
    "seq_no_throughput" : seq_no.values[:,2],
    "seq_on_throughput" : seq_on.values[:,2],
    "ran_no_throughput" : ran_no.values[:,2],
    "ran_on_throughput" : ran_on.values[:,2],
}
throughput_ispadding_df = pd.DataFrame(data=throughput_ispadding).set_index("key-value")
options = {
    'kind' : 'bar',
    'figsize' : (15, 15),
    'xlabel' : "key-value pair",
    'ylabel' : "throughput"
}

throughput_ispadding_df.sort_values(by="payload").loc[:,['seq_no_throughput', 'seq_on_throughput']].plot(**options, title="fillseq throughput according to existence of padding sorting sum of key and value")
throughput_ispadding_df.sort_values(by="payload").loc[:,['ran_no_throughput', 'ran_on_throughput']].plot(**options, title="fillrandom throughput according to existence of padding sorting sum of key and value")

plt.show()