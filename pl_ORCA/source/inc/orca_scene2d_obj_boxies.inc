{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}



//==================== TD2CustomTextBox ================================================

constructor TD2CustomTextBox.Create(AOwner:TComponent);
begin
  inherited;
  FShowCaret:=true;
  FFont:=TD2Font.Create;
  FFont.OnChanged:=FontChanged;
  FFontFill:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FSelectionFill:=TD2Brush.Create(d2BrushSolid,$802A8ADF);
  CanFocused:=true;
  Cursor:=crIBeam;
  TextAlign:=d2TextAlignNear;
  AutoCapture:=true;
  Width:=100;
  Height:=22;
  FCaretPosition:=0;
  FSelStart:=0;
  FSelLength:=0;
  FFirstVisibleChar:=1;
  CreatePopupMenu;
end;

destructor TD2CustomTextBox.Destroy;
begin
  FSelectionFill.Free;
  FFontFill.Free;
  FFont.Free;
  FPopupMenu.Free;
  inherited;
end;

procedure TD2CustomTextBox.CreatePopupMenu;
var
  TmpItem:TMenuItem;
begin
  FPopupMenu:=TPopupMenu.Create(Self);

  TmpItem:=TMenuItem.Create(FPopupMenu);
  with TmpItem do
  begin
    Caption:='Cut';
    OnClick:=DoCut;
  end;
  FPopupMenu.Items.Add(TmpItem);

  TmpItem:=TMenuItem.Create(FPopupMenu);
  with TmpItem do
  begin
    Caption:='Copy';
    OnClick:=DoCopy;
  end;
  FPopupMenu.Items.Add(TmpItem);

  TmpItem:=TMenuItem.Create(FPopupMenu);
  with TmpItem do
  begin
    Caption:='Paste';
    OnClick:=DoPaste;
  end;
  FPopupMenu.Items.Add(TmpItem);

  TmpItem:=TMenuItem.Create(FPopupMenu);
  with TmpItem do
  begin
    Caption:='Delete';
    OnClick:=DoDelete;
  end;
  FPopupMenu.Items.Add(TmpItem);

//  FPopupMenu.Items.NewBottomLine;

  TmpItem:=TMenuItem.Create(FPopupMenu);
  with TmpItem do
  begin
    Caption:='Select All';
    OnClick:=DoSelectAll;
  end;
  FPopupMenu.Items.Add(TmpItem);
end;

procedure TD2CustomTextBox.DoSelectAll(Sender:TObject);
begin
  SelectAll;
end;

procedure TD2CustomTextBox.DoCut(Sender:TObject);
begin
  CutToClipboard;
end;

procedure TD2CustomTextBox.DoCopy(Sender:TObject);
begin
  CopyToClipboard;
end;

procedure TD2CustomTextBox.DoDelete(Sender:TObject);
begin
  ClearSelection;
end;

procedure TD2CustomTextBox.DoPaste(Sender:TObject);
begin
  PasteFromClipboard;
end;

procedure TD2CustomTextBox.UpdatePopupMenuItems;
var
  SelTextEmpty:boolean;
begin
  SelTextEmpty:=SelText <> '';  
  FPopupMenu.Items.Find('Cut').Enabled:=SelTextEmpty and not ReadOnly;
  FPopupMenu.Items.Find('Copy').Enabled:=SelTextEmpty;
  FPopupMenu.Items.Find('Paste').Enabled:=(Clipboard.AsText <> '') and not ReadOnly;
  FPopupMenu.Items.Find('Delete').Enabled:=SelTextEmpty and not ReadOnly;
  FPopupMenu.Items.Find('Select All').Enabled:=SelText <> TextW;
end;

function TD2CustomTextBox.GetData:Variant;
begin
  Result:=FText;
end;

procedure TD2CustomTextBox.SetData(const Value:Variant);
begin
  if VarIsNull(Value) then
    TextW:=''
  else
  if VarIsType(Value,varDate) then
    TextW:=DateTimeToStr(VarToDateTime(Value))
  else
  if VarIsEvent(Value) then
    OnChange:=VariantToEvent(Value)
  else
    TextW:=VarToWideStr(Value);
end;

procedure TD2CustomTextBox.ApplyStyle;
var
  T:TD2Object;
begin
  inherited;
  T:=FindResource('content');
  if (T <> nil) and (T is TD2Content) then
  begin
    TD2Content(T).OnPaint:=DoContentPaint;
  end;
  T:=FindResource('selection');
  if (T <> nil) and (T is TD2BrushObject) then
  begin
    FSelectionFill.Assign(TD2BrushObject(T).Brush);
  end;
  { from style }
  T:=FindResource('foreground');
  if (T <> nil) and (T is TD2BrushObject) then
    FFontFill.Assign(TD2BrushObject(T).Brush);
end;

function TD2CustomTextBox.ContentRect:TD2Rect;
var
  T:TD2Object;
begin
  T:=FindResource('content');
  if (T <> nil) and (T.IsVisual) then
  begin
    Result:=TD2VisualObject(T).ParentedRect;
  end
  else
  begin
    Result:=LocalRect;
  end;
end;

procedure TD2CustomTextBox.Paint;
begin
  inherited;
end;

procedure TD2CustomTextBox.DoContentPaint(Sender:TObject; const Canvas:TD2Canvas;
  const ARect:TD2Rect);
var
  i:integer;
  R:TD2Rect;
  SaveIdx:cardinal;
