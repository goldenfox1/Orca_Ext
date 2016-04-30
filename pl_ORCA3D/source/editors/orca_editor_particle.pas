{***************************************************
 ORCA 3D Library
 Copyright (C) PilotLogic Software House
 http://www.pilotlogic.com
 
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio
***********************************************************************}
   
unit orca_editor_particle;

interface

uses

  LCLProc, LCLType, LMessages, LResources,
  SysUtils, Classes, Graphics, Controls, Forms,
  ComCtrls, StdCtrls, Buttons, Dialogs, ExtCtrls,
  orca_scene2d, orca_scene3d;

type
  TfrmParticleDesign=class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    PropertiesLeft: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    popupBitmap: TD2CompoundPopupBox;
    popupBlendMode: TD2CompoundPopupBox;
    GroupBox1: TD2GroupBox;
    GroupBox2: TD2GroupBox;
    Label1: TD2Label;
    numR: TD2NumberBox;
    numG: TD2NumberBox;
    Label2: TD2Label;
    textRGBA: TD2TextBox;
    numB: TD2NumberBox;
    numA: TD2NumberBox;
    Background1: TD2Background;
    GUIImage1: TD3GUIImage;
    Emitter: TD3ParticleEmitter;
    Camera1: TD3Camera;
    trackParticleCount: TD2CompoundTrackBar;
    imageTexture: TD2Image;
    resizeLayout: TD2Layout;
    rectSelection: TD2Selection;
    trackLifetime: TD2CompoundTrackBar;
    PropertiesRight: TD3GUIScene2DLayer;
    Root4: TD2Layout;
    Background2: TD2Background;
    modalLayout: TD2Layout;
    partGradBack: TD2Rectangle;
    partGrad: TD2Rectangle;
    ColorQuad1: TD2ColorQuad;
    ColorQuad2: TD2ColorQuad;
    ColorPicker1: TD2ColorPicker;
    ColorPicker2: TD2ColorPicker;
    ColorBox1: TD2ColorBox;
    ColorBox2: TD2ColorBox;
    Layout1: TD2Layout;
    Layout2: TD2Layout;
    numR2: TD2NumberBox;
    numG2: TD2NumberBox;
    numB2: TD2NumberBox;
    numA2: TD2NumberBox;
    textRGBA2: TD2TextBox;
    trackSpinMin: TD2CompoundTrackBar;
    trackSpinMax: TD2CompoundTrackBar;
    trackScaleMax: TD2CompoundTrackBar;
    btnAddBitmap: TD2Button;
    trackScaleMin: TD2CompoundTrackBar;
    trackGravityZ: TD2CompoundTrackBar;
    trackRadialMin: TD2CompoundTrackBar;
    trackRadialMax: TD2CompoundTrackBar;
    trackTangentMin: TD2CompoundTrackBar;
    trackVelocityMax: TD2CompoundTrackBar;
    trackTangentMax: TD2CompoundTrackBar;
    trackSpread: TD2CompoundTrackBar;
    trackDirectionAngle: TD2CompoundTrackBar;
    Plane1: TD3Plane;
    trackGravityX: TD2CompoundTrackBar;
    trackPositionDispersionX: TD2CompoundTrackBar;
    trackGravityY: TD2CompoundTrackBar;
    trackPositionDispersionY: TD2CompoundTrackBar;
    trackPositionDispersionZ: TD2CompoundTrackBar;
    trackVelocityMin: TD2CompoundTrackBar;
    trackFriction: TD2CompoundTrackBar;
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure cbBitmapListChange(Sender: TObject);
    procedure rgFillModeClick(Sender: TObject);
    procedure rgShadeModeClick(Sender: TObject);
    procedure tbBitmaptileXChange(Sender: TObject);
    procedure tbBitmaptileYChange(Sender: TObject);
    procedure checkLightChange(Sender: TObject);
    procedure ColorQuad1Change(Sender: TObject);
    procedure numRChange(Sender: TObject);
    procedure textRGBAChange(Sender: TObject);
    procedure trackScaleMinChange(Sender: TObject);
    procedure trackSpinMinChange(Sender: TObject);
    procedure trackParticleCountChange(Sender: TObject);
    procedure trackLifetimeChange(Sender: TObject);
    procedure rectSelectionChange(Sender: TObject);
    procedure trackVelocityMinChange(Sender: TObject);
    procedure trackVelocityMaxChange(Sender: TObject);
    procedure numR2Change(Sender: TObject);
    procedure textRGBA2Change(Sender: TObject);
    procedure ColorQuad2Change(Sender: TObject);
    procedure trackSpinMaxChange(Sender: TObject);
    procedure trackScaleMaxChange(Sender: TObject);
    procedure trackGravityXChange(Sender: TObject);
    procedure trackGravityYChange(Sender: TObject);
    procedure trackGravityZChange(Sender: TObject);
    procedure trackRadialMinChange(Sender: TObject);
    procedure trackRadialMaxChange(Sender: TObject);
    procedure btnAddBitmapClick(Sender: TObject);
    procedure trackTangentMinChange(Sender: TObject);
    procedure trackTangentMaxChange(Sender: TObject);
    procedure trackSpreadChange(Sender: TObject);
    procedure trackDirectionAngleChange(Sender: TObject);
    procedure Plane1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y:single; rayPos, rayDir: TD3Vector);
    procedure Plane1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y:single; rayPos, rayDir: TD3Vector);
    procedure Plane1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y:single; rayPos, rayDir: TD3Vector);
    procedure trackPositionDispersionXChange(Sender: TObject);
    procedure trackPositionDispersionYChange(Sender: TObject);
    procedure trackPositionDispersionZChange(Sender: TObject);
    procedure trackFrictionChange(Sender: TObject);
  private
    FParentScene: TD3Scene;
    FMousePressed: boolean;
    { Private declarations }
    procedure RebuildBitmapList;
    procedure SetParentScene(const Value: TD3Scene);
  public
    { Public declarations }
    procedure AssignFromEmitter(const AEmitter: TD3ParticleEmitter);
    procedure AssignToEmitter(var AEmitter: TD3ParticleEmitter);
    property ParentScene: TD3Scene read FParentScene write SetParentScene;
  end;

  TD3ParticleDialog=class(TComponent)
  private
    FEmitter: TD3ParticleEmitter;
    procedure SetEmitter(const Value: TD3ParticleEmitter);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function Execute(AScene: TD3Scene): boolean;
    property Emitter: TD3ParticleEmitter read FEmitter write SetEmitter;
  published
  end;

