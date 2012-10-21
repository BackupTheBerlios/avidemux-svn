/*! \mainpage Avidemux Scripting
 *
 * \section intro_sec Introduction
 *
 * Avidemux uses the <a href="http://doc.qt.digia.com/4.5/qtscript.html">QtScript scripting engine</a>
 * to provide scripting support for the
 * <a href="http://www.ecma-international.org/publications/standards/Ecma-262.htm">ECMAScript</a> language.  
 * This allows users to execute scripts that query and control the Avidemux environment.  Scripts can either be
 * written by the user or generated by Avidemux.  The QtScript Debugger allows users to inspect the state of the 
 * script environment and control script execution.
 *
 * \section audience_sec Audience
 *
 * This reference guide is intended for users that are familiar with scripting languages.
 * For more information on the ECMAScript language, refer to the
 * <a href="http://www.ecma-international.org/publications/standards/Ecma-262.htm">ECMA-262 standard</a>.
 *
 * \section create_script_sec Creating a Script
 *
 * The easiest way to create a script is to:
 * 1. Open a video in Avidemux
 * 2. Set the desired encoders and filters, etc
 * 3. Choose the <i>QtScript Project</i> -> <i>Save As Project</i> menu item from the <i>%File</i> menu
 *
 * Alternatively, scripts can also be manually written.  Refer to the <a href="annotated.html">Classes</a>, 
 * <a href="examples.html">Examples</a> and <a href="modules.html">Modules</a> sections for more information on 
 * appropriate script syntax.  Scripts designed for the QtScript scripting engine should be given an <i>admjs</i>
 * file extension.
 *
 * \section run_script_sec Running a Script
 *
 * Scripts can be executed through the Avidemux user interface by choosing the <i>QtScript Project</i> -> 
 * <i>Run Project</i> menu item from the <i>%File</i> menu.  Scripts can also be executed on start-up of Avidemux by
 * specifying the <i>run</i> parameter along with the path of the script to run.  Note: Dialog related 
 * commands will be ignored when using the command line interface (avidemux_cli.exe or avidemux3_cli).
 *
 * Example usage of the <i>run</i> parameter on Windows:
 * \code
 * avidemux.exe --run "C:\admScripts\test.admjs"
 * \endcode
 * \code
 * avidemux_cli.exe --run "C:\admScripts\test.admjs"
 * \endcode
 *
 * \section debug_script_sec Debugging a Script 
 *
 * The QtScript Debugger can be started by choosing the <i>QtScript Project</i> -> <i>Debug Project</i> menu
 * item from the <i>%File</i> menu.  Refer to the <a href="http://doc.qt.digia.com/4.5/qtscriptdebugger-manual.html">
 * Qt Script Debugger manual</a> for an overview of the debugger interface and console commands.
 *
 * \section language_sec Core Script Classes
 *
 * Most of Avidemux's functionality for inspecting and manipulating video is exposed by the Editor object.
 * The Directory, File and FileInformation classes provide basic file and directory support.
 * The Dialog and Control classes provide the functionality to display custom dialog boxes.
 *
 */

