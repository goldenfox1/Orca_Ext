{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit benchmarkfrm;

{$mode objfpc}{$H+}

interface

uses
  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, MMSystem, orca_scene2d;

type
  TfrmBenchmark = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Text1: TD2Text;
    ShadowEffect1: TD2ShadowEffect;
    FloatAnimation1: TD2FloatAnimation;
    CheckBox1: TD2CheckBox;
    Timer1: TTimer;
    ApplicationEvents1: TApplicationProperties;
    Button1: TD2Button;
    Selection1: TD2Selection;
    Rectangle1: TD2Rectangle;
    FloatAnimation2: TD2FloatAnimation;
    StringListBox1: TD2StringListBox;
    Ellipse1: TD2Ellipse;
    GlowEffect1: TD2GlowEffect;
    Memo1: TD2Memo;
    Path1: TD2Path;
    GradientAnimation1: TD2GradientAnimation;
    procedure Root1Paint(Sender: TObject; const aCanvas: TD2Canvas;
      const ARect: TD2Rect);
    procedure CheckBox1Change(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure ApplicationEvents1Idle(Sender: TObject; var Done: Boolean);
  private
    { Private declarations }
    RenderCount: integer;
    Fps, RenderTime, StartTime, NewTime, Time: single;
  public
    { Public declarations }
  end;

var
  frmBenchmark: TfrmBenchmark;

implementation

{$R *.lfm}

type

  THackScene = class(TD2Scene);

procedure TfrmBenchmark.FormCreate(Sender: TObject);
begin
  StartTime := {$IFDEF WINDOWS}timeGetTime;{$else}GetTickCount / 1000; {$endif}
end;

procedure TfrmBenchmark.Root1Paint(Sender: TObject; const aCanvas: TD2Canvas; const ARect: TD2Rect);
begin
  RenderCount := RenderCount + 1;
end;

procedure TfrmBenchmark.CheckBox1Change(Sender: TObject);
begin
  d2Scene1.ShowUpdateRects := CheckBox1.IsChecked;
  if not d2Scene1.ShowUpdateRects then
    d2Scene1.AddUpdateRect(d2Rect(0, 0, 2000, 2000));
end;

procedure TfrmBenchmark.Timer1Timer(Sender: TObject);
begin
  Caption := 'orca 2D Benchmark ' + d2FloatToStr(Fps) + ' FPS';
end;

procedure TfrmBenchmark.ApplicationEvents1Idle(Sender: TObject;
  var Done: Boolean);
begin
  Button1.RotateAngle := Button1.RotateAngle + 0.01;

  THackScene(d2Scene1).Draw;

  NewTime := {$IFDEF WINDOWS}(timeGetTime - StartTime) / 1000;{$else}(GetTickCount / 1000) - StartTime; {$endif}
  RenderTime := RenderTime + (NewTime - Time);
  if RenderTime > 1 then
  begin
    Fps := RenderCount / RenderTime;
    RenderTime := 0;
    RenderCount := 0;
  end;
  Time := NewTime;

  Done := false;
end;

end.
