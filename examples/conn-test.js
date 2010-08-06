#!/usr/bin/seed

const lv = imports.gi.LibvirtGObject;
const gio = imports.gi.Gio;
const glib = imports.gi.GLib;

lv.init_object(null, null);

var conn = new lv.Connection({ uri: "test:///default" })
var canc = new gio.Cancellable()

//canc.cancel()
loop = glib.main_loop_new()

function done(conn, result, data) {
    try {
	conn.open_finish(result)

	print("Opened " + conn.uri)

	conn.fetch_domains()
	print ("Fetched")
	doms = conn.get_domains()
	print ("Got " + doms)

	for (var d in doms) {
	    print ("One dom: " + doms[d])
	    print ("Name " + doms[d].get_name())
	}

    } finally {
	glib.main_loop_quit(loop)
    }
}

conn.open_async(canc, done, null)

glib.main_loop_run(loop)