var
  frmParticleDesign: TfrmParticleDesign;

implementation


{$R *.lfm}


{ TD3ParticleDialog }

constructor TD3ParticleDialog.Create(AOwner: TComponent);
begin
  inherited;
  FEmitter:=TD3ParticleEmitter.Create(Self);
end;

destructor TD3ParticleDialog.Destroy;
begin
  FEmitter.Free;
  inherited;
end;

function TD3ParticleDialog.Execute(AScene: TD3Scene): boolean;
var
  frmParticleDesign: TfrmParticleDesign;
begin
  frmParticleDesign:=TfrmParticleDesign.Create(Self);
  frmParticleDesign.ParentScene:=AScene;
  frmParticleDesign.modalLayout.Visible:=true;
  frmParticleDesign.AssignFromEmitter(FEmitter);
  Result:=frmParticleDesign.ShowModal=mrOk;
  frmParticleDesign.AssignToEmitter(FEmitter);
  frmParticleDesign.Free;
end;

procedure TD3ParticleDialog.SetEmitter(const Value: TD3ParticleEmitter);
begin
  FEmitter.Assign(Value);
end;

{ TfrmParticleDesign }

procedure TfrmParticleDesign.FormCreate(Sender: TObject);
begin
  RebuildBitmapList;
end;

