
# On cygwin, the DLL is called cygvirtmod.dll
try:
    import libvirtglibmod
except:
    import cygvirtglibmod as libvirtglibmod


def eventRegister():
    libvirtglibmod.virEventRegisterGLib()

