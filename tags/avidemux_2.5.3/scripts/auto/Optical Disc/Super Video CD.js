//AD
include("video/autoWizard.js");
include("video/functions.js");
include("video/filter.js");

var app = new Avidemux();

if (app.video == null)
    displayError("A video file must be open to use this Auto Wizard.");
else
{
    var result = videoAutoWizard("Super Video CD Auto Wizard");

    if (result)
    {
        var props = getVideoProperties(app.video);
        var fps1000 = props[2];
        var targetX = 480;

        if (getColourEncodingSystem(fps1000) == "NTSC")
            targetY = 576;
        else
            targetY = 480;

        var sourceRatio = result[1][0] + ":" + result[1][1];
        var destinationRatio = result[2][0] + ":" + result[2][1];

        resizeAndFillVideo(targetX, targetY, sourceRatio, destinationRatio);

        app.video.codecPlugin("DBAECD8B-CF29-4846-AF57-B596427FE7D3", "avcodec", "2PASSBITRATE=2000", "<?xml version='1.0'?><Mpeg2Config><Mpeg2Options><minBitrate>0</minBitrate><maxBitrate>2400</maxBitrate><xvidRateControl>true</xvidRateControl><bufferSize>112</bufferSize><widescreen>false</widescreen><interlaced>none</interlaced><gopSize>12</gopSize></Mpeg2Options></Mpeg2Config>");

        if (app.audio.targetTrackInfo.length > 0)
        {
            if (app.audio.targetTrackInfo[0].codec == "MP2" && app.audio.targetTrackInfo[0].frequency == 44100 && app.audio.targetTrackInfo[0].channelCount == 2)
                app.video.codec("Copy", "CQ=4", "0 ");
            else
            {
                app.audio.codec("TwoLame", 160, 8, "a0 00 00 00 01 00 00 00 ");

                if (app.audio.targetTrackInfo[0].frequency != 44100)
                    app.audio.resample = 44100;

                if (app.audio.targetTrackInfo[0].channelCount == 2)
                    app.audio.mixer = "NONE";
                else
                    app.audio.mixer = "DOLBY_PROLOGIC2";
            }
        }

        app.setContainer("PS", "02 00 00 00 00 00 00 00 ");
    }
}