TGT= \
avidemux_icon_small.inc \
gnome_calculator.inc \
gnome_calculator_small.inc \
stock_allow_effects.inc \
preview.inc \
output.inc \
stock_filters_aging.inc \
systray.inc \
systray2.inc \
filter1.inc \
filter2.inc \
filter3.inc \
filter4.inc \
filter5.inc \
filter6.inc \
filter7.inc \
film1.inc \
film3.inc \
film5.inc \
film7.inc \
film9.inc \
film11.inc \
film13.inc \
film15.inc \
film17.inc \
film19.inc \
film21.inc \
film23.inc \
audio_volume_medium.inc \
preview_button.inc \
first_frame.inc \
markA.inc\
next_black_frame.inc \
next_key_frame.inc \
previous_black_frame.inc \
previous_key_frame.inc \
last_frame.inc   \
markB.inc	\
next_frame.inc	 \
play.inc	\
previous_frame.inc	  \
stop.inc

%.inc : ../%.png	
	echo $<
	gdk-pixbuf-csource --raw --name=`basename $@ .inc` $< > $@

%.inc : %.png	
	echo $<
	gdk-pixbuf-csource --raw --name=`basename $@ .inc` $< > $@
all : $(TGT)

clean:
	rm -f $(TGT) 
