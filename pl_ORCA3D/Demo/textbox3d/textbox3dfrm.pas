{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit textbox3dfrm;

{$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d, orca_scene2d;

type
  TfrmTextBox3D = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    TextBox3D1: TD3TextBox3D;
    Light1: TD3Light;
    GUIScene2DLayer1: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    CheckBox1: TD2CheckBox;
    TrackBar1: TD2TrackBar;
    Label1: TD2Label;
    procedure CheckBox1Change(Sender: TObject);
    procedure TrackBar1Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmTextBox3D: TfrmTextBox3D;

implementation

{$R *.lfm}

procedure TfrmTextBox3D.CheckBox1Change(Sender: TObject);
begin
  TextBox3D1.ShowBackground := CheckBox1.IsChecked;
end;

procedure TfrmTextBox3D.TrackBar1Change(Sender: TObject);
begin
  TextBox3D1.RotateAngle.Y := TrackBar1.Value;
end;

end.
