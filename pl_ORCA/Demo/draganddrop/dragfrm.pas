{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit dragfrm;

  {$mode objfpc}{$H+}

interface

uses
  SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, FileUtil, orca_scene2d;

type
  TForm9 = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Panel1: TD2Panel;
    Rectangle1: TD2Rectangle;
    Ellipse1: TD2Ellipse;
    FloatKeyAnimation1: TD2FloatKeyAnimation;
    Line1: TD2Line;
    Label1: TD2Label;
    Label2: TD2Label;
    Label3: TD2Label;
    Panel2: TD2Panel;
    Label4: TD2Label;
    Image1: TD2Image;
    ListBox1: TD2ListBox;
    ListBoxItem1: TD2ListBoxItem;
    Image2: TD2Image;
    ListBoxItem2: TD2ListBoxItem;
    ListBoxItem3: TD2ListBoxItem;
    Image3: TD2Image;
    Image4: TD2Image;
    procedure Rectangle1DragOver(Sender: TObject;
      const Data: TD2DragObject; const Point: TD2Point;
      var Accept: Boolean);
    procedure Rectangle1DragDrop(Sender: TObject;
      const Data: TD2DragObject; const Point: TD2Point);
    procedure Image1DragOver(Sender: TObject; const Data: TD2DragObject;
      const Point: TD2Point; var Accept: Boolean);
    procedure Image1DragDrop(Sender: TObject; const Data: TD2DragObject;
      const Point: TD2Point);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form9: TForm9;

implementation

{$R *.lfm}

procedure TForm9.Rectangle1DragOver(Sender: TObject;
  const Data: TD2DragObject; const Point: TD2Point; var Accept: Boolean);
begin
  Accept := true;
end;

procedure TForm9.Rectangle1DragDrop(Sender: TObject;
  const Data: TD2DragObject; const Point: TD2Point);
begin
  if (Data.Data = '') and (Data.Source <> nil) then
    Label1.Text := 'Dropped object - ' + Data.Source.ClassName
  else
    Label1.Text := 'Dropped object - ' + Data.Data;

  if Data.Source is TD2Image then
  begin
    Image1.Bitmap.Assign(TD2Image(Data.Source).Bitmap);
  end
  else
    if FileExistsUTF8(Data.Data) { *Converted from FileExists*  } and (Pos(ExtractFileExt(Data.Data), GVarD2DefaultFilterClass.GetFileTypes) > 0) then
      Image1.Bitmap.LoadThumbnailFromFile(Data.Data, 300, 300, false);
end;

procedure TForm9.Image1DragOver(Sender: TObject; const Data: TD2DragObject;
  const Point: TD2Point; var Accept: Boolean);
begin
  // accept correct image file or TD2Image 
  Accept :=
    ((Length(Data.Files) > 0) and FileExistsUTF8(Data.Files[0]) { *Converted from FileExists*  } and (Pos(ExtractFileExt(Data.Files[0]), GVarD2DefaultFilterClass.GetFileTypes) > 0))
    or
    (Data.Source is TD2Image);
end;

procedure TForm9.Image1DragDrop(Sender: TObject; const Data: TD2DragObject;
  const Point: TD2Point);
begin
  if Data.Source is TD2Image then
  begin
    Image1.Bitmap.Assign(TD2Image(Data.Source).Bitmap);
  end
  else
  if Length(Data.Files) > 0 then
  begin
    Image1.Bitmap.LoadThumbnailFromFile(Data.Files[0], 300, 300, false);
  end;
  Image1.Repaint;
end;

end.
