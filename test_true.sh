while true
do 
    /usr/bin/time -v ./malloc_sys 2> out_true ; cat out_true | grep Minor
done