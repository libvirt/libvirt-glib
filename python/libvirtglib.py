
# On cygwin, the DLL is called cygvirtmod.dll
try:
    import libvirtglibmod
except:
    import cygvirtglibmod as libvirtglibmod


def event_register():
    libvirtglibmod.event_register()
