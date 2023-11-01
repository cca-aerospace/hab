#!/usr/bin/python

import struct
import sys
import argparse
import pandas as pd
from integrate import Table

fields = [
    ("ms", "i"),
    ("temperature", "f"),
    ("humidity", "f"),
    ("altitude", "f"),
    ("oxygen", "f"),
    ("uv_idx", "f"),
    ("eulerx", "f"),
    ("eulery", "f"),
    ("eulerz", "f"),
    ("accelx", "f"),
    ("accely", "f"),
    ("accelz", "f"),
    ("voltage", "h"),
]

sizes = {"i": 4, "f": 4, "h": 2}

def sizeof(fields):
    global sizes
    return sum([sizes[f[1]] for f in fields])

def unpack (data, fields):
    global sizes
    d = {}
    i = 0
    for name, spec in fields:
        size = sizes[spec]
        d[name] = struct.unpack(spec, data[i:i+size])[0]
        i += size
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

data = []

for i in range(len(raw) // sizeof(fields)):
    data.append(unpack(raw, fields))
    raw = raw[sizeof(fields):]

all = pd.DataFrame(map(lambda e: list(e.values()), data), columns = data[0].keys())
all.to_csv(path_or_buf=args.output_csv)