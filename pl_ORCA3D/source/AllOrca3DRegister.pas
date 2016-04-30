{***************************************************
 ORCA 3D Library
 Copyright (C) PilotLogic Software House
 http://www.pilotlogic.com
 
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio
***********************************************************************}

unit AllOrca3DRegister;

interface

uses
  {$IFDEF WINDOWS}
   Windows,
  {$ENDIF}

  LCLProc, LCLType, LMessages, LResources,

  LazIDEIntf, PropEdits, ComponentEditors,
  Classes, Controls, Dialogs, ExtDlgs, SysUtils, Forms,
  orca_scene3d, orca_scene2d, Graphics;

type

  TD3IDEDesigner = class(TD3Designer)
  private
  public
    procedure SelectObject(ADesigner: TComponent; AObject: TD3Object; MultiSelection: array of TD3Object); override;
    procedure Modified(ADesigner: TComponent); override;
    function  UniqueName(ADesigner: TComponent; ClassName: string): string; override;
    function  IsSelected(ADesigner: TComponent; const AObject: TObject): boolean; override;
    procedure AddObject(AObject: TD3Object); override;
    procedure AddObject2D(AObject: TD2Object); override;
  end;

  TD3MeshProperty = class(TClassProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: String; override;
    procedure Edit; override;
  end;

  TD3MaterialProperty = class(TClassProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: String; override;
    procedure Edit; override;
  end;

  TD3ParticleProperty = class(TClassProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: String; override;
    procedure Edit; override;
  end;

  TD3BitmapProperty = class(TClassProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: String; override;
    procedure Edit; override;
  end;

  TD3BitmapNameProperty = class(TStringProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: string; override;
    procedure GetValues(Proc: TGetStrProc); override;
    procedure SetValue(const Value: string); override;
  end;

  TD3ColorProperty = class(TStringProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: string; override;
    procedure SetValue(const Value: string); override;
    procedure Edit; override;
  end;

  TD3FontProperty = class(TClassProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: String; override;
    procedure Edit; override;
  end;

  TD3TriggerProperty = class(TStringProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: string; override;
    procedure GetValues(Proc: TGetStrProc); override;
    procedure SetValue(const Value: string); override;
  end;

procedure Register;

implementation

  {$R AllOrca3DRegister.res}

uses
  orca_editor_material,
  orca_editor_particle;


//===================== TD3IDEDesigner =========================
// For 3D Objects Editors

procedure TD3IDEDesigner.AddObject(AObject: TD3Object);
begin
  if AObject=nil then exit;
  if GlobalDesignHook =nil then exit;

  GlobalDesignHook.PersistentAdded(AObject,false);
end;

procedure TD3IDEDesigner.AddObject2D(AObject: TD2Object);
begin
  if AObject=nil then exit;
  if GlobalDesignHook =nil then exit;

  GlobalDesignHook.PersistentAdded(AObject,false);
end;

procedure TD3IDEDesigner.Modified(ADesigner: TComponent);
begin
  if (ADesigner is TCustomForm) and (TCustomform(ADesigner).Designer <> nil) then
  begin
    if TCustomForm(ADesigner).Designer <> nil then
    begin
      TCustomForm(ADesigner).Designer.Modified;
    end;
  end;
end;

procedure TD3IDEDesigner.SelectObject(ADesigner: TComponent; AObject: TD3Object; MultiSelection: array of TD3Object);
begin
  if (ADesigner is TCustomForm) and (TCustomform(ADesigner).Designer <> nil) then
  begin
    if TCustomForm(ADesigner).Designer <> nil then
    begin
      TCustomForm(ADesigner).Designer.SelectOnlyThisComponent(AObject);
    end;
  end;
end;

function TD3IDEDesigner.IsSelected(ADesigner: TComponent; const AObject: TObject): boolean;
begin
  Result := false;
end;

function TD3IDEDesigner.UniqueName(ADesigner: TComponent;
  ClassName: string): string;
begin
  Result := '';
  if (ADesigner is TCustomForm) and (TCustomform(ADesigner).Designer <> nil) then
  begin
    if TCustomForm(ADesigner).Designer <> nil then
    begin
      if (ClassName <> '') and (Pos('TD3', ClassName) = 1) then
        Delete(ClassName, 1, 3);
      Result := TCustomForm(ADesigner).Designer.UniqueName('T' + ClassName);
    end;
  end;
end;

//----------------- TD3MeshProperty -----------------------------

procedure TD3MeshProperty.Edit;
var
  M: TD3MeshData;
begin
  M := TD3MeshData(GetOrdValue);
end;

function TD3MeshProperty.GetValue: String;
begin
  Result := '(Mesh)';
end;

function TD3MeshProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paSubProperties, paDialog, paReadOnly];
end;

//----------------- TD3MaterialProperty -----------------------------

procedure TD3MaterialProperty.Edit;
var
  M: TD3Material;
begin
  M := TD3Material(GetOrdValue);
  if frmMaterialDesign = nil then
    frmMaterialDesign := TfrmMaterialDesign.Create(Application);
  frmMaterialDesign.modalLayout.Visible := true;
  if (GetComponent(0) is TD3Object) then
    frmMaterialDesign.ParentScene := TD3Object(GetComponent(0)).Scene;
  frmMaterialDesign.Material := M;
  frmMaterialDesign.ShowModal;
  frmMaterialDesign.Free;
  frmMaterialDesign := nil;
end;

function TD3MaterialProperty.GetValue: String;
begin
  Result := '(Material)';
end;

function TD3MaterialProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paSubProperties, paDialog, paReadOnly];
end;

//----------------- TD3ParticleProperty -----------------------------

procedure TD3ParticleProperty.Edit;
var
  frmParticleDesign: TD3ParticleDialog;
begin
  if not (GetComponent(0) is TD3ParticleEmitter) then Exit;
  frmParticleDesign := TD3ParticleDialog.Create(Application);
  frmParticleDesign.Emitter := TD3ParticleEmitter(GetComponent(0));
  if frmParticleDesign.Execute(TD3ParticleEmitter(GetComponent(0)).Scene) then
    TD3ParticleEmitter(GetComponent(0)).Assign(frmParticleDesign.Emitter);
  frmParticleDesign.Free;
end;

function TD3ParticleProperty.GetValue: String;
begin
  Result := '(Particle Designer)';
end;

function TD3ParticleProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paDialog, paReadOnly];
end;

//----------------- TD3BitmapProperty -----------------------------

procedure TD3BitmapProperty.Edit;
var
  B: TD2Bitmap;
begin
  GvarD2BitmapEditor := TD2BitmapEditor.Create(nil);
  B := TD2Bitmap.Create(1, 1);
  B.Assign(TD3Bitmap(GetOrdValue));
  GvarD2BitmapEditor.AssignFromBitmap(B);
  if GvarD2BitmapEditor.ShowModal = mrOk then
  begin
    GvarD2BitmapEditor.AssignToBitmap(B);

    TD3Bitmap(GetOrdValue).Assign(B);

    if GvarD2BitmapEditor.FileName <> '' then
    begin
      if GetComponent(0) is TD3BitmapStream then
      begin
        if TD3BitmapStream(GetComponent(0)).Name = '' then
          TD3BitmapStream(GetComponent(0)).Name := ExtractFileName(GvarD2BitmapEditor.FileName);
      end;
      if GetComponent(0) is TD3BitmapObject then
      begin
        if TD3BitmapObject(GetComponent(0)).ResourceName = '' then
          TD3BitmapObject(GetComponent(0)).ResourceName := ExtractFileName(GvarD2BitmapEditor.FileName);
      end;
    end;
  end;
  B.Free;
  GvarD2BitmapEditor.Free;
end;

function TD3BitmapProperty.GetValue: String;
begin
  Result := '(Bitmap)';
end;

function TD3BitmapProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paDialog, paReadOnly];
end;

function TD3BitmapNameProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paMultiSelect, paValueList, paRevertable];
end;

procedure TD3BitmapNameProperty.GetValues(Proc: TGetStrProc);
var
  i: Integer;
begin
  try
    if GvarD3BitmapList <> nil then
      for i := 0 to GvarD3BitmapList.Count - 1 do
      begin
        if GvarD3BitmapList.Objects[i] is TD3BitmapStream then
          Proc(TD3BitmapStream(GvarD3BitmapList.Objects[i]).Name);
        if GvarD3BitmapList.Objects[i] is TD3BitmapObject then
          Proc(TD3BitmapObject(GvarD3BitmapList.Objects[i]).ResourceName);
        if GvarD3BitmapList.Objects[i] is TD3BufferLayer then
          Proc(TD3BufferLayer(GvarD3BitmapList.Objects[i]).Name);
      end;
  except
    on E: Exception do ShowMessage(E.Message);
  end;
end;

function TD3BitmapNameProperty.GetValue: string;
begin
  try
    Result := GetStrValue;
  except
    on E: Exception do ShowMessage(E.Message);
  end;
end;

procedure TD3BitmapNameProperty.SetValue(const Value: string);
begin
  try
    SetStrValue(Value);
    Modified;
  except
    on E: Exception do ShowMessage(E.Message);
  end;
end;

//----------------- TD3ColorProperty -----------------------------

procedure TD3ColorProperty.Edit;
var
  D: TD2BrushDialog;
begin
  D := TD2BrushDialog.Create(Application);
  D.ShowStyles := [d2BrushSolid];
  D.ShowBrushList := false;
  D.Brush.Color := GetStrValue;
  if D.Execute then
  begin
    SetStrValue(D.Brush.Color);
  end;
  D.Free;
end;

function TD3ColorProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paDialog, paMultiSelect];
end;

function TD3ColorProperty.GetValue: string;
begin
  try
    Result := GetStrValue;
  except
    on E: Exception do ShowMessage(E.Message);
  end;
end;

procedure TD3ColorProperty.SetValue(const Value: string);
begin
  try
    SetStrValue(Value);
    Modified;
  except
    on E: Exception do ShowMessage(E.Message);
  end;
end;


//----------------- TD3FontProperty -----------------------------

procedure TD3FontProperty.Edit;
var
  F: TFontDialog;
begin
  F := TFontDialog.Create(Application);
  F.Font.Assign(TD3Font(GetOrdValue));
  if F.Execute then
  begin
    TD3Font(GetOrdValue).Assign(F.Font);
    Modified;
  end;
  F.Free;
end;

function TD3FontProperty.GetValue: String;
begin
  Result := '(Font)';
end;

function TD3FontProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paMultiSelect, paSubProperties, paDialog, paReadOnly];
end;

//----------------- TD3TriggerProperty -----------------------------

function TD3TriggerProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paMultiSelect, paValueList, paRevertable];
end;

