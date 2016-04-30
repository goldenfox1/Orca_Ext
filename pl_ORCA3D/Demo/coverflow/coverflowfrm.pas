{**********************************************************************
 orca 3D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}
unit coverflowfrm;

{$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics,
  Controls, Forms, Dialogs, FileUtil,
  orca_scene3d, orca_scene2d;

type
  TForm12 = class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Camera1: TD3Camera;
    OpenDialog1: TOpenDialog;
    Screend2Layer1: TD3Screend2Layer;
    Root2: TD2Layout;
    Rectangle1: TD2Rectangle;
    HudButton1: TD2HudButton;
    CoverScroll: TD2HudScrollBar;
    FloatAnimation1: TD3FloatAnimation;
    coverFlow: TD3ScreenDummy;
    AniIndicator1: TD2AniIndicator;
    procedure HudButton1Click(Sender: TObject);
    procedure CoverScrollChange(Sender: TObject);
    procedure d3Scene1MouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
  private
    CoverIndex: integer;
    procedure AddFolder;
    procedure SetCoverIndex(AIndex: integer);
    procedure DoCoverMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form12: TForm12;

implementation

{$R *.lfm}

type

  TImageThread = class(TThread)
  private
    FImage: TD2Image;
    FTempBitmap: TD2Bitmap;
    FFileName: string;
  protected
    procedure Execute; override;
    procedure Finished;
  public
    constructor Create(const AImage: TD2Image; const AFileName: string);
    destructor Destroy; override;
  end;

{ Td2ImageThread }

constructor TImageThread.Create(const AImage: TD2Image; const AFileName: string);
begin
  inherited Create(true);
  FFileName := AFileName;
  FImage := AImage;
  Priority := tpIdle;
  FreeOnTerminate := true;
  Resume;
end;

destructor TImageThread.Destroy;
begin
  inherited;
end;

procedure TImageThread.Execute;
begin
  Sleep(random(300));
  FTempBitmap := TD2Bitmap.Create(0, 0);
  FTempBitmap.LoadThumbnailFromFile(FFileName, FImage.Width, FImage.Height, false);
  Synchronize(@Finished);
end;

procedure TImageThread.Finished;
begin
  FImage.Bitmap.Assign(FTempBitmap);
  FTempBitmap.Free;
end;

procedure TForm12.AddFolder;
var
  Dir, Ext: string;
  SR: TSearchRec;
  Cover: TD3d2Layer;
  i: integer;
  Root: TD2Layout;
  L: TD2Rectangle;
  Im: TD2Image;
  E: TD2ReflectionEffect;
begin
  OpenDialog1.Filter := 'All Images|' + GvarD3DefaultFilterClass.GetFileTypes;
  if OpenDialog1.Execute then
  begin
    coverFlow.DeleteChildren;
    AniIndicator1.Visible := true;
    Dir := ExtractfilePAth(OpenDialog1.FileName);
    i := 0;
    if FindFirstUTF8(Dir + '*.*',$FFFF,SR) { *Converted from FindFirst*  } = 0 then
    begin
      repeat
        if SR.Name = '.' then Continue;
        if SR.Name = '..' then Continue;
        if SR.Attr and faDirectory = faDirectory then Continue;

        Ext := LowerCase(ExtractFileExt(SR.Name));
        if Pos(Ext, GvarD3DefaultFilterClass.GetFileTypes) > 0 then
        begin
          // Create Cover
          Cover := TD3d2Layer.Create(Self);
          Cover.Parent := coverFlow;
          Cover.Width := Round(coverFlow.Height * 0.6);
          Cover.Height := Round(Round(coverFlow.Height * 0.6) * 1.5);
          Cover.ZWrite := true;
          Cover.Tag := i;
          Cover.OnLayerMouseDown := @DoCoverMouseDown;
          Cover.Position.y := Trunc((CoverFlow.Height - Round(coverFlow.Height * 0.6)) / 2);
          if i = 0 then
          begin
            Cover.Position.x := i * Round(coverFlow.Height * 0.6);
          end
          else
          begin
            Cover.Position.x := (i + 1) * (Round(coverFlow.Height * 0.6) div 3);
            Cover.Position.z := Round(coverFlow.Height * 0.6) * 2;
            Cover.RotateAngle.y := 70;
          end;
          // Child
          Root := TD2Layout.Create(Self);
          Cover.AddObject(Root);
          Cover.Tag := i;

          L := TD2Rectangle.Create(Self);
          L.Parent := Root;
          L.Align := vaTop;
          L.Height := Trunc(Cover.Height / 2);
          L.Fill.Style := d2BrushNone;
          L.Stroke.SolidColor := $FFE0E0E0;
          L.Stroke.Style := d2BrushNone;

          Im := TD2Image.Create(Self);
          Im.Parent := L;
          Im.Padding.Rect := d2Rect(2, 2, 2, 2);
          Im.TagString := Dir + SR.Name;
          with GvarD3DefaultFilterClass.GetImageSize(Im.TagString) do
          begin
            Im.Width := X;
            Im.Height := Y;
          end;
          Im.WrapMode := d2ImageStretch;
          Im.Align := vaFit;
          Im.HitTest := true;
          Im.TagString := Dir + SR.Name;
          with TImageThread.Create(Im, Im.TagString) do
          begin
            Resume;
          end;
          Im.Tag := i;

          E := TD2ReflectionEffect.Create(Self);
          E.Parent := Im;

          // Opacity animation
          Cover.Opacity := 0.01;
          Cover.AnimateFloat('Opacity', 1, 0.5);

          // Load thumb
          Cover.TagObject := Im;

          i := i + 1;

          Application.ProcessMessages;
        end;
      until FindNextUTF8(SR) { *Converted from FindNext*  } <> 0;
      FindCloseUTF8(SR); { *Converted from FindClose*  }
    end;
    CoverIndex := 0;
    CoverScroll.Max := coverFlow.ChildrenCount - 1 + CoverScroll.ViewportSize;
    CoverScroll.Value := 0;
    AniIndicator1.Visible := false;
  end;
end;

procedure TForm12.SetCoverIndex(AIndex: integer);
const
  Duration = 0.5;
var
  i: integer;
  Cover: TD3VisualObject;
  Coeff: single;
begin
  if AniIndicator1.Visible then Exit;
  if AIndex < 0 then AIndex := 0;
  if AIndex >= coverFlow.ChildrenCount then AIndex := coverFlow.ChildrenCount - 1;
  if AIndex <> CoverIndex then
  begin
    { translate all }
    for i := 0 to coverFlow.ChildrenCount - 1 do
    begin
      Cover := TD3VisualObject(coverFlow.Children[i]);
      Cover.AnimateStop('Position.x');
      Cover.AnimateFloat('Position.x', Cover.Position.X + ((CoverIndex - AIndex) * (Round(coverFlow.Height * 0.6) div 3)), 0.2);
    end;
    { transform between old an new value }
    i := CoverIndex;
    while i <> AIndex do
    begin
      Coeff := (0.1 + (Abs(AIndex - i) / Abs(AIndex - CoverIndex))) * 0.7;
      Cover := TD3VisualObject(coverFlow.Children[i]);
      Cover.AnimateStop('Position.x');
      Cover.AnimateStop('RotateAngle.y');
      if CoverIndex > AIndex then
      begin
        Cover.AnimateFloat('RotateAngle.y', 70, Duration * Coeff);
        if i = CoverIndex then
          Cover.AnimateFloat('Position.x', Cover.Position.X + (1 * (Round(coverFlow.Height * 0.6) div 3)), Duration * Coeff)
        else
          Cover.AnimateFloat('Position.x', Cover.Position.X + (2 * (Round(coverFlow.Height * 0.6) div 3)), Duration * Coeff);
      end
      else
      begin
        Cover.AnimateFloat('RotateAngle.y', -70, Duration * Coeff);
        if i = CoverIndex then
          Cover.AnimateFloat('Position.x', Cover.Position.X - (1 * (Round(coverFlow.Height * 0.6) div 3)), Duration * Coeff)
        else
          Cover.AnimateFloat('Position.x', Cover.Position.X - (2 * (Round(coverFlow.Height * 0.6) div 3)), Duration * Coeff);
      end;
      Cover.AnimateFloat('Position.z', Round(coverFlow.Height * 0.6) * 2, Duration * Coeff);
      if AIndex > CoverIndex then
        Inc(i)
      else
        Dec(i);
    end;

    Cover := TD3VisualObject(coverFlow.Children[AIndex]);

    Cover.AnimateStop('Position.x');
    Cover.AnimateStop('Position.z');

    Cover.AnimateFloat('RotateAngle.y', 0, Duration);
    Cover.AnimateFloat('Position.z', 0, Duration);
    if CoverIndex > AIndex then
    begin
      Cover.AnimateFloat('Position.x', Cover.Position.X + (1 * (Round(coverFlow.Height * 0.6) div 3)), Duration);
    end
    else
    begin
      Cover.AnimateFloat('Position.x', Cover.Position.X - (1 * (Round(coverFlow.Height * 0.6) div 3)), Duration);
    end;

    CoverIndex := AIndex;
  end;
end;

procedure TForm12.HudButton1Click(Sender: TObject);
begin
  AddFolder;
end;

procedure TForm12.CoverScrollChange(Sender: TObject);
begin
  SetCoverIndex(Round(CoverScroll.Value));
end;

procedure TForm12.DoCoverMouseDown(Sender: TObject; Button: TMouseButton;
    Shift: TShiftState; X, Y: Integer);
begin
  SetCoverIndex(TD3Object(Sender).Tag);
end;

procedure TForm12.d3Scene1MouseWheel(Sender: TObject; Shift: TShiftState;
  WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
begin
  CoverScroll.Value := CoverIndex - (WheelDelta div 120);
  Handled := true;
end;

end.
