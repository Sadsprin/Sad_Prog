import re
import csv
import os
import sys
from collections import defaultdict

argv = defaultdict(list)

try:
    if len(sys.argv) != 4:
        raise Exception("--csv_file=path --key=keysize --value=valuesize three argument essential")
    else:
        for k, v in ((k.lstrip('-'), v) for k, v in(a.split('=') for a in sys.argv[1:])):
            argv[k].append(v)
except Exception as e:
    print(e)


def write_to_file_from_list(filepath, to_csv_list):
    if os.path.isfile(filepath) == False: # 파일이 존재하지 않을때
        mod = 'w' # write mode
    else:
        mod = 'a' # append mode

    with open(filepath, mod, encoding='utf-8') as f: #파일을 열고, file이 없으면 새로 만듬
            writer = csv.writer(f)
            if os.stat(filepath).st_size == 0: # 파일에 아무 내용이 없을 때 
                writer.writerow(fields) # csv_file columns
            writer.writerow(to_csv_list) # db_bench 실행 결과 파싱한 list를  csv파일에 씀
    f.close()

# initialize variable
latency = 0 
ops = 0
throughput = 0
divide_num = 0
key_value_column = 'k{}v{}'.format(''.join(argv['key']), ''.join(argv['value']))

print("key : {}     value : {}".format(argv['key'], argv['value']))

r = open("./final_result.txt",mode='r',encoding='utf-8') # final_result.txt를 가져와서 
fields = ['key-value','latency', 'ops/sec', 'throughput(MB/s)'] # csv파일의 columns list
regex = re.compile(r'\d+\.?\d+') # regular expression ./final_result.txt의 내용중에 숫자만 추출하는 코드
while True:
    oneline_result = r.readline() # final_result.txt를 한줄 읽어서 oneline_result에 저장 다음 반복에서는 다음줄로
    to_row = regex.findall(oneline_result) # 위 regex를 이용하여 추출한 숫자를 리스트로 만듦
    print(to_row)
    if len(to_row) != 0:
        latency_, ops_, throughput_ = map(float, to_row) # 리스트를 분해해서 3개의 변수로 나눔
        divide_num = divide_num + 1
        latency += latency_
        ops += ops_
        throughput += throughput_
    else:
        break

latency = round(latency / divide_num, 2)# 5개의 latency를 평균낸 값
ops = round(ops / divide_num, 2)# 5개의 ops를 평균낸 값
throughput = round(throughput / divide_num, 2)# 5개의 throughput을 평균낸 값

list_for_csv = [key_value_column, latency, ops, throughput] # 리스트로 만들어서 저장

write_to_file_from_list(''.join(argv['csv_file']), list_for_csv) # write_to_file_from_list를 이용하여 위 리스트의 값을 file_path(csv file)에 씀
