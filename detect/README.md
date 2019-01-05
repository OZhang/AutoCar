# AutoCar

## opencv_annotation.exe -images C:\testProject\opencv\AutoCar\detect\pos /? -annotations C:\testProject\opencv\AutoCar\detect\pos.txt 

## opencv_createsamples.exe -vec pos.vec -info pos.txt -num 500 -w 320 -h 240 -bg neg.txt

## opencv_traincascade.exe -data xml -vec pos.vec -bg neg.txt -numPos 59 -numNeg 600 -numStages 2 -featureType LBP -w 640 -h 480 -minHitRate .999 -maxFalseAlarmRate 0.2 -weightTrimRate 0.95