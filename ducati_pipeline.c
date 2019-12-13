#include "ducati_pipeline.h"

static void
on_pad_added (GstElement *element,
              GstPad     *pad,
              gpointer    data)
{
  GstPad *sinkpad;
  GstElement *decoder = (GstElement *) data;

  /* We can now link this pad with the vorbis-decoder sink pad */
  g_print ("Dynamic pad created, linking demuxer/decoder\n");

  sinkpad = gst_element_get_static_pad (decoder, "sink");

  gst_pad_link (pad, sinkpad);

  gst_object_unref (sinkpad);
}



GstElement* get_ducati_pipeline(char* filename)
{
  printf("IM HERE\n!");

  GstElement *pipeline, *source, *demuxer, *h264parser, *decoder;

  //GstBus *bus;

  /* Create gstreamer elements */
  pipeline    = gst_pipeline_new ("audio-player");
  source      = gst_element_factory_make ("filesrc",        "file-source");
  demuxer     = gst_element_factory_make ("qtdemux",        "demux");
  h264parser  = gst_element_factory_make ("h264parse",      "parse");
  decoder     = gst_element_factory_make ("ducatih264dec",  "decoder");

  if (!pipeline)
  {
    g_printerr ("pipeline could not be created. Exiting.\n");
  }
  else if(!source)
  {
    g_printerr ("source could not be created. Exiting.\n");
  }
  else if(!demuxer) 
  {
    g_printerr ("demuxer could not be created. Exiting.\n");
  }
  else if(!h264parser)
  {
    g_printerr ("h264parser could not be created. Exiting.\n");
  }
  else if(!decoder)
  {
    g_printerr ("decoder could not be created. Exiting.\n");
  }

  /* Set up the pipeline */

  /* we set the input filename to the source element */
  g_object_set (G_OBJECT (source), "location", filename, NULL);

  /* we add a message handler */
  //bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  //bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
  //gst_object_unref (bus);

  /* we add all elements into the pipeline */
  /* file-source | ogg-demuxer | vorbis-decoder | converter | alsa-output */
  gst_bin_add_many (GST_BIN (pipeline),
                    source, demuxer, h264parser, decoder, NULL);

  /* we link the elements together */
  /* file-source -> ogg-demuxer ~> vorbis-decoder -> converter -> alsa-output */
  gst_element_link (source, demuxer);
  gst_element_link_many (h264parser, decoder, NULL);
  g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), h264parser);


  return pipeline;
}
