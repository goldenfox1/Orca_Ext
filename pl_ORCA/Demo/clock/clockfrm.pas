{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit clockfrm;

{$mode objfpc}{$H+}

interface

uses  
  LResources, 
  {$IFDEF DARWIN}
  MacOSAll, CarbonPrivate,
  {$ENDIF}
  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs,
  Math, ExtCtrls, orca_scene2d;

type
  TForm7 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Layout;
    clockFace: TD2Ellipse;
    Ellipse2: TD2Ellipse;
    Ellipse3: TD2Ellipse;
    Rectangle1: TD2Rectangle;
    dayText: TD2Text;
    clockShadow: TD2ShadowEffect;
    Ellipse4: TD2Ellipse;
    secondArrow: TD2Path;
    FloatAnimation1: TD2FloatAnimation;
    FloatAnimation2: TD2FloatAnimation;
    Path2: TD2Path;
    btnClose: TD2Ellipse;
    Layout1: TD2Layout;
    clockLayout: TD2Layout;
    Timer1: TTimer;
    minutArrow: TD2Rectangle;
    hourArrow: TD2Rectangle;
    Ellipse1: TD2Ellipse;
    Ellipse5: TD2Ellipse;
    Ellipse6: TD2Ellipse;
    Ellipse7: TD2Ellipse;
    Ellipse8: TD2Ellipse;
    Ellipse9: TD2Ellipse;
    Text1: TD2Text;
    Text2: TD2Text;
    Text3: TD2Text;
    Text4: TD2Text;
    btnRotate: TD2Ellipse;
    btnScale: TD2Ellipse;
    procedure btnCloseMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single);
    procedure btnRotateMouseMove(Sender: TObject; Shift: TShiftState; X, Y,
      Dx, Dy: Single);
    procedure Timer1Timer(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure btnScaleMouseMove(Sender: TObject; Shift: TShiftState; X, Y,
      Dx, Dy: Single);
    procedure btnScaleMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single);
    procedure btnRotateMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form7: TForm7;

implementation

{$R *.lfm}

procedure TForm7.btnCloseMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Single);
begin
  Close;
end;

procedure TForm7.btnRotateMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Single);
begin
  btnRotate.AutoCapture := true;
end;

procedure TForm7.btnRotateMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y, Dx, Dy: Single);
var
  P: TD2Point;
  V, V1: TD2Vector;
begin
  if ssLeft in Shift then
  begin
    P := btnRotate.LocalToAbsolute(d2Point(X, Y));
    V := d2Vector(10 - (ClientWidth / 2), 10 - (ClientHeight / 2));
    V1 := d2Vector(P.X - (ClientWidth / 2), P.Y - (ClientHeight / 2));
    if d2VectorCrossProductZ(V1, V) > 0 then
      clockLayout.RotateAngle := -d2RadToDeg(ArcCos(d2VectorAngleCosine(V1, V)))
    else
      clockLayout.RotateAngle := d2RadToDeg(ArcCos(d2VectorAngleCosine(V1, V)));
    d2Scene1.UpdateBuffer;
  end;
end;

procedure TForm7.Timer1Timer(Sender: TObject);
var
  H, M, S, MS: word;
begin
  { set times }
  DecodeTime(Now, H, M, S, MS);
  secondArrow.RotateAngle := 360 * (S / 60) + (6 * (MS / 1000));
  minutArrow.RotateAngle := 360 * (M / 60);
  hourArrow.RotateAngle := 360 * ((H mod 12) / 12);
  DecodeDate(Now, H, M, S);
  dayText.Text := IntToStr(S); 
end;

procedure TForm7.FormCreate(Sender: TObject);
begin
  {$IFDEF DARWIN}
  // disable Mac OS X shadow
  ChangeWindowAttributes(TCarbonWindow(Handle).Window, kWindowNoShadowAttribute, kWindowNoAttributes);
  {$ENDIF}
  Timer1Timer(Self);
end;

procedure TForm7.btnScaleMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Single);
begin
  btnScale.AutoCapture := true;
end;

procedure TForm7.btnScaleMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y, Dx, Dy: Single);
var
  P: TD2Point;
  scale: single;
begin
  if ssLeft in Shift then
  begin
    P := btnScale.LocalToAbsolute(d2Point(X, Y));
    scale := (P.Y) / 175;
    if scale < 0.5 then scale := 0.5;
    if scale > 1.5 then scale := 1.5;
    clockLayout.Scale.Point := d2Point(scale, scale);
    d2Scene1.UpdateBuffer;
  end;
end;

end.
