gcc  gst-test.c -o gst-test -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
gcc  run_pipeline.c -g -o run_pipeline -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
