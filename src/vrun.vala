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


//using Gtk;
using GLib;

public static void main(string[] args) {
	Gtk.init(ref args);					//Call Gtk.init();
	new runDialog();					//Create MainClass
	Gtk.main();							//Call Gtk.main():
}

public class runDialog : Gtk.Window {
	
	Gtk.Entry commandEntry;
	List<string> binaries;
	
	public runDialog() {
		base.destroy.connect(Gtk.main_quit);
		base.set_title("Run");
		base.stick();										//show on all desktops
		//base.set_decorated(false);
		//base.set_type_hint(Gdk.WindowTypeHint.MENU);		//removes minimize(/iconify),maximize buttons
															//but starts the window unfocused :(
		
		//base.set_resizable(false);						//when 'false' metacity/mutter will
															//resize window to smallest possible :(
		
		base.set_keep_above(true);
		base.set_deletable(false);
		base.set_position(Gtk.WindowPosition.CENTER);
		base.set_default_size(300,90);
		base.key_press_event.connect(on_escape);
		//base.focus_out_event.connect(on_focus_out_event);	// buggy
		
		
		parseBins();
		buildGui();
		
		this.show_all();
	}
	
	private void buildGui() {
		Gtk.Box mainBox = new Gtk.Box(Gtk.Orientation.VERTICAL, 0);
		base.add(mainBox);
		
		//Label
		Gtk.Label topLabel = new Gtk.Label("Start program:");
		mainBox.pack_start(topLabel, true, true);
		
		//EntryCompletion
		Gtk.ListStore possibilities = new Gtk.ListStore(1, GLib.Type.from_name("gchararray"));
							//Note: GLib.Type = "GString" or "gchararray"
		
		foreach (string s in binaries) {
			Gtk.TreeIter iter1;
			possibilities.append(out iter1);
			possibilities.set_value(iter1, 0, s);
		}
		
		
		//
		Gtk.EntryCompletion completer = new Gtk.EntryCompletion();
		completer.set_model(possibilities);
		completer.set_inline_completion(true);
		completer.set_popup_completion(false);	//completer.set_inline_selection(false);
		completer.set_text_column(0);
		completer.set_minimum_key_length(1);	// ??? doesn't work ???
		
		//Entry
		commandEntry = new Gtk.Entry();
		commandEntry.set_icon_from_stock(Gtk.EntryIconPosition.SECONDARY, Gtk.Stock.EXECUTE);
		commandEntry.set_icon_activatable(Gtk.EntryIconPosition.SECONDARY, false);
		commandEntry.set_completion(completer);
		commandEntry.activate.connect( () => start(null) );
		mainBox.pack_start(commandEntry, true, true);
		
		//Buttons
		Gtk.Box buttonBox = new Gtk.Box(Gtk.Orientation.HORIZONTAL, 0);
		buttonBox.set_homogeneous(true);
		mainBox.pack_start(buttonBox, false, true);
		
		Gtk.Button startButton = new Gtk.Button.with_label("Start");
		startButton.button_press_event.connect( (x) => { start(null); return true; } );
		buttonBox.pack_start(startButton, true, true);
		
		Gtk.Button abortButton = new Gtk.Button.with_label("Abort");
		abortButton.button_press_event.connect( (x) => { quit(); return true; } );
		buttonBox.pack_start(abortButton, true, true);
		
		Gtk.Button searchButton = new Gtk.Button.with_label("Search");
		searchButton.button_press_event.connect( (x) => { open_fileChooser(); return true; } );
		buttonBox.pack_start(searchButton, true, true);
	}
	
	private void open_fileChooser() {
        Gtk.FileChooserDialog file_chooser = new Gtk.FileChooserDialog("Start Program", this,
                                      Gtk.FileChooserAction.OPEN,
                                      Gtk.Stock.CANCEL, Gtk.ResponseType.CANCEL,
                                      Gtk.Stock.OPEN, Gtk.ResponseType.ACCEPT);
		
        if (file_chooser.run() == Gtk.ResponseType.ACCEPT) {
			string filepath = file_chooser.get_filename();
			
			//Check if  program exists
			if (!GLib.FileUtils.test(filepath, GLib.FileTest.EXISTS)) {
				stdout.printf("Does not exist, exiting..\n");
				return;
			}
			
			//start selected filepath
			start(filepath);
        }
        file_chooser.destroy();
    }
	
	private void start(string? str) {
		string? execPath = str;
		
		if (execPath == null) {
			if (commandEntry.get_text_length() > 0) {
				execPath = commandEntry.get_text();
			}
			else {
				this.quit();
			}
		}
		
		
		
		
		
		execPath = execPath.escape("");							//Escapes special characters
		execPath = execPath.replace(" ","\\ ");					//Escapes whitespaces
		
		
		//Launch the program
		try {
			GLib.Process.spawn_command_line_async(execPath);
		}
		catch (SpawnError e) {
			stdout.printf("Error, spawning process, SpawnError-Message:\n");
			stdout.printf(e.message +"\n");
			stderr.printf("Error, spawning process, SpawnError-Message:\n");
			stderr.printf(e.message +"\n");
		}
		
		//Quit the launcher
		this.quit();
	}
	
    private void parseBins() {
		//string[] directories = { "/bin", "/sbin", "/usr/bin", "/usr/sbin", "/usr/local/bin", "/usr/local/sbin" };
		string[] directories = GLib.Environment.get_variable("PATH").split(":");
		binaries = null;
		binaries = new List<string>();
		
		//Scan all directories and add all their files to this.binaries
		foreach (string d in directories) {

			GLib.File directory = GLib.File.new_for_path(d);
			try {
				GLib.FileEnumerator enm = directory.enumerate_children(FileAttribute.STANDARD_NAME, 0);
				
				GLib.FileInfo fileInfo;
				while( (fileInfo = enm.next_file()) != null ) {
					this.binaries.append(fileInfo.get_name());
				}
			}
			catch (Error e) {
				stderr.printf("\n#######################################\n");
				stderr.printf("Error occured while scanning directories\n");
				stderr.printf("Error: \""+e.message+"\"\n");
				stdout.printf("Directory: "+d+"\n");
				stderr.printf("#######################################\n");
			}
		}
	}
	
	/**private bool on_focus_out_event(Gdk.EventFocus event) {
		this.quit();
		return true;	//event has been handeled successful
	 }**/
	
	private bool on_escape(Gdk.EventKey k) {
		//stdout.printf( "\nValue1: ");
		//stdout.printf( Gdk.Key.Escape.to_string()+"\n" );
		//stdout.printf( "Value2: ");
		//stdout.printf( k.keyval.to_string()+"\n" );
		
		//If Escape was pressed exit and set the signal
		//to "has been dealt with"
		if (Gdk.Key.Escape == k.keyval) {
			quit();
			return true;
		}
		else {
			return false;
		}
	}
	
	private void quit() {
		//Exit
		Gtk.main_quit();
		return;
	}

}
