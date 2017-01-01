#
# $Id:$
#
# the ruby dot file

RUBY_INCLUDE=/usr/include
export RUBY_INCLUDE

# the python version only 2 or 3 for now
RUBY_VERSION=2
export RUBY_VERSION

# we set the platform too
# this is the platform for buildenvironment,: linux, cygwin, mingw, ...
RUBY_PLATFORM=linux
export RUBY_PLATFORM

# we set the path to find the module for tests
RUBYPATH=.:$(pwd)
export RUBYPATH

# end of file
