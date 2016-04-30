{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit creditsfrm;

  {$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, orca_scene2d;

type
  TForm13 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    AniLayout: TD2Layout;
    Text1: TD2Text;
    textList: TD2Text;
    CreditsTimer: TTimer;
    Slide: TD2FloatAnimation;
    LogoImage: TD2Image;
    Rectangle2: TD2Rectangle;
    Rectangle1: TD2Rectangle;
    Rectangle3: TD2Rectangle;
    Button1: TD2Button;
    d2Resources1: TD2Resources;
    Button2: TD2Button;
    FirstLayout: TD2Layout;
    Text2: TD2Text;
    Text3: TD2Text;
    GlowEffect1: TD2GlowEffect;
    procedure FormCreate(Sender: TObject);
    procedure CreditsTimerTimer(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form13: TForm13;

implementation

{$R *.lfm}

const
  Names: array [0..3] of string = (
   'Sternas S',
   'Anaconta',
   'Taro',
   'Elena');
   
procedure TForm13.FormCreate(Sender: TObject);
var
  i: integer;
begin
  { Add random Names }
  textList.Text := '';
  for i := 0 to 3 do
    textList.Text := textList.Text + Names[random(Length(Names))] + ' ' + Names[random(Length(Names))] + #10#13;
end;

procedure TForm13.CreditsTimerTimer(Sender: TObject);
begin
  // Start animation with some delay
  CreditsTimer.Enabled := false;
  // Start Animation
  Slide.StartFromCurrent := true;
  Slide.StopValue := -AniLayout.Height;
  Slide.Start;
end;

procedure TForm13.Button1Click(Sender: TObject);
begin
  // Scale Image
  LogoImage.AnimateFloat('Scale.X', 0.5, 0.2);
  LogoImage.AnimateFloat('Scale.Y', 0.5, 0.2);
  // Fade hide animation
  FirstLayout.AnimateFloatWait('Opacity', 0, 0.4);
  // Hide
  FirstLayout.Visible := false;
  // Show Layout
  AniLayout.Visible := true;
  AniLayout.Opacity := 0;
  // Fade Show
  AniLayout.AnimateFloatWait('Opacity', 1, 0.6);
  // Show Timer
  CreditsTimer.Enabled := true;
end;

procedure TForm13.Button2Click(Sender: TObject);
begin
  Application.Terminate;
end;

end.
