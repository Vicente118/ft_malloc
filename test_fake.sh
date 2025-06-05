while true
do 
    ./run.sh /usr/bin/time -v ./malloc 2> out_fake ; cat out_fake | grep Minor
done