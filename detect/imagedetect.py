import numpy as np
import cv2
faceCascade = cv2.CascadeClassifier('xml\cascade.xml')
img = cv2.imread("IMG_0385.jpg")
img = cv2.resize(img, (320,320))
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.18,
        minNeighbors=25,
        minSize=(100,100),
        flags = cv2.IMREAD_GRAYSCALE
    )
for (x, y, w, h) in faces:
    cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)
cv2.namedWindow('image', cv2.WINDOW_AUTOSIZE)
cv2.imshow('image', img)
cv2.waitKey(0)