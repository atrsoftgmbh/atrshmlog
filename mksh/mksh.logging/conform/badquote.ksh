

function badquote {
    print "hallo, welt    #missed a "

    for i in *
    do
	print $i
    done
    print "last line"
    
}

badquote


