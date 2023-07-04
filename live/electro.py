import datetime
import signal
import logging
import collections
import threading
import sys
import fileinput
import time

import pandas as pd
import serial

class ElectroMagnet():
    delay = 200
    def __init__(self):
        self.data = collections.deque(maxlen=200)
        self.data_lock = threading.Lock()

    def get_xyz_data(self):
        with self.data_lock:
            if not self.data:
                return None

            return pd.DataFrame(self.data)

    def get_last_xyz(self):
        with self.data_lock:
            if not self.data:
                return None

            return self.data[-1]

electro = ElectroMagnet()

# thread function
def fetch_data():
    filename = sys.argv[1]
    if not filename.startswith("/dev"):
        logging.info("Reading from file {filename} ...")
        def get_lines():
            for line in open(filename).readlines():
                yield line
                time.sleep(electro.delay / 1000)
    else:
        def get_lines():
            port = filename
            logging.info(f"Reading from {port} ...")

            ser = serial.Serial(port, 9600)
            while True:
                try:
                    yield ser.readline().decode('utf-8').strip()
                except serial.SerialException as e:
                    logging.warning(e)

    for line in get_lines():
        if line.startswith("Go!"):
            electro.delay = int(line.strip()[3:])
            print(f"Delay: {electro.delay}ms")
            continue

        try:
            x, y, z = [int(e) for e in line.split()]
        except Exception:
            logging.warning(f"Could not parse the line '{line.strip()}'")
            continue

        ts = datetime.datetime.now()
        data = dict(x=x, y=y, z=z, time=ts)
        logging.info(data)
        with electro.data_lock:
            electro.data.append(data)


def start_thread():
    thr = threading.Thread(target=fetch_data, daemon=True)
    thr.start()
