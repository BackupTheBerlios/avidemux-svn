//AD
include("video/autoWizard.js");
include("video/filter.js");

var app = new Avidemux();

if (app.video == null)
    displayError("A video file must be open to use this Auto Wizard.");
else
{
    var result = videoAutoWizard("Apple iPod 5.5G Auto Wizard", [[320, 240], [640, 480]]);

    if (result)
    {
        var targetX = result[0][0];
        var targetY = result[0][1];
        var sourceRatio = result[1][0] + ":" + result[1][1];
        var destinationRatio = result[2][0] + ":" + result[2][1];

        resizeAndFillVideo(targetX, targetY, sourceRatio, destinationRatio);

        app.video.codecPlugin("32BCB447-21C9-4210-AE9A-4FCE6C8588AE", "x264", "2PASSBITRATE=1000", "<?xml version='1.0'?><x264Config><presetConfiguration><name>Apple iPod 5.5G</name><type>system</type></presetConfiguration><x264Options></x264Options></x264Config>");

        if (app.audio.targetTrackInfo.length > 0)
        {
            if (app.audio.targetTrackInfo[0].codec != "AAC" || app.audio.targetTrackInfo[0].channelCount != 2)
            {
                app.audio.codec("Faac", 128, 4, "80 00 00 00 ");

                if (app.audio.targetTrackInfo[0].channelCount == 2)
                    app.audio.mixer = "NONE";
                else
                    app.audio.mixer = "STEREO";
            }
        }

        app.setContainer("MP4");
    }
}