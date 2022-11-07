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
        d[field[0]] = struct.unpack(field[1], data[i*4:i*4+4])[0]

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

distance = Table(
    map(lambda e: (e["ms"] / 1000, e["accelx"], e["accely"], e["accelz"]), data),
    columns = ["seconds", "dispx", "dispy", "dispz"]
).integrate().integrate()

all = distance.join(pd.DataFrame(map(lambda e: list(e.values()), data), columns = data[0].keys()).drop("ms", axis=1))
all.to_csv(path_or_buf=args.output_csv)