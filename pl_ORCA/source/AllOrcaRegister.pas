{***************************************************
 ORCA 2D Library
 Copyright (C) PilotLogic Software House
 http://www.pilotlogic.com
 
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio
***********************************************************************}

unit AllOrcaRegister;

interface

uses
  {$IFDEF WINDOWS}
   Windows,
  {$ENDIF}

  LCLProc, LCLType, LMessages, LResources,

  LazIDEIntf, PropEdits, ComponentEditors,
  Classes, Controls, Dialogs, ExtDlgs, SysUtils, Forms,
  orca_scene2d, Graphics;

type

  TD2IDEDesigner = class(TD2Designer)
  private
  public
    procedure SelectObject(ADesigner: TComponent; AObject: TD2Object; MultiSelection: array of TD2Object); override;
    procedure Modified(ADesigner: TComponent); override;
    function  UniqueName(ADesigner: TComponent; ClassName: string): string; override;
    function  IsSelected(ADesigner: TComponent; const AObject: TObject): boolean; override;
    procedure EditStyle(const Res: TD2Resources; const ASelected: string); override;
    procedure AddObject(AObject: TD2Object); override;
  end;

  TD2BrushProperty = class(TClassProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: String; override;
    procedure Edit; override;
  end;

  TD2BitmapProperty = class(TClassProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: String; override;
    procedure Edit; override;
  end;

  TD2PathDataProperty = class(TClassProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: String; override;
    procedure Edit; override;
  end;

  TD2ResourceProperty = class(TClassProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: String; override;
    procedure Edit; override;
  end;

  TD2WideStringsProperty = class(TClassProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: String; override;
    procedure Edit; override;
  end;

  TD2FontProperty = class(TClassProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: String; override;
    procedure Edit; override;
  end;

  TD2TriggerProperty = class(TStringProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: string; override;
    procedure GetValues(Proc: TGetStrProc); override;
    procedure SetValue(const Value: string); override;
  end;

  TD2LangProperty = class(TStringProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    procedure Edit; override;
  end;

  TD2LangEditor = class(TComponentEditor)
  private
  public
    procedure Edit; override;
  end;

  TD2ImgListEditor = class(TComponentEditor)
  private
  public
    procedure Edit; override;
  end;


  TD2GradientProperty = class(TStringProperty)
  private
  public
    function GetAttributes: TPropertyAttributes; override;
    function GetValue: string; override;
    procedure SetValue(const Value: string); override;
    procedure Edit; override;
  end;

procedure Register;

implementation

  {$R AllOrcaRegister.res}


//================= TD2IDEDesigner ========================
// For 2D Objects Editors

procedure TD2IDEDesigner.AddObject(AObject: TD2Object);
begin
  if AObject=nil then exit;
  if GlobalDesignHook =nil then exit;

  GlobalDesignHook.PersistentAdded(AObject,false);
end;

procedure TD2IDEDesigner.Modified(ADesigner: TComponent);
begin
  if (ADesigner is TCustomForm) and (TCustomform(ADesigner).Designer <> nil) then
  begin
    if TCustomForm(ADesigner).Designer <> nil then
    begin
      TCustomForm(ADesigner).Designer.Modified;
    end;
  end;
end;

procedure TD2IDEDesigner.SelectObject(ADesigner: TComponent; AObject: TD2Object; MultiSelection: array of TD2Object);
begin
  if (ADesigner is TCustomForm) and (TCustomform(ADesigner).Designer <> nil) then
  begin
    if TCustomForm(ADesigner).Designer <> nil then
    begin
      TCustomForm(ADesigner).Designer.SelectOnlyThisComponent(AObject);
    end;
  end;
end;

function TD2IDEDesigner.IsSelected(ADesigner: TComponent; const AObject: TObject): boolean;
begin
  Result := false;
end;

function TD2IDEDesigner.UniqueName(ADesigner: TComponent;  ClassName: string): string;
begin
  Result := '';
  if (ADesigner is TCustomForm) and (TCustomform(ADesigner).Designer <> nil) then
  begin
    if TCustomForm(ADesigner).Designer <> nil then
    begin
      if (ClassName <> '') and (Pos('TD2', ClassName) = 1) then
        Delete(ClassName, 1, 3);
      Result := TCustomForm(ADesigner).Designer.UniqueName('T' + ClassName);
    end;
  end;
end;

procedure TD2IDEDesigner.EditStyle(const Res: TD2Resources; const ASelected: string);
begin
  inherited;
  DesignResources(Res, ASelected);
end;

//-------------- TD2BrushProperty -----------------------------

procedure TD2BrushProperty.Edit;
begin
  selectInDesign(TD2Brush(GetOrdValue), GetComponent(0));
end;

function TD2BrushProperty.GetValue: String;
begin
  Result := '(Brush)';
end;

function TD2BrushProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paMultiSelect, paSubProperties, paDialog, paReadOnly];
end;

//----------------- TD2BitmapProperty -----------------------------

procedure TD2BitmapProperty.Edit;
begin
  GvarD2BitmapEditor := TD2BitmapEditor.Create(nil);
  GvarD2BitmapEditor.AssignFromBitmap(TD2Bitmap(GetOrdValue));
  if GvarD2BitmapEditor.ShowModal = mrOk then
  begin
    GvarD2BitmapEditor.AssignToBitmap(TD2Bitmap(GetOrdValue));
    Modified;
  end;
  GvarD2BitmapEditor.Free;
end;

function TD2BitmapProperty.GetValue: String;
begin
  Result := '(Bitmap)';
end;

function TD2BitmapProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paDialog, paReadOnly];
end;

//----------------- TD2PathDataProperty -----------------------------

procedure TD2PathDataProperty.Edit;
var
  S: string;
  EditDlg: TD2PathDataDesigner;
begin
  EditDlg := TD2PathDataDesigner.Create(Application);
  EditDlg.PathData.Lines.Text := TD2PathData(GetOrdValue).Data;
  if EditDlg.ShowModal = mrOk then
  begin
    TD2PathData(GetOrdValue).Data := EditDlg.PathData.Lines.Text;
    if GetComponent(0) is TD2VisualObject then
      TD2VisualObject(GetComponent(0)).Repaint;
    Modified;
  end;
  EditDlg.Free;
end;

function TD2PathDataProperty.GetValue: String;
begin
  Result := '(PathData)';
end;

function TD2PathDataProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paDialog, paReadOnly];
end;

//----------------- TD2ResourceProperty -----------------------------

procedure TD2ResourceProperty.Edit;
begin
  if DesignResources(TD2Resources(GetComponent(0)), '') then
    Modified;
end;

function TD2ResourceProperty.GetValue: String;
begin
  Result := '(Resource)';
end;

function TD2ResourceProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paDialog, paReadOnly];
end;

procedure TD2WideStringsProperty.Edit;
var
  EditDlg: TD2PathDataDesigner;
  SaveWrap: boolean;
begin
  EditDlg := TD2PathDataDesigner.Create(Application);
  EditDlg.PathData.WordWrap := false;
  EditDlg.Caption := 'WideStrings Editor';
  if GetComponent(0) is TD2Memo then
  begin
    EditDlg.PathData.WordWrap := false;
    SaveWrap := TD2Memo(GetComponent(0)).WordWrap;
    TD2Memo(GetComponent(0)).WordWrap := false;
    EditDlg.PathData.Lines.Assign(TD2WideStrings(GetOrdValue));
    TD2Memo(GetComponent(0)).WordWrap := SaveWrap;
  end
  else
  begin
    EditDlg.PathData.WordWrap := true;
    EditDlg.PathData.Lines.Assign(TD2WideStrings(GetOrdValue));
  end;
  EditDlg.previewLayout.Visible := false;
  EditDlg.labelMemo.Text := 'Type items:';
  if EditDlg.ShowModal = mrOk then
  begin
    TD2WideStrings(GetOrdValue).Text := EditDlg.PathData.Lines.Text;
    if GetComponent(0) is TD2VisualObject then
      TD2VisualObject(GetComponent(0)).Repaint;
    Modified;
  end;
  EditDlg.Free;
end;

function TD2WideStringsProperty.GetValue: String;
begin
  Result := '(WideStrings)';
end;

function TD2WideStringsProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paDialog, paReadOnly];
end;

//----------------- TD2FontProperty -----------------------------

procedure TD2FontProperty.Edit;
var
  F: TFontDialog;
begin
  F := TFontDialog.Create(Application);
  F.Font.Assign(TD2Font(GetOrdValue));
  if F.Execute then
  begin
    TD2Font(GetOrdValue).Assign(F.Font);
    Modified;
  end;
  F.Free;
end;

function TD2FontProperty.GetValue: String;
begin
  Result := '(Font)';
end;

function TD2FontProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paMultiSelect, paSubProperties, paDialog, paReadOnly];
end;

//----------------- TD2TriggerProperty -----------------------------

function TD2TriggerProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paMultiSelect, paValueList, paRevertable];
end;

procedure TD2TriggerProperty.GetValues(Proc: TGetStrProc);
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

function TD2TriggerProperty.GetValue: string;
begin
  try
    Result := GetStrValue;
  except
    on E: Exception do ShowMessage(E.Message);
  end;
end;

procedure TD2TriggerProperty.SetValue(const Value: string);
begin
  try
    SetStrValue(Value);
    Modified;
  except
    on E: Exception do ShowMessage(E.Message);
  end;
end;

//----------------- TD2LangProperty -----------------------------

function TD2LangProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paDialog, paRevertable];
end;

