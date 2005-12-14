//AD

var arr = new Array();
//arr[0] = "/home/amistry/fakefile";
//arr[1] = "/usr/X11R6/bin/aterm";

var rtn = exec("/bin/sh",arr,true);
print("Test String\n");
print(rtn);
print(23.564);
print(arr);



var ds = new DirectorySearch();
if(ds.Init("/tmp"))
{
	while(ds.NextFile())
	{
		if(ds.isDirectory)
			print("Directory: "+ds.GetFileName());
		if(ds.isHidden)
			print("Hidden: "+ds.GetFileName());
		if(ds.isArchive)
			print("Archive: "+ds.GetFileName());
		if(ds.isSingleDot)
			print("Single: "+ds.GetFileName());
		if(ds.isDoubleDot)
			print("Double: "+ds.GetFileName());
		if(ds.isNotFile)
			print("Not A File: "+ds.GetFileName());
		if(!ds.isNotFile && !ds.isDirectory && !ds.isHidden && !ds.isArchive && !ds.isSingleDot && !ds.isDoubleDot)
			print("File: "+ds.GetFileName());
	}
}
else
	print("Error initilizing the directory search");

setSuccess(true);
