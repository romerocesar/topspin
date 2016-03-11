
env = Environment()
env.Append(CCFLAGS='-I /opt/local/include')

if ARGUMENTS.get('verb',0):
    env.Append(CCFLAGS='-DVERB')

if ARGUMENTS.get('debug',0):
    env.Append(CCFLAGS=['-g3','-O0'])
elif ARGUMENTS.get('profile',0):
    env.Append(CCFLAGS=['-g','-pg'])
else: 
    env.Append(CCFLAGS=['-O3'])

NoClean('getopt_pp.o')
topspin=env.Program(target='topspin',source=['getopt_pp.cpp','util.cpp','topspin.cpp','search.cpp','main.cpp'])
Default(topspin)
