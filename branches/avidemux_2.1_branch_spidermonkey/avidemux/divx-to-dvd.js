//AD  <- These first 4 characters need to be the first 4 characters to identify the ECMAScript file to Avidemux

var app = new Avidemux();
app.Load("/usr/home/amistry/storage/files/Family.Guy.S04E06.PDTV.XviD-LOL.avi");

app.audioProcess=true;
app.audio.resample=48000;
app.videoProcess=true;
app.audio.ScanVBR();
app.video.Codec("DVD","","divx-to-dvd.vcodec");
app.container = "PS";
app.SaveDVD("/home/amistry/windows-share/test.mpg");
app.Exit();
