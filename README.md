# Text Editor App (Wurd)
Text editor application made for a school project. Capable of basic features such as opening and saving local text files, editing with an undo feature,
and basic spell checking.
## Usage
Boot the project up in Visual Studio 2019 (perhaps other versions work but it must be Visual Studio, the editor GUI is set to expect that environment. You
will need to configure the PDCurses library differently if you wish to run this outside of Visual Studio). Running the project will open up the text editor window.
From there you can open a text file with `Ctrl + L`, save editor contents to a file (existing or not) with `Ctrl + S`, undo your next most recent action with
`Ctrl + Z`, load your own dictionary (if you don't want the default) with `Ctrl + D`, and quit the application with `Ctrl + X`. Positioning your cursor over a
misspelled word will offer spelling suggestions at the bottom of the editor. You can change the theme of the editor in the `main.cpp` file if you like.
