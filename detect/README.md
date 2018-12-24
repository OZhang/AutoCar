# AutoCar

## opencv_createsamples.exe -vec pos.vec -info pos.txt -num 500 -w 320 -h 240 -bg neg.txt

## opencv_traincascade.exe -data xml -vec pos.vec -bg neg.txt -numPos 280 -numNeg 600 -numStages 10 -featureType LBP -w 100 -h 100 -minHitRate .999 -maxFalseAlarmRate 0.2 -weightTrimRate 0.95