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

{$define USE_DELPHICOMPAT} //использовать библиотеку DELPHICOMPAT из пакета lclextensions	//Added by GoldenFox
{$if not defined(USE_DELPHICOMPAT) and not defined(LCLWin)}	//Added by GoldenFox
  {$define INCOMPLETE_WINAPI}	//Added by GoldenFox
{$endif}	//Added by GoldenFox

//{$ifdef WINDOWS}	//Deleted by GoldenFox
//  {$define EnableThreadSupport}	//Deleted by GoldenFox
//{$endif}	//Deleted by GoldenFox

{$define DEBUG_Orca}             //включить отладочную информацию	//Added by GoldenFox
{$if defined(DEBUG_Orca)}	//Added by GoldenFox
  {$define DEBUG_TreeGrid}   //отладка дерева	//Added by GoldenFox
{$endif}	//Added by GoldenFox
