#!/home/shmlog/mksh/mksh.logging/mksh

# you have to adjust the above lines to your box

print $KSH_VERSION

ls
date ; id

ls | wc -l 

if [ $( ls *.c | wc -l ) = "16" ]
then
print ok
else
print nok
fi

ZZZ=`date|wc` 

print $ZZZ

typeset accu

function hugo
{
	for i in $*
	do
	accu="$accu$i"
	print $accu
	done
}

hugo 1 2 3 4711

print $accu

typeset -i z=0

while (( z < 10 ))
do
	  print $z
	  
	  (( z = z + 2 ))
	  
done

print $z

A=$( ls | wc -l )


print $A

for i in 1 2 3 4
do

    print $i
done

print $i

case $i in
    1) print 1 ;;
    2) print 2 ;;
    *) print "ex" ;;
esac

[ -r atrlogfork.c ] && print da

[ -r netda ] && print da

[ -r atrlogfork.c ] || print da

[ -r netda ] || print da

A=4711 B=13 print $B

time ls

for i in *.c
do
    print ${i%%.c}.hugo
done


Z=hugo

U=${Z:-hansi}

V=${Z:+hansi}

W=${Z:=otto}

echo $U $V $W $Z

( ls ; sleep 3 ) &

wait

( read input ; print $input ) |&

ls &

if [[ ! "x" == "y" ]]
then
    print nok
else
    print ok
fi

if [[ "x" == "y" ]]
then
    print nok
elif [[ "z" == "z" ]]
then
    print okok
else
    print ok
fi


while [ true ]
do
    break
done

{ ls ; print hallo ; sleep 1 ; }

ls < /dev/null

ls > /dev/null

ls < /dev/null > /dev/null 2>&1

exec 3< /dev/null

(
    PATH=.
cat autonom.ksh
)

(
    PATH=.
printf "%d" 4711
)


alias hout=echo

hout $Z

eval "hugo=$Z"

print $hugo

. ./dot.me 1 2 3 4 5 6 7 8 9

dot.you

./bash_exec.sh

exit 0

