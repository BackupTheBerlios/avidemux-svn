#cat ../guide.xsl | sed 's/ENCODINGTAG/KOI8-R/' > /tmp/guide.xsl
cat ../guide.xsl | sed 's/ENCODINGTAG/KOI8-R/' | sed 's/PATHTOCSS/..\/plop.css/' > /tmp/guide.xsl
perl foreach.pl *.xml "xsltproc /tmp/guide.xsl %f > %f.html"
cp index.xml.html index.html
