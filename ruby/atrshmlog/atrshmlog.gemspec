Gem::Specification.new do |s|
  s.name        = 'atrshmlog'
  s.version     = '1.2.0'
  s.date        = '2017-01-03'
  s.summary     = 'The atrshmlog layer for ruby !'
  s.description = 'The atrshmlog layer for ruby .'
  s.authors     = [ 'Anton Rommerskirchen' ]
  s.email       = 'atr@atrsoft.de'
  s.files       = [ 'ext/atrshmlog/atrshmlog.so' ]
  s.homepage    = 'http://www.atrsoft.de'
  s.license     = 'APACHE 2.0'
  s.extensions  = %w[ext/atrshmlog/extconf.rb]
  s.platform    = Gem::Platform::CURRENT
end
