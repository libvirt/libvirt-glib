#!/usr/bin/env python3

import gi

gi.require_version('LibvirtGObject', '1.0')
from gi.repository import LibvirtGObject
from gi.repository import Gio
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

LibvirtGObject.init_object_check(None)

conn = LibvirtGObject.Connection(uri="test:///default")
canc = Gio.Cancellable()

def done(conn, result, data):
    try:
        conn.open_finish(result)

        print("Opened " + conn.get_uri())

        conn.fetch_domains(None)
        print ("Fetched")
        doms = conn.get_domains()
        print ("Got " + str(doms))

        for d in doms:
            print ("One dom: " + str(d))
            print ("Name " + d.get_name())
            conf = d.get_config(0)
            print ("Conf " + str(conf))

            try:
                conf.validate()

                print ("Document is valid according to %s" % conf.get_schema())
            except Exception as e:
                print ("Document is not valid according to %s: %s: %s" % (conf.get_schema(), str(e), str(type(e))))

            xml = conf.to_xml()
            print ("XML " + xml)
            print ("Info " + str(d.get_info().memory))

    finally:
        Gtk.main_quit()


conn.open_async(canc, done, None)

Gtk.main()
