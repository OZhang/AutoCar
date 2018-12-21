import cv2
import time

faceCascade = cv2.CascadeClassifier('xml\cascade5.xml')
#调用摄像头
cap = cv2.VideoCapture(0)
#设置图片尺寸
cap.set(3, 320)   # set Width
cap.set(4, 240)   # set Height
#循环抓取图片
while True:
    # 获取视频一帧图像
    ret, img = cap.read()
    # 将图像转换为灰度图像
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.38,
        minNeighbors=20,
        minSize=(120,120)
    )
    count=0
    for (x, y, w, h) in faces:
        cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)
        count+=1
        if(count==len(faces)):
            cv2.imwrite('img/%s.png' % time.time(), img)
    cv2.imshow('video', img)
    k = cv2.waitKey(50) & 0xff#控制相机拍照间隔
    if k == 27:  # press 'ESC' to quit
        break