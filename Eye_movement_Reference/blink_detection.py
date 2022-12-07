import dlib
import sys
import cv2
import time
import numpy as np
from scipy.spatial import distance as dist
from threading import Thread
import playsound
import queue
import argparse


def parse_args():
    parser = argparse.ArgumentParser(description="Blink Detection Demo")

    parser.add_argument("--modelPath", default="models/shape_predictor_68_face_landmarks.dat",
                        help="declare model path")

    parser.add_argument("--FACE_DOWNSAMPLE_RATIO", type=float, default=1.5,
                        help="declare the ratio in face downsampling")
    parser.add_argument("--RESIZE_HEIGHT", type=int, default=460,
                        help="declare resize height for processing image resize")
    parser.add_argument("--thresh", type=float, default=0.27,
                        help="declare thresh for EAR")
    # parser.add_argument("--")

    return parser.parse_args()

args = parse_args()


