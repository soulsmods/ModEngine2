Mod Engine 2 Preview release:

This is a preview release of Mod Engine 2 for Elden Ring and Armored Core 6. Mod Engine 2 is a utility for injecting various
code into the game for the use of mods. Among other things, Mod Engine 2 allows the usage of mods that modify the game files
without having to go through the hassle of unpacking the game archives and patching the game exe. Mod Engine 2 does this
by injecting code into the game's asset system, and redirecting the game to load a modded file inside your mod folder if
one exists instead of loading the original file in the game's big archives.

Mod Engine 2 is a ground up rewrite of Mod Engine and is designed to improve over the existing user experience while also being
more reliable and having more infrastructure to diagnose issues. This public release currently only supports modded file injection,
but additional features like GUI mod management, param merging, game save management with mods, and other potential features are
being considered and some in progress.

If you've installed mods with the original mod engine for Sekiro or DS3, Mod Engine 2 installation should be somewhat similar.
However, while Mod Engine 1 was a dinput8.dll file that you had to copy to the game directory, Mod Engine 2 is a standalone
utility with a custom game launcher and does not have to be installed in the game directory. If your game is installed via
Steam, all you need to do is extract the Mod Engine 2 distribution wherever you want, copy your modded files into the "mod"
directory, and double-click "launchmod_eldenring.bat" and the game will automatically be launched with Mod Engine 2 injected.
This includes the game launching with EAC disabled so you can play with mods offline with no fear of being banned.

Mod Engine 2 can also be extracted directly into the game directory if you wish, but this method is not recommended compared
to using launchmod_eldenring.bat to launch the game no matter where Mod Engine 2 is installed. If you do this, you can double-click
"modengine2_launcher.exe" directly to launch the game with the default configuration file "config_eldenring.toml". You can also
rename the launcher to "start_protected_game.exe" (after backing up the existing one) and have Steam automatically launch with your
mod, though game updates will restore the existing exe and overwrite the Mod Engine 2 launcher if you do so.

Advanced options can be found in config_eldenring.toml, which contains the default configuration for Mod Engine 2. With it you can
specify custom mod paths, add multiple mod paths, enable the debug console, and inject scylla hide to bypass the game's antidebugging.
To edit you can do open with and select Notepad from the applications, or use an editor like Notepad++ or VSCode.