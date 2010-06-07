//AD
include("video/autoWizard.js");
include("video/functions.js");
include("video/filter.js");

var app = new Avidemux();

if (app.video == null)
    displayError(QT_TR_NOOP("A video file must be open to use this Auto Wizard."));
else
{
    var result = videoAutoWizard(QT_TR_NOOP("Video CD Auto Wizard"));

    if (result)
    {
        var props = getVideoProperties(app.video);
        var fps1000 = props[2];
        var targetX = 352;
        var targetY;

        if (getColourEncodingSystem(fps1000) == "NTSC")
            targetY = 240;
        else
            targetY = 288;

        var sourceRatio = result[1][0] + ":" + result[1][1];
        var destinationRatio = result[2][0] + ":" + result[2][1];

        resizeAndFillVideo(targetX, targetY, sourceRatio, destinationRatio);

        app.video.codecPlugin("056FE919-C1D3-4450-A812-A767EAB07786", "mpeg2enc", "CBR=1000", "<?xml version='1.0'?><Mpeg1Config><presetConfiguration><name>Video CD</name><type>system</type></presetConfiguration><Mpeg1Options></Mpeg1Options></Mpeg1Config>");

        if (app.audio.targetTrackInfo.length > 0)
        {
            if (app.audio.targetTrackInfo[0].codec == "MP2" && app.audio.targetTrackInfo[0].frequency == 44100 && app.audio.targetTrackInfo[0].channelCount == 2)
                app.audio.codec("copy",0,0,"");
            else
            {
                app.audio.codec("TwoLame", 224, 8, "e0 00 00 00 01 00 00 00 ");

                if (app.audio.targetTrackInfo[0].frequency != 44100)
                    app.audio.resample = 44100;

                if (app.audio.targetTrackInfo[0].channelCount == 2)
                    app.audio.mixer = "NONE";
                else
                    app.audio.mixer = "DOLBY_PROLOGIC2";
            }
        }

        app.setContainer("PS", "01 00 00 00 00 00 00 00 ");
    }
}