procedure TfrmParticleDesign.AssignFromEmitter(  const AEmitter: TD3ParticleEmitter);
begin
  popupBitmap.Value:=popupBitmap.PopupBox.Items.IndexOf(AEmitter.Bitmap);
  cbBitmapListChange(Self);
  popupBlendMode.Value:=integer(AEmitter.BlendingMode);
  trackGravityX.Value:=AEmitter.Gravity.X;
  trackGravityY.Value:=AEmitter.Gravity.Y;
  trackGravityZ.Value:=AEmitter.Gravity.Z;
  trackParticleCount.Value:=AEmitter.ParticlePerSecond;
  trackSpread.Value:=AEmitter.SpreadAngle;
  trackDirectionAngle.Value:=AEmitter.DirectionAngle;
  trackLifetime.Value:=AEmitter.LifeTime;

  trackFriction.Value:=AEmitter.Friction;

  trackRadialMin.Value:=AEmitter.CentrifugalVelMin;
  trackRadialMax.Value:=AEmitter.CentrifugalVelMax;

  trackVelocityMin.Value:=AEmitter.VelocityMin;
  trackVelocityMax.Value:=AEmitter.VelocityMax;

  trackTangentMin.Value:=AEmitter.TangentVelMin;
  trackTangentMax.Value:=AEmitter.TangentVelMax;

  trackSpinMin.Value:=AEmitter.SpinBegin;
  trackSpinMax.Value:=AEmitter.SpinEnd;

  trackScaleMin.Value:=AEmitter.ScaleBegin;
  trackScaleMax.Value:=AEmitter.ScaleEnd;

  ColorPicker1.Color:=d3StrToColor(AEmitter.ColorBegin);
  ColorPicker2.Color:=d3StrToColor(AEmitter.ColorEnd);

  trackPositionDispersionX.Value:=AEmitter.PositionDispersion.X;
  trackPositionDispersionY.Value:=AEmitter.PositionDispersion.Y;
  trackPositionDispersionZ.Value:=AEmitter.PositionDispersion.Z;

  with AEmitter.Rect do
  begin
    rectSelection.SetBounds(left * resizeLayout.Width, top * resizeLayout.Height, (right - left) * resizeLayout.Width,
      (bottom - top) * resizeLayout.Height);
  end;
//    FVelocityMode:=TD3ParticleEmitter(Source).VelocityMode;
//    FFollowToOwner:=TD3ParticleEmitter(Source).FollowToOwner;
//    FDispersionMode:=TD3ParticleEmitter(Source).DispersionMode;
//    FDirectionAngle:=TD3ParticleEmitter(Source).DirectionAngle;
//    FFriction:=TD3ParticleEmitter(Source).Friction;

  Emitter.Assign(AEmitter);
end;

procedure TfrmParticleDesign.AssignToEmitter(var AEmitter: TD3ParticleEmitter);
begin
  AEmitter.Assign(Emitter);
end;

procedure TfrmParticleDesign.RebuildBitmapList;
var
  i: integer;
begin
  popupBitmap.PopupBox.Items.Clear;
  popupBitmap.PopupBox.Items.Add('(empty)');
  for i:=0 to GvarD3BitmapList.Count - 1 do
  begin
    if (GetBitmapParent(GvarD3BitmapList[i])=ParentScene) or (GetBitmapParent(GvarD3BitmapList[i]) is TD3BitmapList) then
      popupBitmap.PopupBox.Items.Add(GvarD3BitmapList[i]);
  end;
end;

procedure TfrmParticleDesign.Button1Click(Sender: TObject);
begin
  Close;
end;

procedure TfrmParticleDesign.cbBitmapListChange(Sender: TObject);
var
  B: TD3Bitmap;
