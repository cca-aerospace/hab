#!/usr/bin/python

import struct
import sys
import argparse

fields = [
    ("ms", "i"),
    ("temperature", "f"),
    ("humidity", "f"),
    ("pressure", "f"),
    ("oxygen", "f"),
    ("uv_idx", "i"),
    ("eulerx", "f"),
    ("eulery", "f"),
    ("eulerz", "f"),
    ("accelx", "f"),
    ("accely", "f"),
    ("accelz", "f"),
]

def sizeof(fields):
    sizes = {"i": 4, "f": 4}
    return sum([sizes[f[1]] for f in fields])

def unpack (data, fields):
    d = {}
    for i in range(len(fields)):
        field = fields[i]
        d[field[0]] = struct.unpack(field[1], data[i*4:i*4+4])

    return d

parser = argparse.ArgumentParser(
    prog = "raw2csv",
    description = "converts raw sensor data from file into a csv file",
    epilog = "")
parser.add_argument("raw_data")
parser.add_argument("output_csv")

args = parser.parse_args()

if args.raw_data == args.output_csv:
    print(f"input and output files must be different")
    exit(-1)

try:
    with open(args.raw_data, "rb") as raw_data_file:
        raw = raw_data_file.read()
except Exception as e:
    print(f"error: {e}")
    exit(-1)

try:
    output_cvs = open(args.output_csv, "w+")
except Exception as e:
    print(f"error: {e}")
    exit(-1)

data = []

for i in range(len(raw) // sizeof(fields)):
    data.append(unpack(raw, fields))
    raw = raw[sizeof(fields):]

column_names = ",".join([field[0] for field in fields])
column_values = [",".join([str(n[0]) for n in datum.values()]) for datum in data]
all = "\n".join([column_names] + column_values)
output_cvs.write(all)
output_cvs.close()