{***************************************************
 ORCA 3D Library
 Copyright (C) PilotLogic Software House
 http://www.pilotlogic.com
 
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio
***********************************************************************}
   
unit orca_editor_material;

interface

uses

  LCLProc, LCLType, LMessages, LResources,
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, Buttons,
  ComCtrls, orca_scene2d, orca_scene3d;

type

  { TfrmMaterialDesign }

  TfrmMaterialDesign=class(TForm)
    d3Scene1: TD3Scene;
    Root1: TD3VisualObject;
    Light1: TD3Light;
    rotate: TD3FloatAnimation;
    Cube1: TD3Cylinder;
    GUIObjectLayer1: TD3GUIScene2DLayer;
    Root2: TD2Layout;
    checkLight: TD2CheckBox;
    popupBitmap: TD2CompoundPopupBox;
    popupModulation: TD2CompoundPopupBox;
    popupFillMode: TD2CompoundPopupBox;
    popupShadeMode: TD2CompoundPopupBox;
    GroupBox1: TD2GroupBox;
    trackTileX: TD2CompoundTrackBar;
    trackTileY: TD2CompoundTrackBar;
    Cube2: TD3Cube;
    FloatAnimation1: TD3FloatAnimation;
    GroupBox2: TD2GroupBox;
    colorList: TD2StringListBox;
    ColorQuad1: TD2ColorQuad;
    ColorPicker1: TD2ColorPicker;
    ColorBox1: TD2ColorBox;
    Layout1: TD2Layout;
    Label1: TD2Label;
    numR: TD2NumberBox;
    numG: TD2NumberBox;
    Label2: TD2Label;
    textRGBA: TD2TextBox;
    numB: TD2NumberBox;
    numA: TD2NumberBox;
    Background1: TD2Background;
    GUIImage1: TD3GUIImage;
    modalLayout: TD3GUIScene2DLayer;
    Root3: TD2Layout;
    Background2: TD2Background;
    Button1: TD2Button;
    Button2: TD2Button;
    btnAddBitmap: TD2Button;
    procedure Button1Click(Sender: TObject);
    procedure rgModulationClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure cbBitmapListChange(Sender: TObject);
    procedure rgFillModeClick(Sender: TObject);
    procedure rgShadeModeClick(Sender: TObject);
    procedure tbBitmaptileXChange(Sender: TObject);
    procedure tbBitmaptileYChange(Sender: TObject);
    procedure checkLightChange(Sender: TObject);
    procedure ColorQuad1Change(Sender: TObject);
    procedure colorListChange(Sender: TObject);
    procedure numRChange(Sender: TObject);
    procedure textRGBAChange(Sender: TObject);
    procedure btnAddBitmapClick(Sender: TObject);
  private
    FMaterial: TD3Material;
    FParentScene: TD3Scene;
    procedure SetMaterial(const Value: TD3Material);
    procedure RebuildBitmapList;
    { Private declarations }
  public
    { Public declarations }
    property Material: TD3Material read FMaterial write SetMaterial;
    property ParentScene: TD3Scene read FParentScene write FParentScene;
  end;

  TD3MaterialDialog=class(TComponent)
  private
    FMaterial: TD3Material;
    procedure SetMaterial(const Value: TD3Material);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function Execute: boolean;
    property Material: TD3Material read FMaterial write SetMaterial;
  published
  end;

var
  frmMaterialDesign: TfrmMaterialDesign;

implementation


{$R *.lfm}

procedure TfrmMaterialDesign.FormCreate(Sender: TObject);
begin
  RebuildBitmapList;
end;

procedure TfrmMaterialDesign.RebuildBitmapList;
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

procedure TfrmMaterialDesign.Button1Click(Sender: TObject);
begin   
  Close;
end;

procedure TfrmMaterialDesign.SetMaterial(const Value: TD3Material);
var
  i: integer;
begin
  FMaterial:=Value;
  if Assigned(FMaterial) then
  begin
    Cube1.Material.Assign(FMaterial);
    Cube2.Material.Assign(FMaterial);

    ColorPicker1.Color:=Cube1.Material.NativeDiffuse;

    RebuildBitmapList;
    if FMaterial.Bitmap='' then
      popupBitmap.Value:=0
    else
      for i:=1 to popupBitmap.PopupBox.Items.Count - 1 do
      begin
        if FMaterial.Bitmap=popupBitmap.PopupBox.Items[i] then
        begin
          popupBitmap.Value:=i;
          Break;
        end;
      end;
    checkLight.IsChecked:=FMaterial.Lighting;
    popupModulation.Value:=Integer(FMaterial.BitmapMode);
    popupFillMode.Value:=Integer(FMaterial.FillMode);
    popupShadeMode.Value:=Integer(FMaterial.ShadeMode);
    trackTileX.Value:=Round(FMaterial.BitmapTileX);
    trackTileY.Value:=Round(FMaterial.BitmapTileY);
  end;
end;

procedure TfrmMaterialDesign.rgModulationClick(Sender: TObject);
begin
  { cube }
  Cube1.Material.BitmapMode:=TD3TexMode(popupModulation.Value);
  Cube2.Material.BitmapMode:=TD3TexMode(popupModulation.Value);
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material);
end;

procedure TfrmMaterialDesign.cbBitmapListChange(Sender: TObject);
begin
  { cube }
  if popupBitmap.Value=0 then
    Cube1.Material.Bitmap:=''
  else
    Cube1.Material.Bitmap:=popupBitmap.PopupBox.Items[popupBitmap.Value];
  Cube2.Material.Bitmap:=Cube1.Material.Bitmap;
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material);
end;