begin
  { draw TextW }
  if FText='' then Exit;

  SaveIdx:=Canvas.SaveCanvas;
  Canvas.IntersectClipRect(ARect);

  Canvas.Font.Assign(Font);
  Canvas.Fill.Assign(FFontFill);
  if FPassword then
  begin
    R:=ARect;
    R.Right:=R.Left + GetPasswordCharWidth - 1;
    R.Top:=(d2RectHeight(ARect) - d2RectWidth(R)) / 2;
    R.Bottom:=R.Top + d2RectWidth(R);
    for i:=FFirstVisibleChar to Length(TextW) do
    begin
      Canvas.FillEllipse(R,AbsoluteOpacity);
      d2OffsetRect(R,d2RectWidth(R) + 1,0);
    end;
  end
  else
  begin
    R:=ARect;
    if Textalign=d2TextAlignNear then
      R.Right:=R.Right + (Font.Size / 2); // disable GDI+ clippping
    Canvas.FillText(R,R,Copy(TextW,FFirstVisibleChar,Length(TextW) - FFirstVisibleChar + 1),
      false,AbsoluteOpacity,TextAlign,d2TextAlignCenter);
  end;
  { carret }
  if IsFocused then
  begin
    { selection }
    if SelLength > 0 then
    begin
      Canvas.Fill.Assign(FSelectionFill);
      R:=GetSelRect;
      with ContentRect do
        d2OffsetRect(R,-Left,-Top);
      Canvas.FillRect(R,0,0,AllCorners,AbsoluteOpacity);
    end;
  end;

  Canvas.RestoreCanvas(SaveIdx);
end;

procedure TD2CustomTextBox.InsertText(const AText:WideString);
var
  TmpS:WideString;
begin
  if ReadOnly then Exit;

  TmpS:=TextW;
//  FActionStack.FragmentDeleted(SelStart + 1,Copy(TmpS,SelStart+1,SelLength));
  Delete(TmpS,SelStart + 1,SelLength);
//  FActionStack.FragmentInserted(SelStart + 1,Length(AText),SelLength <> 0);
  Insert(AText,TmpS,SelStart + 1);
  if (MaxLength <= 0) or (Length(TmpS) <= MaxLength) then
  begin
    TextW:=TmpS;
    CaretPosition:=SelStart + Length(AText);
  end;
  SelLength:=0;
end;

procedure TD2CustomTextBox.UpdateFirstVisibleChar;
var
  LEditRect:TD2Rect;
begin
  if FFirstVisibleChar >= (FCaretPosition + 1) then
  begin
    FFirstVisibleChar:=FCaretPosition;
    if FFirstVisibleChar < 1 then
      FFirstVisibleChar:=1;
  end
  else
  begin
    LEditRect:=ContentRect;

    if Password then
      while ((FCaretPosition - FFirstVisibleChar + 1) * GetPasswordCharWidth > LEditRect.Right - LEditRect.Left) and (FFirstVisibleChar < Length(TextW)) do
        Inc(FFirstVisibleChar)
    else
    begin
      while (TextWidth(Copy(TextW,FFirstVisibleChar,FCaretPosition - FFirstVisibleChar + 1)) > LEditRect.Right - LEditRect.Left) and (FFirstVisibleChar < Length(TextW)) do
      begin
        if TextWidth(Copy(TextW,FFirstVisibleChar + 500,(FCaretPosition - FFirstVisibleChar + 500) + 1)) > LEditRect.Right - LEditRect.Left then
          Inc(FFirstVisibleChar,500)
        else
          if TextWidth(Copy(TextW,FFirstVisibleChar + 100,(FCaretPosition - FFirstVisibleChar + 100) + 1)) > LEditRect.Right - LEditRect.Left then
            Inc(FFirstVisibleChar,100)
          else
            if TextWidth(Copy(TextW,FFirstVisibleChar + 50,(FCaretPosition - FFirstVisibleChar + 100) + 1)) > LEditRect.Right - LEditRect.Left then
              Inc(FFirstVisibleChar,50)
            else
              if TextWidth(Copy(TextW,FFirstVisibleChar + 10,(FCaretPosition - FFirstVisibleChar + 10) + 1)) > LEditRect.Right - LEditRect.Left then
                Inc(FFirstVisibleChar,10)
              else
                Inc(FFirstVisibleChar);
      end;
    end;
  end;
  Repaint;
end;

procedure TD2CustomTextBox.UpdateCaretePosition;
begin
  SetCaretPosition(CaretPosition);
end;

function TD2CustomTextBox.GetPasswordCharWidth:single;
begin
  Result:=Font.Size / 2;
end;

function TD2CustomTextBox.TextWidth(const Str:WideString):single;
var
  R:TD2Rect;
begin
  R:=ContentRect;
  R.Right:=10000;
  Canvas.Font.Assign(Font);
  if FPassword then
  begin
    R.Right:=R.Left + GetPasswordCharWidth * Length(Str);
  end
  else
    Canvas.MeasureText(R,R,Str,false,d2TextAlignNear,d2TextAlignCenter);
  Result:=d2RectWidth(R);
end;

function TD2CustomTextBox.GetCoordinatePosition(x:single):integer;
var
  //CurX:double;
  TmpX,
  WholeTextWidth,
  EditRectWidth :single;
  Str,StrA:WideString;
