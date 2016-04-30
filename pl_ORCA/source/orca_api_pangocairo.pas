{***************************************************
 ORCA 2D Library
 Copyright (C) PilotLogic Software House
 http://www.pilotlogic.com
 
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio
***********************************************************************}

unit orca_api_pangocairo;

{$I orca_define.inc}
{.$mode objfpc}
interface

uses
 SysUtils,
  {$IFDEF UNIX}
    {$IFDEF DARWIN}
     dynlibs,
     agl, gl, glext, MacOsAll, CarbonPrivate, CarbonDef,
    {$ELSE}
     dynlibs,
     gl, glext, glx, cairo, cairoXlib, xlib, x, xutil, gtk2def, gtk2proc, gtk2, gdk2, gdk2x, gdk2pixbuf,
     glib2, pango,
    {$ENDIF}
  {$ENDIF}

  LCLProc, LCLIntf, LCLType;

const
{$ifdef UseCustomLibs}
 pangocairolib = '';
{$else}
 pangocairolib = 'libpangocairo-1.0.so.0';
{$endif}


type
  PGInputStream = Pointer;

function  gdk_screen_get_rgba_colormap(const screen: PGdkScreen): PGdkColormap; cdecl; external gdklib;
function  gdk_pixbuf_get_file_info(filename:Pchar; var width, height: gint): integer; cdecl; external gdkpixbuflib;
function  gdk_pixbuf_save_to_buffer(pb: PGdkPixbuf; var bug: PByte; var Len: gsize; const typ: PChar; Error: Pointer): gboolean; cdecl; external gdkpixbuflib;
function  gdk_pixbuf_save_to_bufferv(pb: PGdkPixbuf; var bug: PByte; var Len: gsize; const typ: PChar; option_keys:PPchar; option_values:PPchar; Error: Pointer): gboolean; cdecl; external gdkpixbuflib;

function  pango_cairo_create_layout(cr: Pcairo_t): PPangoLayout; cdecl; external pangocairolib;
procedure pango_cairo_show_layout(cr: Pcairo_t; layout: PPangoLayout); cdecl; external pangocairolib;
procedure pango_cairo_layout_path(cr: Pcairo_t; layout: PPangoLayout); cdecl; external pangocairolib;
procedure pango_layout_set_height(layout:PPangoLayout; height:longint); cdecl; external pangolib;


implementation

end.