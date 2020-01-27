GST_DEBUG=5 gst-launch-1.0 filesrc location=TearOfSteel-Short-1920x800.mov ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! ducatih264dec ! audiofiltertemplate ! waylandsink --gst-debug-no-color 2>&1 | grep -i ducati