procedure TD2LangProperty.Edit;
begin
  ShowDsgnLang(TD2Lang(GetComponent(0)));
end;

//----------------- TD2LangEditor -----------------------------

procedure TD2LangEditor.Edit;
begin
  ShowDsgnLang(TD2Lang(Component));
end;

//----------------- TD2ImgListEditor -----------------------------

procedure TD2ImgListEditor.Edit;
begin
  ShowDsgnImageList(TD2ImageList(Component));
end;

//----------------- TD2GradientProperty -----------------------------

procedure TD2GradientProperty.Edit;
var
  D: TD2BrushDialog;
begin
  D := TD2BrushDialog.Create(Application);
  D.ShowStyles := [d2BrushGradient];
  D.ShowBrushList := false;
  D.Brush.Style := d2BrushGradient;
  D.Brush.Gradient.Assign(TD2Gradient(GetOrdValue));
  if D.Execute then
  begin
    TD2Gradient(GetOrdValue).Assign(D.Brush.Gradient);
  end;
  D.Free;
end;

function TD2GradientProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paDialog, paReadOnly];
end;

function TD2GradientProperty.GetValue: string;
begin
  Result := '(Gradient)';
end;

procedure TD2GradientProperty.SetValue(const Value: string);
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
                               TD2Scene,
                               TD2BrushDialog,
                               TD2Resources,
                               TD2Lang,
                               TD2ImageList
                               ]);
