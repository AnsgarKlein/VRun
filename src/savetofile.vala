/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
**/


//using GLib;

private static async void parse_to_file(string path_for_file_to_save_to) {
	//Create file to write to
	GLib.File saveFile = GLib.File.new_for_path(path_for_file_to_save_to);
	GLib.IOStream io_stream;
	try {
		//Create file if it doesn't exist
		//& create stream so we can write to it
		if (saveFile.query_exists()) {
			io_stream = saveFile.open_readwrite();
		}
		else {
			io_stream = saveFile.create_readwrite(GLib.FileCreateFlags.NONE);
		}
					
	}
	catch (GLib.Error e) {
		stderr.printf("\n#######################################\n");
		stderr.printf("Error occured while creating file\n");
		stderr.printf("Error: \""+e.message+"\"\n");
		stdout.printf("File: "+saveFile.get_path()+"\n");
		stderr.printf("#######################################\n");
		return;
	}
	
	//Scan all directories and add files to string
	string fileContent = "";
	string[] directories = GLib.Environment.get_variable("PATH").split(":");
	
	foreach (string d in directories) {
		
		GLib.File directory = GLib.File.new_for_path(d);
		try {
			GLib.FileEnumerator enm = directory.enumerate_children(FileAttribute.STANDARD_NAME, 0);
			
			GLib.FileInfo fileInfo;
			while( (fileInfo = enm.next_file()) != null ) {
				fileContent += fileInfo.get_name()+"\n";
			}
		}
		catch (GLib.Error e) {
			stderr.printf("\n#######################################\n");
			stderr.printf("Error occured while scanning directories\n");
			stderr.printf("Error: \""+e.message+"\"\n");
			stdout.printf("Directory: "+d+"\n");
			stderr.printf("#######################################\n");
		}
	}
	
	//Write string to file
	try {
		GLib.OutputStream outstr = io_stream.get_output_stream();
		GLib.DataOutputStream dataoutstr = new GLib.DataOutputStream(outstr);
		dataoutstr.put_string(fileContent, null);
	}
	catch (GLib.IOError e) {
		stderr.printf("\n##########################################\n");
		stderr.printf("Error occured while writing results to file\n");
		stderr.printf("Error: \""+e.message+"\"\n");
		stdout.printf("File: "+saveFile.get_path()+"\n");
		stderr.printf("##########################################\n");
	}
}
