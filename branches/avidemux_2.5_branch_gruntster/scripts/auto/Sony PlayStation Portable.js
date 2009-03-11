//AD
include("video/autoWizard.js");
include("video/filter.js");

var result = videoAutoWizard("Sony PSP Auto Wizard", [[480, 272], [720, 480]]);

if (result)
{
    var app = new Avidemux();
    var targetX = result[0][0];
    var targetY = result[0][1];
    var sourceRatio = result[1][0] + ":" + result[1][1];
    var destinationRatio = result[2][0] + ":" + result[2][1];

    resizeAndFillVideo(targetX, targetY, sourceRatio, destinationRatio);

    app.video.codecPlugin("32BCB447-21C9-4210-AE9A-4FCE6C8588AE", "x264", "2PASSBITRATE=1000", "<?xml version='1.0'?><x264Config><presetConfiguration><name>Sony PlayStation Portable</name><type>system</type></presetConfiguration><x264Options></x264Options></x264Config>");
    app.audio.codec("Faac", 128, 4, "80 00 00 00 ");

    if (app.audio.getNbChannels(0) != 2)
        app.audio.mixer("STEREO");

    app.setContainer("PSP");
}