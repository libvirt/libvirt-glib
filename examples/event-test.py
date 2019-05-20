#!/usr/bin/env python3

import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
import libvirt
import getopt
import sys

gi.require_version('LibvirtGLib', '1.0')
from gi.repository import LibvirtGLib

def eventToString(event):
    eventStrings = ( "Added",
                     "Removed",
                     "Started",
                     "Suspended",
                     "Resumed",
                     "Stopped",
                     "Saved",
                     "Restored" )
    return eventStrings[event]

def myDomainEventCallback1 (conn, dom, event, detail, opaque):
    print("myDomainEventCallback1 EVENT: Domain %s(%s) %s" % (dom.name(), dom.ID(), eventToString(event)))

def myDomainEventCallback2 (conn, dom, event, detail, opaque):
    print("myDomainEventCallback2 EVENT: Domain %s(%s) %s" % (dom.name(), dom.ID(), eventToString(event)))

def usage():
        print("usage: "+os.path.basename(sys.argv[0])+" [uri]")
        print("   uri will default to qemu:///system")

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h", ["help"] )
    except getopt.GetoptError as err:
        # print help information and exit:
        print(str(err)) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()

    if len(sys.argv) > 1:
        uri = sys.argv[1]
    else:
        uri = "qemu:///system"

    print("Using uri:" + uri)

    LibvirtGLib.init()
    LibvirtGLib.event_register()
    vc = libvirt.open(uri)

    #Add 2 callbacks to prove this works with more than just one
    vc.domainEventRegister(myDomainEventCallback1,None)
    vc.domainEventRegister(myDomainEventCallback2,None)

    Gtk.main()

if __name__ == "__main__":
    main()
