{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2CustomButton ================================

procedure TD2CustomButton.ApplyStyle;
begin
  inherited;
  StartTriggerAnimation(Self,'IsPressed');
  ApplyTriggerEffect(Self,'IsPressed');
end;

constructor TD2CustomButton.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=true;
  FWordWrap:=false;
  Width:=80;
  Height:=22;
  AutoCapture:=true;
  CanFocused:=true;
end;

destructor TD2CustomButton.Destroy;
begin
  if FRepeatTimer <> nil then
    FRepeatTimer.Free;
  inherited;
end;

procedure TD2CustomButton.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  inherited;
  if (Key=VK_RETURN) or (Key=VK_SPACE) then
  begin
    Click;
  end;
end;

procedure TD2CustomButton.DoRepeatTimer(Sender:TObject);
begin
  Click;
end;

procedure TD2CustomButton.DoRepeatDelayTimer(Sender:TObject);
begin
  FRepeatTimer.OnTimer:=DoRepeatTimer;
  FRepeatTimer.Interval:=100;
end;

procedure TD2CustomButton.DblClick;
begin
  inherited;
  Click;
end;

procedure TD2CustomButton.Click;
var
  O:TComponent;
begin
  if Assigned(Self) and (ModalResult <> mrNone) then
  begin
    O:=Scene.GetComponent;
    while O <> nil do
    begin
      if (O is TCustomForm) then
      begin
        TCustomForm(O).ModalResult:=FModalResult;
        Break;
      end;
      O:=O.Owner;
    end;
  end;
  inherited;
end;

procedure TD2CustomButton.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  if Button=mbLeft then
  begin
    FPressing:=true;
    if FStaysPressed then
      FIsPressed:=not FIsPressed
    else
    begin
      FIsPressed:=true;
      if FRepeat then
      begin
        if FRepeatTimer=nil then
        begin
          FRepeatTimer:=TD2Timer.Create(Self);
          FRepeatTimer.Interval:=500;
        end;
        FRepeatTimer.OnTimer:=DoRepeatDelayTimer;
        FRepeatTimer.Enabled:=true;
      end;
    end;
    StartTriggerAnimation(Self,'IsPressed');
    ApplyTriggerEffect(Self,'IsPressed');
  end;
end;

procedure TD2CustomButton.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
begin
  inherited;
  if (ssLeft in Shift) and (FPressing) then
  begin
    if FIsPressed <> d2PtInRect(d2Point(X,Y),LocalRect) then
    begin
      if not FStaysPressed then
      begin
        FIsPressed:=d2PtInRect(d2Point(X,Y),LocalRect);
        StartTriggerAnimation(Self,'IsPressed');
        ApplyTriggerEffect(Self,'IsPressed');
      end;
    end;
  end;
end;

procedure TD2CustomButton.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  if FPressing then
  begin
    if FRepeatTimer <> nil then
      FRepeatTimer.Enabled:=false;
    FPressing:=false;
    if not FStaysPressed then
    begin
      FIsPressed:=false;
      StartTriggerAnimation(Self,'IsPressed');
      ApplyTriggerEffect(Self,'IsPressed');
    end;
  end;
  inherited;
end;

procedure TD2CustomButton.SetData(const Value:Variant);
begin
  if VarIsEvent(Value) then
    OnClick:=VariantToEvent(Value);
end;

procedure TD2CustomButton.SetIsPressed(const Value:boolean);
begin
  if FStaysPressed then
  begin
    if Value <> FIsPressed then
    begin
      FIsPressed:=Value;
      StartTriggerAnimation(Self,'IsPressed');
      ApplyTriggerEffect(Self,'IsPressed');
    end;
  end;
end;

//=============== TD2Button ==================================

procedure TD2Button.DialogKey(var Key:Word; Shift:TShiftState);
begin
  inherited;
  if Default and (Key=VK_RETURN) then
  begin
    Click;
    Key:=0;
  end;
  if Cancel and (Key=VK_ESCAPE) then
  begin
    Click;
    Key:=0;
  end;
end;

//=============== TD2CircleButton =============================

constructor TD2CircleButton.Create(AOwner:TComponent);
begin
  inherited;
  Width:=21;
  Height:=21;
end;

//=============== TD2PopupButton =================================

constructor TD2PopupButton.Create(AOwner:TComponent);
begin
  inherited;
end;

procedure TD2PopupButton.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FPopupMenu) then
    FPopupMenu:=nil;
