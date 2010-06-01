<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" encoding="utf-8" indent="yes" doctype-public=""/>
  <xsl:template match="TS">
    <TS>
      <xsl:copy-of select="@*"/>
      <xsl:for-each select="context">
        <xsl:call-template name="processContext"/>
      </xsl:for-each>
    </TS>
  </xsl:template>

  <xsl:template name="processContext">
    <xsl:variable name="filename" select="message[1]/location[1]/@filename"/>
    <xsl:variable name="ext" select="substring($filename, string-length($filename) - 2)"/>

    <context>
      <xsl:for-each select="name">
        <name>
          <xsl:if test="$ext = '.ui'">
            <xsl:value-of select="../name"/>
          </xsl:if>
        </name>
      </xsl:for-each>

      <xsl:for-each select="message[not(source = preceding::message/source and $ext != '.ui')]">
        <message>
          <xsl:copy-of select="*"/>
        </message>
      </xsl:for-each>
    </context>
  </xsl:template>
</xsl:stylesheet>
