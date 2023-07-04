#! /usr/bin/env python3

import sys
import datetime
import signal
import logging
import collections
import threading
import time
import sys
import tty, termios

import simpleaudio as sa
import pandas as pd

def getch():
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(fd)
        ch = sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return ch

# define an object to play
SOUNDS = [
    sa.WaveObject.from_wave_file('berim-2.wav'),
    sa.WaveObject.from_wave_file('berim-1.wav'),
    sa.WaveObject.from_wave_file('berim-3.wav'),
    sa.WaveObject.from_wave_file('A_Sitar_01_697.wav'),
    sa.WaveObject.from_wave_file('C#m_Sitar_01_697.wav'),
    sa.WaveObject.from_wave_file('C#m_BendUpSitar_02_697.wav'),
]

def play_sound(note):
    if note >= len(SOUNDS):
        print(f"Note {note} is to high")
        return

    SOUNDS[note].play()

class ElectroDrum():
    delay = 200
    def __init__(self):
        self.data = collections.deque(maxlen=200)
        self.data_lock = threading.Lock()

    def get_data(self):
        with self.data_lock:
            if not self.data:
                return None

            return pd.DataFrame(self.data)

    def get_last_data(self):
        with self.data_lock:
            if not self.data:
                return None

            return self.data[-2]

drum = ElectroDrum()

# thread function
def fetch_data():
    if len(sys.argv) >= 2:
        filename = sys.argv[1]
    else:
        filename = None
    if not filename:
        logging.info("Reading from file stdin ...")
        def get_notes():
            while char := getch():
                if char == "q":
                    return
                try:
                    yield "asdfghjkl".index(char)
                except ValueError:
                    print(f"? {char}")
    else:
        def get_notes():
            port = filename
            logging.info(f"Reading from {port} ...")

            ser = serial.Serial(port, 9600)
            while True:
                try:
                    line = ser.readline().decode('utf-8').strip()
                except serial.SerialException as e:
                    logging.warning(e)
                try:
                    yield int(line.strip())
                except Exception:
                    logging.warning(f"Could not parse the line '{line.strip()}'")

    YOTA = datetime.timedelta(microseconds=1)
    for note in get_notes():
        play_sound(note)
        ts = datetime.datetime.now()

        data = dict(note=str(note), level=1, time=ts)

        logging.info(data)
        with drum.data_lock:
            drum.data.append(dict(note=str(note), level=0, time=ts - YOTA))
            drum.data.append(data)
            drum.data.append(dict(note=str(note), level=0, time=ts + YOTA))

    print("hell")

def start_thread():
    thr = threading.Thread(target=fetch_data, daemon=True)
    thr.start()
