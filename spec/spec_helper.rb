require 'pathname'
require 'rspec'
SPEC_DIR = Pathname.new(__FILE__).dirname
$LOAD_PATH.unshift(SPEC_DIR.join('..', 'lib').to_s)
$LOAD_PATH.unshift(SPEC_DIR.to_s)

require 'ent'

# Requires supporting files with custom matchers and macros, etc,
# in ./support/ and its subdirectories.
Dir["#{File.dirname(__FILE__)}/support/**/*.rb"].each {|f| require f}

RSpec.configure do |config|
  
end

def sample_path(fname)
  Pathname.new(__FILE__).dirname.join("samples", fname)
end

def sample_file(fname)
  sample_path(fname).to_s
end

