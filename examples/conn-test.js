#!/usr/bin/seed

const lv = imports.gi.LibvirtGObject;
const gio = imports.gi.Gio;
const gtk = imports.gi.Gtk;

lv.init_object(null, null);

var conn = new lv.Connection({ uri: "test:///default" })
var canc = new gio.Cancellable()

//canc.cancel()

function done(conn, result, data) {
    try {
	conn.open_finish(result)

	print("Opened " + conn.uri)

	conn.fetch_domains()
	print ("Fetched")
	var doms = conn.get_domains()
	print ("Got " + doms)

	for (var d in doms) {
	    print ("One dom: " + doms[d])
	    print ("Name " + doms[d].get_name())

	    var conf = doms[d].get_config()
	    var xml = conf.get_doc()
	    print ("XML " + xml)
	}

    } finally {
	gtk.main_quit()
    }
}

conn.open_async(canc, done, null)

gtk.main(null, null)

