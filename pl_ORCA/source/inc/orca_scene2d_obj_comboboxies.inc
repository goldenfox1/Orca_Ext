{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2ComboBox ===================================================

constructor TD2ComboBox.Create(AOwner:TComponent);
begin
  inherited;
  DropDownCount:=8;
  CanFocused:=true;
  FPopup:=TD2Popup.Create(Self);
  FPopup.PlacementTarget:=Self;
  FPopup.StaysOpen:=false;
  FPopup.Stored:=false;
  FPopup.Parent:=Self;
  FPopup.Locked:=true;
  FPopup.DesignHide:=true;
  FListBox:=TD2ComboListBox.Create(Self);
  FListBox.Parent:=FPopup;
  FListBox.FComboBox:=Self;
  FListBox.Stored:=false;
  FListBox.Align:=vaClient;
  FListBox.ShowCheckboxes:=false;
  Width:=100;
  Height:=22;
end;

destructor TD2ComboBox.Destroy;
begin
  inherited;
end;

function TD2ComboBox.ItemClass:string;
begin
  Result:='TD2ListBoxItem';
end;

procedure TD2ComboBox.ApplyStyle;
var
  T:TD2Object;
begin
  inherited;
  T:=FindResource('Content');
  if (T <> nil) and (T is TD2Content) then
  begin
    TD2Content(T).OnPaint:=DoContentPaint;
    if (FListBox <> nil) and (FListBox.ItemByIndex(ItemIndex) <> nil) then
    begin
      FListBox.ItemByIndex(ItemIndex).Width:=TD2Content(T).Width;
    end;
  end;
end;

procedure TD2ComboBox.Realign;
begin
  inherited;
  if FDisableAlign then Exit;
  FDisableAlign:=true;
 
  if FPopup <> nil then
    begin                                                                    
       FPopup.Width:=Width;
       FListBox.Width:=Width;                                             
    end;  
                                                                
  FDisableAlign:=false;
end;

procedure TD2ComboBox.Paint;
begin
  inherited;
end;

procedure TD2ComboBox.PaintChildren;
begin
  inherited;
end;

procedure TD2ComboBox.DoContentPaint(Sender:TObject;const Canvas:TD2Canvas; const ARect:TD2Rect);
var
  SOpacity:single;
  Item:TD2ListBoxItem;
  SaveSize:TD2Point;
  SaveScene:Id2Scene;

begin
  if FListBox <> nil then
  begin
    Item:=FListBox.ItemByIndex(FListBox.ItemIndex);
    if Item <> nil then
    begin
      Item.ApplyResource;
      SOpacity:=Item.FAbsoluteOpacity;
      SaveSize:=d2Point(Item.Width,Item.Height);
      SaveScene:=Item.Scene;
      Item.SetNewScene(Scene);
      Item.SetSizeWithoutChange(d2RectWidth(ARect),d2RectHeight(ARect));
      Item.FAbsoluteOpacity:=Opacity;
      Item.RecalcOpacity;
      Item.FRecalcOpacity:=false;
      Item.PaintTo(Canvas,ARect,TD2Object(Sender));
      Item.FAbsoluteOpacity:=SOpacity;
      Item.RecalcOpacity;
      Item.SetSizeWithoutChange(SaveSize.X,SaveSize.Y);
      Item.SetNewScene(SaveScene);
    end;
  end;
end;

procedure TD2ComboBox.DropDown;
var
  Count,i:integer;
begin
  if not FPopup.IsOpen then
  begin
    FPopup.Placement:=FPlacement;
    FPopup.Width:=Width;
    Count:=DropDownCount;
    if FListBox.Count < Count then
    Count:=FListBox.Count;
    if FListbox.ItemHeight > 0 then
      FPopup.Height:=(Count * FListbox.ItemHeight) + 4
    else
      FPopup.Height:=Count * (Height - 4);
    FPopup.IsOpen:=true;
    if FPopup.IsOpen then
      FListBox.SetFocus;
  end
  else
  begin
    FPopup.IsOpen:=false;
  end;
end;

procedure TD2ComboBox.MouseDown(Button:TMouseButton; Shift:TShiftState;
  X,Y:single);
begin
  inherited;
  if Button=mbLeft then
  begin
    DropDown;
  end;
end;

procedure TD2ComboBox.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
begin
  inherited;
end;

procedure TD2ComboBox.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
end;

procedure TD2ComboBox.MouseWheel(Shift:TShiftState; WheelDelta:integer;
  var Handled:boolean);
begin
  inherited;
  if WheelDelta < 0 then
  begin
    if ItemIndex < Count - 1 then
      ItemIndex:=ItemIndex + 1
  end
  else
  begin
    if ItemIndex > 0 then
      ItemIndex:=ItemIndex - 1;
  end;
  Handled:=true;             
end;

procedure TD2ComboBox.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
var
  i:integer;
begin
  inherited ;
  if Count > 0 then
  begin

    if KeyChar <> #0 then
    begin
      for i:=0 to Count - 1 do
        if (FListBox.Items[i].TextW <> '') and (WideLowerCase(FListBox.Items[i].TextW[1])=WideLowerCase(KeyChar)) then
        begin
          ItemIndex:=i;
          Break;
        end;
      KeyChar:=#0;
    end;

    case Key of
      VK_HOME:ItemIndex:=0;
      VK_END:ItemIndex:=Count - 1;
      VK_UP:If ItemIndex > 0 then
        begin
          ItemIndex:=ItemIndex - 1;
          if ItemIndex < 0 then ItemIndex:=0;
        end;
      VK_DOWN:
        begin
          if ssAlt in Shift then
          begin
            DropDown;
          end
          else
          begin
            If ItemIndex < Count - 1 then ItemIndex:=ItemIndex + 1;
            if ItemIndex > Count - 1 then ItemIndex:=Count - 1;
          end;
        end;
      VK_LEFT:If ItemIndex > 0 then ItemIndex:=ItemIndex - 1;
      VK_RIGHT:If ItemIndex < Count - 1 then ItemIndex:=ItemIndex + 1;
    else
      Exit;
    end;

    Key:=0;
  end;
end;

procedure TD2ComboBox.Clear;
begin
  if FListBox <> nil then FListBox.Clear;
end;

procedure TD2ComboBox.AddObject(AObject:TD2Object);
begin
  if (FListBox <> nil) and ((AObject is TD2ListBoxItem) or (AObject is TD2SelectionItem)) then
  begin
    FListBox.AddObject(AObject);
  end else
    inherited;
end;

procedure TD2ComboBox.DoListBoxChange(Sender:TObject);
begin
 //....
end;

function TD2ComboBox.GetItemIndex:integer;
begin
  if FListBox <> nil then
    Result:=FListBox.ItemIndex
  else
    Result:=-1;
end;

function TD2ComboBox.GetCount:integer;
begin
  if FListBox <> nil then
    Result:=FListBox.Count
  else
    Result:=0;
end;

procedure TD2ComboBox.SetItemIndex(const Value:integer);
begin
  if FListBox <> nil then
  begin
    FListBox.ItemIndex:=Value;
    if Assigned(FBindingObjects) then
      ToBindingObjects;
    if Assigned(FOnChange) then
      FOnChange(Self);
    if (FResourceLink <> nil) then
      FResourceLink.Visual.UpdateEffects;
    Repaint;
  end;
end;

procedure TD2ComboBox.DesignClick;
begin
  inherited ;
  FPopup.DesignHide:=not FPopup.DesignHide;
  FPopup.Width:=Width;
  if FListbox.ItemHeight > 0 then
    FPopup.Height:=DropDownCount * FListbox.ItemHeight
  else
    FPopup.Height:=DropDownCount * (Height - 4);
  FPopup.Position.X:=0;
  FPopup.Position.Y:=Height;
  FPopup.Locked:=FPopup.DesignHide;
end;

procedure TD2ComboBox.GetChildren(Proc:TGetChildProc; Root:TComponent);
var
  j:integer;
begin
  inherited;
  if (FListBox <> nil) and (FListBox.FContent <> nil) then
    if (FListBox.FContent.ChildrenCount > 0) then
    begin
      for j:=0 to FListBox.FContent.ChildrenCount - 1 do
        if FListBox.FContent.Children[j].Stored then
          Proc(TComponent(FListBox.FContent.Children[j]));
    end;
end;

function TD2ComboBox.GetListBoxResource:string;
begin
  Result:=FListBox.Resource;
end;

procedure TD2ComboBox.SetListBoxResource(const Value:string);
begin
  FListBox.Resource:=Value;
end;

function TD2ComboBox.GetItemHeight:single;
begin
  Result:=FListBox.ItemHeight;
end;

procedure TD2ComboBox.SetItemHeight(const Value:single);
begin
  FListBox.ItemHeight:=Value;
end;
          

//=============== TD2ComboListBox ========================================================================

constructor TD2ComboListBox.Create(AOwner:TComponent);
begin
  inherited;
  HideSelectionUnfocused:=false;
end;

destructor TD2ComboListBox.Destroy;
begin
  inherited;
end;

procedure TD2ComboListBox.ApplyResource;
begin
  inherited;
end;

procedure TD2ComboListBox.GetChildren(Proc:TGetChildProc; Root:TComponent);
begin
  inherited ;
end;

function TD2ComboListBox.GetParentComponent:TComponent;
begin
  Result:=inherited GetParentComponent;
end;

procedure TD2ComboListBox.KillFocus;
begin
  inherited;
end;

procedure TD2ComboListBox.MouseDown(Button:TMouseButton; Shift:TShiftState; X,Y:single);
begin
  inherited;
end;

procedure TD2ComboListBox.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
begin
  inherited;
end;

procedure TD2ComboListBox.KeyDown(var Key:Word; var KeyChar:System.WideChar; Shift:TShiftState);
begin
  inherited;
  if Key=VK_RETURN then
  begin
    if (Parent is TD2Popup) and (TD2Popup(Parent).IsOpen) and (FComboBox <> nil) and (FComboBox is TD2ComboBox) then
    begin
      TD2ComboBox(FComboBox).ItemIndex:=ItemIndex;
      TD2Popup(Parent).IsOpen:=false;
    end;
    if (Parent is TD2Popup) and (TD2Popup(Parent).IsOpen) and (FComboBox <> nil) and (FComboBox is TD2ComboTextBox) then
    begin
      TD2ComboTextBox(FComboBox).ItemIndex:=ItemIndex;
      TD2Popup(Parent).IsOpen:=false;
    end;
  end;
end;

procedure TD2ComboListBox.MouseUp(Button:TMouseButton; Shift:TShiftState;
  X,Y:single);
begin
  inherited;

  if (Parent is TD2Popup) and (TD2Popup(Parent).IsOpen) and (FComboBox <> nil) and (FComboBox is TD2ComboBox) then
  begin
    if d2PtInRect(d2Point(X,Y),LocalRect) then
      if ItemByPoint(X,Y) <> nil then
        TD2ComboBox(FComboBox).ItemIndex:=ItemByPoint(X,Y).Index;
    TD2Popup(Parent).IsOpen:=false;
  end;

  if (Parent is TD2Popup) and (TD2Popup(Parent).IsOpen) and (FComboBox <> nil) and (FComboBox is TD2ComboTextBox) then
  begin
    if d2PtInRect(d2Point(X,Y),LocalRect) then
      if ItemByPoint(X,Y) <> nil then
        TD2ComboTextBox(FComboBox).ItemIndex:=ItemByPoint(X,Y).Index;
    TD2Popup(Parent).IsOpen:=false;
  end;

end;

procedure TD2ComboListBox.MouseWheel(Shift:TShiftState; WheelDelta:integer; var Handled:boolean);
begin
  inherited;
end;     

//================== TD2ComboColorBox ==================================

constructor TD2ComboColorBox.Create(AOwner:TComponent);
begin
  inherited;
  Width:=60;
  Height:=22;
  CanFocused:=true;
  AutoCapture:=true;
  FResource:='comboboxstyle';
  FPopup:=TD2Popup.Create(Self);
  FPopup.FResource:='combopopupstyle';
  FPopup.PlacementTarget:=Self;
  FPopup.StaysOpen:=false;
  FPopup.Stored:=false;
  FPopup.Parent:=Self;
  FPopup.Locked:=true;
  FPopup.DesignHide:=true;
  FPopup.Width:=240;
  FPopup.Height:=160;
  FPopup.Margins.Rect:=d2Rect(5,5,5,5);
  FColorBox:=TD2ColorBox.Create(Self);
  FColorBox.Width:=50;
  FColorBox.Parent:=FPopup;
  FColorBox.Stored:=false;
  FColorBox.Align:=vaRight;
  FColorBox.Padding.Rect:=d2Rect(15,70,15,30);
  FColorText:=TD2TextBox.Create(Self);
  FColorText.Parent:=FPopup;
  FColorText.Stored:=false;
  FColorText.Locked:=true;
  FColorText.FilterChar:='#0123456789abcdefABCDEF';
  FColorText.SetBounds(160,20,70,22);
  FColorText.Align:=vaTopRight;
  FColorText.DisableFocusEffect:=true;
  FColorText.OnChange:=DoTextChange;
  FColorPanel:=TD2ColorPanel.Create(Self);
  FColorPanel.Parent:=FPopup;
  FColorPanel.Stored:=false;
  FColorPanel.DisableFocusEffect:=true;
  FColorPanel.Align:=vaClient;
  FColorPanel.OnChange:=DoColorChange;
  FColorPanel.ColorBox:=FColorBox;
end;

destructor TD2ComboColorBox.Destroy;
begin
  inherited;
end;

procedure TD2ComboColorBox.DoTextChange(Sender:TObject);
var
  S:string;
begin
  try
    S:=Color;
    Color:=FColorText.TextW;
  except
    Color:=S;
  end;
end;

procedure TD2ComboColorBox.DoColorChange(Sender:TObject);
begin
  FColorText.TextW:=Color;
  Repaint;
  if Assigned(FOnChange) then FOnChange(Self);
end;

procedure TD2ComboColorBox.DropDown;
var
  i:integer;
begin
  if not FPopup.IsOpen then
  begin
    FPopup.Placement:=FPlacement;
    FColorPanel.ApplyResource;
    FPopup.IsOpen:=true;
  end
  else
  begin
    FPopup.IsOpen:=false;
  end;
end;

procedure TD2ComboColorBox.MouseDown(Button:TMouseButton; Shift:TShiftState; X,Y:single);
begin
  inherited;
  if (Button=mbLeft) then
  begin
    DropDown;
  end;
end;

procedure TD2ComboColorBox.ChangeParent;
begin
  inherited;
end;

function TD2ComboColorBox.GetValue:string;
begin
  Result:=FColorPanel.Color
end;

procedure TD2ComboColorBox.SetValue(const Value:string);
begin
  FColorPanel.Color:=Value;
end;

procedure TD2ComboColorBox.ApplyStyle;
var
  T:TD2Object;
begin
  inherited;
  T:=FindResource('Content');
  if (T <> nil) and (T is TD2Content) then
  begin
    TD2Content(T).OnPaint:=DoContentPaint;
  end;
end;

procedure TD2ComboColorBox.DoContentPaint(Sender:TObject; const Canvas:TD2Canvas; const ARect:TD2Rect);
var
  R:TD2Rect;
  i,j,SaveIndex:integer;
begin
  R:=ARect;
  d2InflateRect(R,-0.5 - 2,-0.5 - 2);

  SaveIndex:=Canvas.SaveCanvas;
  Canvas.IntersectClipRect(R);
  Canvas.Stroke.Style:=d2BrushNone;
  Canvas.Fill.Style:=d2BrushSolid;
  Canvas.Fill.SolidColor:=$FFFFFFFF;
  Canvas.FillRect(R,0,0,AllCorners,AbsoluteOpacity);
  Canvas.Fill.SolidColor:=$FFD3D3D3;

  for i:=0 to Trunc(d2RectWidth(R) / 5) + 1 do
    for j:=0 to Trunc(d2RectHeight(R) / 5) + 1 do
    begin
      if Odd(i + j) then
      begin
        Canvas.FillRect(d2Rect(i * 5,j * 5,(i + 1) * 5,(j + 1) * 5),0,0,AllCorners,AbsoluteOpacity);
      end;
    end;

  Canvas.RestoreCanvas(SaveIndex);
  Canvas.Fill.Style:=d2BrushSolid;
  Canvas.Fill.Color:=Color;
  Canvas.FillRect(R,0,0,AllCorners,AbsoluteOpacity);
  Canvas.Stroke.Color:=vcBlack;
  Canvas.Stroke.Style:=d2BrushSolid;
  Canvas.DrawRect(R,0,0,AllCorners,AbsoluteOpacity);
end;

function TD2ComboColorBox.GetUseAlpha:boolean;
begin
  Result:=FColorPanel.UseAlpha;
end;

procedure TD2ComboColorBox.SetUseAlpha(const Value:boolean);
begin
  FColorPanel.UseAlpha:=Value;
end;

function TD2ComboColorBox.GetData:Variant;
begin
  Result:=Color;
end;

procedure TD2ComboColorBox.SetData(const Value:Variant);
begin
  if VarIsEvent(Value) then
     Onchange:=VariantToEvent(Value)
  else
  if VarIsStr(Value) then
     Color:=Value
  else
  if VarIsOrdinal(Value) then
     Color:=d2ColorToStr(Value);
end;


//=============== TD2HudComboColorBox ===========================

constructor TD2HudComboColorBox.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='hudcomboboxstyle';
  FPopup.FResource:='hudcombopopupstyle';
end;


//================== TD2ComboTextBox =========================================

type
  TD2HackComboListBox=class(TD2ComboListBox);

constructor TD2ComboTextBox.Create(AOwner:TComponent);
begin
  inherited;

  DropDownCount:=8;
  Cursor:=crDefault;
  FItemHeight:=19;
  FItems:=TD2WideStringList.Create;
  TD2WideStringList(FItems).OnChange:=DoItemsChanged;
  FPopup:=TD2Popup.Create(Self);
  FPopup.PlacementTarget:=Self;
  FPopup.StaysOpen:=false;
  FPopup.Stored:=false;
  FPopup.Parent:=Self;
  FPopup.Locked:=true;
  FPopup.DesignHide:=true;
  FPopup.OnClosePopup:=DoClosePopup;
  FListBox:=TD2ComboListBox.Create(Self);
  FListBox.Parent:=FPopup;
  TD2HackComboListBox(FListBox).FComboBox:=Self;
  FListBox.ItemHeight:=ItemHeight;
  FListBox.Stored:=false;
  FListBox.Align:=vaClient;
  FListBox.ShowCheckboxes:=false;
  FListBox.ItemIndex:=-1;
  OnTyping:=DoTyping;
end;

destructor TD2ComboTextBox.Destroy;
begin
  FreeAndNil(FItems);
  inherited;
end;

procedure TD2ComboTextBox.DoItemsChanged(Sender:TObject);
begin
  RebuildList;
end;

procedure TD2ComboTextBox.RebuildList;
var
  SaveI,i:integer;
  Item:TD2ListBoxItem;
begin
  if csDestroying in ComponentState then Exit;
  if Items=nil then Exit;
  BeginUpdate;
  SaveI:=FListbox.ItemIndex;
  TD2HackComboListBox(FListBox).FItemIndex:=-1;
  FListBox.Clear;

  for i:=0 to FItems.Count - 1 do
  begin
    Item:=TD2ListBoxItem.Create(Self);
    Item.AutoTranslate:=FAutoTranslate;
    Item.Parent:=FListBox;
    Item.Height:=FItemHeight;
    Item.Stored:=false;
    Item.Locked:=true;
    Item.TextW:=FItems[i];
  end;

  EndUpdate;
  TD2HackComboListBox(FListbox).FItemIndex:=SaveI;

  if TD2HackComboListBox(FListbox).FItemIndex >= FListbox.Count then
    TD2HackComboListBox(FListbox).FItemIndex:=FListbox.Count - 1;
end;

procedure TD2ComboTextBox.KeyDown(var Key:Word;
  var KeyChar:System.WideChar; Shift:TShiftState);
var
  i:integer;
begin
  inherited ;
  if Count > 0 then
  begin

    if ReadOnly and (KeyChar <> #0) then
    begin
      for i:=0 to Count - 1 do
        if (FListBox.Items[i].TextW <> '') and (WideLowerCase(FListBox.Items[i].TextW[1])=WideLowerCase(KeyChar)) then
        begin
          ItemIndex:=i;
          Break;
        end;
      KeyChar:=#0;
    end;

    case Key of
      VK_UP:If ItemIndex > 0 then
        begin
          ItemIndex:=ItemIndex - 1;
          if ItemIndex < 0 then ItemIndex:=0;
        end;
      VK_DOWN:
        begin
          If ItemIndex < Count - 1 then ItemIndex:=ItemIndex + 1;
          if ItemIndex > Count - 1 then ItemIndex:=Count - 1;
        end;
    else
      Exit;
    end;

    Key:=0;
  end;
end;

procedure TD2ComboTextBox.Realign;
begin
  inherited;
  if FDisableAlign then Exit;
  FDisableAlign:=true;

  if FPopup <> nil then   FPopup.Width:=Width;
  if FListBox <> nil then FListBox.Width:=Width;

  FDisableAlign:=false;
end;

procedure TD2ComboTextBox.DoClosePopup(Sender:TObject);
begin
  if ShowCaret and IsFocused then ShowCaretProc;
end;

procedure TD2ComboTextBox.DropDown;
var
  Count,i:integer;
begin
  if not FPopup.IsOpen then
  begin
    if ShowCaret then  HideCaret;
    FPopup.Placement:=FPlacement;
    FPopup.Width:=Width;
    Count:=DropDownCount;
    if FListBox.Count < Count then
    Count:=FListBox.Count;

    if FListbox.ItemHeight > 0 then
      FPopup.Height:=Count * (FListbox.ItemHeight) + 4
    else
      FPopup.Height:=Count * (Height - 4);

    TD2HackComboListBox(FListBox).FNeedResource:=true;
    FListBox.ApplyResource;
    FPopup.IsOpen:=true;
    FListBox.SetFocus;
    TD2HackComboListBox(FListbox).UpdateSelection;
  end else
  begin
    FPopup.IsOpen:=false;
  end;
end;

procedure TD2ComboTextBox.MouseDown(Button:TMouseButton;
  Shift:TShiftState; X,Y:single);
begin
  inherited;
  if (Button=mbLeft) and not d2PtInRect(d2Point(X,Y),ContentRect) then
  begin
    DropDown;
  end;
end;

procedure TD2ComboTextBox.SetItemHeight(const Value:single);
begin
  if FItemHeight <> Value then
  begin
    FItemHeight:=Value;
    FListBox.ItemHeight:=FItemHeight;
  end;
end;

procedure TD2ComboTextBox.SetItems(const Value:TD2WideStrings);
begin
  FItems.Assign(Value);
end;

procedure TD2ComboTextBox.ChangeParent;
begin
  inherited;
end;

function TD2ComboTextBox.GetItemIndex:integer;
begin
  if FListBox <> nil then
    Result:=FListBox.ItemIndex
  else
    Result:=-1;
end;

procedure TD2ComboTextBox.SetItemIndex(const Value:integer);
begin
  if FListBox <> nil then
  begin
    FListBox.ItemIndex:=Value;
    if (ItemIndex >= 0) and (ItemIndex < Items.Count) then
    begin
      TextW:=Items[ItemIndex];
      FNeedChange:=false;
      if not Scene.ShowKeyboardForControl(Self) then
        SelectAll;
      if Assigned(FBindingObjects) then
        ToBindingObjects;
      if Assigned(FOnChange) then
        FOnChange(Self);
      if (FResourceLink <> nil) then
        FResourceLink.Visual.UpdateEffects;
      Repaint;
    end;
  end;
end;

function TD2ComboTextBox.GetCount:integer;
begin
  if FListBox <> nil then
    Result:=FListBox.Count
  else
    Result:=0;
end;

procedure TD2ComboTextBox.DoTyping(Sender:TObject);
var
  i,l:integer;
  UT:WideString;
begin
  if (FLastChar=#0) and ((FLastKey=VK_DELETE) or (FLastKey=8)) then Exit;

  {$IFDEF DARWIN}
  Exit;
  {$ENDIF}
  
  UT:=WideUpperCase(TextW);
  exit;
  for i:=0 to Items.Count - 1 do
    if pos(UT,WideUpperCase(Items.Strings[i]))=1 then
    begin
      l:=length(TextW);
      TextW:=Items.Strings[i];
      SelStart:=l;
      SelLength:=length(Items.Strings[i]) - l;
      Exit;
    end;
end;

function TD2ComboTextBox.GetListBoxResource:string;
begin
  Result:=FListBox.Resource;
end;

procedure TD2ComboTextBox.SetListBoxResource(const Value:string);
begin
  FListBox.Resource:=Value;
end;

//===================== TD2ComboTrackBar ====================================

constructor TD2ComboTrackBar.Create(AOwner:TComponent);
begin
  inherited;
  FFilterChar:='0123456789.,-+';
  TextW:='0';
  Width:=100;
  Height:=22;
  FResource:='combotextboxstyle';
  FPopup:=TD2Popup.Create(Self);
  FPopup.FResource:='combopopupstyle';
  FPopup.PlacementTarget:=Self;
  FPopup.StaysOpen:=false;
  FPopup.Stored:=false;
  FPopup.Parent:=Self;
  FPopup.Locked:=true;
  FPopup.DesignHide:=true;
  FPopup.Margins.Rect:=d2Rect(5,2,5,2);
  FPopup.OnClosePopup:=DoClosePopup;
  FTrackBar:=TD2TrackBar.Create(Self);
  FTrackBar.Parent:=FPopup;
  FTrackBar.Stored:=false;
  FTrackBar.DisableFocusEffect:=true;
  FTrackBar.Align:=vaVertCenter;
  FTrackBar.OnChange:=DoTrackChange;
end;

destructor TD2ComboTrackBar.Destroy;
begin
  inherited;
end;

procedure TD2ComboTrackBar.DoTrackChange(Sender:TObject);
begin
  TextW:=d2FloatToStr(FTrackBar.Value);
  SelectAll;
  if Assigned(FOnChange) then
    FOnChange(Self);
end;

procedure TD2ComboTrackBar.KeyDown(var Key:Word;
  var KeyChar:System.WideChar; Shift:TShiftState);
begin
  inherited ;
{  if Count > 0 then
  begin
    case Key of
      VK_UP:If ItemIndex > 0 then
        begin
          ItemIndex:=ItemIndex - 1;
          if ItemIndex < 0 then ItemIndex:=0;
        end;
      VK_DOWN:
        begin
          If ItemIndex < Count - 1 then ItemIndex:=ItemIndex + 1;
          if ItemIndex > Count - 1 then ItemIndex:=Count - 1;
        end;
    else
      Exit;
    end;
    Key:=0;
  end; }
end;

procedure TD2ComboTrackBar.DoClosePopup(Sender:TObject);
begin
  if ShowCaret and IsFocused then
    ShowCaretProc;
end;

procedure TD2ComboTrackBar.DropDown;
var
  i:integer;
begin
  if not FPopup.IsOpen then
  begin
    if ShowCaret then
      HideCaret;
    FPopup.Placement:=FPlacement;
    if Width < 100 then
      FPopup.Width:=100
    else
      FPopup.Width:=Width;
    FPopup.Height:=30;
    FTrackBar.ApplyResource;
    FPopup.IsOpen:=true;
  end
  else
  begin
    FPopup.IsOpen:=false;
  end;
end;

procedure TD2ComboTrackBar.Change;
begin
  try
    Value:=StrToFloat(TextW);
  except
    FText:=FloatToStr(Value);
  end;
  Repaint;
  inherited;
end;

procedure TD2ComboTrackBar.MouseDown(Button:TMouseButton;
  Shift:TShiftState; X,Y:single);
begin
  inherited;
  if (Button=mbLeft) and not d2PtInRect(d2Point(X,Y),ContentRect) then
  begin
    DropDown;
  end;
end;

procedure TD2ComboTrackBar.ChangeParent;
begin
  inherited;
  FPopup.Parent:=Parent;
end;

function TD2ComboTrackBar.GetFrequency:single;
begin
  Result:=FTrackBar.Frequency;
end;

function TD2ComboTrackBar.GetMax:single;
begin
  Result:=FTrackBar.Max;
end;

function TD2ComboTrackBar.GetMin:single;
begin
  Result:=FTrackBar.Min;
end;

procedure TD2ComboTrackBar.SetFrequency(const Value:single);
begin
  FTrackBar.Frequency:=Value;
end;

procedure TD2ComboTrackBar.SetMax(const Value:single);
begin
  FTrackBar.Max:=Value;
end;

procedure TD2ComboTrackBar.SetMin(const Value:single);
begin
  FTrackBar.Min:=Value;
end;

procedure TD2ComboTrackBar.SetValue(const AValue:single);
begin
  FTrackBar.Value:=AValue;
end;

function TD2ComboTrackBar.GetValue:single;
begin
  Result:=FTrackBar.Value;
end;

//================= TD2TextBoxClearBtn ======================================

constructor TD2TextBoxClearBtn.Create(AOwner:TComponent);
begin
  inherited;
end;

destructor TD2TextBoxClearBtn.Destroy;
begin
  inherited;
end;

procedure TD2TextBoxClearBtn.ApplyStyle;
var
  B:TD2Object;
begin
  inherited;
  B:=FindResource('clearbutton');
  if (B <> nil) and (B is TD2CustomButton) then
  begin
    FClearBtn:=TD2CustomButton(B);
    FClearBtn.OnClick:=DoClearBtnClick;
  end;
end;

procedure TD2TextBoxClearBtn.DoClearBtnClick(Sender:TObject);
begin
  SetFocus;
  TextW:='';
  Change;
end;

procedure TD2TextBoxClearBtn.FreeStyle;
begin
  FClearBtn:=nil;
  inherited;
end;

//==================== TD2HudComboTextBox ==============================

constructor TD2HudComboTextBox.Create(AOwner:TComponent);
begin
  inherited;
  FListBox.FResource:='hudcombolistboxstyle';
  FResource:='hudcombotextboxstyle';
end;

//==================== TD2HudComboTrackBar =============================

constructor TD2HudComboTrackBar.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='hudcombotextboxstyle';
  FPopup.FResource:='hudcombopopupstyle';
  FTrackBar.FResource:='hudtrackbarstyle';
end;


//=============== TD2StringComboBox =======================================

constructor TD2StringComboBox.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=true;
  FItems:=TD2WideStringList.Create;
  TD2WideStringList(FItems).OnChange:=DoItemsChanged;
  FItemHeight:=19;
  FNeedResource:=true;
  FResource:='comboboxstyle';
  FItemStyle:='listboxitemstyle';
end;

destructor TD2StringComboBox.Destroy;
begin
  FreeAndNil(FItems);
  inherited;
end;

procedure TD2StringComboBox.Clear;
begin
  inherited;
end;

procedure TD2StringComboBox.DoItemsChanged(Sender:TObject);
begin
  RebuildList;
end;

procedure TD2StringComboBox.RebuildList;
var
  SaveI,i:integer;
  Item:TD2ListBoxItem;
begin
  if csDestroying in ComponentState then Exit;
  if Items=nil then Exit;
  BeginUpdate;
  SaveI:=FListbox.ItemIndex;
  FListbox.FItemIndex:=-1;
  Clear;
  for i:=0 to FItems.Count - 1 do
  begin
    Item:=TD2ListBoxItem.Create(Self);
    Item.AutoTranslate:=FAutoTranslate;
    Item.Parent:=Self;
    Item.Height:=FItemHeight;
    Item.Stored:=false;
    Item.Locked:=true;
    Item.FResource:=FItemStyle;
    Item.TextW:=FItems[i];
    Item.TextAlign:=TextAlign;
  end;
  EndUpdate;
  FListbox.FItemIndex:=SaveI;
  if FListbox.FItemIndex >= FListbox.Count then
    FListbox.FItemIndex:=FListbox.Count - 1;
end;

procedure TD2StringComboBox.SetItemHeight(const Value:single);
begin
  if FItemHeight <> Value then
  begin
    FItemHeight:=Value;
    RebuildList;
  end;
end;

procedure TD2StringComboBox.SetItems(const Value:TD2WideStrings);
begin
  FItems.Assign(Value);
end;

procedure TD2StringComboBox.SetTextAlign(const Value:TD2TextAlign);
begin
  if FTextAlign <> Value then
  begin
    FTextAlign:=Value;
    if not (csLoading in ComponentState) then
      RebuildList;
  end;
end;

procedure TD2StringComboBox.ApplyStyle;
begin
  inherited;
end;

function TD2StringComboBox.GetData:Variant;
begin
  if (ItemIndex >= 0) and (ItemIndex < FItems.Count) then
    Result:=FItems[ItemIndex]
  else
    Result:='';
end;

procedure TD2StringComboBox.SetData(const Value:Variant);
begin
  if (ItemIndex >= 0) and (ItemIndex < FItems.Count) then
    FItems[ItemIndex]:=Value;
end;

//=============== TD2HudStringComboBox ==================================

constructor TD2HudStringComboBox.Create(AOwner:TComponent);
begin
  inherited;
  FNeedResource:=true;
  FResource:='hudcomboboxstyle';
  FListBox.FResource:='hudcombolistboxstyle';
  FItemStyle:='hudlistboxitemstyle';
end;


//=============== TD2HudComboBox =======================================

constructor TD2HudComboBox.Create(AOwner:TComponent);
begin
  inherited;
  FListBox.FResource:='hudcombolistboxstyle';
end;                  
       
     
                             
