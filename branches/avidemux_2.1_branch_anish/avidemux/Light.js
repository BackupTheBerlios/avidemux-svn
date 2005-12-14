/* Light.js */
/* This is a simple example how to create an object with ECMAScript. */

function Light(initialState, sName)
{
	// define properties
	this.name = sName;
	this.on = initialState;
	this.color = "green";
	// define methods
	this.display = function ()
	{// begin display
		print("Light \""+this.name+"\" is currently "+(this.on ? "on" : "off")+".");
	}// end display
	this.toggle = function ()
	{// begin toggle
		this.on = this.on ? false : true;
	}// end toggle
}

