{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit imagefxfrm;

{$mode objfpc}{$H+}

interface

uses
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, FileUtil,
  orca_scene2d, orca_scene3d;

type
  TForm11 = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Camera1: TD3Camera;
    PreviewLayer: TD3ObjectLayer;
    Root4: TD2Layout;
    PreviewLayout: TD2Layout;
    Preview: TD2Rectangle;
    ReflectionEffect1: TD2ReflectionEffect;
    dragLayout: TD2Rectangle;
    Rectangle1: TD2Rectangle;
    Ellipse1: TD2Ellipse;
    Line1: TD2Line;
    FloatKeyAnimation1: TD2FloatKeyAnimation;
    Text1: TD2Text;
    Text3D1: TD3Text3D;
    Light1: TD3Light;
    d3BitmapList1: TD3BitmapList;
    Dummy1: TD3Dummy;
    Dummy2: TD3Dummy;
    procedure dragLayoutDragOver(Sender: TObject;
      const Data: TD2DragObject; const Point: TD2Point;
      var Accept: Boolean);
    procedure dragLayoutDragDrop(Sender: TObject;
      const Data: TD2DragObject; const Point: TD2Point);
    procedure Text3D1DragOver(Sender: TObject; const Data: TD3DragObject;
      const Point: TD3Point; var Accept: Boolean);
    procedure Text3D1DragDrop(Sender: TObject; const Data: TD3DragObject;
      const Point: TD3Point);
    procedure d3Scene1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure d3Scene1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PreviewDragOver(Sender: TObject; const Data: TD2DragObject;
      const Point: TD2Point; var Accept: Boolean);
  private
    { Private declarations }
    Down: TD3Point;
  public
    { Public declarations }
  end;

var
  Form11: TForm11;

implementation

{$R *.lfm}


procedure TForm11.dragLayoutDragOver(Sender: TObject;
  const Data: TD2DragObject; const Point: TD2Point; var Accept: Boolean);
begin
  Accept := true;
end;

procedure TForm11.dragLayoutDragDrop(Sender: TObject;
  const Data: TD2DragObject; const Point: TD2Point);
begin
  // 
  if (Length(Data.Files) > 0) and FileExistsUTF8(Data.Files[0]) { *Converted from FileExists*  } and (Pos(ExtractFileExt(Data.Files[0]), GVarD3DefaultFilterClass.GetFileTypes) > 0) then
  begin
    Preview.Fill.Bitmap.Bitmap.LoadFromFile(Data.Files[0]);
    Preview.UpdateEffects;
    Preview.Repaint;
    PreviewLayout.Repaint;
  end;
end;

procedure TForm11.Text3D1DragOver(Sender: TObject;
  const Data: TD3DragObject; const Point: TD3Point; var Accept: Boolean);
begin
  Accept := (Length(Data.Files) > 0) and FileExistsUTF8(Data.Files[0]) { *Converted from FileExists*  } and (Pos(ExtractFileExt(Data.Files[0]), GVarD3DefaultFilterClass.GetFileTypes) > 0);
end;

procedure TForm11.Text3D1DragDrop(Sender: TObject; const Data: TD3DragObject; const Point: TD3Point);
begin
  TD3BitmapStream(d3BitmapList1.Bitmaps.Items[0]).Bitmap.LoadFromFile(Data.Files[0]);
end;

procedure TForm11.d3Scene1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  if ssLeft in Shift then
  begin
    Dummy1.RotateAngle.Z := Dummy1.RotateAngle.Z + (X - Down.X);
    Dummy2.RotateAngle.X := Dummy2.RotateAngle.X + (Y - Down.Y);
    Down := d3Point(X, Y, 0);
  end;
end;

procedure TForm11.d3Scene1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  Down := d3Point(X, Y, 0);
end;

procedure TForm11.PreviewDragOver(Sender: TObject;
  const Data: TD2DragObject; const Point: TD2Point; var Accept: Boolean);
begin
  Accept := true;
end;

end.
