#include <gst/gst.h>
#include <glib.h>
#include <stdio.h>

static void
on_pad_added (GstElement *element,
              GstPad     *pad,
              gpointer    data);



GstElement* get_ducati_pipeline(char* filename);