begin
  if popupBitmap.Value=0 then
    Emitter.Bitmap:=''
  else
    Emitter.Bitmap:=popupBitmap.PopupBox.Items[popupBitmap.Value];

  B:=GetBitmapByName(Emitter.Bitmap);
  if B <> nil then
  begin
    imageTexture.Bitmap.SetSize(B.Width, B.Height);
    Move(B.Bits^, imageTexture.Bitmap.StartLine^, B.Width * B.Height * 4);
    if (B.Width < imageTexture.Width) and (B.Height < imageTexture.Height) then
    begin
      resizeLayout.SetBounds(0, 0, B.Width, B.Height);
      resizeLayout.Align:=vaCenter;
    end
    else
    begin
      resizeLayout.SetBounds(0, 0, B.Width, B.Height);
      resizeLayout.Align:=vaFit;
    end;
    rectSelection.Visible:=true;
  end
  else
  begin
    imageTexture.Bitmap.SetSize(1, 1);
    rectSelection.Visible:=false;
  end;
  imageTexture.Repaint;
end;

procedure TfrmParticleDesign.rgFillModeClick(Sender: TObject);
begin
(*  { cube }
  Cube1.Material.FillMode:=TD3FillMode(popupFillMode.Value);
  Cube2.Material.FillMode:=TD3FillMode(popupFillMode.Value);
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material); *)
end;

procedure TfrmParticleDesign.rgShadeModeClick(Sender: TObject);
begin
  Emitter.BlendingMode:=TD3BlendingMode(popupBlendMode.Value);
end;

procedure TfrmParticleDesign.tbBitmaptileXChange(Sender: TObject);
begin
(*  Cube1.Material.BitmapTileX:=trackTileX.Value;
  Cube2.Material.BitmapTileX:=trackTileX.Value;
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material); *)
end;

procedure TfrmParticleDesign.tbBitmaptileYChange(Sender: TObject);
begin
(*  Cube1.Material.BitmapTileY:=trackTileY.Value;
  Cube2.Material.BitmapTileY:=trackTileY.Value;
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material); *)
end;

procedure TfrmParticleDesign.checkLightChange(Sender: TObject);
begin
(*  { cube }
  Cube1.Material.Lighting:=checkLight.IsChecked;
  Cube2.Material.Lighting:=checkLight.IsChecked;
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material); *)
end;

procedure TfrmParticleDesign.numRChange(Sender: TObject);
var
  Color: TD3Color;
begin
  Color:=ColorPicker1.Color;
  TD3ColorRec(Color).R:=trunc(numR.Value);
  TD3ColorRec(Color).G:=trunc(numG.Value);
  TD3ColorRec(Color).B:=trunc(numB.Value);
  TD3ColorRec(Color).A:=trunc(numA.Value);
  textRGBA.Text:=d3ColorToStr(ColorBox1.Color);
  ColorPicker1.Color:=Color;
  partGrad.Fill.Gradient.Points[0].Color:=textRGBA.Text;
  partGrad.Repaint;
end;

procedure TfrmParticleDesign.textRGBAChange(Sender: TObject);
begin
  ColorPicker1.Color:=d3StrToColor(textRGBA.Text);
end;

procedure TfrmParticleDesign.ColorQuad1Change(Sender: TObject);
begin
  Emitter.ColorBegin:=d3ColorToStr(ColorBox1.Color);
  numR.Value:=TD3ColorRec(ColorBox1.Color).R;
  numG.Value:=TD3ColorRec(ColorBox1.Color).G;
  numB.Value:=TD3ColorRec(ColorBox1.Color).B;
  numA.Value:=TD3ColorRec(ColorBox1.Color).A;
  textRGBA.Text:=d3ColorToStr(ColorBox1.Color);
  partGrad.Fill.Gradient.Points[0].Color:=textRGBA.Text;
  partGrad.Repaint;
end;

procedure TfrmParticleDesign.trackScaleMinChange(Sender: TObject);
begin
  Emitter.ScaleBegin:=trackScaleMin.Value;
end;

