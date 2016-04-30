{***************************************************
 ORCA 3D Library
 Copyright (C) PilotLogic Software House
 http://www.pilotlogic.com
 
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio
***********************************************************************}



{$IFDEF WINDOWS}
 {$DEFINE ORCA_3D_USE_DIRECTX9}
 {.$DEFINE ORCA_3D_USE_OPENGL}
 {.$DEFINE ORCA_3D_USE_OPENGL_DLG}
 {.$DEFINE ORCA_3D_USE_MESA}
{$ENDIF}

//Use this definition ONLY for Win Vista Win7 and Win8, NOT for windows XP
{.$DEFINE USE_DIRECT3D_VERSION_9_VISTA}

{$IFDEF UNIX}
 {.$DEFINE ORCA_3D_USE_OPENGL}
 {$DEFINE ORCA_3D_USE_OPENGL_DLG}
 {.$DEFINE ORCA_3D_USE_MESA}
{$ENDIF}

{$IFDEF DARWIN}
 {.$DEFINE ORCA_3D_USE_OPENGL}
 {$DEFINE ORCA_3D_USE_OPENGL_DLG}
 {.$DEFINE ORCA_3D_USE_MESA}
{$ENDIF}

//===============================================

{$ALIGN ON}
{$BOOLEVAL OFF}
{$LONGSTRINGS ON}
{$WRITEABLECONST ON}
{$WARNINGS OFF}
{$HINTS OFF}


{$B-} {- Complete Boolean Evaluation }
{$R-} {- Range-Checking }
{$V-} {- Var-String Checking }
{$T-} {- Typed @ operator }
{$X+} {- Extended syntax }
{$J+} {- Enable writeable const}




