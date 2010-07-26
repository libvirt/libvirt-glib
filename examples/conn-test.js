#!/usr/bin/seed

const lv = imports.gi.LibvirtGObject;
const gio = imports.gi.Gio;
const glib = imports.gi.GLib;

lv.object_init(null, null);

var conn = new lv.VirGConnection({ uri: "test:///default" })
var canc = new gio.Cancellable()

//canc.cancel()
loop = glib.main_loop_new()

function done(conn, result, data) {
    try {
	conn.open_finish(result)
	print("Opened " + conn.uri)
    } finally {
	glib.main_loop_quit(loop)
    }
}

conn.open(null done, null)

glib.main_loop_run(loop)

