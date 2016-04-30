{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit huddemofrm;

{$mode objfpc}{$H+}

interface

uses
  LResources,  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d, orca_scene3d;

type
  TfrnHudDemo = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    GUIScene2DLayer1: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    HudWindow1: TD2HudWindow;
    HudLabel1: TD2HudLabel;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frnHudDemo: TfrnHudDemo;

implementation

{$R *.lfm}

end.
