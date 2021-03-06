/*
 * libvirt-gobject-output-stream.h: libvirt gobject integration
 *
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Authors: Daniel P. Berrange <berrange@redhat.com>
 *          Marc-André Lureau <marcandre.lureau@redhat.com>
 */

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#pragma once

#include <gio/gio.h>
#include "libvirt-gobject-stream.h"

G_BEGIN_DECLS

#define GVIR_TYPE_OUTPUT_STREAM            (_gvir_output_stream_get_type ())
#define GVIR_OUTPUT_STREAM(inst)           (G_TYPE_CHECK_INSTANCE_CAST ((inst), GVIR_TYPE_OUTPUT_STREAM, GVirOutputStream))
#define GVIR_OUTPUT_STREAM_CLASS(class)    (G_TYPE_CHECK_CLASS_CAST ((class), GVIR_TYPE_OUTPUT_STREAM, GVirOutputStreamClass))
#define GVIR_IS_OUTPUT_STREAM(inst)        (G_TYPE_CHECK_INSTANCE_TYPE ((inst), GVIR_TYPE_OUTPUT_STREAM))
#define GVIR_IS_OUTPUT_STREAM_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE ((class), GVIR_TYPE_OUTPUT_STREAM))
#define GVIR_OUTPUT_STREAM_GET_CLASS(inst) (G_TYPE_INSTANCE_GET_CLASS ((inst), GVIR_TYPE_OUTPUT_STREAM, GVirOutputStreamClass))

typedef struct _GVirOutputStreamPrivate GVirOutputStreamPrivate;
typedef struct _GVirOutputStreamClass GVirOutputStreamClass;
typedef struct _GVirOutputStream GVirOutputStream;

struct _GVirOutputStreamClass
{
    GOutputStreamClass parent_class;
};

struct _GVirOutputStream
{
    GOutputStream parent_instance;
    GVirOutputStreamPrivate *priv;
};

GType _gvir_output_stream_get_type(void) G_GNUC_CONST;
GVirOutputStream *_gvir_output_stream_new(GVirStream *stream);

G_END_DECLS
