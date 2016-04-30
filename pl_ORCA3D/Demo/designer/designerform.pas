{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}
unit designerform;

{$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d, orca_scene2d, StdCtrls;

type
  TForm8 = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Camera1: TD3Camera;
    Light1: TD3Light;
    RoundCube1: TD3RoundCube;
    Plane1: TD3Plane;
    Sphere1: TD3Sphere;
    Text3D1: TD3Text3D;
    d2Layer1: TD3ObjectLayer;
    Root2: TD2Layout;
    Rectangle1: TD2Rectangle;
    Rectangle2: TD2Rectangle;
    Button1: TD2Button;
    TextBox1: TD2TextBox;
    CompoundTrackBar1: TD2CompoundTrackBar;
    Button2: TButton;
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form8: TForm8;

implementation

{$R *.lfm}

procedure TForm8.FormCreate(Sender: TObject);
begin
  // Set Design-time mode
  d3Scene1.DesignTime := true;
  d2Layer1.DesignTime := true;
end;

end.
