# $Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/detModel/detModelLib.py,v 1.3 2009/01/23 00:21:07 ecephas Exp $
def generate(env, **kw):
    if not kw.get('depsOnly', 0):
        env.Tool('addLibrary', library = ['detModel'])
    env.Tool('addLibrary', library = env['clhepLibs'])
    env.Tool('xmlUtilLib')
    env.Tool('xmlBaseLib')
    env.Tool('identsLib')
    env.Tool('addLibrary', library = env['xercesLibs'])
def exists(env):
    return 1;