begin
  Result:=FFirstVisibleChar - 1;
  if Length(TextW)=0 then
    Exit;

  if FPassword then
    WholeTextWidth:=Length(TextW) * GetPasswordCharWidth
  else
    WholeTextWidth:=TextWidth(Copy(TextW,1,Length(TextW)));

  EditRectWidth:=ContentRect.Right - ContentRect.Left;
  TmpX:=x;
  if WholeTextWidth < EditRectWidth then
    case TextAlign of
      d2TextAlignFar:TmpX:=x - (EditRectWidth - WholeTextWidth);
      d2TextAlignCenter:TmpX:=x - ((EditRectWidth - WholeTextWidth) / 2);
    end;

  if FPassword then
  begin
    Result:=Result + Trunc((TmpX - ContentRect.Left) / GetPasswordCharWidth);
    if Result < 0 then
      Result:=0
    else
      if Result > Length(TextW) then
        Result:=Length(TextW);
  end
  else
  begin
    TmpX:=TmpX - ContentRect.Left;
    StrA:=System.Copy(TextW,FFirstVisibleChar,Result - FFirstVisibleChar + 1);
    Str:=System.Copy(TextW,FFirstVisibleChar,Result - FFirstVisibleChar + 2);
    while (TextWidth(StrA) < TmpX) and (Result < Length(TextW)) do
    begin
      if (TmpX > TextWidth(StrA) + ((TextWidth(Str) - TextWidth(StrA)) / 2)) and (TmpX < TextWidth(Str)) then
      begin
        Result:=Result + 1;
        Break;
      end;
      if TmpX < TextWidth(Str) then Break;
      Result:=Result + 1;
      StrA:=Str;
      Str:=Copy(TextW,FFirstVisibleChar,Result - FFirstVisibleChar + 2);
    end;
  end;
end;

function TD2CustomTextBox.GetCharX(a:integer):single;
var
  WholeTextWidth:single;
  EditRectWidth:single;
  R:TD2Rect;
  T:WideString;
begin
  if FPassword then
  begin
    WholeTextWidth:=Length(TextW) * GetPasswordCharWidth;
    Result:=ContentRect.Left;
    if a > 0 then
    begin
      if FPassword then
      begin
        if a <= Length(TextW) then
          Result:=Result + (a - FFirstVisibleChar + 1) * GetPasswordCharWidth
        else
          Result:=Result + (Length(TextW) - FFirstVisibleChar + 1) * GetPasswordCharWidth;
      end
    end;
    EditRectWidth:=ContentRect.Right - ContentRect.Left;
    if WholeTextWidth < EditRectWidth then
      case TextAlign of
        d2TextAlignFar :Result:=Result + (EditRectWidth - WholeTextWidth);
        d2TextAlignCenter:Result:=Result + ((EditRectWidth - WholeTextWidth) / 2);
      end;
    Exit;
  end;

  R:=ContentRect;
  Canvas.Font.Assign(Font);
  T:=TextW;
  if TextW='' then
    T:='a';
  Canvas.MeasureText(R,R,T,false,d2TextAlignNear,d2TextAlignCenter);
  WholeTextWidth:=R.Right - ContentRect.Left;
  Result:=ContentRect.Left;

  if a > 0 then
  begin
    if a <= Length(TextW) then
    begin
      R:=ContentRect;
      Canvas.MeasureText(R,R,Copy(TextW,FFirstVisibleChar,a - FFirstVisibleChar + 1),false,d2TextAlignNear,d2TextAlignCenter);
      Result:=R.Right;
    end
    else
    begin
      R:=ContentRect;
    end;
  end;

  EditRectWidth:=ContentRect.Right - ContentRect.Left;
  if WholeTextWidth < EditRectWidth then
    case TextAlign of
      d2TextAlignFar:Result:=Result + (EditRectWidth - WholeTextWidth);
      d2TextAlignCenter:Result:=Result + ((EditRectWidth - WholeTextWidth) / 2);
    end;
end;

function TD2CustomTextBox.GetNextWordBeging(StartPosition:integer):integer;
var
  SpaceFound,WordFound:boolean;
begin
  Result:=StartPosition;
  SpaceFound:=false;
  WordFound:=false;
  while (Result + 2 <= Length(TextW)) and
    ((not ((TextW[Result + 1] <> d2WideSpace) and SpaceFound))
    or not WordFound) do
  begin
    if TextW[Result + 1]=d2WideSpace then
      SpaceFound:=true;
    if TextW[Result + 1] <> d2WideSpace then
    begin
      WordFound:=true;
      SpaceFound:=false;
    end;

    Result:=Result + 1;
  end;
  if not SpaceFound then
    Result:=Result + 1;
end;

function TD2CustomTextBox.GetPrivWordBeging(StartPosition:integer):integer;
var
  WordFound:boolean;
begin
  Result:=StartPosition;
  WordFound:=false;
  while (Result > 0) and
    ((TextW[Result] <> d2WideSpace) or not WordFound) do
  begin
    if TextW[Result] <> d2WideSpace then
      WordFound:=true;
    Result:=Result - 1;
  end;
end;

function TD2CustomTextBox.GetSelStart:integer;
begin
  if FSelLength > 0 then
    Result:=FSelStart
  else
    if FSelLength < 0 then
      Result:=FSelStart + FSelLength
    else
      Result:=CaretPosition;
end;

function TD2CustomTextBox.GetSelRect:TD2Rect;
begin
  Result:=ContentRect;
  Result.Left:=GetCharX(SelStart);
  Result.Right:=GetCharX(SelStart + SelLength) + 1;
end;

function TD2CustomTextBox.GetSelLength:integer;
begin
  Result:=Abs(FSelLength);
end;

function TD2CustomTextBox.GetSelText:WideString;
begin
  Result:=Copy(TextW,SelStart + 1,SelLength);
end;

procedure TD2CustomTextBox.SetSelLength(const Value:integer);
begin
  if FSelLength <> Value then
  begin
    FSelLength:=Value;
    Repaint;
  end;
