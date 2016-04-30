{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit effectsfrm;

  {$mode objfpc}{$H+}

interface

uses
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs,  StdCtrls, orca_scene2d;

type
  TfrmEffects = class(TForm)
    d2Scene1: TD2Scene;
    VectorImageRoot: TD2Background;
    StylesList: TD2ListBox;
    Text1: TD2Text;
    ListBoxItem1: TD2ListBoxItem;
    ListBoxItem2: TD2ListBoxItem;
    Rectangle1: TD2Rectangle;
    Layout1: TD2Layout;
    Text2: TD2Text;
    Track1: TD2TrackBar;
    StyleControls: TD2Layout;
    Text3: TD2Text;
    Text4: TD2Text;
    textOpacity: TD2Text;
    Track2: TD2TrackBar;
    ListBoxItem3: TD2ListBoxItem;
    Text6: TD2Text;
    Rectangle2: TD2Rectangle;
    Text7: TD2Text;
    Text8: TD2Text;
    Track3: TD2TrackBar;
    ShadowEffect1: TD2ShadowEffect;
    BlurEffect1: TD2BlurEffect;
    GlowEffect1: TD2GlowEffect;
    textDirection: TD2Text;
    trackDirection: TD2TrackBar;
    ColorAnimation1: TD2ColorAnimation;
    ColorAnimation2: TD2ColorAnimation;
    Rectangle3: TD2Rectangle;
    Text9: TD2Text;
    GlowEffect2: TD2GlowEffect;
    ListBoxItem4: TD2ListBoxItem;
    Text10: TD2Text;
    BevelEffect1: TD2BevelEffect;
    Image1: TD2Image;
    ReflectionEffect1: TD2ReflectionEffect;
    Text5: TD2Text;
    Text11: TD2Text;
    ReflectionEffect2: TD2ReflectionEffect;
    Text12: TD2Text;
    Text13: TD2Text;
    Text14: TD2Text;
    Text15: TD2Text;
    Path1: TD2Path;
    Text16: TD2Text;
    BevelEffect2: TD2BevelEffect;
    RoundRect1: TD2RoundRect;
    InnerGlowEffect1: TD2InnerGlowEffect;
    Text17: TD2Text;
    procedure StylesListChange(Sender: TObject);
    procedure Track1Change(Sender: TObject);
    procedure Track2Change(Sender: TObject);
    procedure Track3Change(Sender: TObject);
    procedure trackDirectionChange(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmEffects: TfrmEffects;

implementation

{$R *.lfm}

procedure TfrmEffects.StylesListChange(Sender: TObject);
begin
  ShadowEffect1.Enabled := false;
  GlowEffect1.Enabled := false;
  BlurEffect1.Enabled := false;
  BevelEffect1.Enabled := false;

  case StylesList.ItemIndex of
    0: ShadowEffect1.Enabled := true;
    1: GlowEffect1.Enabled := true;
    2: BlurEffect1.Enabled := true;
    3: BevelEffect1.Enabled := true;
  end;

  textDirection.Visible := ShadowEffect1.Enabled or BevelEffect1.Enabled;
  trackDirection.Visible := ShadowEffect1.Enabled or BevelEffect1.Enabled;
end;

procedure TfrmEffects.Track1Change(Sender: TObject);
begin
  StyleControls.Scale.Point := d2Point(Track1.Value, Track1.Value);
end;

procedure TfrmEffects.Track2Change(Sender: TObject);
begin
  StyleControls.Opacity := Track2.Value;
end;

procedure TfrmEffects.Track3Change(Sender: TObject);
begin
  case StylesList.ItemIndex of
    0: ShadowEffect1.Softness := Track3.Value;
    1: GlowEffect1.Softness := Track3.Value;
    2: BlurEffect1.Softness := Track3.Value;
    3: BevelEffect1.Size := Trunc(Track3.Value * 20);
  end;
  InnerGlowEffect1.Softness := Track3.Value;
end;

procedure TfrmEffects.trackDirectionChange(Sender: TObject);
begin
  ShadowEffect1.Direction := trackDirection.Value;
  BevelEffect1.Direction := trackDirection.Value;
  BevelEffect2.Direction := trackDirection.Value;
end;

end.
