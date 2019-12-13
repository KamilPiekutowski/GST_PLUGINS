CC = gcc
CFLAGS = -Wall -g
DEPS = ducati_pipeline.h
OBJ = run_pipeline.o ducati_pipeline.o
LDFLAGS += `pkg-config --cflags --libs gstreamer-1.0`

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS) 
run_pipeline: $(OBJ)
	$(CC) $(FLAGS) -o $@ $^ $(LDFLAGS)


.PHONY: gst-test
.PHONY: clean

gst-test:
	$(CC) gst-test.c -o gst-test $(CFLAGS) $(LDFLAGS)

clean:
	rm *.o *~ gst-test run_pipeline
