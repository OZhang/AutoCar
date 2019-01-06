# import tkinter as tk
import numpy as np
import socket
import getopt, sys
import time
import os
import cv2
import pygame
from pygame.locals import *

class Controller:
    DEFAULT_PORT = 1111
    img_save_dir = ".\Webcam\chessboard_cali_1280_960"

    def __init__(self, ip, input_size):
        self.pressed = {}
        self.prevPressed = {}
        self._initPresses()
        # self._create_ui()
        self._host = ip
        self._port = self.DEFAULT_PORT
        self.send_inst = True
        self.input_size = input_size

        # create labels
        self.k = np.zeros((4, 4), 'float')
        for i in range(4):
            self.k[i, i] = 1
        pygame.init()
        pygame.display.set_mode((250, 250))

    def _netcat(self, content):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.sendto(content, (self._host, self._port))

    def _initPresses(self):
        self.pressed["w"] = False
        self.pressed["a"] = False
        self.pressed["s"] = False
        self.pressed["d"] = False
        self.prevPressed["w"] = False
        self.prevPressed["a"] = False
        self.prevPressed["s"] = False
        self.prevPressed["d"] = False

    def start(self, ):

        source = "http://192.168.1.230:8080/videofeed"
        cam = cv2.VideoCapture(source)

        saved_frame = 0
        total_frame = 0

        # collect images for training
        print("Start collecting images...")
        print("Press 'q' or 'x' to finish...")
        start = cv2.getTickCount()

        X = np.empty((0, self.input_size))
        y = np.empty((0, 4))

        # stream video frames one by one
        try:
            # stream_bytes = b' '
            frame = 1
            while self.send_inst:
            #while(cam.isOpened()):
                if cam.isOpened():
                    ret, frame = cam.read()
                    cv2.imshow('frame', frame)
                    if not ret:
                        break
                    # press ESC to escape (ESC ASCII value: 27)
                    # if cv2.waitKey(1) & 0xFF == 27:
                    #     break
                    image = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
                    height, width = image.shape
                    # height, width = frame.shape[:2]
                    roi = image[int(height/2):height, :]

                    # reshape the roi image into a vector
                    temp_array = roi.reshape(1, int(height/2) * width).astype(np.float32)
                            
                    frame += 1
                    #print(str(frame))
                    total_frame += 1

                # # get input from human driver
                for event in pygame.event.get():
                    if event.type == KEYDOWN:
                        key_input = pygame.key.get_pressed()
                        # print(key_input)
                        # simple orders
                        if key_input[pygame.K_UP]:
                            print("Forward")
                            saved_frame += 1
                            X = np.vstack((X, temp_array))
                            y = np.vstack((y, self.k[2]))
                            self._netcat(b"\x01")
                        elif key_input[pygame.K_DOWN]:
                            print("Reverse")
                            self._netcat(b"\x03")
                        elif key_input[pygame.K_RIGHT]:
                            print("Right")
                            X = np.vstack((X, temp_array))
                            y = np.vstack((y, self.k[1]))
                            saved_frame += 1
                            self._netcat(b"\x05")
                        elif key_input[pygame.K_LEFT]:
                            print("Left")
                            X = np.vstack((X, temp_array))
                            y = np.vstack((y, self.k[0]))
                            saved_frame += 1
                            self._netcat(b"\x07")
                        elif key_input[pygame.K_x] or key_input[pygame.K_q]:
                            print("exit")
                            self.send_inst = False
                            self._netcat(b"\x09")
                            break

                    elif event.type == pygame.KEYUP:
                        self._netcat(b"\x09")

            # save data as a numpy file
            file_name = str(int(time.time()))
            directory = "training_data"
            if not os.path.exists(directory):
                os.makedirs(directory)
            try:
                print(len(X))
                print(len(y))
                np.savez(directory + '/' + file_name + '.npz', train=X, train_labels=y)
            except IOError as e:
                print(e)

            end = cv2.getTickCount()
            # calculate streaming duration
            print("Streaming duration: , %.2fs" % ((end - start) / cv2.getTickFrequency()))

        finally:
            cam.release()
            cv2.destroyAllWindows()

    def _check_for_press(self, key, command):
        if self._is_pressed(key):
            self.prevPressed[key] = True
            self._netcat(command)
            print(key + " pressed")

    def _check_for_release(self, key, command):
        if self._is_released(key):
            self.prevPressed[key] = False
            self._netcat(command)
            print(key + " released")

    def _check_key_press(self):
        self._check_for_press("w", b"\x01")
        self._check_for_release("w", b"\x02")
        self._check_for_press("s", b"\x03")
        self._check_for_release("s", b"\x04")
        self._check_for_press("d", b"\x05")
        self._check_for_release("d", b"\x06")
        self._check_for_press("a", b"\x07")
        self._check_for_release("a", b"\x08")

        # self.root.after(20, self._check_key_press)

    def _is_pressed(self, key):
        return self.pressed[key] and self.prevPressed[key] == False

    def _is_released(self, key):
        return self.pressed[key] == False and self.prevPressed[key]

    # def _create_ui(self):
    #     self.root = tk.Tk()
    #     self.root.geometry('400x300')
    #     self._set_bindings()

    # def _set_bindings(self):
    #     for char in ["w","s","d", "a"]:
    #         self.root.bind("<KeyPress-%s>" % char, self._pressed)
    #         self.root.bind("<KeyRelease-%s>" % char, self._released)
    #         self.pressed[char] = False

    def _pressed(self, event):
        self.pressed[event.char] = True

    def _released(self, event):
        self.pressed[event.char] = False

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h:", ["host="])
    except getopt.GetoptError as err:
        print(str(err))
        usage()
        sys.exit(2)
    host = ""
    for o, a in opts:
        if o in ("-h", "--host"):
            host = a

    if host == "":
        print("Did not define host, use -h or --host to pass the host name of the car")
        sys.exit(2)
    # vector size, half of the image
    s = 120 * 320
    p = Controller(host, s)
    p.start()

def usage():
    print("Available options:")
    print("-h, --host  Define RC car IP address")


if __name__ == "__main__":
    main()