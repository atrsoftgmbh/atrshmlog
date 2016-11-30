
# start the HelloJNI demo

# we use this time the package version. so its with the classspath / package
# the shared lib is also moved to the package dir here
java -Djava.library.path=./myjnipackage/lib myjnipackage.HelloJNI 
