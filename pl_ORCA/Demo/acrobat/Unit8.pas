{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit Unit8;

{$mode objfpc}{$H+}

interface

uses
  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TForm8 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Rectangle1: TD2Rectangle;
    d2Resources1: TD2Resources;
    redBtn: TD2BitmapButton;
    greenBtn: TD2BitmapButton;
    blueBtn: TD2BitmapButton;
    yellowBtn: TD2BitmapButton;
    greenPaddingOut: TD2RectAnimation;
    Container: TD2Layout;
    greenWidthOut: TD2FloatAnimation;
    containerOut: TD2FloatAnimation;
    greenWnd: TD2Rectangle;
    redPaddingOut: TD2RectAnimation;
    redWidthOut: TD2FloatAnimation;
    redWnd: TD2Rectangle;
    blueWnd: TD2Rectangle;
    yellowWnd: TD2Rectangle;
    bluePaddingOut: TD2RectAnimation;
    blueWidthOut: TD2FloatAnimation;
    yellowPaddingOut: TD2RectAnimation;
    yellowWidthOut: TD2FloatAnimation;
    procedure greenBtnClick(Sender: TObject);
    procedure greenPaddingOutFinish(Sender: TObject);
    procedure redBtnClick(Sender: TObject);
    procedure redPaddingOutFinish(Sender: TObject);
    procedure blueBtnClick(Sender: TObject);
    procedure bluePaddingOutFinish(Sender: TObject);
    procedure yellowBtnClick(Sender: TObject);
    procedure yellowPaddingOutFinish(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    procedure Reset;
  end;

var
  Form8: TForm8;

implementation

{$R *.lfm}

procedure TForm8.Reset;
begin
  greenWnd.Visible := false;
  greenBtn.Width := 110;
  greenBtn.Padding.Rect := d2Rect(0, 0, 0, 0);

  redWnd.Visible := false;
  redBtn.Width := 110;
  redBtn.Padding.Rect := d2Rect(0, 0, 0, 0);

  blueWnd.Visible := false;
  blueBtn.Width := 110;
  blueBtn.Padding.Rect := d2Rect(0, 0, 0, 0);

  yellowWnd.Visible := false;
  yellowBtn.Width := 110;
  yellowBtn.Padding.Rect := d2Rect(0, 0, 0, 0);

  Container.Width := 450;
end;

procedure TForm8.greenBtnClick(Sender: TObject);
begin
  Reset;

  greenPaddingOut.Start;
  greenWidthOut.Start;

  containerOut.Start;
end;

procedure TForm8.greenPaddingOutFinish(Sender: TObject);
begin
  greenWnd.Visible := true;
  greenWnd.Opacity := 0;
  greenWnd.AnimateFloat('Opacity', 1, 0.2);
end;

procedure TForm8.redBtnClick(Sender: TObject);
begin
  Reset;

  redPaddingOut.Start;
  redWidthOut.Start;

  containerOut.Start;
end;

procedure TForm8.redPaddingOutFinish(Sender: TObject);
begin
  redWnd.Visible := true;
  redWnd.Opacity := 0;
  redWnd.AnimateFloat('Opacity', 1, 0.2);
end;

procedure TForm8.blueBtnClick(Sender: TObject);
begin
  Reset;

  bluePaddingOut.Start;
  blueWidthOut.Start;

  containerOut.Start;
end;

procedure TForm8.bluePaddingOutFinish(Sender: TObject);
begin
  blueWnd.Visible := true;
  blueWnd.Opacity := 0;
  blueWnd.AnimateFloat('Opacity', 1, 0.2);
end;

procedure TForm8.yellowBtnClick(Sender: TObject);
begin
  Reset;

  yellowPaddingOut.Start;
  yellowWidthOut.Start;

  containerOut.Start;
end;

procedure TForm8.yellowPaddingOutFinish(Sender: TObject);
begin
  yellowWnd.Visible := true;
  yellowWnd.Opacity := 0;
  yellowWnd.AnimateFloat('Opacity', 1, 0.2);
end;

end.
