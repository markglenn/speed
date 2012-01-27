###############################################################################
# SCONS build script
###############################################################################

# Set the include paths
includepaths = [ '.' ]

# Convert the includes to a string
includepath = ""
for v in includepaths:
	includepath += "-I" + v + " "

# This is the g++ command
buildcommand = '/triton/cross-gcc-3.4.3-2.6.17/i686-pc-linux-gnu/bin/arm-linux-g++'
linkcommand  = '/triton/cross-gcc-3.4.3-2.6.17/i686-pc-linux-gnu/bin/arm-linux-ar'

# Set the build environment
bld = Builder(action = buildcommand + ' ' + includepath + ' -c $SOURCE -o $TARGET ')
link = Builder(action = linkcommand + ' cr $TARGET $SOURCES' )

env = Environment(BUILDERS = {'compile' : bld, 'link' : link})

###############################################################################
# Defines the sources of the project
###############################################################################

sources = [ "Timer.cpp" ]

# Threading
sources += [ 
	"Threading/Mutex.cpp", 
	"Threading/Semaphore.cpp", 
	"Threading/Thread.cpp" ]

# Types
sources += [
	"Types/Boolean.cpp",
	"Types/Float.cpp",
	"Types/Integer64.cpp",
	"Types/Integer.cpp" ]

# Memory
sources += [ "Memory/CircularBuffer.cpp" ]

# TinyXPath/TinyXML
sources += [
	"TinyXPath/action_store.cpp",
	"TinyXPath/lex_util.cpp",
	"TinyXPath/node_set.cpp",
	"TinyXPath/tinystr.cpp",
	"TinyXPath/tinyxml.cpp",
	"TinyXPath/tinyxmlerror.cpp",
	"TinyXPath/tinyxmlparser.cpp",
	"TinyXPath/tokenlist.cpp",
	"TinyXPath/xml_util.cpp",
	"TinyXPath/xpath_expression.cpp",
	"TinyXPath/xpath_processor.cpp",
	"TinyXPath/xpath_stack.cpp",
	"TinyXPath/xpath_static.cpp",
	"TinyXPath/xpath_stream.cpp",
	"TinyXPath/xpath_syntax.cpp" ]

# Engine
sources += [ 
	"Engine/Kernel.cpp", 
	"Engine/ThreadedService.cpp",
	"Engine/TimerService.cpp",
	"Engine/Configuration.cpp",
	"Engine/ConfigurationNode.cpp" ]

###############################################################################
# Compiles the project
###############################################################################

objects = []
for source in sources:
	object = source.replace ('.cpp', '.o')
	objects += [object]
	env.compile ( object, source )
		
env.link ( '../lib/libspeedlib.a', objects );