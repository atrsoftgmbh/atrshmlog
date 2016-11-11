#!/home/kshell/builds/mksh/mksh/mksh

print $KSH_VERSION

echo "SHMID $ATRLOG_SHMID"

echo $$

typeset -i j=0;

# for (( i = 0 ; i < 1000000 ; i++ ))
# do
#    (( j++ ))
#done

typeset -i i=0

while (( i < 1000000 ))
do
    (( j++ ))
    (( i++ ))
done

    


    

