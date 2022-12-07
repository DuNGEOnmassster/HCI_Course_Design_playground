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
    parser.add_argument("--GAMMA", type= float, default=1.5,
                        help="declare GAMMA")
    parser.add_argument("--drowsyTime", type=float, default=1.5,
                        help = "declare 1200ms as a basic drowsy time")
    parser.add_argument("--blinkTime", type=float, default=0.15,
                        help="declare 150ms as a basic blink time")

    return parser.parse_args()


def gamma_correction(image):
    invGamma = 1.0/args.GAMMA
    table = np.array([((i / 255.0) ** invGamma) * 255 for i in range(0, 256)]).astype("uint8")
    return cv2.LUT(image, table)


def histogram_equalization(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    return cv2.equalizeHist(gray) 


def soundAlert(path, threadStatusQ):
    while True:
        if not threadStatusQ.empty():
            FINISHED = threadStatusQ.get()
            if FINISHED:
                break
        playsound.playsound(path)


def eye_aspect_ratio(eye):
    A = dist.euclidean(eye[1], eye[5])
    B = dist.euclidean(eye[2], eye[4])
    C = dist.euclidean(eye[0], eye[3])
    ear = (A + B) / (2.0 * C)

    return ear


def checkEyeStatus(landmarks):
    mask = np.zeros(frame.shape[:2], dtype = np.float32)
    
    hullLeftEye = []
    for i in range(0, len(leftEyeIndex)):
        hullLeftEye.append((landmarks[leftEyeIndex[i]][0], landmarks[leftEyeIndex[i]][1]))

    cv2.fillConvexPoly(mask, np.int32(hullLeftEye), 255)

    hullRightEye = []
    for i in range(0, len(rightEyeIndex)):
        hullRightEye.append((landmarks[rightEyeIndex[i]][0], landmarks[rightEyeIndex[i]][1]))


    cv2.fillConvexPoly(mask, np.int32(hullRightEye), 255)

    lenLeftEyeX = landmarks[leftEyeIndex[3]][0] - landmarks[leftEyeIndex[0]][0]
    lenLeftEyeY = landmarks[leftEyeIndex[3]][1] - landmarks[leftEyeIndex[0]][1]

    lenLeftEyeSquared = (lenLeftEyeX ** 2) + (lenLeftEyeY ** 2)
    eyeRegionCount = cv2.countNonZero(mask)

    normalizedCount = eyeRegionCount/np.float32(lenLeftEyeSquared)

    #############################################################################
    leftEAR = eye_aspect_ratio(hullLeftEye)
    rightEAR = eye_aspect_ratio(hullRightEye)

    ear = (leftEAR + rightEAR) / 2.0
    #############################################################################

    eyeStatus = 1          # 1 -> Open, 0 -> closed
    if (ear < args.thresh):
        eyeStatus = 0

    return eyeStatus  


def checkBlinkStatus(eyeStatus):
    global state, blinkCount, drowsy
    if(state >= 0 and state <= falseBlinkLimit):
        if(eyeStatus):
            state = 0

        else:
            state += 1

    elif(state >= falseBlinkLimit and state < drowsyLimit):
        if(eyeStatus):
            blinkCount += 1 
            state = 0

        else:
            state += 1

    else:
        if(eyeStatus):
            state = 0
            drowsy = 1
            blinkCount += 1

        else:
            drowsy = 1


def getLandmarks(im):
    imSmall = cv2.resize(im, None, 
                            fx = 1.0/args.FACE_DOWNSAMPLE_RATIO, 
                            fy = 1.0/args.FACE_DOWNSAMPLE_RATIO, 
                            interpolation = cv2.INTER_LINEAR)

    rects = detector(imSmall, 0)
    if len(rects) == 0:
        return 0

    newRect = dlib.rectangle(int(rects[0].left() * args.FACE_DOWNSAMPLE_RATIO),
                            int(rects[0].top() * args.FACE_DOWNSAMPLE_RATIO),
                            int(rects[0].right() * args.FACE_DOWNSAMPLE_RATIO),
                            int(rects[0].bottom() * args.FACE_DOWNSAMPLE_RATIO))

    points = []
    [points.append((p.x, p.y)) for p in predictor(im, newRect).parts()]
    return points

args = parse_args()

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(args.modelPath)

leftEyeIndex = [36, 37, 38, 39, 40, 41]
rightEyeIndex = [42, 43, 44, 45, 46, 47]

blinkCount = 0
drowsy = 0
state = 0

ALARM_ON = False
threadStatusQ = queue.Queue()

capture = cv2.VideoCapture(0)


for i in range(10):
    ret, frame = capture.read()

totalTime = 0.0
validFrames = 0
dummyFrames = 100

print("Caliberation in Progress!")
    
while(validFrames < dummyFrames):
    validFrames += 1
    t = time.time()
    ret, frame = capture.read()
    height, width = frame.shape[:2]
    IMAGE_RESIZE = np.float32(height)/args.RESIZE_HEIGHT
    frame = cv2.resize(frame, None, 
                        fx = 1/IMAGE_RESIZE, 
                        fy = 1/IMAGE_RESIZE, 
                        interpolation = cv2.INTER_LINEAR)

    # adjusted = gamma_correction(frame)
    adjusted = histogram_equalization(frame)

    landmarks = getLandmarks(adjusted)
    timeLandmarks = time.time() - t

    if landmarks == 0:
        validFrames -= 1
        cv2.putText(frame, "Unable to detect face, Please check proper lighting", (10, 30), cv2.FONT_HERSHEY_COMPLEX, 0.5, (0, 0, 255), 1, cv2.LINE_AA)
        cv2.putText(frame, "or decrease FACE_DOWNSAMPLE_RATIO", (10, 50), cv2.FONT_HERSHEY_COMPLEX, 0.5, (0, 0, 255), 1, cv2.LINE_AA)
        cv2.imshow("Blink Detection Demo", frame)
        if cv2.waitKey(1) & 0xFF == 27:
            sys.exit()

    else:
        totalTime += timeLandmarks
        # cv2.putText(frame, "Caliberation in Progress", (200, 30), cv2.FONT_HERSHEY_COMPLEX, 0.5, (0, 0, 255), 1, cv2.LINE_AA)
        # cv2.imshow("Blink Detection Demo", frame)
        
    # if cv2.waitKey(1) & 0xFF == 27:
    #         sys.exit()

print("Caliberation Complete!")

spf = totalTime/dummyFrames
print("Current SPF (seconds per frame) is {:.2f} ms".format(spf * 1000))

drowsyLimit = args.drowsyTime/spf
falseBlinkLimit = args.blinkTime/spf
print("drowsy limit: {}, false blink limit: {}".format(drowsyLimit, falseBlinkLimit))


if __name__ == "__main__":
    vid_writer = cv2.VideoWriter('output-low-light-2.avi',cv2.VideoWriter_fourcc('M','J','P','G'), 15, (frame.shape[1],frame.shape[0]))
    while(1):
        try:
            t = time.time()
            ret, frame = capture.read()
            height, width = frame.shape[:2]
            IMAGE_RESIZE = np.float32(height)/args.RESIZE_HEIGHT
            frame = cv2.resize(frame, None, 
                                fx = 1/IMAGE_RESIZE, 
                                fy = 1/IMAGE_RESIZE, 
                                interpolation = cv2.INTER_LINEAR)

            # adjusted = gamma_correction(frame)
            adjusted = histogram_equalization(frame)

            landmarks = getLandmarks(adjusted)
            if landmarks == 0:
                validFrames -= 1
                cv2.putText(frame, "Unable to detect face, Please check proper lighting", (10, 30), cv2.FONT_HERSHEY_COMPLEX, 0.5, (0, 0, 255), 1, cv2.LINE_AA)
                cv2.putText(frame, "or decrease FACE_DOWNSAMPLE_RATIO", (10, 50), cv2.FONT_HERSHEY_COMPLEX, 0.5, (0, 0, 255), 1, cv2.LINE_AA)
                cv2.imshow("Blink Detection Demo", frame)
                if cv2.waitKey(1) & 0xFF == 27:
                    break
                continue

            eyeStatus = checkEyeStatus(landmarks)
            checkBlinkStatus(eyeStatus)

            for i in range(0, len(leftEyeIndex)):
                cv2.circle(frame, (landmarks[leftEyeIndex[i]][0], landmarks[leftEyeIndex[i]][1]), 1, (0, 0, 255), -1, lineType=cv2.LINE_AA)

            for i in range(0, len(rightEyeIndex)):
                cv2.circle(frame, (landmarks[rightEyeIndex[i]][0], landmarks[rightEyeIndex[i]][1]), 1, (0, 0, 255), -1, lineType=cv2.LINE_AA)

            if drowsy:
                cv2.putText(frame, "! ! ! DROWSINESS ALERT ! ! !", (70, 50), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 0, 255), 2, cv2.LINE_AA)

            else:
                cv2.putText(frame, "Blinks : {}".format(blinkCount), (460, 80), cv2.FONT_HERSHEY_COMPLEX, 0.8, (0,0,255), 2, cv2.LINE_AA)
                # (0, 400)
                ALARM_ON = False

            cv2.imshow("Blink Detection Demo", frame)
            vid_writer.write(frame)

            k = cv2.waitKey(100) 
            if k == ord('r'):
                state = 0
                drowsy = 0
                ALARM_ON = False
                threadStatusQ.put(not ALARM_ON)

            elif k == 27:
                break

        except Exception as e:
            print(e)

    capture.release()
    vid_writer.release()
    cv2.destroyAllWindows()


