{***************************************************
 ORCA 2D Library
 Copyright (C) PilotLogic Software House
 http://www.pilotlogic.com
 
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio
***********************************************************************}


{$ALIGN ON}
{$BOOLEVAL OFF}
{$LONGSTRINGS ON}
{$WRITEABLECONST ON}
{$WARNINGS OFF}
{$HINTS OFF}


{$B-} // Complete Boolean Evaluation
{$R-} // Range-Checking
{$V-} // Var-String Checking
{$T-} // Typed @ operator
{$X+} // Extended syntax
{$J+} // Enable writeable const

{$define USE_DELPHICOMPAT} //использовать библиотеку DELPHICOMPAT из пакета lclextensions
{$if not defined(USE_DELPHICOMPAT) and not defined(LCLWin)}
  {$define INCOMPLETE_WINAPI}
{$endif}

//{$ifdef WINDOWS}
//  {$define EnableThreadSupport}
//{$endif}

{$define DEBUG_Orca}             //включить отладочную информацию
{$if defined(DEBUG_Orca)}
  {$define DEBUG_TreeGrid}   //отладка дерева
{$endif}



