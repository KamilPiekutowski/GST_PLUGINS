if [ "$1" == "DEBUG" ]; then
	GST_DEBUG=5 gst-launch-1.0 filesrc location=TearOfSteel-Short-1920x800.mov ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! ducatih264dec ! myfilter ! waylandsink
else
	gst-launch-1.0 filesrc location=TearOfSteel-Short-1920x800.mov ! qtdemux name=demux demux.video_0 ! queue ! h264parse ! ducatih264dec ! myfilter ! waylandsink 
fi
