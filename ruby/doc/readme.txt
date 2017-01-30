Hello !

I use a simple trick from a article to get my things in.

The extension is put into a dir struct as suggested on rubygems.

And i have in place the setting of CC according to a environment
variable  - if it is set.

So before you run ruby extconf.rb you have to set CC.

I do this with the dot file as for all other layers.

For a ruby only guy this is an unexpected thing.

So you have to source dot.ruby.sh first, then do the usual ruby things.

If you insist you can change the extconf.rb by setting the things there.

But then you couple the settings for the include header with the
settings for ruby build - which i think is
- not necesary
- not needed

so the build steps are:

dot.platform.sh in BASEDIR
dot.ruby.sh in ruby/src
ruby extconf.rb in ext/atrshmlog
make in ext/atrshmlog

In the next version i will try to make it to a real gem or rake.
