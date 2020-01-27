#include <gst/gst.h>
#include <glib.h>

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



int main (int argc, char *argv[])
{
  GMainLoop *loop;

  GstElement *pipeline, *source, *demuxer, *h264parser, *decoder, *sink;
  GstBus *bus;
  guint bus_watch_id;

  /* Initialisation */
  gst_init (&argc, &argv);

  loop = g_main_loop_new (NULL, FALSE);

  /* Check input arguments */
  if (argc != 2) {
    g_printerr ("Usage: %s <Ogg/Vorbis filename>\n", argv[0]);
    return -1;
  }


  /* Create gstreamer elements */
  pipeline    = gst_pipeline_new ("audio-player");
  source      = gst_element_factory_make ("filesrc",        "file-source");
  demuxer     = gst_element_factory_make ("qtdemux",        "demux");
  h264parser  = gst_element_factory_make ("h264parse",      "parse");
  decoder     = gst_element_factory_make ("ducatih264dec",  "decoder");
  sink        = gst_element_factory_make ("glimagesink",    "videosink");

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
  else if(!sink) 
  {
    g_printerr ("videosink could not be created. Exiting.\n");
  }

  /* Set up the pipeline */

  /* we set the input filename to the source element */
  g_object_set (G_OBJECT (source), "location", argv[1], NULL);

  /* we add a message handler */
  //bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  //bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
  gst_object_unref (bus);

  /* we add all elements into the pipeline */
  /* file-source | ogg-demuxer | vorbis-decoder | converter | alsa-output */
  gst_bin_add_many (GST_BIN (pipeline),
                    source, demuxer, h264parser, decoder, sink, NULL);

  /* we link the elements together */
  /* file-source -> ogg-demuxer ~> vorbis-decoder -> converter -> alsa-output */
  gst_element_link (source, demuxer);
  gst_element_link_many (h264parser, decoder, sink, NULL);
  g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), h264parser);

  /* note that the demuxer will be linked to the decoder dynamically.
     The reason is that Ogg may contain various streams (for example
     audio and video). The source pad(s) will be created at run time,
     by the demuxer when it detects the amount and nature of streams.
     Therefore we connect a callback function which will be executed
     when the "pad-added" is emitted.*/


  /* Set the pipeline to "playing" state*/
  g_print ("Now playing: %s\n", argv[1]);
  gst_element_set_state (pipeline, GST_STATE_PLAYING);


  /* Iterate */
  g_print ("Running...\n");
  g_main_loop_run (loop);

  /* Out of the main loop, clean up nicely */
  g_print ("Returned, stopping playback\n");
  gst_element_set_state (pipeline, GST_STATE_NULL);

  g_print ("Deleting pipeline\n");
  gst_object_unref (GST_OBJECT (pipeline));
  g_source_remove (bus_watch_id);
  g_main_loop_unref (loop);

  return 0;
}
