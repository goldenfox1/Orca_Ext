{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit layerfrm;

  {$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d, ComCtrls;

type
  TfrmLayered = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Image1: TD2Image;
    Rectangle1: TD2Rectangle;
    Text1: TD2Text;
    GlowEffect1: TD2GlowEffect;
    Text2: TD2Text;
    Ellipse1: TD2Ellipse;
    GlowEffect2: TD2GlowEffect;
    RoundRect1: TD2RoundRect;
    RoundRect2: TD2RoundRect;
    TrackBar1: TD2TrackBar;
    Selection1: TD2Selection;
    Rectangle2: TD2Rectangle;
    Layout1: TD2Layout;
    Text3: TD2Text;
    AniIndicator1: TD2AniIndicator;
    TextBox1: TD2TextBox;
    HudCloseButton1: TD2HudCloseButton;
    procedure TrackBar1Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmLayered: TfrmLayered;

implementation

{$R *.lfm}

procedure TfrmLayered.TrackBar1Change(Sender: TObject);
begin
  Image1.Opacity := TrackBar1.Value;
  Ellipse1.Opacity := TrackBar1.Value;
end;

end.
