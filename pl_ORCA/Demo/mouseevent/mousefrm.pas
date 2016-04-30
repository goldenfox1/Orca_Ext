{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit mousefrm;

  {$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TForm10 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Image1: TD2Image;
    Label1: TD2Label;
    Layout1: TD2Layout;
    Text1: TD2Text;
    ShadowEffect1: TD2ShadowEffect;
    Ellipse1: TD2Ellipse;
    Text2: TD2Text;
    procedure FormCreate(Sender: TObject);
    procedure Layout1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single);
    procedure Layout1MouseMove(Sender: TObject; Shift: TShiftState; X, Y,
      Dx, Dy: Single);
    procedure Layout1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single);
    procedure Ellipse1MouseEnter(Sender: TObject);
    procedure Ellipse1MouseLeave(Sender: TObject);
  private
    { Private declarations }
    Down: boolean;
    DownPos: TD2Point;
  public
    { Public declarations }
  end;

var
  Form10: TForm10;

implementation

{$R *.lfm}

procedure TForm10.FormCreate(Sender: TObject);
begin
  Layout1.AutoCapture := true; // Set AutoCapture to true - for get mouse events whene mouse not over control
end;

procedure TForm10.Layout1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Single);
begin
  if Button = mbLeft then
  begin
    Text2.Text := 'Mouse Down';
    DownPos := d2Point(X, Y);
    Down := true;
  end;
end;

procedure TForm10.Layout1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y, Dx, Dy: Single);
begin
  if Down then
  begin

    TD2VisualObject(Sender).Position.X := TD2VisualObject(Sender).Position.X + (X - DownPos.X);
    TD2VisualObject(Sender).Position.Y := TD2VisualObject(Sender).Position.Y + (Y - DownPos.Y);
  end;
end;

procedure TForm10.Layout1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Single);
begin
  Text2.Text := 'Mouse Up';
  Down := false;
end;

procedure TForm10.Ellipse1MouseEnter(Sender: TObject);
begin
  Text2.Text := 'Mouse Enter';
end;

procedure TForm10.Ellipse1MouseLeave(Sender: TObject);
begin
  Text2.Text := 'Mouse Leave';
end;

end.
