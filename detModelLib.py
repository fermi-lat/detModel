# $Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/detModel/detModelLib.py,v 1.1 2008/07/09 21:13:46 glastrm Exp $
def generate(env, **kw):
    if not kw.get('depsOnly', 0):
        env.Tool('addLibrary', library = ['detModel'])
    env.Tool('addLibrary', library = env['clhepLibs'])
    env.Tool('xmlUtilLib')
def exists(env):
    return 1;
