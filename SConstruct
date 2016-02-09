#!python

import os

env = Environment()

env.Append( CPPPATH = ['include'] )

env.Append( CXXFLAGS = ['-Wall', '-Wextra', '-Wformat-nonliteral', '-Wcast-align', '-Wpointer-arith', '-Wmissing-declarations', '-Winline', '-Wundef', '-Wcast-qual', '-Wshadow', '-Wwrite-strings', '-Wno-unused-parameter', '-Wfloat-equal', '-pedantic', '-ansi'] )

sources = []
for root,dirs,files in os.walk('./src'):
    for f in files:
        if os.path.splitext(f)[1] == '.cc':
            sources.append(os.path.join(root,f))

csml = env.SharedLibrary(target = 'csml', source = sources)

inst = env.Install('/usr/lib', csml)
env.Alias('install', '/usr/lib')
