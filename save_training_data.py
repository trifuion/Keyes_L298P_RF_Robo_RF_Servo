import cv2
import sys

import serial
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)


def save_image_and_steering_angle(video_file):

    cap = cv2.VideoCapture(0)
    #cap = cv2.VideoCapture(video_file + '.avi')

    try:
        i = 0
        while cap.isOpened():
            _, frame = cap.read()

            arduino_serial=ser.readline().decode('ascii')
            angle=int(arduino_serial)
            print(angle)
            cv2.imwrite("%s_%03d_%03d.png" % (video_file, i, angle), frame)
            i += 1
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
    finally:
        cap.release()
        cv2.destroyAllWindows()


if __name__ == '__main__':
    save_image_and_steering_angle('/media/pi/Storage/AI/Keyes_L298P_RF_Robo_RF_Servo/training_video/video')