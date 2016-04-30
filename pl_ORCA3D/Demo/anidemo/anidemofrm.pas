{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit anidemofrm;

{$mode objfpc}{$H+}

interface

uses
  SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  orca_scene2d, orca_scene3d;

type
  TfrmAniDemo = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Sphere1: TD3Sphere;
    Light1: TD3Light;
    Text1: TD3Text;
    ColorAnimation1: TD3ColorAnimation;
    ColorAnimation2: TD3ColorAnimation;
    Sphere2: TD3Sphere;
    Text2: TD3Text;
    FloatAnimation1: TD3FloatAnimation;
    StrokeCube1: TD3StrokeCube;
    Text3: TD3Text;
    FloatAnimation2: TD3FloatAnimation;
    Cube1: TD3Cube;
    Plane1: TD3Plane;
    RoundCube1: TD3RoundCube;
    FloatAnimation3: TD3FloatAnimation;
    Text4: TD3Text;
    Text3D1: TD3Text3D;
    FloatAnimation4: TD3FloatAnimation;
    GUIText1: TD3GUIText;
    ObjectLayer1: TD3ObjectLayer;
    Root2: TD2Layout;
    RoundRect1: TD2RoundRect;
    PathAni: TD2PathAnimation;
    Path1: TD2Path;
    Background1: TD2Background;
    Text5: TD2Text;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmAniDemo: TfrmAniDemo;

implementation

{$R *.lfm}

end.
