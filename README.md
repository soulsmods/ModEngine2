# Mod Engine 2
This is a ground up rewrite of Mod Engine, a runtime injection library for adding useful modding functionality for Souls games. Mod Engine 2 is currently under development, but has the following features currently planned:\

* Automatic Scyllahide injection for bypassing the antidebug and allowing debuggers such as WinDbg and x64dbg to be attached
* Runtime injection of file based mods: set a mod directory and game files will be loaded from there instead of the archives if they exist
* Integration of the DS3 debug menu and boot menu patches. Runtime translation of debug menu text is planned.
* Blocking of all networking.
