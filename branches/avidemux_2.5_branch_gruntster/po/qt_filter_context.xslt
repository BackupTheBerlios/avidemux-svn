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
    <xsl:variable name="hasUiNodes" select="count(message/location[substring(@filename, string-length(@filename) - 2) = '.ui']) > 0"/>

    <context>
      <xsl:for-each select="name">
        <name>
          <xsl:if test="$hasUiNodes = 'true'">
            <xsl:value-of select="../name"/>
          </xsl:if>
        </name>
      </xsl:for-each>

      <xsl:for-each select="message[not(source = preceding::message/source) or $hasUiNodes = 'true']">
        <message>
          <xsl:copy-of select="*"/>
        </message>
      </xsl:for-each>
    </context>
  </xsl:template>
</xsl:stylesheet>
