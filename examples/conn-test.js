#!/usr/bin/env gjs

const lv = imports.gi.LibvirtGObject;
const gio = imports.gi.Gio;
const gtk = imports.gi.Gtk;

lv.init_object(null, null);

var conn = new lv.Connection({ uri: "test:///default" })
var canc = new gio.Cancellable()

function done(conn, result, data) {
    try {
	conn.open_finish(result)

	print("Opened " + conn.get_uri())

	conn.fetch_domains(null)
	print ("Fetched")
	var doms = conn.get_domains()
	print ("Got " + doms)

	for (var d in doms) {
	    print ("One dom: " + doms[d])
	    print ("Name " + doms[d].get_name())
	    var conf = doms[d].get_config(0)
	    print ("Conf " + conf)
	    var xml = conf.to_xml()
	    print ("XML " + xml)
	    print ("Info " + doms[d].get_info().memory)
	}

    } finally {
	gtk.main_quit()
    }
}

conn.open_async(canc, done, null)

gtk.main(null, null)