end;

procedure TD2CustomTextBox.SetSelStart(const Value:integer);
begin
  if FSelStart <> Value then
  begin
    SelLength:=0;
    FSelStart:=Value;
    CaretPosition:=FSelStart;
    Repaint;
  end;
end;

procedure TD2CustomTextBox.SetCaretPosition(const Value:integer);
begin
  if Value < 0 then
    FCaretPosition:=0
  else
    if Value > Length(TextW) then
      FCaretPosition:=Length(TextW)
    else
      FCaretPosition:=Value;

  UpdateFirstVisibleChar;

  if SelLength <= 0 then
    FSelStart:=Value;

  Repaint;
  if IsFocused and FShowCaret then
  begin
    SetCaretSize(d2Point(2,(Font.Size * 1.25)));
    SetCaretPos(d2Point(GetCharX(FCaretPosition) - 1,(ContentRect.Top + ContentRect.Bottom - (Font.Size * 1.25)) / 2));
    SetCaretColor(FFontFill.SolidColor);
  end;
end;

procedure TD2CustomTextBox.SetMaxLength(const Value:integer);
begin
  if FMaxLength <> Value then
  begin
    FMaxLength:=Value;
  end;
end;

procedure TD2CustomTextBox.CopyToClipboard;
//var                                        Superfluous *** DB ***
//Data:THandle;                                         *** DB ***
//DataPtr:Pointer;                                      *** DB ***
//Size:Cardinal;                                        *** DB ***
//S:WideString;                                         *** DB ***
begin
  if SelText <> '' then
    Clipbrd.Clipboard.AsText:=UTF8Encode(SelText);
end;

procedure TD2CustomTextBox.PasteFromClipboard;
//var
  //Data:THandle;
  //Insertion:WideString;
begin
  if ReadOnly then Exit;

  InsertText(UTF8Decode(Clipbrd.Clipboard.AsText));
end;

procedure TD2CustomTextBox.ClearSelection;
var
  TmpS:WideString;
begin
  if ReadOnly then Exit;

  TmpS:=TextW;
  Delete(TmpS,SelStart + 1,SelLength);
  CaretPosition:=SelStart;
  TextW:=TmpS;
  SelLength:=0;
end;

procedure TD2CustomTextBox.CutToClipboard;
begin
    CopyToClipboard;
  ClearSelection;
end;

procedure TD2CustomTextBox.SelectAll;
begin
  SelStart:=0;
  SelLength:=Length(TextW);
  SetCaretPosition(Length(TextW));
  Repaint;
end;

procedure TD2CustomTextBox.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
var
  S:wideString;
  TmpS:WideString;
  OldCaretPosition:integer;
begin
  inherited ;
  OldCaretPosition:=CaretPosition;
  FLastChar:=KeyChar;
  FLastKey:=Key;
  case Key of
    VK_RETURN:Change;
    VK_END:CaretPosition:=Length(TextW);
    VK_HOME:CaretPosition:=0;
    VK_LEFT:
      if ssCtrl in Shift then
        CaretPosition:=GetPrivWordBeging(CaretPosition)
      else
        CaretPosition:=CaretPosition - 1;
    VK_RIGHT:
      if ssCtrl in Shift then
        CaretPosition:=GetNextWordBeging(CaretPosition)
      else
        CaretPosition:=CaretPosition + 1;
    VK_DELETE,8:{Delete or BackSpace key was pressed}
      if not ReadOnly then
      begin
        if SelLength <> 0 then
        begin
          if Shift=[ssShift] then
            CutToClipboard
          else
            ClearSelection;
        end
        else
        begin
          TmpS:=TextW;
          if TmpS <> '' then
            if Key=VK_DELETE then
            begin
