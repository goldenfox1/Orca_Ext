{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit Unit1;

  {$mode objfpc}{$H+}

interface

uses
  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d,
  ExtCtrls;

type
  TForm1 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    ScrollBox1: TD2ScrollBox;
    SizeGrip1: TD2SizeGrip;
    Selection1: TD2Selection;
    Rectangle1: TD2Rectangle;
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
    FileList: TStrings;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

type
  TD2HackScrollBox = class(TD2Scrollbox);

procedure TForm1.FormCreate(Sender: TObject);
var
  i, j: integer;
  R: TD2Rectangle;
begin
  ScrollBox1.BeginUpdate;
  for i := 0 to 99 do
    for j := 0 to 99 do
    begin
      R := TD2Rectangle.Create(nil);
      with R do
      begin
        parent := ScrollBox1;
        setBounds(i * 40 + 2, j * 40 + 2, 36, 36);
        fill.SolidColor := ((50 + random(205)) shl 24) or random($FFFFFF);
        stroke.style := d2Brushnone;
        hitTest := false;
      end;
    end;
  ScrollBox1.EndUpdate;
end;

end.
