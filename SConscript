# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/detModel/SConscript,v 1.6 2009/11/06 01:55:03 jrb Exp $
# Authors: Riccardo Giannitrapani <riccardo@fisica.uniud.it>, Joanne Bogart <jrb@slac.stanford.edu>
# Version: detModel-02-21-01
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

detModel = libEnv.StaticLibrary('detModel',
                                listFiles(['src/Management/*.cxx',
                                           'src/Sections/*.cxx',
                                           'src/Constants/*.cxx',
                                           'src/Utilities/*.cxx',
                                           'src/Materials/*cxx', 'src/Gdd.cxx']))

progEnv.Tool('detModelLib')
test_detModel = progEnv.Program('test_detModel',[ 'src/main.cxx'])

progEnv.Tool('registerTargets', package = 'detModel',
             staticLibraryCxts = [[detModel, libEnv]],
             testAppCxts = [[test_detModel, progEnv]],
             includes = listFiles(['detModel/*'], recursive = 1))




