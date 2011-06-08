#!/usr/bin/python

from gi.repository import LibvirtGObject;
from gi.repository import Gio;
from gi.repository import Gtk;

LibvirtGObject.init_object_check(None);

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

                print "Document is valid according to %s" % conf.get_schema()
            except Exception, e:
                print "Document is not valid according to %s: %s: %s" % (conf.get_schema(), str(e), str(type(e)))

	    xml = conf.get_doc()
	    print ("XML " + xml)
	    print ("Info " + str(d.get_info().memory))

    finally:
	Gtk.main_quit()


conn.open_async(canc, done, None)

Gtk.main()


