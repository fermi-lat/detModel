# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/detModel/SConscript,v 1.11 2012/08/16 23:57:27 jrb Exp $
# Authors: Riccardo Giannitrapani <riccardo@fisica.uniud.it>, Joanne Bogart <jrb@slac.stanford.edu>
# Version: detModel-02-22-00
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('addLinkDeps', package = 'detModel', toBuild='static')
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




