{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit mainfrm;

{$mode objfpc}{$H+}

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d, orca_scene2d;

type
  TfrmMain = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    GUIText1: TD3GUIText;
    ObjectLayer1: TD3ObjectLayer;
    Root2: TD2Layout;
    Frame1: TD2Frame;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

uses bindingfrm;

{$R *.lfm}

end.