procedure TfrmMaterialDesign.rgFillModeClick(Sender: TObject);
begin
  { cube }
  Cube1.Material.FillMode:=TD3FillMode(popupFillMode.Value);
  Cube2.Material.FillMode:=TD3FillMode(popupFillMode.Value);
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material);
end;

procedure TfrmMaterialDesign.rgShadeModeClick(Sender: TObject);
begin
  { cube }
  Cube1.Material.ShadeMode:=TD3ShadeMode(popupShadeMode.Value);
  Cube2.Material.ShadeMode:=TD3ShadeMode(popupShadeMode.Value);
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material);
end;

procedure TfrmMaterialDesign.tbBitmaptileXChange(Sender: TObject);
begin
  Cube1.Material.BitmapTileX:=trackTileX.Value;
  Cube2.Material.BitmapTileX:=trackTileX.Value;
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material);
end;

procedure TfrmMaterialDesign.tbBitmaptileYChange(Sender: TObject);
begin
  Cube1.Material.BitmapTileY:=trackTileY.Value;
  Cube2.Material.BitmapTileY:=trackTileY.Value;
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material);
end;

procedure TfrmMaterialDesign.checkLightChange(Sender: TObject);
begin
  { cube }
  Cube1.Material.Lighting:=checkLight.IsChecked;
  Cube2.Material.Lighting:=checkLight.IsChecked;
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material);
end;

procedure TfrmMaterialDesign.ColorQuad1Change(Sender: TObject);
begin
  { cube }
  case colorList.ItemIndex of
    0: begin
      Cube1.Material.NativeAmbient:=ColorBox1.Color;
      Cube2.Material.NativeAmbient:=ColorBox1.Color;
      numR.Value:=TD3ColorRec(ColorBox1.Color).R;
      numG.Value:=TD3ColorRec(ColorBox1.Color).G;
      numB.Value:=TD3ColorRec(ColorBox1.Color).B;
      numA.Value:=TD3ColorRec(ColorBox1.Color).A;
      textRGBA.Text:=d3ColorToStr(ColorBox1.Color);
    end;
    1: begin
      Cube1.Material.NativeDiffuse:=ColorBox1.Color;
      Cube2.Material.NativeDiffuse:=ColorBox1.Color;
      numR.Value:=TD3ColorRec(ColorBox1.Color).R;
      numG.Value:=TD3ColorRec(ColorBox1.Color).G;
      numB.Value:=TD3ColorRec(ColorBox1.Color).B;
      numA.Value:=TD3ColorRec(ColorBox1.Color).A;
      textRGBA.Text:=d3ColorToStr(ColorBox1.Color);
    end;
  end;
  { material }
  if Assigned(FMaterial) then
    FMaterial.Assign(Cube1.Material);
end;

procedure TfrmMaterialDesign.colorListChange(Sender: TObject);
begin
  case colorList.ItemIndex of
    0: begin
      ColorPicker1.Color:=Cube1.Material.NativeAmbient;
    end;
    1: begin
      ColorPicker1.Color:=Cube1.Material.NativeDiffuse;
    end;
  end;
end;

procedure TfrmMaterialDesign.numRChange(Sender: TObject);
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
end;

procedure TfrmMaterialDesign.textRGBAChange(Sender: TObject);
begin
  ColorPicker1.Color:=d3StrToColor(textRGBA.Text);
end;

procedure TfrmMaterialDesign.btnAddBitmapClick(Sender: TObject);
var
  B: TD2Bitmap;
  BitmapObject: TD3BitmapObject;
  vgBitmapEditor: TD2BitmapEditor;
begin
  if ParentScene=nil then Exit;
  
  vgBitmapEditor:=TD2BitmapEditor.Create(nil);
  if vgBitmapEditor.ShowModal=mrOk then
  begin
    B:=TD2Bitmap.Create(1, 1);
    vgBitmapEditor.AssignToBitmap(B);

    BitmapObject:=TD3BitmapObject.Create(ParentScene.Owner);
    BitmapObject.Parent:=ParentScene.Root;
    if GvarD3Designer <> nil then
      BitmapObject.Name:=GvarD3Designer.UniqueName(ParentScene.Owner, BitmapObject.ClassName);
    BitmapObject.Bitmap.Assign(B);
    if vgBitmapEditor.FileName <> '' then
      BitmapObject.ResourceName:=ExtractFileName(vgBitmapEditor.FileName);

    RebuildBitmapList;
    popupBitmap.PopupBox.ItemIndex:=popupBitmap.PopupBox.Items.IndexOf(BitmapObject.ResourceName);
    
    B.Free;
  end;
  vgBitmapEditor.Free;
end;

{ TD3MaterialDialog }

constructor TD3MaterialDialog.Create(AOwner: TComponent);
begin
  inherited;
  FMaterial:=TD3Material.Create;
end;

destructor TD3MaterialDialog.Destroy;
begin
  FMaterial.Free;
  inherited;
end;

function TD3MaterialDialog.Execute: boolean;
var
  Dialog: TfrmMaterialDesign;
  EditMaterial: TD3Material;
begin
  Dialog:=TfrmMaterialDesign.Create(Application);

  Dialog.modalLayout.Visible:=true;

  Dialog.Material:=FMaterial;
  Result:=Dialog.ShowModal=mrOk;
  Dialog.Free;
end;

procedure TD3MaterialDialog.SetMaterial(const Value: TD3Material);
begin
  FMaterial.Assign(Value);;
end;


end.
