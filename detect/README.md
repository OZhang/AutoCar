# AutoCar

## opencv_createsamples.exe -vec pos.vec -info pos.txt -num 500 -w 60 -h 60 -bg neg.txt

## opencv_traincascade.exe -data xml -vec pos.vec -bg neg.txt -numPos 90 -numNeg 500 -numStages 10 -featureType LBP -w 60 -h 60 -minHitRate .999 -maxFalseAlarmRate 0.2 -weightTrimRate 0.95