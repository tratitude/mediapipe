# apt install plotutils
./store_gesture.o
for i in {0..9};
do
    graph -T png -S 3 0.1 < $i.gestureLog > $i.png
done