//-- 2D Components ---------------------------------

  RegisterNoIcon([
    TD2RectAnimation,
    TD2BitmapListAnimation, TD2GradientAnimation, TD2FloatKeyAnimation, TD2PathSwitcher, TD2ColorKeyAnimation, TD2PathAnimation, 
    TD2ColorAnimation, TD2FloatAnimation, TD2BitmapAnimation, TD2CalendarBox, TD2GroupBox, TD2PathCheckBox, 
    TD2CheckBox, TD2RadioButton, TD2PopupBox, TD2PopupButton, TD2CornerButton, TD2CircleButton, 
    TD2AngleButton, TD2Button, TD2PathButton, TD2ColorButton, TD2BitmapStateButton, TD2SpeedButton, 
    TD2RoundButton, TD2BitmapButton, TD2ColorPanel, TD2ComboColorBox, TD2ColorQuad, TD2GradientEdit, 
    TD2AlphaTrackBar, TD2BWTrackBar, TD2HueTrackBar, TD2ColorBox, TD2ColorPicker, TD2CompoundTrackBar, 
    TD2Calendar, TD2CompoundNumberBox, TD2CompoundTextBox, TD2CompoundPopupBox, TD2CompoundImage, TD2CompoundMemo, 
    TD2CompoundColorButton, TD2CompoundAngleBar, TD2ProgressBar, TD2CalloutPanel, TD2VertScrollBox, TD2TrackBar, 
    TD2FramedScrollBox, TD2ScrollBar, TD2ScrollBox, TD2ValueLabel, TD2AniIndicator, TD2Track,
    TD2Splitter, TD2Expander, TD2ImageControl, TD2Label, TD2FramedVertScrollBox, TD2SmallScrollBar, 
    TD2Panel, TD2TabControl, TD2DBLabel, TD2DBTextBox, TD2DBNavigator, TD2DBMemo, 
    TD2DBImage, TD2DBGrid, TD2DesignFrame, TD2Selection, TD2SelectionPoint, TD2Inspector, 
    TD2ReflectionEffect, TD2InnerGlowEffect, TD2BlurEffect, TD2ShadowEffect, TD2GlowEffect, TD2BevelEffect, 
    TD2ImageViewer, TD2DropTarget, TD2IPhoneButton, TD2Header, TD2StringGrid, TD2Grid, 
    TD2ProgressColumn, TD2ImageColumn, TD2Column, TD2PopupColumn, TD2CheckColumn, TD2HudNumberBox, 
    TD2HudScrollBar, TD2HudStringComboBox, TD2HudAlphaTrackBar, TD2HudCloseButton, TD2HudCornerButton, TD2HudSizeGrip, 
    TD2HudRadioButton, TD2HudComboColorBox, TD2HudMemo, TD2HudStringListBox, TD2HudTrack, TD2HudHorzImageListBox, 
    TD2HudSpeedButton, TD2HudWindow, TD2HudListBox, TD2HudHorzListBox, TD2HudRoundTextBox, TD2HudLabel, 
    TD2HudCircleButton, TD2HudGroupBox, TD2HudStatusBar, TD2HudPanel, TD2HudBWTrackBar, TD2HudHueTrackBar, 
    TD2HudComboTrackBar, TD2HudImageListBox, TD2HudSpinBox, TD2HudComboBox, TD2HudAngleButton, TD2HudButton, 
    TD2HudRoundButton, TD2HudCheckBox, TD2HudPopupBox, TD2HudTextBox, TD2HudComboTextBox, TD2HudTrackBar, 
    TD2HudTabControl, TD2HeaderItem, TD2ImageListBoxItem, TD2HudTabItem, TD2TabItem, TD2TreeViewItem, 
    TD2ListBoxItem, TD2GridLayout, TD2Frame, TD2Layout, TD2SplitLayout, TD2ScaledLayout, 
    TD2Nond2Layout, TD2StringComboBox, TD2HorzImageListBox, TD2StringListBox, TD2ComboBox, TD2HorzListBox,
    TD2ImageListBox, TD2ListBox, TD2PlotGrid, TD2Popup, TD2MessagePopup, TD2BitmapObject, 
    TD2PathObject, TD2BrushObject, TD2Pie, TD2ScrollArrowRight, TD2Arc, TD2Path, 
    TD2PaintBox, TD2Circle, TD2BlurRoundRect, TD2BlurRectangle, TD2Rectangle, TD2ScrollArrowLeft, 
    TD2Line, TD2CalloutRectangle, TD2Text, TD2Image, TD2SidesRectangle, TD2Ellipse, 
    TD2RoundRect, TD2TextBoxClearBtn, TD2TextBox, TD2ComboTextBox, TD2NumberBox, TD2RoundTextBox, 
    TD2Memo, TD2CalendarTextBox, TD2SpinBox, TD2ComboTrackBar, TD2StatusBar, TD2ToolButton, 
    TD2ToolPathButton, TD2ToolBar, TD2TreeView, TD2SizeGrip, TD2Background, TD2CloseButton
  ]);

  RegisterComponentEditor(TD2ImageList, TD2ImgListEditor);
  RegisterComponentEditor(TD2Lang, TD2LangEditor);

  RegisterPropertyEditor(TypeInfo(String), TD2Lang, 'Lang', TD2LangProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2Effect, 'Trigger', TD2TriggerProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2Animation, 'Trigger', TD2TriggerProperty);
  RegisterPropertyEditor(TypeInfo(String), TD2Animation, 'TriggerInverse', TD2TriggerProperty);
  RegisterPropertyEditor(TypeInfo(TD2Gradient), nil, '', TD2GradientProperty);
  RegisterPropertyEditor(TypeInfo(TD2Brush), nil, '', TD2BrushProperty);
  RegisterPropertyEditor(TypeInfo(TD2Bitmap), nil, '', TD2BitmapProperty);
  RegisterPropertyEditor(TypeInfo(TD2PathData), nil, '', TD2PathDataProperty);
  RegisterPropertyEditor(TypeInfo(TD2WideStrings), nil, '', TD2WideStringsProperty);
  RegisterPropertyEditor(TypeInfo(TD2Font), nil, '', TD2FontProperty);
  RegisterPropertyEditor(TypeInfo(TStrings), TD2Resources, 'Resource', TD2ResourceProperty);

end;

initialization

  GvarD2Designer := TD2IDEDesigner.Create(nil);

finalization

  GvarD2Designer.Free;
  GvarD2Designer := nil;
end.

