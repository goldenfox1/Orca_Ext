{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit pathgenfrm;

  {$mode objfpc}{$H+}

interface

uses
 
  LResources,
  Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs, 
  orca_scene2d;

type
  TForm16 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    HudButton1: TD2HudButton;
    charMap: TD2HudListBox;
    PathData: TD2HudMemo;
    FontDialog1: TFontDialog;
    labelSample: TD2Label;
    samplePath: TD2Path;
    HudButton2: TD2HudButton;
    procedure HudButton1Click(Sender: TObject);
    procedure charMapChange(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure HudButton2Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    procedure BuildList;
  end;

var
  Form16: TForm16;

implementation

{$R *.lfm}

procedure TForm16.BuildList;
var
  c: integer;
  ch: WideString;
  item: TD2ListBoxItem;
  d2path: TD2Path;
  i: integer;
begin
  charMap.Clear;
  for c := 33 to 255 do
  begin
    item := TD2ListBoxItem.Create(Self);
    item.Parent := charMap;
    d2path := TD2Path.Create(Self);
    d2path.Parent := item;
    d2path.Align := vaClient;
    d2path.Padding.Rect := d2Rect(4,4,4,4);
    d2path.HitTest := false;
    d2path.Stroke.Style := d2BrushNone;
    d2path.WrapMode := d2PathFit;
    item.TagObject := d2path;
    ch := WideChar(c);

    d2Scene1.Canvas.Font.Family:= labelSample.Text;
    d2Scene1.Canvas.TextToPath(d2path.Data, d2Rect(0, 0, 64, 64), ch, false, d2TextAlignCenter, d2TextAlignCenter);
    d2path.Data.Scale(10, 10);
  end;
end;

procedure TForm16.HudButton1Click(Sender: TObject);
begin
  if FontDialog1.Execute then
  begin
    labelSample.Text := FontDialog1.Font.Name;
    BuildList;
  end;
end;

procedure TForm16.charMapChange(Sender: TObject);
begin
  if charMap.Selected <> nil then
  begin
    PathData.Lines.Text := TD2Path(charMap.Selected.TagObject).Data.Data;
    samplePath.Data.Assign(TD2Path(charMap.Selected.TagObject).Data);
  end;
end;

procedure TForm16.FormCreate(Sender: TObject);
begin
  BuildList;
end;

procedure TForm16.HudButton2Click(Sender: TObject);
begin
  PathData.SelectAll;
  PathData.CopyToClipboard;
end;

end.
