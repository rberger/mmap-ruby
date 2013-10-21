# encoding: utf-8

require 'rubygems'
require 'bundler'
begin
  Bundler.setup(:default, :development)
rescue Bundler::BundlerError => e
  $stderr.puts e.message
  $stderr.puts "Run `bundle install` to install missing gems"
  exit e.status_code
end
require 'rake'

require 'jeweler'
Jeweler::Tasks.new do |gem|
  # gem is a Gem::Specification... see http://docs.rubygems.org/read/chapter/20 for more options
  gem.name = "mmap-ruby"
  gem.homepage = "http://github.com/brianosaurus/mmap-ruby"
  gem.license = "MIT"
  gem.summary = %Q{Very basic mmap interface with mlock and munlock}
  gem.description = %Q{}
  gem.email = "me@brian.biz"
  gem.authors = ["Brian Woods"]
  # dependencies defined in Gemfile
end
Jeweler::RubygemsDotOrgTasks.new

gem 'rake-compiler', '>= 0.7.0'
require 'rake/extensiontask'
Rake::ExtensionTask.new('mmap_ruby') do |ext|
    ext.name = "mmap"
    ext.lib_dir = File.join('lib', 'mmap_ruby')
end

require 'rake/testtask'
Rake::TestTask.new(:test) do |test|
  test.libs << 'lib' << 'test'
  test.pattern = 'test/**/test_*.rb'
  test.verbose = true
end

task :default => :test

