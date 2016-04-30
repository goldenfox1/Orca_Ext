{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit fillandstroke;

  {$mode objfpc}{$H+}

interface

uses
  Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TForm11 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Ellipse1: TD2Ellipse;
    Rectangle1: TD2Rectangle;
    Path1: TD2Path;
    TrackBar1: TD2TrackBar;
    Label1: TD2Label;
    PopupBox1: TD2PopupBox;
    PopupBox2: TD2PopupBox;
    Label2: TD2Label;
    Label3: TD2Label;
    PopupBox3: TD2PopupBox;
    Label4: TD2Label;
    Text1: TD2Text;
    PopupBox4: TD2PopupBox;
    Label5: TD2Label;
    ModernStyle: TD2Resources;
    VistaStyle: TD2Resources;
    Label6: TD2Label;
    StyleBox: TD2PopupBox;
    procedure TrackBar1Change(Sender: TObject);
    procedure PopupBox1Change(Sender: TObject);
    procedure PopupBox2Change(Sender: TObject);
    procedure PopupBox3Change(Sender: TObject);
    procedure Rectangle1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Single);
    procedure PopupBox4Change(Sender: TObject);
    procedure StyleBoxChange(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form11: TForm11;

implementation

{$R *.lfm}

procedure TForm11.TrackBar1Change(Sender: TObject);
begin
  Ellipse1.StrokeThickness := TrackBar1.Value;
  Rectangle1.StrokeThickness := TrackBar1.Value;
  Path1.StrokeThickness := TrackBar1.Value;
end;

procedure TForm11.PopupBox1Change(Sender: TObject);
begin
  Ellipse1.StrokeCap := TD2StrokeCap(PopupBox1.ItemIndex);
  Rectangle1.StrokeCap := TD2StrokeCap(PopupBox1.ItemIndex);
  Path1.StrokeCap := TD2StrokeCap(PopupBox1.ItemIndex);
end;

procedure TForm11.PopupBox2Change(Sender: TObject);
begin
  Ellipse1.StrokeDash := TD2StrokeDash(PopupBox2.ItemIndex);
  Rectangle1.StrokeDash := TD2StrokeDash(PopupBox2.ItemIndex);
  Path1.StrokeDash := TD2StrokeDash(PopupBox2.ItemIndex);
end;

procedure TForm11.PopupBox3Change(Sender: TObject);
begin
  Ellipse1.StrokeJoin := TD2StrokeJoin(PopupBox3.ItemIndex);
  Rectangle1.StrokeJoin := TD2StrokeJoin(PopupBox3.ItemIndex);
  Path1.StrokeJoin := TD2StrokeJoin(PopupBox3.ItemIndex);
end;

procedure TForm11.Rectangle1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Single);
begin
  { }
  SelectInDesign(Rectangle1.Fill, Rectangle1);
end;

procedure TForm11.PopupBox4Change(Sender: TObject);
begin
  Root1.Scale.x := StrToInt(Copy(PopupBox4.Text, 1, Length(PopupBox4.Text) - 1)) / 100;
  Root1.Scale.y := StrToInt(Copy(PopupBox4.Text, 1, Length(PopupBox4.Text) - 1)) / 100;
  d2Scene1.RealignRoot;
end;

procedure TForm11.StyleBoxChange(Sender: TObject);
begin
  case StyleBox.ItemIndex of
    0: d2Scene1.Style := nil;
    1: d2Scene1.Style := ModernStyle;
    2: d2Scene1.Style := VistaStyle;
  end;
end;

end.
