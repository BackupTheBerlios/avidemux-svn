cat ../guide.xsl | sed 's/ENCODINGTAG/iso-8859-15/' | sed 's/PATHTOCSS/..\/plop.css/' > /tmp/guide.xsl
perl foreach.pl *.xml "xsltproc /tmp/guide.xsl %f > %f.html"
cp index.xml.html index.html