procedure TfrmParticleDesign.trackScaleMaxChange(Sender: TObject);
begin
  Emitter.ScaleEnd:=trackScaleMax.Value;
end;

procedure TfrmParticleDesign.trackSpinMinChange(Sender: TObject);
begin
  Emitter.SpinBegin:=trackSpinMin.Value;
end;

procedure TfrmParticleDesign.trackSpinMaxChange(Sender: TObject);
begin
  Emitter.Spinend:=trackSpinMax.Value;
end;

procedure TfrmParticleDesign.trackParticleCountChange(Sender: TObject);
begin
  Emitter.ParticlePerSecond:=trackParticleCount.Value;
end;

procedure TfrmParticleDesign.trackLifetimeChange(Sender: TObject);
begin
  Emitter.LifeTime:=trackLifetime.Value;
end;

procedure TfrmParticleDesign.rectSelectionChange(Sender: TObject);
begin
  with rectSelection.ParentedRect do
    Emitter.Rect.Rect:=d2Rect(left / resizeLayout.Width, top / resizeLayout.Height,
      right / resizeLayout.Width, bottom / resizeLayout.Height);
end;

procedure TfrmParticleDesign.trackVelocityMinChange(Sender: TObject);
begin
  Emitter.VelocityMin:=trackVelocityMin.Value;
end;

procedure TfrmParticleDesign.trackVelocityMaxChange(Sender: TObject);
begin
  Emitter.VelocityMax:=trackVelocityMax.Value;
end;

procedure TfrmParticleDesign.numR2Change(Sender: TObject);
var
  Color: TD3Color;
begin
  Color:=ColorPicker2.Color;
  TD3ColorRec(Color).R:=trunc(numR2.Value);
  TD3ColorRec(Color).G:=trunc(numG2.Value);
  TD3ColorRec(Color).B:=trunc(numB2.Value);
  TD3ColorRec(Color).A:=trunc(numA2.Value);
  textRGBA2.Text:=d3ColorToStr(ColorBox2.Color);
  partGrad.Fill.Gradient.Points[1].Color:=textRGBA2.Text;
  partGrad.Repaint;
  ColorPicker2.Color:=Color;
end;

procedure TfrmParticleDesign.textRGBA2Change(Sender: TObject);
begin
  ColorPicker2.Color:=d3StrToColor(textRGBA.Text);
end;

procedure TfrmParticleDesign.ColorQuad2Change(Sender: TObject);
begin
  Emitter.ColorEnd:=d3ColorToStr(ColorBox2.Color);
  numR2.Value:=TD3ColorRec(ColorBox2.Color).R;
  numG2.Value:=TD3ColorRec(ColorBox2.Color).G;
  numB2.Value:=TD3ColorRec(ColorBox2.Color).B;
  numA2.Value:=TD3ColorRec(ColorBox2.Color).A;
  textRGBA2.Text:=d3ColorToStr(ColorBox2.Color);
  partGrad.Fill.Gradient.Points[1].Color:=textRGBA2.Text;
  partGrad.Repaint;
end;

procedure TfrmParticleDesign.trackGravityXChange(Sender: TObject);
begin
  Emitter.Gravity.X:=trackGravityX.Value;
end;

procedure TfrmParticleDesign.trackGravityYChange(Sender: TObject);
begin
  Emitter.Gravity.Y:=trackGravityY.Value;
end;

procedure TfrmParticleDesign.trackGravityZChange(Sender: TObject);
begin
  Emitter.Gravity.Z:=trackGravityZ.Value;
end;

procedure TfrmParticleDesign.trackRadialMinChange(Sender: TObject);
begin
  Emitter.CentrifugalVelMin:=trackRadialMin.Value;
end;

procedure TfrmParticleDesign.trackRadialMaxChange(Sender: TObject);
begin
  Emitter.CentrifugalVelMax:=trackRadialMax.Value;
end;

