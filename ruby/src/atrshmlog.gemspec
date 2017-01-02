Gem::Specification.new do |s|
  s.name        = 'atrshmlog'
  s.version     = '1.2.0'
  s.date        = '2017-01-01'
  s.summary     = "The atrshmlog layer for ruby !"
  s.description = "The atrshmlog layer for ruby ."
  s.authors     = ["Anton Rommerskirchen"]
  s.email       = 'atr@atrsoft.de'
  s.files       = ["lib/atrshmlog.rb", "ext/atrshmlog/atrshmlog.so" ]
  s.homepage    = 'http://www.atrsoft.de'
  s.license     = 'APACHE'
  s.extensions  = %w[ext/atrshmlog/extconf.rb]
end
