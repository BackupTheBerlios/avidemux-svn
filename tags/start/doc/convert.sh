cat guide.xsl | sed 's/ENCODINGTAG/iso-8859-15/' | sed 's/PATHTOCSS/.\/plop.css/' > /tmp/guide.xsl
xsltproc /tmp/guide.xsl index.xml > index.html