procedure TD3TriggerProperty.GetValues(Proc: TGetStrProc);
var
  i: Integer;
begin
  try
    Proc('IsMouseOver=true');
    Proc('IsMouseOver=false');
    Proc('IsFocused=true');
    Proc('IsFocused=false');
    Proc('IsVisible=true');
    Proc('IsVisible=false');
    Proc('IsDragOver=true');
    Proc('IsDragOver=false');
    Proc('IsOpen=true');
    Proc('IsOpen=false');
  except
    on E: Exception do ShowMessage(E.Message);
  end;
end;

function TD3TriggerProperty.GetValue: string;
begin
  try
    Result := GetStrValue;
  except
    on E: Exception do ShowMessage(E.Message);
  end;
end;

procedure TD3TriggerProperty.SetValue(const Value: string);
begin
  try
    SetStrValue(Value);
    Modified;
  except
    on E: Exception do ShowMessage(E.Message);
  end;
end;

//================== Register Routines ===============

procedure Register;
begin

//------ Visible Components -----------------------

    RegisterComponents('ORCA', [
                               TD3Scene,
                               TD3MaterialDialog,
                               TD3ParticleDialog,
                               TD3BitmapList
                               ]);


  RegisterPropertyEditor(TypeInfo(String), TD2Brush, 'Color', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2ColorKey, 'Value', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2GlowEffect, 'GlowColor', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2ShadowEffect, 'ShadowColor', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2ColorAnimation, 'StartValue', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2ColorAnimation, 'StopValue', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2ColorKey, 'Value', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2ColorButton, 'Color', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2CompoundColorButton, 'Value', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2ColorPanel, 'Color', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2ComboColorBox, 'Color', TD3ColorProperty);