//              FActionStack.FragmentDeleted(CaretPosition + 1,TmpS[CaretPosition + 1]);
              Delete(TmpS,CaretPosition + 1,1);
            end
            else
            begin {BackSpace key was pressed}
{              if CaretPosition > 0 then
                FActionStack.FragmentDeleted(CaretPosition,TmpS[CaretPosition]);}
              Delete(TmpS,CaretPosition,1);
              CaretPosition:=CaretPosition - 1;
            end;
          TextW:=TmpS;
          if Assigned(FOnTyping) then FOnTyping(Self);
        end;
      end;
    VK_INSERT:
      if Shift=[ssCtrl] then
      begin
        CopyToClipboard;
      end
      else
        if Shift=[ssShift] then
        begin
          PasteFromClipboard;
          if Assigned(FOnTyping) then FOnTyping(Self);
        end;
  end;

  if (KeyChar <> #0) and (FFilterChar <> '') and (Pos(KeyChar,FFilterChar)=0) then
  begin
    KeyChar:=#0;
  end;
  case KeyChar of
    'a','A':
      if Shift=[ssCtrl] then
      begin
        SelectAll;
        KeyChar:=#0;
      end;
    'c','C':
      if Shift=[ssCtrl] then
      begin
        CopyToClipboard;
        KeyChar:=#0;
      end;
    'v','V':
      if Shift=[ssCtrl] then
      begin
        PasteFromClipboard;
        if Assigned(FOnTyping) then FOnTyping(Self);
        KeyChar:=#0;
      end;
    'x','X':
      if Shift=[ssCtrl] then
      begin
        CutToClipboard;
        KeyChar:=#0;
      end;
    'z','Z':
      if Shift=[ssCtrl] then
      begin
        {UnDo};
        KeyChar:=#0;
      end;
  end;

  if Key in [VK_END,VK_HOME,VK_LEFT,VK_RIGHT] then
  begin
    if ssShift in Shift then
    begin
      if SelLength=0 then
        FSelStart:=OldCaretPosition;
      FSelStart:=CaretPosition;
      FSelLength:=FSelLength - (CaretPosition - OldCaretPosition);
    end
    else
      FSelLength:=0;
    Repaint;
  end;

  if (Ord(KeyChar) >= 32) and not ReadOnly then
  begin
    S:=KeyChar;
    InsertText(S);
    if Assigned(FOnTyping) then FOnTyping(Self);
  end;
  if FResourceLink <> nil then
    FResourceLink.Visual.UpdateEffects;
  UpdateCaretePosition;
end;

procedure TD2CustomTextBox.KeyUp(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  inherited ;
end;

procedure TD2CustomTextBox.DblClick;
begin
  inherited;
  SelectAll;
end;

procedure TD2CustomTextBox.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
var
  OldFocused:boolean;
begin
  OldFocused:=IsFocused;
  inherited;
  if Button=mbLeft then
    FLMouseSelecting:=true;

  if (Button=mbLeft) and not (ssDouble in Shift) then
  begin
    if OldFocused then // clear only already focused
    begin
      CaretPosition:=GetCoordinatePosition(x);
      SelLength:=0;
    end;
  end;
end;

procedure TD2CustomTextBox.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
var
  OldCaretPosition:integer;
  TmpNewPosition :integer;
begin
  inherited;
  if FLMouseSelecting then
  begin
    TmpNewPosition:=GetCoordinatePosition(x);
    OldCaretPosition:=CaretPosition;
    if (x > ContentRect.Right) then
      CaretPosition:=TmpNewPosition +1
    else
      CaretPosition:=TmpNewPosition;
    if SelLength=0 then
      FSelStart:=OldCaretPosition;
    FSelStart:=CaretPosition;
    FSelLength:=FSelLength - (CaretPosition - OldCaretPosition);
  end;
end;

procedure TD2CustomTextBox.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  FLMouseSelecting:=false;
end;

procedure TD2CustomTextBox.Change;
begin
  if FNeedChange then
  begin
    if Assigned(FBindingObjects) then
      ToBindingObjects;
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

procedure TD2CustomTextBox.ContextMenu(const ScreenPosition:TD2Point);
begin
  inherited;
  if csDesigning in ComponentState then Exit;
  if Popup <> nil then Exit;

  UpdatePopupMenuItems;
  FPopupMenu.PopupComponent:=Self;
  FPopupMenu.Popup(round(ScreenPosition.X),round(ScreenPosition.Y));
end;

procedure TD2CustomTextBox.EnterFocus;
begin
  inherited;
  FNeedChange:=false;
  if FShowCaret then
    ShowCaretProc;
  if FScene.ShowKeyboardForControl(Self) then
    CaretPosition:=Length(TextW)
  else
    SelectAll;
end;

procedure TD2CustomTextBox.KillFocus;
begin
  if not Assigned(FScene) then
  begin
    inherited ;
    Exit;
  end;
  FScene.HideKeyboardForControl(Self);
  if FShowCaret then
    HideCaret;
  inherited ;
  Change;
end;

procedure TD2CustomTextBox.SetTextW(const Value:WideString);
begin
  if FText <> Value then
  begin
    FText:=Value;
    if FCaretPosition > Length(TextW) then
      SetCaretPosition(Length(TextW));
    if not (csLoading in ComponentState) and Assigned(OnChangeTracking) then
      OnChangeTracking(Self);
    FNeedChange:=true;
    Repaint;
  end;
end;

procedure TD2CustomTextBox.SetText(const Value:String);
 var ss:WideString;
begin
  ss:=UTF8Decode(Value);
  SetText(ss);
end;

Function TD2CustomTextBox.GetTextW:WideString;
begin
  Result:=ftext;
end;

Function TD2CustomTextBox.GetText:string;
var s: string;
begin
  Result:=UTF8Encode(ftext);
end;

procedure TD2CustomTextBox.SetPassword(const Value:boolean);
begin
  if FPassword <> Value then
  begin
    FPassword:=Value;
    Repaint;
  end;
end;

procedure TD2CustomTextBox.SetFont(const Value:TD2Font);
begin
  FFont.Assign(Value);
end;

procedure TD2CustomTextBox.SetTextAlign(const Value:TD2TextAlign);
begin
  if FTextAlign <> Value then
  begin
    FTextAlign:=Value;
    Repaint;
  end;
end;

procedure TD2CustomTextBox.FontChanged(Sender:TObject);
begin
  Repaint;
end;

{ TD2NumberBox ================================================================}

constructor TD2NumberBox.Create(AOwner:TComponent);
begin
  inherited;
  FDecimalDigits:=2;
  FFilterChar:='0123456789.,-+';
  Max:=10;
  VertIncrement:=5;
  HorzIncrement:=1;
  TextW:='0';
  Value:=0;
  AutoCapture:=true;
end;

destructor TD2NumberBox.Destroy;
begin
  inherited;
end;

function TD2NumberBox.GetData:Variant;
begin
  Result:=Value;
end;

procedure TD2NumberBox.SetData(const Value:Variant);
begin
  if VarIsEvent(Value) then
    OnChange:=VariantToEvent(Value)
  else
    Self.Value:=Value;
end;

procedure TD2NumberBox.Change;
begin
  try
    FValue:=StrToFloat(TextW);
    if FValue > FMax then FValue:=FMax;
    if FValue < FMin then FValue:=FMin;
    if (frac(FValue)=0) or (FValueType=d2ValueInteger) then
      FText:=IntToStr(Trunc(FValue))
    else
      FText:=FloattoStr(FValue);
  except
    if (frac(FValue)=0) or (FValueType=d2ValueInteger) then
      FText:=IntToStr(Trunc(FValue))
    else
      FText:=FloattoStr(FValue);
  end;
  Repaint;
  inherited;
end;

procedure TD2NumberBox.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  case Key of
    VK_UP:Value:=Value + HorzIncrement;
    VK_DOWN:Value:=Value - HorzIncrement;
  else
    inherited;
    Exit;
  end;
  Key:=0;
end;

procedure TD2NumberBox.MouseDown(Button:TMouseButton; Shift:TShiftState;
  X,Y:single);
begin
  inherited;
  if Button=mbLeft then
  begin
    FPressed:=true;
    FPressedPos:=d2Point(X,Y);
    FPressedVert:=false;
    FPressedInc:=0;
  end;
end;

procedure TD2NumberBox.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
begin
  inherited;
  if FPressed then
  begin
    if Abs(X - FPressedPos.X) >= Abs(Y - FPressedPos.Y) then
    begin
      { horz }
      if X > FPressedPos.X then
        Value:=Value + HorzIncrement
      else
        Value:=Value - HorzIncrement;
      FPressedInc:=X - FPressedPos.X;
      FPressedVert:=false;
    end
    else
    begin
      { vert }
      if Y < FPressedPos.Y then
        Value:=Value + VertIncrement
      else
        Value:=Value - VertIncrement;
      FPressedInc:=X - FPressedPos.X;
      FPressedVert:=true;
    end;
    FNeedChange:=true;
    FPressedPos:=d2Point(X,Y);
  end;
end;

procedure TD2NumberBox.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
  if FPressed then
  begin
    FPressed:=false;
    Change;
    Repaint;
  end;
end;

procedure TD2NumberBox.Paint;
begin
  inherited ;
end;

procedure TD2NumberBox.PaintChildren;
var
  R:TD2Rect;
begin
  if FPressed then
    FDisableCaret:=true;
  inherited;
  if FPressed then
  begin
    Canvas.SetMatrix(AbsoluteMatrix);
    Canvas.Fill.Style:=d2BrushSolid;
    Canvas.Fill.SolidColor:=$AA505050;
    R:=LocalRect;
    if FPressedVert then
    begin
      d2InflateRect(R,-1,-1);
      R.Left:=R.Right - 5;
      Canvas.FillRect(R,1,1,AllCorners,AbsoluteOpacity);
      d2InflateRect(R,-1,-1);
{      if FPressedInc > 0 then
      begin
        Canvas.Fill.SolidColor:=$AA202020;
        R.Top:=(d2RectHeight(R) / 2);
        R.Bottom:=R.Top + (Height / 2.1);
        Canvas.FillRect(R,1,1,AbsoluteOpacity);
      end;
      if FPressedInc < 0 then
      begin
        Canvas.Fill.SolidColor:=$AA202020;
        R.Bottom:=(d2RectHeight(R) / 2);
        R.Top:=R.Bottom - (Height / 2.1);
        Canvas.FillRect(R,1,1,AbsoluteOpacity);
      end;}
    end
    else
    begin
      d2InflateRect(R,-1,-1);
      R.Top:=R.Bottom - 5;
      Canvas.FillRect(R,1,1,AllCorners,AbsoluteOpacity);
      d2InflateRect(R,-1,-1);
{      if FPressedInc > 0 then
      begin
        Canvas.Fill.SolidColor:=$AA202020;
        R.Left:=(d2RectWidth(R) / 2);
        R.Right:=R.Left + (Width / 2.1);
        Canvas.FillRect(R,1,1,AbsoluteOpacity);
      end;
      if FPressedInc < 0 then
      begin
        Canvas.Fill.SolidColor:=$AA202020;
        R.Right:=(d2RectWidth(R) / 2);
        R.Left:=R.Right - (Width / 2.1);
        Canvas.FillRect(R,1,1,AbsoluteOpacity);
      end;}
    end;
  end;
  if FPressed then
    FDisableCaret:=false;
end;

procedure TD2NumberBox.SetMax(const Value:single);
begin
  FMax:=Value;
end;

procedure TD2NumberBox.SetMin(const Value:single);
begin
  FMin:=Value;
end;

procedure TD2NumberBox.SetDecimalDigits(const Value:integer);
begin
  FDecimalDigits:=Value;
end;

procedure TD2NumberBox.SetValue(const AValue:single);
begin
  if FValue <> AValue then
  begin
    FValue:=AValue;
    if FValue > FMax then FValue:=FMax;
    if FValue < FMin then FValue:=FMin;
    if (frac(FValue)=0) or (FValueType=d2ValueInteger) then
      FText:=IntToStr(Trunc(FValue))
    else
      FText:=Format('%.' + IntToStr(FDecimalDigits) + 'f',[FValue]);
    SelLength:=0;
    Repaint;
  end;
end;

procedure TD2NumberBox.SetValueType(const Value:TD2ValueType);
begin
  FValueType:=Value;
end;

procedure TD2NumberBox.SetText(const Value:String); // 7777
begin
  inherited;
end;

//============================= TD2SpinBox ===========================================

constructor TD2SpinBox.Create(AOwner:TComponent);
begin
  inherited;
  FDecimalDigits:=2;
  FFilterChar:='0123456789.,-+';
  TextAlign:=d2TextAlignCenter;
  Increment:=1;
  Max:=10;
  TextW:='0';
  Value:=0;
end;

destructor TD2SpinBox.Destroy;
begin
  inherited;
end;

procedure TD2SpinBox.ApplyStyle;
var
  B:TD2Object;
begin
  inherited;
  B:=FindResource('minusbutton');
  if (B <> nil) and (B is TD2CustomButton) then
  begin
    FMinus:=TD2CustomButton(B);
    FMinus.OnClick:=DoMinusClick;
  end;
  B:=FindResource('plusbutton');
  if (B <> nil) and (B is TD2CustomButton) then
  begin
    FPlus:=TD2CustomButton(B);
    FPlus.OnClick:=DoPlusClick;
  end;
end;

procedure TD2SpinBox.FreeStyle;
begin
  inherited;
  FMinus:=nil;
  FPlus:=nil;
end;

procedure TD2SpinBox.DoMinusClick(Sender:TObject);
begin
  SetFocus;
  Value:=Value - Increment;
  FNeedChange:=true;
  Change;
end;

procedure TD2SpinBox.DoPlusClick(Sender:TObject);
begin
  SetFocus;
  Value:=Value + Increment;
  FNeedChange:=true;
  Change;
end;

function TD2SpinBox.GetData:Variant;
begin
  Result:=Value;
end;

procedure TD2SpinBox.SetData(const Value:Variant);
begin
  if VarIsEvent(Value) then
    OnChange:=VariantToEvent(Value)
  else
    Self.Value:=Value;
end;

procedure TD2SpinBox.KeyDown(var Key:Word; var KeyChar:System.WideChar; Shift:TShiftState);
begin
  case Key of
    VK_UP:Value:=Value + Increment;
    VK_DOWN:Value:=Value - Increment;
  else
    inherited;
    Exit;
  end;
  Key:=0;
end;

procedure TD2SpinBox.Change;
begin
  try
    FValue:=StrToFloat(TextW);
    if FValue > FMax then FValue:=FMax;
    if FValue < FMin then FValue:=FMin;
    if (frac(FValue)=0) or (FValueType=d2ValueInteger) then
      FText:=IntToStr(Trunc(FValue))
    else
      FText:=FloattoStr(FValue);
  except
    if (frac(FValue)=0) or (FValueType=d2ValueInteger) then
      FText:=IntToStr(Trunc(FValue))
    else
      FText:=FloattoStr(FValue);
  end;
  Repaint;
  inherited;
end;

procedure TD2SpinBox.SetMax(const Value:single);
begin
  FMax:=Value;
end;

procedure TD2SpinBox.SetMin(const Value:single);
begin
  FMin:=Value;
end;

procedure TD2SpinBox.SetValue(const AValue:single);
begin
  if FValue <> AValue then
  begin
    FValue:=AValue;
    if FValue > FMax then FValue:=FMax;
    if FValue < FMin then FValue:=FMin;
    if (frac(FValue)=0) or (FValueType=d2ValueInteger) then
      FText:=IntToStr(Trunc(FValue))
    else
      FText:=Format('%' + DecimalSeparator + IntToStr(FDecimalDigits) + 'f',[FValue]);
    SelLength:=0;
    Repaint;
  end;
end;

procedure TD2SpinBox.SetValueType(const Value:TD2ValueType);
begin
  if FValueType <> Value then
  begin
    FValueType:=Value;
    if (FValueType=d2ValueInteger) and (Frac(Increment) <> 0) then
      Increment:=1;
  end;
end;

procedure TD2SpinBox.SetDecimalDigits(const Value:integer);
begin
  FDecimalDigits:=Value;
end;

procedure TD2SpinBox.SetText(const Value:String); // 7777
begin
  inherited;
end;

//============  TD2ColorBox ==========================

constructor TD2ColorBox.Create(AOwner:TComponent);
begin
  inherited;
end;

procedure TD2ColorBox.Paint;
var
  i,j:integer;
  SaveIndex:integer;
begin
  SaveIndex:=Canvas.SaveCanvas;
  Canvas.IntersectClipRect(LocalRect);
  Canvas.Stroke.Style:=d2BrushNone;
  Canvas.Fill.Style:=d2BrushSolid;
  Canvas.Fill.SolidColor:=$FFFFFFFF;
  Canvas.FillRect(LocalRect,0,0,AllCorners,AbsoluteOpacity);
  Canvas.Fill.SolidColor:=$FFD3D3D3;
  for i:=0 to Trunc(Width / 5) + 1 do
    for j:=0 to Trunc(Height / 5) + 1 do
    begin
      if Odd(i + j) then
      begin
        Canvas.FillRect(d2Rect(i * 5,j * 5,(i + 1) * 5,(j + 1) * 5),0,0,AllCorners,AbsoluteOpacity);
      end;
    end;
  Canvas.RestoreCanvas(SaveIndex);

  Canvas.Fill.Style:=d2BrushSolid;
  Canvas.Fill.SolidColor:=FColor;
  Canvas.FillRect(LocalRect,0,0,AllCorners,AbsoluteOpacity);
end;

procedure TD2ColorBox.SetColor(const Value:TD2Color);
begin
  if FColor <> Value then
  begin
    FColor:=Value;
    Repaint;
  end;
end;


//=============== TD2CalendarBox =================================================

constructor TD2CalendarBox.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=true;
  Cursor:=crDefault;
  TextAlign:=d2TextAlignNear;
  FResource:='combotextboxstyle';
  FPopup:=TD2Popup.Create(Self);
  FPopup.FResource:='combopopupstyle';
  FPopup.PlacementTarget:=Self;
  FPopup.StaysOpen:=false;
  FPopup.Stored:=false;
  FPopup.Parent:=Self;
  FPopup.Locked:=true;
  FPopup.DesignHide:=true;
  FPopup.OnClosePopup:=DoClosePopup;
  FPopup.Width:=186;
  FPopup.Height:=166;
  FCalendar:=TD2Calendar.Create(Self);
  FCalendar.Parent:=FPopup;
  FCalendar.Stored:=false;
  FCalendar.Padding.Rect:=d2Rect(3,3,3,3);
  FCalendar.Align:=vaClient;
  FCalendar.OnChange:=DoCalendarChanged;
  FCalendar.OnDayChange:=DoDayChanged;
  TextW:=DateTimeToStr(FCalendar.DateTime);
  Width:=100;
  Height:=22;
end;

destructor TD2CalendarBox.Destroy;
begin
  inherited;
end;

procedure TD2CalendarBox.DoCalendarChanged(Sender:TObject);
begin
  TextW:=DateTimeToStr(FCalendar.DateTime);
end;

procedure TD2CalendarBox.DoDayChanged(Sender:TObject);
begin
  if FPopup.IsOpen then
    DropDown;
end;

procedure TD2CalendarBox.DoClosePopup(Sender:TObject);
begin
end;

procedure TD2CalendarBox.DropDown;
begin
  if not FPopup.IsOpen then
  begin
    FPopup.Placement:=FPlacement;
    FPopup.IsOpen:=true;
  end
  else
  begin
    FPopup.IsOpen:=false;
  end;
end;

function TD2CalendarBox.GetDate:TDate;
begin
  Result:=FCalendar.Date;
end;

procedure TD2CalendarBox.SetDate(const Value:TDate);
begin
  FCalendar.Date:=Value;
  TextW:=DateTimeToStr(FCalendar.DateTime);
end;

procedure TD2CalendarBox.MouseDown(Button:TMouseButton;
  Shift:TShiftState; X,Y:single);
begin
  inherited;
  if (Button=mbLeft) then
  begin
    DropDown;
  end;
end;

procedure TD2CalendarBox.DoContentPaint(Sender:TObject;
  const Canvas:TD2Canvas; const ARect:TD2Rect);
begin
  Canvas.Font.Assign(Font);
  Canvas.Fill.Assign(FontFill);
  Canvas.FillText(ARect,ARect,TextW,false,AbsoluteOpacity,TextAlign,d2TextAlignCenter);
end;

procedure TD2CalendarBox.ApplyStyle;
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

//=============== TD2CalendarTextBox =====================================================

constructor TD2CalendarTextBox.Create(AOwner:TComponent);
begin
  inherited;
  Cursor:=crDefault;
  FFilterChar:='0123456789./';
  FResource:='combotextboxstyle';
  FPopup:=TD2Popup.Create(Self);
  FPopup.FResource:='combopopupstyle';
  FPopup.PlacementTarget:=Self;
  FPopup.StaysOpen:=false;
  FPopup.Stored:=false;
  FPopup.Parent:=Self;
  FPopup.Locked:=true;
  FPopup.DesignHide:=true;
  FPopup.OnClosePopup:=DoClosePopup;
  FPopup.Width:=186;
  FPopup.Height:=166;
  FCalendar:=TD2Calendar.Create(Self);
  FCalendar.Parent:=FPopup;
  FCalendar.Stored:=false;
  FCalendar.Padding.Rect:=d2Rect(3,3,3,3);
  FCalendar.Align:=vaClient;
  FCalendar.OnChange:=DoCalendarChanged;
  FCalendar.OnDayChange:=DoDayChanged;
  TextW:=DateTimeToStr(FCalendar.DateTime);
end;

destructor TD2CalendarTextBox.Destroy;
begin
  inherited;
end;

procedure TD2CalendarTextBox.DoCalendarChanged(Sender:TObject);
begin
  if FPopup.IsOpen then
  begin
    TextW:=DateTimeToStr(FCalendar.DateTime);
    CaretPosition:=Length(TextW);
    Change;
    FNeedChange:=false;
    DropDown;
  end;
end;

procedure TD2CalendarTextBox.DoDayChanged(Sender:TObject);
begin
end;

procedure TD2CalendarTextBox.DoClosePopup(Sender:TObject);
begin
  if ShowCaret and IsFocused then
    ShowCaretProc;
end;

procedure TD2CalendarTextBox.DropDown;
begin
  if not FPopup.IsOpen then
  begin
    if ShowCaret then
      HideCaret;
    FPopup.Placement:=FPlacement;
    FPopup.IsOpen:=true;
  end
  else
  begin
    FPopup.IsOpen:=false;
  end;
end;

function TD2CalendarTextBox.GetDate:TDate;
begin
  Result:=FCalendar.Date;
end;

procedure TD2CalendarTextBox.SetDate(const Value:TDate);
begin
  FCalendar.Date:=Value;
  TextW:=DateTimeToStr(FCalendar.DateTime);
end;

procedure TD2CalendarTextBox.MouseDown(Button:TMouseButton;
  Shift:TShiftState; X,Y:single);
begin
  inherited;
  if (Button=mbLeft) and not d2PtInRect(d2Point(X,Y),ContentRect) then
  begin
    DropDown;
  end;
end;
 
//=============== TD2PaintBox ===========================================

constructor TD2PaintBox.Create(AOwner:TComponent);
begin
  inherited;
end;

destructor TD2PaintBox.Destroy;
begin
  inherited;
end;

procedure TD2PaintBox.Paint;
begin
  inherited;
  if Assigned(FOnPaint) then
    FOnPaint(Self,Canvas);
end;                   
                       
