svn log --stop-on-copy --xml $SOURCEDIR > svn.xml
xsltproc svnlog.xslt svn.xml > "$BUILDDIR/Change Log.html"
xsltproc revision.xslt svn.xml > revision.nsh
rm svn.xml