procedure TfrmParticleDesign.btnAddBitmapClick(Sender: TObject);
var
  B: TD2Bitmap;
  BitmapObject: TD3BitmapObject;
begin
  if ParentScene=nil then Exit;

  GvarD2BitmapEditor:=TD2BitmapEditor.Create(nil);
  if GvarD2BitmapEditor.ShowModal=mrOk then
  begin
    B:=TD2Bitmap.Create(1, 1);
    GvarD2BitmapEditor.AssignToBitmap(B);

    BitmapObject:=TD3BitmapObject.Create(ParentScene.Owner);
    BitmapObject.Parent:=ParentScene.Root;
    BitmapObject.Bitmap.Assign(B);
    if GvarD2BitmapEditor.FileName <> '' then
      BitmapObject.ResourceName:=ExtractFileName(GvarD2BitmapEditor.FileName);

    RebuildBitmapList;
    popupBitmap.PopupBox.ItemIndex:=popupBitmap.PopupBox.Items.IndexOf(BitmapObject.ResourceName);

    B.Free;
  end;
  GvarD2BitmapEditor.Free;
end;

procedure TfrmParticleDesign.SetParentScene(const Value: TD3Scene);
begin
  FParentScene:=Value;
  RebuildBitmapList;
end;

procedure TfrmParticleDesign.trackTangentMinChange(Sender: TObject);
begin
  Emitter.TangentVelMin:=trackTangentMin.Value;
end;

procedure TfrmParticleDesign.trackTangentMaxChange(Sender: TObject);
begin
  Emitter.TangentVelMax:=trackTangentMax.Value;
end;

procedure TfrmParticleDesign.trackSpreadChange(Sender: TObject);
begin
  Emitter.SpreadAngle:=trackSpread.Value;
end;

procedure TfrmParticleDesign.trackDirectionAngleChange(Sender: TObject);
begin
  Emitter.DirectionAngle:=trackDirectionAngle.Value;
end;

procedure TfrmParticleDesign.Plane1MouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y:single; rayPos,
  rayDir: TD3Vector);
var
  Int: TD3Vector;
begin
  if Plane1.RayCastIntersect(rayPos, rayDir, Int) and (Button=mbLeft) then
  begin
    Emitter.AnimateFloat('Position.X', Int.X, 0.2);
    Emitter.AnimateFloat('Position.Z', Int.Z, 0.2);
    FMousePressed:=true;
  end;
end;

procedure TfrmParticleDesign.Plane1MouseMove(Sender: TObject;
  Shift: TShiftState; X, Y:single; rayPos, rayDir: TD3Vector);
var
  Int: TD3Vector;
begin
  if FMousePressed and Plane1.RayCastIntersect(rayPos, rayDir, Int) then
  begin
    Emitter.Position.Point:=d3Point(Int);
    Application.ProcessMessages;
  end;
end;

procedure TfrmParticleDesign.Plane1MouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y:single; rayPos,
  rayDir: TD3Vector);
begin
  if FMousePressed then
  begin
    Emitter.AnimateFloat('Position.X', 0, 0.3);
    Emitter.AnimateFloat('Position.Z', 0, 0.3);
    FMousePressed:=false;
  end;
end;

procedure TfrmParticleDesign.trackPositionDispersionXChange(
  Sender: TObject);
begin
  Emitter.PositionDispersion.X:=trackPositionDispersionX.Value;
end;

procedure TfrmParticleDesign.trackPositionDispersionYChange(
  Sender: TObject);
begin
  Emitter.PositionDispersion.Y:=trackPositionDispersionY.Value;
end;

procedure TfrmParticleDesign.trackPositionDispersionZChange(
  Sender: TObject);
begin
  Emitter.PositionDispersion.Y:=trackPositionDispersionY.Value;
end;

procedure TfrmParticleDesign.trackFrictionChange(Sender: TObject);
begin
  Emitter.Friction:=trackFriction.Value;
end;

end.
