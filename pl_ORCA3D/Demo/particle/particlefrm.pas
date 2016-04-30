{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit particlefrm;

{$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene3d, orca_editor_particle,
  orca_scene2d;

type
  TfrmParticle = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Light1: TD3Light;
    Cylinder1: TD3Cylinder;
    Cylinder2: TD3Cylinder;
    ParticleEmitter1: TD3ParticleEmitter;
    FloatAnimation1: TD3FloatAnimation;
    Sphere1: TD3Sphere;
    ParticleEmitter2: TD3ParticleEmitter;
    DefaultPfx: TD3BitmapObject;
    GUIScene2DLayer1: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    HudButton1: TD2HudButton;
    procedure HudButton1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmParticle: TfrmParticle;

implementation

{$R *.lfm}

procedure TfrmParticle.HudButton1Click(Sender: TObject);
var
  frmParticleDesign: TD3ParticleDialog;
begin
  frmParticleDesign := TD3ParticleDialog.Create(Application);
  frmParticleDesign.Emitter := ParticleEmitter1;
  if frmParticleDesign.Execute(ParticleEmitter1.Scene) then
    ParticleEmitter1.Assign(frmParticleDesign.Emitter);
  frmParticleDesign.Free;
end;

end.
