Installation Instructions for VRun
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Gtk3
----

To install VRun you will need to have Gtk3 installed.



Vala
----

Since VRun is written in Vala you also need the vala compiler (valac) and it's dependencies.

You will need at least version 0.16 to compile successfully.

On Ubuntu theses packages are called:
	* ``valac-0.16``
	* ``valac-0.16-vapi``
	* ``libvala-0.16-0``
	(If thats not enough try installing ``libvala-0.16-dev``)

If you use Ubuntu you can add a Vala ppa:
``ppa:vala-team/ppa``



Build
-----

When you have all dependencies installed simply do:
	$ make

	$ make install	(you might want to sudo this)



Usage
-----

Once you have installed VRun you can start it with the 'vrun' command.

I recommend binding the 'vrun' command to a keyboard
shortcut like SUPER+R (Windows+R)
