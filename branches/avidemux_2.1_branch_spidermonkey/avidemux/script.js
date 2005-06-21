//AD  <- These first 4 characters need to be the first 4 characters to identify the ECMAScript file to Avidemux
var app = new Avidemux();
app.Load("/usr/home/amistry/imps_chapter1_DAVENPORT_928735.avi");
/*
app.videoProcess=true;
app.audioProcess=true;
app.markerA=20;
app.markerB=30;

if(app.markerA == 20 && app.markerB == 30 && app.videoProcess == true && app.audioProcess == true)
	app.Exit();

//app.Append('/usr/home/amistry/storage/files/imps_chapter1_DAVENPORT_928735.avi');
//app.Save("./test.avi");
app.audio.ScanVBR();
app.audio.downsample=true;
app.audio.resample=44100;
app.audio.delay=300;
app.audio.pal2film=true;
app.audio.film2pal=true;
app.audio.normalize=true;
app.audio.mono2stereo=true;
app.audio.stereo2mono=true;
app.audio.Codec("lame",44100);
app.audioProcess=false;
app.audio.Save("/home/amistry/tmp/test.mp3");
//app.audio.Reset();
//app.audio.Load("mp3","/usr/home/amistry/tmp/siege_pt2_full.mp3");

*/
app.audioProcess=true;
app.audio.resample=48000;
app.videoProcess=true;
app.audio.ScanVBR();
app.currentframe = 5000;
app.markerA=2000;
app.markerB=3000;
//app.video.Clear();
app.video.Codec("DVD","","DVD");
app.video.SaveJPEG("/home/amistry/tmp/test.jpg");
app.video.Save("/home/amistry/tmp/test.avi");
//app.container = "OGM";
app.SaveDVD("/home/amistry/tmp/test.mpg");
//app.SaveOGM("/home/amistry/tmp/test.ogm");
//app.Save("/home/amistry/tmp/test1.avi");
//app.video.ListBlackFrames("/home/amistry/tmp/test-blackframe.lst");

