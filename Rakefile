require 'rubygems'
require 'rake'

begin
  require 'jeweler'
  Jeweler::Tasks.new do |gem|
    gem.name = "mmap-ruby"
    gem.email = "me@brian.biz"
    gem.homepage = "http://github.com/brianosaurus/mmap-ruby"
    gem.summary = "Mmap interface.  Reading, writing, locking."
    gem.authors = ["Brian Woods"]

    gem.extensions = FileList["ext/**/extconf.rb"]
    gem.files = FileList['Rakefile', 'ext/**/*.{c,rb}', 'lib/**/*.rb', 'test/**/*.rb']
    gem.require_paths = ["ext", "lib"]

    gem.add_development_dependency "rake-compiler", "0.7.0"
  end
  Jeweler::GemcutterTasks.new
rescue LoadError
  puts "Jeweler (or a dependency) not available. Install it with: gem install jeweler"
end

gem 'rake-compiler', '>= 0.7.0'
require 'rake/extensiontask'
Rake::ExtensionTask.new('mmap_ruby') do |ext|
  ext.name = "mmap"
  ext.lib_dir = File.join('lib', 'mmap_ruby')
end

require 'rake/testtask'
Rake::TestTask.new(:test => :compile) do |test|
  test.libs << 'ext'
  test.verbose = true
end

task :default => :test