end;

procedure TD2PopupButton.Click;
var
  VP:TD2Point;
begin
  inherited;
  if FPopupMenu=nil then Exit;
  if Scene <> nil then
  begin
    VP:=LocalToAbsolute(d2Point(0,Height));
    VP:=Scene.LocalToScreen(VP);
    FPopupMenu.Popup(round(VP.X),round(VP.Y));
  end;
end;

//=============== TD2BitmapButton ==================================

constructor TD2BitmapButton.Create(AOwner:TComponent);
begin
  inherited;
  FBitmap:=TD2Bitmap.Create(1,1);
  FBitmap.OnChange:=DoBitmapChanged;
  FBitmapLayout:=d2GlyphLeft;
  FBitmapSize:=32;
  FBitmapPadding:=2;
  Width:=50;
  Height:=60;
end;

destructor TD2BitmapButton.Destroy;
begin
  FreeAndNil(FBitmap);
  inherited;
end;

procedure TD2BitmapButton.ActionChange(Sender:TObject; CheckDefaults:boolean);

  procedure CopyImage(ImageList:TCustomImageList; Index:integer);
  begin
    BitmapSize:=ImageList.Width;
    if ImageList is TD2ImageList then
      Bitmap.Assign(TD2ImageList(ImageList).Images[Index]);
  end;

