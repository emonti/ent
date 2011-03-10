require 'mkmf'
require 'rbconfig'

extension_name = "ent_native"
have_library('m')
dir_config(extension_name)
create_makefile(extension_name)

