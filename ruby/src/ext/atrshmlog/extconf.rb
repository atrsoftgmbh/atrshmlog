require 'mkmf'

# we use the CC in place - or the supported one from the env
RbConfig::MAKEFILE_CONFIG['CC'] = ENV['CC'] if ENV['CC']

extension_name = 'atrshmlog'

# checks go here ....

find_library('atrshmlog', 'atrshmlog_attach')

# checks done. we create the things we need
create_header

# the makefile
create_makefile('atrshmlog/atrshmlog')

# end of file