begin
  inherited ActionChange(Sender,CheckDefaults);
  if Sender is TCustomAction then
    with TCustomAction(Sender) do
    begin
      { Copy image from action's imagelist }
      if (Bitmap.Width <= 1) and (ActionList <> nil) and (ActionList.Images <> nil) and (ActionList.Images is TD2ImageList) and
        (ImageIndex >= 0) and (ImageIndex < TD2ImageList(ActionList.Images).Count) then
        CopyImage(ActionList.Images,ImageIndex);
    end;
end;

procedure TD2BitmapButton.ApplyStyle;
var
  T:TD2Object;
begin
  inherited;
  DoBitmapChanged(Self);
  T:=FindResource('text');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    TD2VisualObject(T).Padding.Rect:=d2Rect(FBitmapPadding,FBitmapPadding,FBitmapPadding,FBitmapPadding);
  end;
end;

procedure TD2BitmapButton.DoBitmapChanged(Sender:TObject);
var
  T:TD2Object;
begin
  T:=FindResource('image');
  if (T <> nil) and (T is TD2Image) then
  begin
    TD2Image(T).Bitmap.Assign(FBitmap);
    TD2VisualObject(T).Padding.Rect:=d2Rect(FBitmapPadding,FBitmapPadding,FBitmapPadding,FBitmapPadding);
    if ((TD2Image(T).Bitmap.Width=1) or (TD2Image(T).Bitmap.Height=1)) and (TD2Image(T).Bitmap.ResourceName='') then
    begin
      TD2VisualObject(T).Align:=vaNone;
    end
    else
      case FBitmapLayout of
        d2GlyphLeft:begin
          TD2VisualObject(T).Align:=vaLeft;
          TD2VisualObject(T).Width:=FBitmapSize;
          TD2VisualObject(T).Padding.right:=FBitmapSpacing;
        end;
        d2GlyphRight:begin
          TD2VisualObject(T).Align:=vaRight;
          TD2VisualObject(T).Width:=FBitmapSize;
          TD2VisualObject(T).Padding.left:=FBitmapSpacing;
        end;
        d2GlyphTop:begin
          TD2VisualObject(T).Align:=vaTop;
          TD2VisualObject(T).Height:=FBitmapSize;
          TD2VisualObject(T).Padding.bottom:=FBitmapSpacing;
        end;
        d2GlyphBottom:begin
          TD2VisualObject(T).Align:=vaBottom;
          TD2VisualObject(T).Height:=FBitmapSize;
          TD2VisualObject(T).Padding.top:=FBitmapSpacing;
        end;
        d2GlyphCenter:begin
          TD2VisualObject(T).Align:=vaCenter;
          TD2VisualObject(T).Width:=FBitmapSize;
          TD2VisualObject(T).Height:=FBitmapSize;
          TD2VisualObject(T).Padding.Rect:=d2Rect(FBitmapSpacing,FBitmapSpacing,FBitmapSpacing,FBitmapSpacing);
        end;
      end;
  end;
end;

procedure TD2BitmapButton.SetBitmap(const Value:TD2Bitmap);
begin
  FBitmap.Assign(Value);
end;

procedure TD2BitmapButton.SetBitmapLayout(const Value:TD2ButtonLayout);
begin
  if FBitmapLayout <> Value then
  begin
    FBitmapLayout:=Value;
    ApplyStyle;
  end;
end;

procedure TD2BitmapButton.SetBitmapSpacing(const Value:single);
begin
  if FBitmapSpacing <> Value then
  begin
    FBitmapSpacing:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2BitmapButton.SetBitmapSize(const Value:single);
begin
  if FBitmapSize <> Value then
  begin
    FBitmapSize:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2BitmapButton.SetBitmapPadding(const Value:single);
begin
  if FBitmapPadding <> Value then
  begin
    FBitmapPadding:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

//=============== TD2PathButton =================================

constructor TD2PathButton.Create(AOwner:TComponent);
begin
  inherited;
  FPath:=TD2PathData.Create;
  FPath.OnChanged:=DoPathChanged;
  FPathLayout:=d2GlyphLeft;
  FPathSize:=32;
  FPathPadding:=2;
  FFill:=TD2Brush.Create(d2BrushSolid,$FFFFFFFF);
  FFill.OnChanged:=DoPathChanged;
  FStroke:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FStroke.SolidColor:=$FF000000;
  FStroke.OnChanged:=DoPathChanged;
  FStrokeThickness:=1;
  Width:=50;
  Height:=60;
end;

destructor TD2PathButton.Destroy;
begin
  FFill.Free;
  FStroke.Free;
  FPath.Free;
  inherited;
end;

procedure TD2PathButton.ApplyStyle;
var
  T:TD2Object;
begin
  inherited;
  T:=FindResource('path');
  if (T <> nil) and (T is TD2Path) then
  begin
    TD2Path(T).Data.Assign(FPath);
    TD2Path(T).Fill.Assign(FFill);
    TD2Path(T).Stroke.Assign(FStroke);
    TD2Path(T).StrokeThickness:=FStrokeThickness;
    TD2Path(T).StrokeCap:=FStrokeCap;
    TD2Path(T).StrokeJoin:=FStrokeJoin;
    TD2Path(T).StrokeDash:=FStrokeDash;
  end;
  T:=FindResource('pathowner');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    TD2VisualObject(T).Padding.Rect:=d2Rect(FPathPadding,FPathPadding,FPathPadding,FPathPadding);
    if (Length(FPath.PathData)=0) then
    begin
      TD2VisualObject(T).Align:=vaNone;
    end
    else
      case FPathLayout of
        d2GlyphLeft:begin
          TD2VisualObject(T).Align:=vaLeft;
          TD2VisualObject(T).Width:=FPathSize;
          TD2VisualObject(T).Padding.right:=FPathSpacing;
        end;
        d2GlyphRight:begin
          TD2VisualObject(T).Align:=vaRight;
          TD2VisualObject(T).Width:=FPathSize;
          TD2VisualObject(T).Padding.left:=FPathSpacing;
        end;
        d2GlyphTop:begin
          TD2VisualObject(T).Align:=vaTop;
          TD2VisualObject(T).Height:=FPathSize;
          TD2VisualObject(T).Padding.bottom:=FPathSpacing;
        end;
        d2GlyphBottom:begin
          TD2VisualObject(T).Align:=vaBottom;
          TD2VisualObject(T).Height:=FPathSize;
          TD2VisualObject(T).Padding.top:=FPathSpacing;
        end;
        d2GlyphCenter:begin
          TD2VisualObject(T).Align:=vaCenter;
          TD2VisualObject(T).Width:=FPathSize;
          TD2VisualObject(T).Height:=FPathSize;
          TD2VisualObject(T).Padding.Rect:=d2Rect(FPathSpacing,FPathSpacing,FPathSpacing,FPathSpacing);
        end;
      end;
  end;
  T:=FindResource('text');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    TD2VisualObject(T).Padding.Rect:=d2Rect(FPathPadding,FPathPadding,FPathPadding,FPathPadding);
  end;
end;

procedure TD2PathButton.DoPathChanged(Sender:TObject);
begin
  Repaint;
  ApplyStyle;
end;

procedure TD2PathButton.SetPath(const Value:TD2PathData);
begin
  FPath.Assign(Value);
end;

procedure TD2PathButton.SetPathLayout(const Value:TD2ButtonLayout);
begin
  if FPathLayout <> Value then
  begin
    FPathLayout:=Value;
    ApplyStyle;
  end;
end;

procedure TD2PathButton.SetPathPadding(const Value:single);
begin
  if FPathPadding <> Value then
  begin
    FPathPadding:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2PathButton.SetPathSize(const Value:single);
begin
  if FPathSize <> Value then
  begin
    FPathSize:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2PathButton.SetPathSpacing(const Value:single);
begin
  if FPathSpacing <> Value then
  begin
    FPathSpacing:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

function TD2PathButton.isStrokeThicknessStored:boolean;
begin
  Result:=FStrokeThickness <> 1;
end;

procedure TD2PathButton.SetFill(const Value:TD2Brush);
begin
  FFill:=Value;
end;

procedure TD2PathButton.SetStroke(const Value:TD2Brush);
begin
  FStroke:=Value;
end;

procedure TD2PathButton.SetStrokeCap(const Value:TD2StrokeCap);
begin
  if FStrokeCap <> Value then
  begin
    FStrokeCap:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2PathButton.SetStrokeDash(const Value:TD2StrokeDash);
begin
  if FStrokeDash <> Value then
  begin
    FStrokeDash:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2PathButton.SetStrokeJoin(const Value:TD2StrokeJoin);
begin
  if FStrokeJoin <> Value then
  begin
    FStrokeJoin:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

procedure TD2PathButton.SetStrokeThickness(const Value:single);
begin
  if FStrokeThickness <> Value then
  begin
    FStrokeThickness:=Value;
    if not (csLoading in ComponentState) then
      ApplyStyle;
  end;
end;

//=============== TD2ToolButton ==================================

constructor TD2ToolButton.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=false;
  FBitmapLayout:=d2GlyphTop;
end;

//=============== TD2ToolPathButton ==============================

constructor TD2ToolPathButton.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=false;
  FPathLayout:=d2GlyphTop;
end;

//=============== TD2SpeedButton ===================================

constructor TD2SpeedButton.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=false;
  FRepeat:=true;
  Width:=23;
  Height:=23;
  TextW:='';
end;

destructor TD2SpeedButton.Destroy;
begin
  inherited;
end;

//=============== TD2ColorButton =====================================

constructor TD2ColorButton.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=false;
  FColor:=vcBlack;
  FUseStandardDialog:=true;
end;

destructor TD2ColorButton.Destroy;
begin
  inherited;
end;

procedure TD2ColorButton.ApplyStyle;
var
  O:TD2Object;
begin
  inherited;
  O:=FindResource('fill');
  if (O <> nil) and (O is TD2Shape) then
  begin
    FFill:=TD2Shape(O);
    FFill.Fill.Color:=FColor;
  end;
end;

procedure TD2ColorButton.FreeStyle;
begin
  inherited;
  FFill:=nil;
end;

procedure TD2ColorButton.SetColor(const Value:string);
begin
  FColor:=Value;
  if FFill <> nil then
    FFill.Fill.Color:=FColor;
  if not (csLoading in ComponentState) then
    if Assigned(FOnChange) then
      FOnChange(Self);
end;

function SwapColor(const C:TD2Color):TD2Color;
begin
  Result:=C;
  TD2ColorRec(Result).R:=TD2ColorRec(C).B;
  TD2ColorRec(Result).B:=TD2ColorRec(C).R;
end;

procedure TD2ColorButton.Click;
var
  C:TColorDialog;
  V:TD2BrushDialog;
begin
  inherited;
  if FUseStandardDialog then
  begin
    C:=TColorDialog.Create(nil);
    C.Color:=SwapColor(d2StrToColor(FColor)) and $FFFFFF;
    if C.Execute then
    begin
      Color:=d2ColorToStr($FF000000 or SwapColor(C.Color));
    end;
    C.Free;
  end
  else
  begin
    V:=TD2BrushDialog.Create(nil);
    V.Brush.Style:=d2BrushSolid;
    V.Brush.Color:=FColor;
    V.ShowStyles:=[d2BrushSolid];
    if V.Execute then
    begin
      Color:=V.Brush.Color;
    end;
    V.Free;
  end;
end;

//=============== TD2CornerButton ===============================

constructor TD2CornerButton.Create(AOwner:TComponent);
begin
  inherited;
  FCorners:=AllCorners;
  FxRadius:=3;
  FyRadius:=3;
  FSides:=AllSides;
end;

destructor TD2CornerButton.Destroy;
begin

  inherited;
end;

procedure TD2CornerButton.ApplyStyle;
var
  Background:TD2Object;
begin
  inherited;
  Background:=FindResource('Background');
  if (Background <> nil) and (Background is TD2Rectangle) then
  begin
    TD2Rectangle(Background).CornerType:=FCornerType;
    TD2Rectangle(Background).Corners:=FCorners;
    TD2Rectangle(Background).xRadius:=xRadius;
    TD2Rectangle(Background).yRadius:=yRadius;
    TD2Rectangle(Background).Sides:=FSides;
  end;
end;

function TD2CornerButton.IsCornersStored:boolean;
begin
  Result:=FCorners <> AllCorners;
end;

function TD2CornerButton.IsSidesStored:boolean;
begin
  Result:=FSides * AllSides <> []
end;

procedure TD2CornerButton.SetCorners(const Value:TD2Corners);
begin
  if FCorners <> Value then
  begin
    FCorners:=Value;
    ApplyStyle;
  end;
end;

procedure TD2CornerButton.SetCornerType(const Value:TD2CornerType);
begin
  if FCornerType <> Value then
  begin
    FCornerType:=Value;
    ApplyStyle;
  end;
end;

procedure TD2CornerButton.SetxRadius(const Value:single);
begin
  if FxRadius <> Value then
  begin
    FxRadius:=Value;
    ApplyStyle;
  end;
end;

procedure TD2CornerButton.SetyRadius(const Value:single);
begin
  if FyRadius <> Value then
  begin
    FyRadius:=Value;
    ApplyStyle;
  end;
end;

procedure TD2CornerButton.SetSides(const Value:TD2Sides);
begin
  if FSides <> Value then
  begin
    FSides:=Value;
    ApplyStyle;
  end;
end;


//=============== TD2CloseButton ======================================

constructor TD2CloseButton.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=false;
  CloseForm:=true;
  Width:=20;
  Height:=20;
end;

destructor TD2CloseButton.Destroy;
begin

  inherited;
end;

procedure TD2CloseButton.MouseDown(Button:TMouseButton;
  Shift:TShiftState; X,Y:single);
begin
  inherited;
  if Button=mbLeft then
    FPressing:=true;
end;

procedure TD2CloseButton.MouseMove(Shift:TShiftState; X,Y,Dx,
  Dy:single);
begin
  inherited;
end;

procedure TD2CloseButton.MouseUp(Button:TMouseButton; Shift:TShiftState;
  X,Y:single);
var
  O:TComponent;
begin
  inherited;
  if FPressing then
  begin
    if FCloseForm and (Scene <> nil) then
    begin
      O:=Scene.GetComponent;
      while O <> nil do
      begin
        if (O is TCustomForm) then
        begin
          TCustomForm(O).Close;
          Break;
        end;
        O:=O.Owner;
      end;
    end;
    FPressing:=false;
    if Assigned(FOnClick) then
      FOnClick(Self);
  end;
end;
               
//=============== TD2ExpanderButton =================================================

constructor TD2ExpanderButton.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=false;
end;

destructor TD2ExpanderButton.Destroy;
begin
  inherited;
end;        

//=============== TD2IPhoneButton ======================================================

constructor TD2IPhoneButton.Create(AOwner:TComponent);
begin
  inherited;
  FBackground:=TD2Brush.Create(d2BrushSolid,$FF808080);
  FBackground.OnChanged:=BackChanged;
end;

destructor TD2IPhoneButton.Destroy;
begin
  FBackground.Free;
  inherited;
end;

procedure TD2IPhoneButton.BackChanged(Sender:TObject);
var
  T:TD2Object;
begin
  T:=FindResource('background');
  if (T <> nil) and (T is TD2Shape) then
    TD2Shape(T).Fill.Assign(FBackground);
end;

procedure TD2IPhoneButton.ApplyStyle;
var
  T:TD2Object;
begin
  inherited ;
  T:=FindResource('background');
  if (T <> nil) and (T is TD2Shape) then
    TD2Shape(T).Fill.Assign(FBackground);
end;

procedure TD2IPhoneButton.SetBackground(const Value:TD2Brush);
begin
  
end;    

//=============== TD2AngleButton =================================================

constructor TD2AngleButton.Create(AOwner:TComponent);
begin
  inherited;
  Width:=30;
  Height:=30;
  FFrequency:=0;
  FTracking:=true;
  AutoCapture:=true;
end;

destructor TD2AngleButton.Destroy;
begin
  inherited;
end;

function TD2AngleButton.GetData:Variant;
begin
  Result:=Value;
end;

procedure TD2AngleButton.SetData(const Value:Variant);
begin
  if VarIsEvent(Value) then
    OnChange:=VariantToEvent(Value)
  else
    if VarIsNumeric(Value) then
      Self.Value:=Value
    else
      Self.Value:=0
end;

procedure TD2AngleButton.ApplyStyle;
begin
  inherited;
  Tick;
  Text;
end;

function TD2AngleButton.Tick:TD2VisualObject;
var
  T:TD2Object;
begin
  T:=FindResource('tick');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    Result:=TD2VisualObject(T);
    Result.RotateAngle:=-FValue;
  end
  else
    Result:=nil;
end;

function TD2AngleButton.Text:TD2Text;
var
  T:TD2Object;
begin
  T:=FindResource('tracktext');
  if (T <> nil) and (T is TD2Text) then
  begin
    TD2Text(T).Visible:=false; //FPressing;
    TD2Text(T).TextW:=IntToStr(Round(Value)) + System.WideChar($B0);
    if FPressing and not FTracking then
      TD2Text(T).Opacity:=1
    else
      TD2Text(T).Opacity:=0;
  end;

  T:=FindResource('text');
  if (T <> nil) and (T is TD2Text) then
  begin
    Result:=TD2Text(T);
    Result.Visible:=FShowValue;
    Result.TextW:=IntToStr(Round(Value)) + System.WideChar($B0);
    if not FShowValue then
      Result.Opacity:=0
    else
      Result.Opacity:=1;
  end
  else
    Result:=nil;
end;

procedure TD2AngleButton.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  if Button=mbLeft then
  begin
    FPressing:=true;
    FOldPos:=d2Point(X,Y);
    FSaveValue:=Value;
    Text;
  end;
end;

procedure TD2AngleButton.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
begin
  inherited;
  if (ssLeft in Shift) and (FPressing) then
  begin
    Value:=d2VectorAngle(d2Vector(1,0),d2Vector(X - (Width / 2),Y - (Height / 2)));
    FOldPos:=d2Point(X,Y);
    Text;
  end;
end;

procedure TD2AngleButton.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  if FPressing then
  begin
    FPressing:=false;
    Text;
    if Value <> FSaveValue then
      if Assigned(FOnChange) then
        FOnChange(Self);
  end;
end;

procedure TD2AngleButton.Paint;
begin
  inherited;
end;

procedure TD2AngleButton.SetValue(const Value:single);
begin
  if (FFrequency=0) then
  begin
    if (FValue <> Value) then
    begin
      FValue:=Value;
      if Tick <> nil then
        Tick.RotateAngle:=-FValue
      else
        Repaint;
      Text;
      if Assigned(FOnChange) and (not FPressing or FTracking) then
        FOnChange(Self);
    end;
  end
  else
  begin
    if FValue <> Round(Value / FFrequency) * FFrequency then
    begin
      FValue:=Round(Value / FFrequency) * FFrequency;
      if Tick <> nil then
        Tick.RotateAngle:=-FValue
      else
        Repaint;
      Text;
      if Assigned(FOnChange) and (not FPressing or FTracking) then
        FOnChange(Self);
    end;
  end;
end;

procedure TD2AngleButton.SetShowValue(const Value:boolean);
begin
  if FShowValue <> Value then
  begin
    FShowValue:=Value;
    Text;
    Repaint;
  end;
end;

procedure TD2TrackBar.Loaded;
begin
  inherited;
end;

procedure TD2TrackBar.SetMax(const Value:single);
begin
  if FMax <> Value then
  begin
    inherited;
  end;
end;

procedure TD2TrackBar.SetMin(const Value:single);
begin
  inherited;
  if FMin <> Value then
  begin
    inherited;
  end;
end;


//=============== TD2BitmapStateButton =================================================

constructor TD2BitmapStateButton.Create(AOwner:TComponent);
begin
  inherited;
  FBitmap:=TD2Bitmap.Create(1,1);
  FBitmap.OnChange:=DoBitmapChanged;
  FBitmapHot:=TD2Bitmap.Create(1,1);
  FBitmapDown:=TD2Bitmap.Create(1,1);
  Width:=64;
  Height:=64;
end;

destructor TD2BitmapStateButton.Destroy;
begin
  FBitmap.Free;
  FBitmapHot.Free;
  FBitmapDown.Free;
  inherited;
end;

procedure TD2BitmapStateButton.ApplyStyle;
begin
  inherited;
end;

procedure TD2BitmapStateButton.DoBitmapChanged(Sender:TObject);
begin
  Repaint;
end;

procedure TD2BitmapStateButton.SetBitmap(const Value:TD2Bitmap);
begin
end;

procedure TD2BitmapStateButton.SetBitmapDown(const Value:TD2Bitmap);
begin
end;

procedure TD2BitmapStateButton.SetBitmapHot(const Value:TD2Bitmap);
begin
end;

procedure TD2BitmapStateButton.Paint;
var
  scale:single;
  R:TD2Rect;
  B:TD2Bitmap;
begin
  if IsPressed then
    B:=FBitmapDown
  else
    if IsMouseOver then
      B:=FBitmapHot
    else
      B:=FBitmap;

  R:=d2Rect(0,0,B.Width,B.Height);
  scale:=d2FitRect(R,LocalRect);
  if scale > 1 then
    Canvas.DrawBitmap(B,d2Rect(0,0,B.Width,B.Height),R,AbsoluteOpacity)
  else
  begin
    R:=d2Rect(0,0,B.Width,B.Height);
    d2RectCenter(R,LocalRect);
    Canvas.DrawBitmap(B,d2Rect(0,0,B.Width,B.Height),R,AbsoluteOpacity)
  end;
end;

procedure TD2BitmapStateButton.MouseEnter;
begin
  inherited;
  Repaint; 
end;

procedure TD2BitmapStateButton.MouseLeave;
begin
  inherited;
  Repaint;
end;

procedure TD2BitmapStateButton.StartTriggerAnimation(AInstance:TD2Object;
  ATrigger:string);
begin
  inherited;
  if Pos('IsPressed',ATrigger) > 0 then
  begin
    Repaint;
  end;
end;
        
                                 
