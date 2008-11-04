/*
 * libvirt-glib.c: Binding the glib event loop integration to python
 *
 * Copyright (C) 2008 Red Hat, Inc.
 *
 * Daniel Berrange <berrange@redhat.com>
 */

//#include <config.h>

/* Horrible kludge to work around even more horrible name-space pollution
   via Python.h.  That file includes /usr/include/python2.5/pyconfig*.h,
   which has over 180 autoconf-style HAVE_* definitions.  Shame on them.  */
#undef HAVE_PTHREAD_H

#include <Python.h>
#include <glib.h>
#include "libvirt-glib/libvirt-glib.h"

#ifndef __CYGWIN__
extern void initlibvirtglibmod(void);
#else
extern void initcygvirtglibmod(void);
#endif

#define VIR_PY_NONE (Py_INCREF (Py_None), Py_None)

static PyObject *
libvirt_virEventRegisterGLib(PyObject *self G_GNUC_UNUSED, PyObject *args G_GNUC_UNUSED) {
    virEventRegisterGLib();

    return VIR_PY_NONE;
}


static PyMethodDef libvirtGLibMethods[] = {
    {(char *) "virEventRegisterGLib", libvirt_virEventRegisterGLib, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

void
#ifndef __CYGWIN__
initlibvirtglibmod
#else
initcygvirtglibmod
#endif
  (void)
{
    /* initialize the python extension module */
    Py_InitModule((char *)
#ifndef __CYGWIN__
                  "libvirtglibmod"
#else
                  "cygvirtglibmod"
#endif
                  , libvirtGLibMethods);

}
