require 'rubygems'
require 'rake'
require 'rake/extensiontask'

require 'jeweler'
Jeweler::Tasks.new do |gem|
  # gem is a Gem::Specification... see http://docs.rubygems.org/read/chapter/20 for more options
  gem.name = "ent"
  gem.homepage = "http://github.com/emonti/ent"
  gem.license = "MIT"
  gem.summary = %Q{Calculate the entropy of data}
  gem.description = %Q{Calculate the entropy of data}
  gem.email = "esmonti@gmail.com"
  gem.authors = ["Eric Monti"]

  gem.extensions = FileList['ext/**/extconf.rb']
  gem.extra_rdoc_files += FileList['ext/**/*.c']

end

Jeweler::RubygemsDotOrgTasks.new

Rake::ExtensionTask.new("ent_native")

CLEAN.include("lib/*.bundle")
CLEAN.include("ext/ent_native/*.bundle")
CLEAN.include("ext/ent_native/*.o")
CLEAN.include("ext/ent_native/Makefile")
CLEAN.include("ext/ent_native/mkmf.log")
CLEAN.include("ext/ent_native/extconf.h")
CLEAN.include("lib/*.so")
CLEAN.include("tmp/")
CLEAN.include("doc/")
CLEAN.include("rdoc/")
CLEAN.include("coverage/")

require 'rspec/core'
require 'rspec/core/rake_task'
RSpec::Core::RakeTask.new(:spec => :compile ) do |spec|
  spec.pattern = FileList['spec/**/*_spec.rb']
end

RSpec::Core::RakeTask.new(:rcov) do |spec|
  spec.pattern = 'spec/**/*_spec.rb'
  spec.rcov = true
end

task :default => :spec

require 'rake/rdoctask'
Rake::RDocTask.new do |rdoc|
  version = File.exist?('VERSION') ? File.read('VERSION') : ""

  rdoc.rdoc_dir = 'rdoc'
  rdoc.title = "ent #{version}"
  rdoc.rdoc_files.include('README*')
  rdoc.rdoc_files.include('lib/**/*.rb')
end