//-- 3D Components ---------------------------------

  RegisterNoIcon([
    TD3Camera, TD3Light,  TD3Dummy, TD3Cube, TD3Mesh, TD3Sphere, TD3Grid, TD3Text, TD3Image,
    TD3Cylinder, TD3RoundCube, TD3Cone, TD3BitmapObject, TD3StrokeCube,
    TD3VisualObject, TD3ProxyObject,
    TD3ColorAnimation, TD3FloatAnimation,
    TD3BufferLayer, TD3CanvasLayer, TD3ObjectLayer,
    // shape 3d
    TD3Text3D, TD3Path3D, TD3Rectangle3D, TD3Ellipse3D,
    // controls
    TD3TextBox3D,
    // dynamics
    TD3ParticleEmitter,
    // gui
    TD3GUILayout, TD3GUIScene2DLayer, TD3GUIImage, TD3GUIPlane, TD3GUIText, TD3GUISelection,
    // deprecated
    TD3d2Layer, TD3ScreenCanvasLayer, TD3Screend2Layer, TD3ScreenImage, TD3ScreenDummy
  ]);

  RegisterPropertyEditor(TypeInfo(TD3MeshData), nil, '', TD3MeshProperty);
  RegisterPropertyEditor(TypeInfo(TD3Material), nil, '', TD3MaterialProperty);
  RegisterPropertyEditor(TypeInfo(TD3Bitmap), nil, '', TD3BitmapProperty);
  RegisterPropertyEditor(TypeInfo(TD3Font), nil, '', TD3FontProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3Material, 'Bitmap', TD3BitmapNameProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3Material, 'Diffuse', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3Material, 'Ambient', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3Scene, 'FillColor', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3Scene, 'AmbientColor', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3CustomLayer, 'ModulationColor', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3Animation, 'Trigger', TD3TriggerProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3ColorAnimation, 'StartValue', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3ColorAnimation, 'StopValue', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3ParticleEmitter, 'ColorBegin', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3ParticleEmitter, 'ColorEnd', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3ParticleEmitter, 'Bitmap', TD3BitmapNameProperty);
  RegisterPropertyEditor(TypeInfo(integer),TD3ParticleEmitter, 'Editor', TD3ParticleProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3TextBox3D, 'Background', TD3ColorProperty);
  RegisterPropertyEditor(TypeInfo(String), TD3TextBox3D, 'Selection', TD3ColorProperty);


end;

initialization

  GvarD3Designer := TD3IDEDesigner.Create(nil);

finalization

  GvarD3Designer.Free;
  GvarD3Designer := nil;
end.

