//AD  <- These first 4 characters need to be the first 4 characters to identify the ECMAScript file to Avidemux
var app = new Avidemux();
var file="/work/samples/avi/2mn.avi";
var goodwidth=512;
var goodheight=384;
var fps;
/* Load file
*/
app.load(file);
/* Test get fps 
*/
fps=app.video.width;
//app.displayInfo("FPS "+fps);
if(fps==goodwidth)
{
}
else
{
	app.displayError("Wrong width "+fps+"expected "+goodwidth);
}
fps=app.video.height;
//app.displayInfo("FPS "+fps);
if(fps==goodheight)
{
}
else
{
	app.displayError("Wrong height "+fps+"expected "+goodheight);
}

print("all ok");
/* End of test
*/
