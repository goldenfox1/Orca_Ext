The package imagesforlazarus.lpk adds LCL support for some extra image formats like tga.

Normal usage is to add it to your project in the project inspector.
You can install it in the IDE. Then the IDE picture open dialogs will be able to load some more file formats.

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
We used a simple naming schema to help non expert working with this package.
The support type name and the support unit name are :

T$(most_common_extension)Image is in laz$(most_common_extension).pas

The Typhon TPNGImage is in lazpng.pas
The Typhon TPNMImage is in lazpnm.pas
The Typhon TJPGImage is in lazjpg.pas
The Typhon TBMPImage is in lazbmp.pas
The Typhon TTGAImage is in laztga.pas
The Typhon TXPMImage is in lazxpm.pas

This package uses the fpimage libs provided by FreePascal in the FCL/Image.
See there for in more detailed jpeg handling.

Please report bugs to "Mazen NEIFER" <mazen@freepascal.org>
