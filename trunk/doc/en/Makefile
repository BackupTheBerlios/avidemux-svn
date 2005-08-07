ALL_XML=audio.xml  \
bframe.xml.html \
capture2SVCD.xml.html \
command.xml.html \
cutting.xml.html \
divx2vcd.xml.html \
dualaudio.xml.html \
dvd.xml.html \
dvd2divx.xml.html \
edl.xml.html \
ffv1rec.xml.html \
guitour.xml.html \
spidermonkey.xml.html \
index.xml.html \
input.xml.html \
install.xml.html \
mpegcutter.xml.html \
output.xml.html \
script.xml.html \
standalone.xml.html \
vbrmp3.xml.html \
video.xml.html \
video1.xml.html \
video2.xml.html \
video3.xml.html \
video4.xml.html \
video5.xml.html \
video6.xml.html

%.xml.html:%.xml
	xsltproc /tmp/guide.xsl $< > $<.html
all:$(ALL_XML)
	cat ../guide.xsl | sed 's/ENCODINGTAG/iso-8859-15/' | sed 's/PATHTOCSS/.. \/plop.css/' > /tmp/guide.xsl
