{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2MemoLines ==================================================

procedure TD2MemoLines.Clear;
begin
  FMemo.ClearLines;
  FMemo.Change;
end;

procedure TD2MemoLines.Delete(Index:integer);
begin
  FMemo.DeleteLine(Index);
  FMemo.Change;
end;

procedure TD2MemoLines.Insert(Index:integer; const S:WideString);
begin
  FMemo.InsertLine(Index,S);
  FMemo.Change;
end;

function TD2MemoLines.Get(Index:integer):WideString;
begin
  Result:=FMemo.GetLine(Index);
end;

function TD2MemoLines.GetCount:integer;
begin
  Result:=FMemo.GetLineCount;
end;

procedure TD2MemoLines.SetUpdateState(Updating:boolean);
begin
  inherited;
  FMemo.SetUpdateState(Updating);
end;

function ComposeCaretPos(ALine,APos :integer) :TCaretPosition;
begin
  with Result do
  begin
    Line:=ALine;
    Pos:=APos;
  end;
end;

//=============== TD2Memo ===============================================

constructor TD2Memo.Create(AOwner:TComponent);
begin
  inherited Create(AOwner);
  FTextAlign:=d2TextAlignNear;
  FFont:=TD2Font.Create;
  FFont.OnChanged:=FontChanged;
  FFontFill:=TD2Brush.Create(d2BrushSolid,$FF000000);
  FSelectionFill:=TD2Brush.Create(d2BrushSolid,$802A8ADF);
  FLines:=TD2MemoLines.Create;
  (FLines as TD2MemoLines).FMemo:=Self;
  CanFocused:=true;
  Cursor:=crIBeam;
  FInternalMustUpdateLines:=true;

  CreatePopupMenu;

  FActionStack:=TEdtActionStack.Create(Self);

  FTextAlignment:=taLeftJustify;
  FAutoSelect:=true;
  FCharCase:=d2ecNormal;
  FHideSelection:=true;
  FMaxLength:=0;
  FReadOnly:=false;

  FLMouseSelecting:=false;
  FOldMPt:=d2Point(0,0);

  FUpdating:=false;


  with FCaretPosition do
  begin
    Line:=0;
    Pos:=0;
  end;

  FSelStart:=ComposeCaretPos(0,0);
  FSelEnd:=ComposeCaretPos(0,0);
  FSelected:=false;

  FOldSelStartPos:=-1;
  FOldSelEndPos:=-1;
  FOldCaretPos:=-1;

  AutoCapture:=true;

  FWidesLineIndex:=0;
  SetLength(FTextWidth,0);

  Width:=100;
end;

destructor TD2Memo.Destroy;
begin
  if FUnwrapLines <> nil then
    FUnwrapLines.Free;
  FSelectionFill.Free;
  FFontFill.Free;
  FFont.Free;
  FActionStack.Free;
  FPopupMenu.Free;
  FLines.Free;
  inherited;
end;

procedure TD2Memo.EnterFocus;
begin
  inherited;
  FNeedChange:=false;
  UpdateCaretPosition(false);
  ShowCaretProc;
  if FScene.ShowKeyboardForControl(Self) then
  begin
    CaretPosition:=TextPosToPos(Length(TextW));
  end
  else
  begin
    with FCaretPosition do
    begin
      Line:=0;
      Pos:=0;
    end;
    if AutoSelect then
      SelectAll;
  end;
end;

procedure TD2Memo.Killfocus;
begin
  FScene.HideKeyboardForControl(Self);
  inherited;
  HideCaret;
  Change;
end;

function TD2Memo.TextWidth(const Str:WideString):single;
var
  R:TD2Rect;
begin
  R:=ContentRect;
  R.Right:=10000;
  Canvas.Font.Assign(Font);
  Canvas.MeasureText(R,R,Str,false,TextAlign,d2TextAlignCenter);
  Result:=d2RectWidth(R);
end;

function TD2Memo.GetPositionPoint(ACaretPos :TCaretPosition):TD2Point;
var
  WholeTextWidth :single;
  EditRectWidth :single;
  LineText :WideString;
begin
  Result.X:=ContentRect.Left;
  Result.Y:=ContentRect.Top + (GetLineHeight * ACaretPos.Line) - VScrollBarValue;
  WholeTextWidth:=0;
  if Canvas=nil then Exit;

  if (ACaretPos.Line < Lines.Count) and (Lines.Count > 0) then
  begin
    LineText:=Lines[ACaretPos.Line];

    WholeTextWidth:=TextWidth(LineText);

    if ACaretPos.Pos > 0 then
    begin
      if ACaretPos.Pos <= Length(LineText) then
        Result.X:=Result.X + TextWidth(Copy(LineText,1,ACaretPos.Pos))
      else
        Result.X:=Result.X + TextWidth(LineText);
    end;
  end;
  EditRectWidth:=ContentRect.Right - ContentRect.Left;
  if WholeTextWidth < EditRectWidth then
    case FTextAlign of
      d2TextAlignFar:Result.X:=Result.X + (EditRectWidth-WholeTextWidth);
      d2TextAlignCenter:Result.X:=Result.X + ((EditRectWidth-WholeTextWidth) / 2);
    end;
  Result.X:=Result.X - HScrollBarValue;
end;

function TD2Memo.GetPointPosition(Pt :TD2Point):TCaretPosition;
var
  CurX:double;
  TmpX,
  WholeTextWidth,
  EdiTD2RectWidth :single;
  LineText :WideString;
  LLine :integer;
  LPos :integer;
  TmpPt :TD2Point;
  LEdiTD2Rect :TD2Rect;
begin
  with Result do
  begin
    Line:=0;
    Pos:=0;
  end;

  if Lines.Count <= 0 then
    Exit;

  LEdiTD2Rect:=ContentRect;

  with LEdiTD2Rect,Pt do begin
    if x < Left then
      TmpPt.x:=Left
    else
      if x > Right then
        TmpPt.x:=Right
      else
        TmpPt.x:=x;

    if y < Top then
      TmpPt.y:=Top
    else
      if y > Bottom then
        TmpPt.y:=Bottom
      else
        TmpPt.y:=y;
  end;

  LLine:=trunc((TmpPt.Y - ContentRect.Top) / GetLineHeight + trunc(VScrollBarValue / GetLineHeight));

  LPos:=0;

  if LLine > Lines.Count-1 then
    LLine:=Lines.Count-1;

  LineText:=Lines[LLine];

  if Length(LineText) > 0 then
  begin
    WholeTextWidth:=TextWidth(LineText);

    EdiTD2RectWidth:=ContentRect.Right - ContentRect.Left;
    TmpX:=TmpPt.x;
    if WholeTextWidth < EdiTD2RectWidth then
      case TextAlign of
        d2TextAlignFar:TmpX:=TmpPt.x - (EdiTD2RectWidth-WholeTextWidth);
        d2TextAlignCenter:TmpX:=TmpPt.x - ((EdiTD2RectWidth-WholeTextWidth) / 2);
      end;

    TmpX:=TmpX + HScrollBarValue;

    CurX:=ContentRect.Left + TextWidth(LineText[1]) / 2;
    while (CurX < TmpX) and (LPos + 1 <= Length(LineText)) and (CurX < ContentRect.Right + HScrollBarValue) do
    begin
      CurX:=ContentRect.Left + TextWidth(Copy(LineText,1,LPos + 1)) + (Font.Size / 4);
      Inc(LPos);
    end;
  end;
  with Result do
  begin
    Line:=LLine;
    Pos:=LPos;
  end;
end;

procedure TD2Memo.KeyDown(var Key:Word; var KeyChar:System.WideChar; Shift:TShiftState);
var
  TmpS:WideString;
  OldCaretPosition:TCaretPosition;
  WasSelection :boolean;
  LTmpOptions :TInsertOptions;
begin
  inherited KeyDown(Key,KeyChar,Shift);
  OldCaretPosition:=CaretPosition;
  if (Key=VK_RETURN) then
  begin
    WasSelection:=SelLength > 0;
    if WasSelection then
      DeleteFrom(GetSelBeg,SelLength,[doMoveCaret,doCanUndo]{true,true,false});
    if WasSelection then
      LTmpOptions:=[ioUnDoPairedWithPriv]
    else
      LTmpOptions:=[];
    TmpS:=#13#10;
    InsertAfter(CaretPosition,TmpS,LTmpOptions+[ioMoveCaret,ioCanUndo]{false,true,true,WasSelection});
    SelLength:=0;
    Key:=0;
  end;
  case Key of
    VK_END:if ssCtrl in Shift then
              GoToTextEnd
            else
              GoToLineEnd;
    VK_HOME:if ssCtrl in Shift then
               GoToTextBegin
             else
               GoToLineBegin;
    VK_LEFT:
      if ssCtrl in Shift then
        CaretPosition:=GetPrivWordBeging(CaretPosition)
      else
        MoveCaretLeft;
    VK_RIGHT:
      if ssCtrl in Shift then
        CaretPosition:=GetNextWordBeging(CaretPosition)
      else
        MoveCaretRight;
    VK_UP:
      MoveCaretUp;
    VK_DOWN:
      MoveCaretDown;
    VK_PRIOR:
      MoveCaretPageUp;
    VK_NEXT:
      MoveCaretPageDown;
    VK_DELETE,8:{Delete or BackSpace key was pressed}
      if not ReadOnly then
      begin
        if SelLength <> 0 then
        begin
          if ssShift in Shift then
            CutToClipboard
          else
            ClearSelection;
        end
        else
        begin
          TmpS:=TextW;
          if Key=VK_DELETE then
            DeleteFrom(CaretPosition,1,[doMoveCaret,doCanUndo])
          else {BackSpace key was pressed}
            if PosToTextPos(CaretPosition) > 0 then
              DeleteFrom(GetPositionShift(CaretPosition,-1),1,[doMoveCaret,doCanUndo]);
        end;
      end;
    VK_INSERT:
      if ssCtrl in Shift then
        CopyToClipboard
      else
        if ssShift in Shift then
          PasteFromClipboard;
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

  if ((Ord(Keychar) >= 32) or (Keychar=#13)) and not ReadOnly then
  begin
    WasSelection:=SelLength > 0;
    if WasSelection then
      DeleteFrom(GetSelBeg,SelLength,[doMoveCaret,doCanUndo]{true,true,false});
    if WasSelection then
      LTmpOptions:=[ioUnDoPairedWithPriv]
    else
      LTmpOptions:=[];
    if Keychar <> #13 then
    begin
      InsertAfter(CaretPosition,KeyChar,LTmpOptions+[ioMoveCaret,ioCanUndo]{false,true,true,WasSelection});
    end
    else
    begin
      TmpS:=#13#10;
      InsertAfter(CaretPosition,TmpS,LTmpOptions+[ioMoveCaret,ioCanUndo]{false,true,true,WasSelection});
    end;
    SelLength:=0;
    Keychar:=#0;
  end;
  
  if Key in [VK_END,VK_HOME,VK_LEFT,VK_RIGHT,VK_UP,VK_DOWN,VK_PRIOR,VK_NEXT] then
  begin
    if ssShift in Shift then
    begin
      if not FSelected then
        SelectAtPos(OldCaretPosition);
      SelectAtPos(CaretPosition);
      Repaint;
    end else
      if FSelected then begin
        FSelected:=false;
        Repaint;
      end;
  end;
  UpdateCaretPosition(true);
end;

procedure TD2Memo.MouseDown(Button:TMouseButton; Shift:TShiftState; x,y:single);
begin
  inherited;
  if (Button=mbLeft) and  (ssDouble in Shift) then
  begin
    if d2PtInRect(d2Point(x,y),ContentRect) then
    begin
      FLMouseSelecting:=false;
      SelectWord;
    end;
  end;
  if (Button=mbLeft) and  d2PtInRect(d2Point(x,y),ContentRect) then
  begin
    FLMouseSelecting:=true;
    CaretPosition:=GetPointPosition(d2Point(x,y));
    FSelected:=false;
    SelectAtPos(CaretPosition);
    Repaint;
  end;
end;

function TD2Memo.ContentPos:TD2Point;
var
  T:TD2Object;
begin
  T:=FindResource('content');
  if (T <> nil) and (T.IsVisual) then
  begin
    Result:=TD2VisualObject(T).Position.Point;
  end;
end;

procedure TD2Memo.DoContentPaint(Sender:TObject; const Canvas:TD2Canvas; const ARect:TD2Rect);
var
  R,TmpRect:TD2Rect;
  LSelArea :TSelArea;
  CurSelRect :integer;
  SaveIndex:integer;
  CurChar,CurLine,LEndLine:integer;
  TmpPt :TD2Point;
  LPageSize :single;
  LLeftTopCharPt :TD2Point;
begin
  with Canvas do
  begin
    SaveIndex:=Canvas.SaveCanvas;
    Canvas.IntersectClipRect(ARect);

    Font.Assign(Font);
    Fill.Assign(FFontFill);

    // text
    R:=ContentRect;
    TmpRect:=ARect;
    Canvas.Font.Assign(Font);
    LPageSize:=GetPageSize;

    LLeftTopCharPt.X:=TmpRect.Left;
    LLeftTopCharPt.Y:=TmpRect.Top;

    CurLine:=trunc(VScrollBarValue / GetLineHeight);
    if round(VScrollBarValue / GetLineHeight) + LPageSize - 1 < Lines.Count-1 then
      LEndLine:=Round(round(VScrollBarValue / GetLineHeight) + LPageSize-1)
    else
      LEndLine:=Lines.Count-1;
    while CurLine <= LEndLine do
    begin
      TmpPt:=GetPositionPoint(ComposeCaretPos(CurLine,0));
      Canvas.FillText(d2Rect(TmpPt.X - R.Left,TmpPt.Y - R.Top,$FFFF,TmpPt.Y - R.Top + (GetLineHeight * 1.25)),LocalRect,Lines[CurLine],false,1,d2TextAlignNear,d2TextAlignNear);
      Inc(CurLine);
    end;

    // selection
    if IsFocused then
    begin
      LSelArea:=GetSelArea;
      if GetShowSelection then
      begin
        Fill.Assign(FSelectionFill);
        for CurSelRect:=Low(LSelArea) to High(LSelArea) do
        begin
          d2IntersectRect(TmpRect,LSelArea[CurSelRect],d2Rect(0,0,1000,1000));
          d2OffsetRect(TmpRect,-R.Left,-R.Top);
          FillRect(TmpRect,0,0,[],1);
        end;
      end;
    end;
    Canvas.RestoreCanvas(SaveIndex);
  end;
end;

procedure TD2Memo.ApplyStyle;
var
  T:TD2Object;
begin
  inherited;
  T:=FindResource('content');
  if (T <> nil) and (T.IsVisual) then
  begin
    TD2VisualObject(T).OnPaint:=DoContentPaint;
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

procedure TD2Memo.UpdateHScrlBarByCaretPos;
var
  LEdiTD2Rect:TD2Rect;
  LCaretLinePos :integer;
  LCaretLine :integer;
  CurCaretX :integer;
begin
  if Lines.Count <= 0 then
    Exit;
  if Canvas=nil then Exit;

  LEdiTD2Rect:=ContentRect;
  CurCaretX:=Round(GetPositionPoint(CaretPosition).X);

  if not ((CurCaretX < LEdiTD2Rect.Left) or
          (CurCaretX > LEdiTD2Rect.Right)) then
    Exit;

  LCaretLinePos:=CaretPosition.Pos;
  LCaretLine:=CaretPosition.Line;

  if FFirstVisibleChar >= (LCaretLinePos + 1) then
  begin
    FFirstVisibleChar:=LCaretLinePos;
    if FFirstVisibleChar < 1 then
      FFirstVisibleChar:=1;
  end
  else
  begin                                                                                                                                          // caret
    while (TextWidth(Copy(Lines[LCaretLine],FFirstVisibleChar,LCaretLinePos - FFirstVisibleChar + 1)) > LEdiTD2Rect.Right - LEdiTD2Rect.Left - 5) and (FFirstVisibleChar < Length(Lines[LCaretLine])) do
      Inc(FFirstVisibleChar);
  end;
  if (HScrollBar <> nil) and (HScrollBar.Visible) then
    HScrollBar.Value:=TextWidth(Copy(Lines[LCaretLine],1,FFirstVisibleChar-1));
end;

procedure TD2Memo.MouseMove(Shift:TShiftState; x,y,dx,dy:single);
var
  LEdiTD2Rect :TD2Rect;
begin
  inherited;
  FOldMPt:=d2Point(x,y);

  if FLMouseSelecting then
  begin
    LEdiTD2Rect:=ContentRect;

{    if y < LEdiTD2Rect.Top then
      VScrollBar.AutoScrollUp:=true
    else
      if y > LEdiTD2Rect.Bottom then
        VScrollBar.AutoScrollDown:=true
      else begin
        VScrollBar.AutoScrollDown:=false;
        VScrollBar.AutoScrollUp:=false;
      end;}

    SelectAtMousePoint;
  end;
end;

procedure TD2Memo.MouseUp(Button:TMouseButton; Shift:TShiftState;
  x,y:single);
begin
  inherited;
  FLMouseSelecting:=false;
  if SelLength=0 then
    FSelected:=false;
end;

procedure TD2Memo.CopyToClipboard;
var
  Data:THandle;
  DataPtr:Pointer;
  Size:Cardinal;
  S:WideString;
begin
  if SelText <> '' then
    Clipbrd.Clipboard.AsText:=UTF8Encode(SelText);
end;

procedure TD2Memo.PasteFromClipboard;
var
  WasSelection :boolean;
  Data:THandle;
  Insertion:WideString;
begin
  if ReadOnly then Exit;
  try

    WasSelection:=SelLength >0;
    if WasSelection then
    begin
      DeleteFrom(GetSelBeg,SelLength,[doMoveCaret,doCanUndo]);
      InsertAfter(GetSelBeg,UTF8Decode(ClipBoard.AsText),[ioMoveCaret,ioCanUndo,ioUndoPairedWithPriv]);
    end
    else
      InsertAfter(CaretPosition,UTF8Decode(ClipBoard.AsText),[ioMoveCaret,ioCanUndo]);


    Change;
  finally
  end;
end;

procedure TD2Memo.CreatePopupMenu;
var
  TmpItem:TMenuItem;
begin

  FPopupMenu:=TPopupMenu.Create(Self);

  TmpItem:=TMenuItem.Create(FPopupMenu);
  with TmpItem do
  begin
    Caption:='Undo';
    OnClick:=DoUndo;
  end;
  FPopupMenu.Items.Add(TmpItem);

//  FPopupMenu.Items.NewBottomLine;

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

procedure TD2Memo.DoCut(Sender:TObject);
begin
  CutToClipboard;
end;

procedure TD2Memo.DoCopy(Sender:TObject);
begin
  CopyToClipboard;
end;

procedure TD2Memo.DoDelete(Sender:TObject);
begin
  ClearSelection;
end;

procedure TD2Memo.DoPaste(Sender:TObject);
begin
  PasteFromClipboard;
end;

procedure TD2Memo.UpdatePopupMenuItems;
var
  SelTextEmpty:boolean;
begin
  SelTextEmpty:=SelText <> '';
  FPopupMenu.Items.Find('Undo').Enabled:=FActionStack.AtLeast(1) and not ReadOnly;
  FPopupMenu.Items.Find('Cut').Enabled:=SelTextEmpty and not ReadOnly;
  FPopupMenu.Items.Find('Copy').Enabled:=SelTextEmpty;
  FPopupMenu.Items.Find('Paste').Enabled:=(ClipBoard.AsText <> '') and not ReadOnly;
  FPopupMenu.Items.Find('Delete').Enabled:=SelTextEmpty and not ReadOnly;
  FPopupMenu.Items.Find('Select All').Enabled:=SelText <> TextW;
end;

function TD2Memo.GetNextWordBeging(StartPosition:TCaretPosition):TCaretPosition;
var
  SpaceFound,
  WordFound:boolean;
  LLineText :WideString;
  CurPos :integer;
  CurLine :integer;
begin
  CurPos:=StartPosition.Pos;
  CurLine:=StartPosition.Line;

  if StartPosition.Pos < Length(GetLine(StartPosition.Line)) then begin
    LLineText:=GetLine(StartPosition.Line);

    SpaceFound:=false;
    WordFound:=false;
    while (CurPos + 2 <= Length(LLineText)) and
      ((not ((LLineText[CurPos + 1] <> ' ') and SpaceFound))
      or not WordFound) do
    begin
      if LLineText[CurPos + 1]=' ' then
        SpaceFound:=true;
      if LLineText[CurPos + 1] <> ' ' then begin
        WordFound:=true;
        SpaceFound:=false;
      end;

      CurPos:=CurPos + 1;
    end;
    if not SpaceFound then
      CurPos:=CurPos + 1;
  end else
    if StartPosition.Line < Lines.Count-1 then begin
      CurLine:=CurLine+1;
      CurPos:=0;
    end;

  with Result do begin
    Line:=CurLine;
    Pos:=CurPos;
  end
end;

function TD2Memo.GetPrivWordBeging(StartPosition:TCaretPosition):TCaretPosition;
var
  WordFound:boolean;
  LLineText :WideString;
  CurPos :integer;
  CurLine :integer;
begin
  Result:=StartPosition;

  CurPos:=StartPosition.Pos;
  CurLine:=StartPosition.Line;

  if StartPosition.Pos > 0 then begin
    LLineText:=GetLine(StartPosition.Line);

    WordFound:=false;
    while (CurPos > 0) and
      ((LLineText[CurPos] <> ' ') or not WordFound) do
    begin
      if LLineText[CurPos] <> ' ' then
        WordFound:=true;
      CurPos:=CurPos - 1;
    end;
  end else
    if (StartPosition.Line-1 >= 0) and (StartPosition.Line-1<=Lines.Count-1) then begin
      CurLine:=CurLine-1;
      CurPos:=Length(GetLine(CurLine));
    end;

  with Result do begin
    Line:=CurLine;
    Pos:=CurPos;
  end
end;


procedure TD2Memo.ClearSelection;
begin
  if not ReadOnly then
    DeleteFrom(GetSelBeg,SelLength,[doMoveCaret,doCanUndo]);
end;

procedure TD2Memo.CutToClipboard;
begin
  CopyToClipboard;
  ClearSelection;
end;

procedure TD2Memo.SelectAll;
begin
  FSelStart:=TextPosToPos(Length(FText));
  FSelEnd:=ComposeCaretPos(0,0);
  FSelected:=true;
  GoToTextEnd;
  Repaint;
end;

procedure TD2Memo.DoSelectAll(Sender:TObject);
begin
  SelectAll;
end;

procedure TD2Memo.DrawPasswordChar(SymbolRect:TD2Rect; Selected:boolean);
var
  LRect :TD2Rect;
begin
  d2IntersectRect(LRect,SymbolRect,ContentRect);
  Canvas.Font.Assign(Font);
end;

function TD2Memo.CanAutoSize(var NewWidth,NewHeight:integer):boolean;
begin
  Result:=True;
  NewHeight:=Round(GetLineHeight + ContentRect.Top*2);
end;

procedure TD2Memo.SelectWord;
begin
  FSelStart:=GetPrivWordBeging(CaretPosition);
  FSelEnd:=GetNextWordBeging(CaretPosition);
  FSelected:=true;
  Repaint;
end;

procedure TD2Memo.Change;
begin
  if FNeedChange then
  begin
    if Assigned(FBindingObjects) then
      ToBindingObjects;
    if Assigned(FOnChange) then
      FOnChange(Self);
  end;
end;

procedure TD2Memo.ContextMenu(const ScreenPosition:TD2Point);
begin
  inherited;
  if csDesigning in ComponentState then Exit;

  UpdatePopupMenuItems;
  FPopupMenu.PopupComponent:=Self;
  FPopupMenu.Popup(round(ScreenPosition.X),round(ScreenPosition.Y));
end;

procedure TD2Memo.FontChanged(Sender:TObject);
begin
  inherited;
  if not (csLoading in ComponentState) then
  begin
    UpdateLines;
    UpdateCaretPosition(false);
    if not FUpdating then
      Realign;
  end;
end;

procedure TD2Memo.SetText(const Value:WideString);
begin
  if not ValidText(Value) then Exit;

  if Value <> FText then
  begin
    if (Value <> '') and (CharCase <> d2ecNormal) then
      case CharCase of

        d2ecUpperCase:FText:=WideUpperCase(Value);
        d2ecLowerCase:FText:=WideUpperCase(Value);
      end
    else
      FText:=Value;

    if FInternalMustUpdateLines then
    begin
      UpdateLines;
      if not FUpdating then
        Realign;
    end;

    if not (csLoading in ComponentState) and Assigned(OnChangeTracking) then
      OnChangeTracking(Self);
    FNeedChange:=true;
  end;
end;

procedure TD2Memo.SetText(const Value:String);
 var ss:WideString;
begin
  ss:=UTF8Decode(Value);
  SetText(ss);
end;

Function TD2Memo.GetText:WideString;
begin
  Result:=Ftext;
end;

Function TD2Memo.GetText:string;
begin
  Result:=UTF8Encode(Ftext);
end;

procedure TD2Memo.SetCaretPosition(const Value:TCaretPosition);
begin
  if Value.Line > Lines.Count-1 then
    FCaretPosition.Line:=Lines.Count-1
  else
    FCaretPosition.Line:=Value.Line;

  if FCaretPosition.Line < 0 then
    FCaretPosition.Line:=0;

  if Value.Pos < 0 then
    FCaretPosition.Pos:=0
  else
    if Value.Pos > Length(Lines[FCaretPosition.Line]) then
      FCaretPosition.Pos:=Length(Lines[FCaretPosition.Line])
    else
      FCaretPosition.Pos:=Value.Pos;

  UpdateCaretPosition(true);
end;

procedure TD2Memo.SetSelLength(const Value:integer);
begin
  FSelEnd:=TextPosToPos(PosToTextPos(FSelStart)+value);
end;

procedure TD2Memo.SetSelStart(const Value:integer);
begin
  FSelStart:=TextPosToPos(value);
end;

procedure TD2Memo.SetAutoSelect(const Value:boolean);
begin
  if FAutoSelect <> Value then
    FAutoSelect:=Value;
end;

function TD2Memo.GetSelStart:integer;
begin
  if FSelected then
    Result:=PosToTextPos(GetSelBeg)
  else
    Result:=PosToTextPos(CaretPosition);
end;

function TD2Memo.GetSelArea:TSelArea;
var
  BegLine,EndLine,CurLine :integer;
  LPageSize :single;
  SelBegLineVisible,SelEndLineVisible :boolean;
begin
  if not FSelected then begin
    SetLength(Result,0);
    Exit;
  end;

  SelBegLineVisible:=true;
  SelEndLineVisible:=true;

  BegLine:=GetSelBeg.Line;

  if BegLine < trunc(VScrollBarValue / GetLineHeight) then
  begin
    BegLine:=trunc(VScrollBarValue / GetLineHeight);
    SelBegLineVisible:=false;
  end;

  EndLine:=GetSelEnd.Line;
  LPageSize:=GetPageSize;

  if EndLine > round(VScrollBarValue / GetLineHeight) + LPageSize-1 then
  begin
    EndLine:=Round(round(VScrollBarValue / GetLineHeight) + LPageSize-1);
    SelEndLineVisible:=false;
  end;

  if EndLine < BegLine then
    EndLine:=BegLine;

  SetLength(Result,EndLine-BegLine+1);

  CurLine:=BegLine;
  while (CurLine <= EndLine) and (CurLine < Lines.Count) do
  begin
    with Result[CurLine-BegLine] do
    begin
      Left:=GetPositionPoint(ComposeCaretPos(CurLine,0)).x;
      Right:=GetPositionPoint(ComposeCaretPos(CurLine,Length(Lines[CurLine]))).x;
      Top:=GetPositionPoint(ComposeCaretPos(CurLine,0)).y;
      Bottom:=GetPositionPoint(ComposeCaretPos(CurLine,0)).y+GetLineHeight;
    end;
    Inc(CurLine);
  end;

  if EndLine-BegLine >= 0 then begin
    if SelBegLineVisible then
      Result[0].Left:=GetPositionPoint(ComposeCaretPos(BegLine,GetSelBeg.Pos)).x;
    if SelEndLineVisible then
      Result[EndLine-BegLine].Right:=GetPositionPoint(ComposeCaretPos(EndLine,GetSelEnd.Pos)).x;
  end;
end;

function TD2Memo.GetSelLength:integer;
begin
  if FSelected then
    Result:=PosToTextPos(GetSelEnd)-PosToTextPos(GetSelBeg)
  else
    Result:=0;
end;

function TD2Memo.GetSelText:WideString;
var
  LSelStart,
  LSelLength :integer;
begin
  if FSelected then begin
    LSelStart:=SelStart;
    LSelLength:=SelLength;
    Result:=Copy(TextW,LSelStart + 1,LSelLength);
  end else
    Result:='';
end;

procedure TD2Memo.SetCharCase(const Value:TEditCharCase);
var
  TmpS:WideString;
begin
  if FCharCase <> Value then
  begin
    FCharCase:=Value;
    if TextW <> '' then
    begin
      TmpS:=TextW;
{      case Value of
        vgecUpperCase:TextW:=WideString(CharUpper(PChar(TmpS)));
        vgecLowerCase:TextW:=WideString(CharLower(PChar(TmpS)));
      end;}
    end;
  end;
end;

procedure TD2Memo.SetHideSelection(const Value:boolean);
begin
  if FHideSelection <> Value then
  begin
    FHideSelection:=Value;
    Repaint;
  end;
end;

procedure TD2Memo.SetMaxLength(const Value:integer);
begin
  if FMaxLength <> Value then
  begin
    FMaxLength:=Value;
  end;
end;

function TD2Memo.ValidText(NewText:WideString):boolean;
begin
  Result:=true;
end;

procedure TD2Memo.SetTextAlignment(const Value:TAlignment);
begin
  if FTextAlignment <> Value then
  begin
    FTextAlignment:=Value;
    Repaint;
  end;
end;

procedure TD2Memo.UpdateCaretPosition(UpdateScrllBars :boolean);
var
  TmpPt :TD2Point;
  TmpRect :TD2Rect;
begin
  if UpdateScrllBars then
  begin
    UpdateVScrlBarByCaretPos;
    UpdateHScrlBarByCaretPos;
  end;
  TmpRect:=ContentRect;
  TmpPt:=GetPositionPoint(CaretPosition);
  TmpPt.X:=TmpPt.X + HScrollBarValue - TmpRect.Left;
  TmpPt.Y:=TmpPt.Y + VScrollBarValue - TmpRect.Top;
  SetCaretSize(d2Point(2,GetLineHeight));
  SetCaretPos(TmpPt);
  SetCaretColor(FFontFill.SolidColor);
end;

function TD2Memo.GetLineRealEnd(AStartPos :TCaretPosition; PText :PWideString) :TCaretPosition;
begin
  Result.Line:=AStartPos.Line;
  while (Result.Line+1 <= Lines.Count-1) and
    (GetLineInternal(Result.Line)=GetLine(Result.Line)) do
    Result.Line:=Result.Line + 1;

  if (Result.Line <= Lines.Count-1) and (Lines.Count > 0) then begin
    Result.Pos:=Length(GetLine(Result.Line)) + FLinesBegs[Result.Line]-1
  end else
    Result.Pos:=0;
end;

function TD2Memo.FillLocalLinesBegs(PText:PWideString; ABegChar,AEndChar :integer; TmpLinesBegs :PLinesBegs):integer;
var
  WStartChar,WSaveChar,WCurChar:integer;
  LCurChar,LSaveChar:integer;
  TmpS :WideString;
  TmpSWidth,WWidth:single;
  LTmpWidth:single;
  LEditRectWidth :single;
  Tok:WideString;
  LLongestLineWidth :single;
  CurLineIsEmpty :boolean;
  LWidth:single;
  i:integer;
  LLocalWidesLineWidth:integer;
begin
  Result:=0;
  SetLength(TmpLinesBegs^,0);

  if PText^='' then
    Exit;

  LCurChar:=ABegChar;
  TmpS:='';
  LTmpWidth:=0;
  CurLineIsEmpty:=true;

  with ContentRect do
    LEditRectWidth:=Right-Left;

  Result:=-1;
  LLocalWidesLineWidth:=-1;
  while LCurChar <= AEndChar do
  begin
    if (PText^[LCurChar]=#13) or (PText^[LCurChar]=#10) then
    begin
      LSaveChar:=LCurChar - 1; // before #13
      if (PText^[LCurChar]=#13) and (LCurChar + 1 <= Length(PText^)) then
        if PText^[LCurChar + 1]=#10 then
          Inc(LCurChar);

      TmpSWidth:=TextWidth(TmpS);
      if LLongestLineWidth < TmpSWidth then
      begin
        LLongestLineWidth:=TmpSWidth;
        Result:=Length(TmpLinesBegs^) - 1;
      end;

      if FWordWrap and (TextWidth(TmpS) > LEditRectWidth) then
      begin
        WCurChar:=1;
        WStartChar:=1;
        WSaveChar:=1;
        Tok:=d2WideGetToken(WCurChar,TmpS,' ,-');
        while Tok <> '' do
        begin
          WWidth:=TextWidth(Copy(TmpS,WStartChar,WCurChar - WStartChar));
          if WWidth > LEditRectWidth then
          begin
            WStartChar:=WSaveChar;
            SetLength(TmpLinesBegs^,Length(TmpLinesBegs^) + 1);
            TmpLinesBegs^[Length(TmpLinesBegs^)-1]:=LCurChar - Length(TmpS) + WSaveChar - 2;
          end;
          WSaveChar:=WCurChar;
          Tok:=d2WideGetToken(WCurChar,TmpS,' ,-');
          if WSaveChar=WCurChar then
            Break; {!!! - error }
        end;
      end;

      SetLength(TmpLinesBegs^,Length(TmpLinesBegs^) + 1);
      TmpLinesBegs^[Length(TmpLinesBegs^)-1]:=LCurChar + 1;

      TmpS:='';
      LTmpWidth:=0;
      CurLineIsEmpty:=true;
    end
    else
    begin
      CurLineIsEmpty:=false;
      TmpS:=TmpS + PText^[LCurChar];
    end;
    Inc(LCurChar);
  end;
  if FWordWrap and (TextWidth(TmpS) > LEditRectWidth) then
  begin
    WCurChar:=1;
    WStartChar:=1;
    WSaveChar:=1;
    Tok:=d2WideGetToken(WCurChar,TmpS,' ,-');
    while Tok <> '' do
    begin
      WWidth:=TextWidth(Copy(TmpS,WStartChar,WCurChar - WStartChar));
      if WWidth > LEditRectWidth then
      begin
        WStartChar:=WSaveChar;
        SetLength(TmpLinesBegs^,Length(TmpLinesBegs^) + 1);
        TmpLinesBegs^[Length(TmpLinesBegs^)-1]:=LCurChar - Length(TmpS) + WSaveChar - 1;
      end;
      WSaveChar:=WCurChar;
      Tok:=d2WideGetToken(WCurChar,TmpS,' ,-');
      if WSaveChar=WCurChar then
        Break; {!!! - error }
    end;
  end
  else
  begin
    LWidth:=Canvas.TextWidth(TmpS);
    if LWidth > LLocalWidesLineWidth then
      Result:=Length(TmpLinesBegs^) - 1;
  end;
  if Length(TmpLinesBegs^)=0 then
    Result:=0;
end;

procedure TD2Memo.UpdateLines;
const
  Sep:WideString=' ,-.;:'#65292;
var
  WStartChar,WSaveChar,WCurChar:integer;
  LCurChar,LSaveChar:integer;
  TmpS :WideString;
  TmpSWidth,WWidth:single;
  LTmpWidth:single;
  LEditRectWidth :single;
  Tok,LText:WideString;
  LLongestLineWidth :single;
  CurLineIsEmpty :boolean;
  i:integer;
begin
  FWidesLineIndex:=0;
  SetLength(FLinesBegs,0);
  if TextW='' then
    Exit;

  SetLength(FLinesBegs,0);
  SetLength(FLinesBegs,1);
  with ContentRect do
    LEditRectWidth:=Right - Left;
  if Canvas=nil then Exit;

  // first check linecreaks
  LText:=TextW;
  TmpS:='';
  LCurChar:=1;
  LTmpWidth:=0;
  CurLineIsEmpty:=true;
  FLinesBegs[0]:=1;
  LLongestLineWidth:=0;
  while LCurChar <= Length(LText) do
  begin
    if (LText[LCurChar]=#13) or (LText[LCurChar]=#10) then
    begin
      LSaveChar:=LCurChar - 1; // before #13
      if (LText[LCurChar]=#13) and (LCurChar + 1 <= Length(LText)) then
        if LText[LCurChar + 1]=#10 then
          Inc(LCurChar);

      TmpSWidth:=TextWidth(TmpS);
      if LLongestLineWidth < TmpSWidth then
      begin
        LLongestLineWidth:=TmpSWidth;
        FWidesLineIndex:=Length(FLinesBegs) - 1;
      end;

      if FWordWrap and (TextWidth(TmpS) > LEditRectWidth) then
      begin
        WCurChar:=1;
        WStartChar:=1;
        WSaveChar:=1;
        Tok:=d2WideGetToken(WCurChar,TmpS,Sep);
        while Tok <> '' do
        begin
          WWidth:=TextWidth(Copy(TmpS,WStartChar,WCurChar - WStartChar));
          if WWidth > LEditRectWidth then
          begin
            WStartChar:=WSaveChar;
            SetLength(FLinesBegs,Length(FLinesBegs) + 1);
            FLinesBegs[Length(FLinesBegs) - 1]:=LCurChar - Length(TmpS) + WSaveChar - 2;
          end;
          WSaveChar:=WCurChar;
          Tok:=d2WideGetToken(WCurChar,TmpS,Sep);
          if WSaveChar=WCurChar then
            Break; {!!! - error }
        end;
      end;

      SetLength(FLinesBegs,Length(FLinesBegs) + 1);
      FLinesBegs[Length(FLinesBegs) - 1]:=LCurChar + 1;

      TmpS:='';
      LTmpWidth:=0;
      CurLineIsEmpty:=true;
    end
    else
    begin
      CurLineIsEmpty:=false;
      TmpS:=TmpS + LText[LCurChar];
    end;
    Inc(LCurChar);
  end;
  // last line
  if FWordWrap and (TextWidth(TmpS) > LEditRectWidth) then
  begin
    WCurChar:=1;
    WStartChar:=1;
    WSaveChar:=1;
    Tok:=d2WideGetToken(WCurChar,TmpS,Sep);
    while Tok <> '' do
    begin
      WWidth:=TextWidth(Copy(TmpS,WStartChar,WCurChar - WStartChar));
      if WWidth > LEditRectWidth then
      begin
        WStartChar:=WSaveChar;
        SetLength(FLinesBegs,Length(FLinesBegs) + 1);
        FLinesBegs[Length(FLinesBegs) - 1]:=LCurChar - Length(TmpS) + WSaveChar - 1;
      end;
      WSaveChar:=WCurChar;
      Tok:=d2WideGetToken(WCurChar,TmpS,Sep);
      if WSaveChar=WCurChar then
        Break; {!!! - error }
    end;
  end
  else
    if LLongestLineWidth < Canvas.TextWidth(TmpS) then
      FWidesLineIndex:=Length(FLinesBegs) - 1;
end;

procedure TD2Memo.UpdateRngLinesBegs(PText:PWideString; AUpdBegLine,AUpdEndLine,AUpdBegChar,AUpdEndChar,ACharDelta,AOldWideslLineWidth:integer);
var
  LUpdEndChar,
  LNewWidesLineIndex,
  LLineDelta,i :integer;
  LTmpLinesBegs :TLinesBegs;
begin
  if (Length(FLinesBegs)=0) and (PText^ <> '') then
  begin
    SetLength(FLinesBegs,1);
    FLinesBegs[0]:=1;
  end;

  LUpdEndChar:=AUpdEndChar + ACharDelta;
  LNewWidesLineIndex:=FillLocalLinesBegs(PText,AUpdBegChar,LUpdEndChar,@LTmpLinesBegs) + AUpdBegLine;

  LLineDelta:=Length(LTmpLinesBegs) - (AUpdEndLine-AUpdBegLine);

  if LLineDelta > 0 then
  begin
    SetLength(FLinesBegs,Length(FLinesBegs) + LLineDelta);
    for i:=Length(FLinesBegs)-1 downto AUpdEndLine+1+LLineDelta do
      FLinesBegs[i]:=FLinesBegs[i-LLineDelta] + ACharDelta;
  end
  else
  begin
    for i:=AUpdBegLine+1 to Length(FLinesBegs)-1+LLineDelta do
      FLinesBegs[i]:=FLinesBegs[i-LLineDelta] + ACharDelta;
    SetLength(FLinesBegs,Length(FLinesBegs) + LLineDelta);
  end;

  for i:=0 to Length(LTmpLinesBegs) - 1 do
    if AUpdBegLine+i+1 <= Length(FLinesBegs)-1 then
      FLinesBegs[AUpdBegLine+i+1]:=LTmpLinesBegs[i];

  if FWidesLineIndex > Length(FLinesBegs)-1 then
    FWidesLineIndex:=Round(GetWidestLine)
  else
  if LineWidth[LNewWidesLineIndex] >= AOldWideslLineWidth then
    FWidesLineIndex:=LNewWidesLineIndex
  else
    if not ((FWidesLineIndex < AUpdBegLine) or (FWidesLineIndex > AUpdEndLine)) then
      FWidesLineIndex:=GetWidestLine;

  if not FUpdating then
    Realign;
end;

procedure TD2Memo.InsertAfter(Position:TCaretPosition; S:WideString; Options :TInsertOptions);
var
  LText :WideString;
  Insertion :WideString;
  LUpdBegLine,LUpdBegChar,LUpdEndLine,LUpdEndChar :integer;
  R:integer;
  LInsertionLength :integer;
  LOldWideslLineWidth :single;
begin
  R:=PosToTextPos(CaretPosition);
  LText:=TextW;
  Insertion:=S;
  if MaxLength > 0 then
    Insertion:=Copy(Insertion,1,MaxLength - Length(LText));

  if ioCanUndo in Options then
    FActionStack.FragmentInserted(PosToTextPos(Position),Length(S),ioUnDoPairedWithPriv in Options);

  LUpdBegLine:=Position.Line;
  if (Length(FLinesBegs) > 0) and (Position.Line <= Length(FLinesBegs) - 1) then
    LUpdBegChar:=FLinesBegs[Position.Line]
  else
    LUpdBegChar:=1;

  with GetLineRealEnd(Position,@LText) do
  begin
    LUpdEndLine:=Line;
    LUpdEndChar:=Pos;
  end;

  LInsertionLength:=Length(Insertion);
  LOldWideslLineWidth:=LineWidth[FWidesLineIndex];

  Insert(Insertion,LText,PosToTextPos(Position)+1);
  try
    FInternalMustUpdateLines:=false;
    TextW:=LText;
  finally
    FInternalMustUpdateLines:=true;
  end;

  UpdateRngLinesBegs(@LText,LUpdBegLine,LUpdEndLine,
    LUpdBegChar,LUpdEndChar,LInsertionLength,Round(LOldWideslLineWidth));

  if ioSelected in Options then
  begin
    FSelStart:=Position;
    FSelEnd:=GetPositionShift(Position,Length(Insertion));
    FSelected:=true;
    CaretPosition:=FSelEnd;
  end
  else
  begin
    if not (csLoading in ComponentState) then
    begin
      CaretPosition:=TextPosToPos(R + Length(Insertion));
      UpdateCaretPosition(false);
    end;
  end;

  if not FUpdating then
    Realign;
end;

procedure TD2Memo.DeleteFrom(Position :TCaretPosition; ALength:integer; Options :TDeleteOptions);
var
  LUpdBegLine,LUpdEndLine,
  LUpdBegChar,LUpdEndChar :integer;
  LText :WideString;
  LTmpPos,LTmpLength :integer;
  LOldWideslLineWidth :integer;
begin
  LText:=TextW;

  LTmpLength:=ALength;
  LTmpPos:= PosToTextPos(Position)+1;

  if (LTmpPos+ALength-1+1 <= System.Length(LText)) and
     (LTmpPos+ALength-1 >= 1) and
     (LText[LTmpPos+ALength-1]=#13) and
     (LText[LTmpPos+ALength-1+1]=#10) then
    LTmpLength:=LTmpLength + 1;

  if (LTmpPos-1 >= 0) and
     (LTmpPos <= System.Length(LText)) and
     (LText[LTmpPos]=#10) and
     (LText[LTmpPos-1]=#13) then begin
    LTmpLength:=LTmpLength + 1;
    LTmpPos:=LTmpPos-1;
  end;

  if (doCanUndo in Options) and (LTmpLength > 0) then
    FActionStack.FragmentDeleted(LTmpPos,Copy(LText,LTmpPos,LTmpLength));

  LUpdBegLine:=Position.Line;
  if Position.Line <= Length(FLinesBegs)-1 then
    LUpdBegChar:=FLinesBegs[Position.Line]
  else
    LUpdBegChar:=1;

  with GetLineRealEnd(GetPositionShift(Position,LTmpLength-1),@LText) do begin
    LUpdEndLine:=Line;
    LUpdEndChar:=Pos;
  end;

  LOldWideslLineWidth:=Round(LineWidth[FWidesLineIndex]);

  Delete(LText,LTmpPos,LTmpLength);

  try
    FInternalMustUpdateLines:=false;
    TextW:=LText;
  finally
    FInternalMustUpdateLines:=true;
  end;

  UpdateRngLinesBegs(@LText,LUpdBegLine,LUpdEndLine,
    LUpdBegChar,LUpdEndChar,-LTmpLength,LOldWideslLineWidth);

  if (doMoveCaret in Options) or (SelLength <> 0) then begin
    FSelected:=false;
    CaretPosition:=Position;
  end;

  if not FUpdating then
    Realign;
end;

procedure TD2Memo.DoUndo(Sender:TObject);
begin
  UnDo;
end;

procedure TD2Memo.UnDo;
begin
  FActionStack.RollBackAction;
end;

function TD2Memo.GetContentBounds:TD2Rect;
begin
  Result:=inherited GetContentBounds;
  if FWordWrap then
  begin
    StorePositions;
    UpdateLines;
    RestorePositions;
  end;
  if Lines.Count > 0 then
    Result.Bottom:=Result.Top + (Lines.Count * GetLineHeight);
  // Updating Horizontal scrollbar params
  if not FWordWrap and (TextWidth(Lines[FWidesLineIndex]) > (Result.Right - Result.Left)) then
    Result.Right:=Result.Left + TextWidth(Lines[FWidesLineIndex]) + 10;// for caret
  UpdateHScrlBarByCaretPos;
end;

procedure TD2Memo.SetLines(const Value:TD2WideStrings);
begin
  FLines.Assign(Value);
end;

function TD2Memo.TextPosToPos(APos :integer) :TCaretPosition;
var
  CurRangeBeg,CurRangeEnd :integer;
  TmpI :integer;
begin
  with Result do begin
    Line:=0;
    Pos:=0;
  end;

  if Lines.Count <= 0 then
    Exit;

  CurRangeBeg:=0;
  CurRangeEnd:=Length(FLinesBegs)-1;
  repeat
    if ((CurRangeBeg < Length(FLinesBegs)-1) and
        (APos+1>=FLinesBegs[CurRangeBeg]) and
        (APos+1<FLinesBegs[CurRangeBeg+1]))
    or ((CurRangeBeg=Length(FLinesBegs)-1) and
        (APos+1>=FLinesBegs[CurRangeBeg]))
    then
      CurRangeEnd:=CurRangeBeg
    else
    begin
      if APos+1 < FLinesBegs[CurRangeBeg] then begin
        TmpI:=CurRangeEnd - CurRangeBeg+1;
        CurRangeEnd:=CurRangeBeg;
        CurRangeBeg:=CurRangeBeg - TmpI div 2;
      end else
        if APos+1 >= FLinesBegs[CurRangeEnd] then begin
          TmpI:=CurRangeEnd - CurRangeBeg+1;
          CurRangeBeg:=CurRangeEnd;
          CurRangeEnd:=CurRangeEnd + TmpI div 2;
        end else
          CurRangeEnd:=(CurRangeBeg + CurRangeEnd) div 2;

      if CurRangeBeg < 0 then
        CurRangeBeg:=0;

      if CurRangeEnd < 0 then
        CurRangeEnd:=0;

      if CurRangeEnd > Length(FLinesBegs)-1 then
        CurRangeEnd:=Length(FLinesBegs)-1;

      if CurRangeBeg > Length(FLinesBegs)-1 then
        CurRangeBeg:=Length(FLinesBegs)-1;
    end;

  until CurRangeBeg=CurRangeEnd;
  Result.Line:=CurRangeBeg;

  if Result.Line <= Length(FLinesBegs)-1 then
    Result.Pos:=APos-FLinesBegs[Result.Line]+1;
end;

procedure TD2Memo.MoveCaretLeft;
begin
  MoveCareteBy(-1);
end;

procedure TD2Memo.MoveCaretRight;
begin
  MoveCareteBy(1);
end;

procedure TD2Memo.MoveCareteBy(Delta :integer);
begin
  CaretPosition:=GetPositionShift(CaretPosition,Delta);
end;

procedure TD2Memo.VScrollChange(Sender:TObject);
begin
  inherited ;
  UpdateCaretPosition(false);
end;

function TD2Memo.GetLineHeight:single;
begin
  Result:=round(FFont.Size * (1.25));
end;

procedure TD2Memo.HScrlBarChange(Sender:TObject);
begin
  Repaint;
  UpdateCaretPosition(false);
end;

procedure TD2Memo.UpdateVScrlBarByCaretPos;
var
  LCaretPosLine :integer;
  LPageSize :single;
begin
  LCaretPosLine:=CaretPosition.Line;
  LPageSize:=GetPageSize;

  if (VScrollBar <> nil) and (LCaretPosLine * GetLineHeight < VScrollBarValue) then
    VScrollBar.Value:=LCaretPosLine * GetLineHeight;

  if (VScrollBar <> nil) and ((LCaretPosLine + 1) * GetLineHeight > VScrollBarValue + ContentLayout.Height) then
    VScrollBar.Value:=(LCaretPosLine - LPageSize + 1) * GetLineHeight;
end;

procedure TD2Memo.SetWordWrap(const Value:boolean);
begin
  if FWordWrap <> Value then
  begin
    FWordWrap:=Value;
    UpdateLines;
    if not FUpdating then
      Realign;
    UpdateCaretPosition(false);
  end;
end;

procedure TD2Memo.GetLineBounds(LineIndex:integer; var LineBeg,
  LineLength:integer);
begin
  if Length(FLinesBegs)=0 then
  begin
    LineBeg:=1;
    LineLength:=0;
    Exit;
  end;

  if (LineIndex <= Length(FLinesBegs)-1) and (LineIndex >= 0) then
  begin
    LineBeg:=FLinesBegs[LineIndex];
    if (LineIndex+1 < Length(FLinesBegs))then
      LineLength:=FLinesBegs[LineIndex+1]-LineBeg
    else
      LineLength:=Length(TextW)-LineBeg+1;
  end
  else
  begin
    LineBeg:=0;
    LineLength:=0;
  end;
end;

function TD2Memo.GetLineCount:integer;
begin
  if TextW <> '' then
    Result:=Length(FLinesBegs)
  else
    Result:=0;
end;

function TD2Memo.GetLine(Index:integer):WideString;
begin
  Result:=GetLineInternal(Index);
  if Length(Result) > 0 then
    if Result[Length(Result)]=#10 then
      Delete(Result,Length(Result),1);
  if Length(Result) > 0 then
    if Result[Length(Result)]=#13 then
      Delete(Result,Length(Result),1);
end;

procedure TD2Memo.InsertLine(Index:integer; const S:WideString);
begin
  if Index < GetLineCount then
    InsertAfter(ComposeCaretPos(Index,0),S+#13+#10,[])
  else
    if (Index > 0) and (GetLineCount > 0) then
    begin
      InsertAfter(ComposeCaretPos(Index - 1,Length(GetLineInternal(Index - 1))),#13+#10+S,[])
    end
    else
      InsertAfter(ComposeCaretPos(Index,0),S,[]);
end;

procedure TD2Memo.DeleteLine(Index:integer);
begin
  if Index=GetLineCount - 1 then
    DeleteFrom(ComposeCaretPos(Index-1,Length(GetLineInternal(Index-1))){LLineBeg-1} ,Length(GetLineInternal(Index))+1,[])
  else
    DeleteFrom(ComposeCaretPos(Index,0){LLineBeg},Length(GetLineInternal(Index))+1,[]);
end;

procedure TD2Memo.ClearLines;
begin
  TextW:='';
end;

procedure TD2Memo.SelectAtMousePoint;
var
  TmpPt :TD2Point;
  LEdiTD2Rect :TD2Rect;
begin
  LEdiTD2Rect:=ContentRect;
  TmpPt:=FOldMPt;
  with TmpPt,LEdiTD2Rect do begin
    if y < Top then
      y:=Top
    else
      if y > Bottom then
        y:=Bottom;

    if x < Left then
      x:=Left
    else
      if x > Right then
        x:=Right;
  end;

  CaretPosition:= GetPointPosition(TmpPt);
  SelectAtPos(CaretPosition);
  Repaint;
end;

function TD2Memo.GetPageSize:single;
begin
  with ContentRect do
    Result:=(Bottom-Top) / GetLineHeight;
end;

function TD2Memo.GetLineWidth(LineNum:integer):single;
begin
  if (LineNum >= 0) and (LineNum <= Lines.Count-1) then
  begin
    Result:=Canvas.TextWidth(Lines[LineNum]);
  end
  else
    Result:=0;
end;

function TD2Memo.PosToTextPos(APostion:TCaretPosition):integer;
var
  LTmpLine :integer;
begin
  Result:=0;
  if TextW='' then
    Exit;

  with APostion do begin
    if Line <= Length(FLinesBegs)-1 then
      LTmpLine:=Line
    else
      LTmpLine:=Length(FLinesBegs)-1;

    if LTmpLine < 0 then
      exit;

    Result:=FLinesBegs[LTmpLine];

    if Pos <= Length(GetLineInternal(LTmpLine)) then
      Result:=Result + Pos -1
    else
      Result:=Result + Length(GetLineInternal(LTmpLine)) -1;

  end;
end;

function TD2Memo.GetLineInternal(Index:integer):WideString;
var
  LLineBeg,LLineLength :integer;
begin
  GetLineBounds(Index,LLineBeg,LLineLength);
  Result:=Copy(TextW,LLineBeg,LLineLength);
end;

procedure TD2Memo.GoToTextBegin;
begin
  with FCaretPosition do
  begin
    Line:=0;
    Pos:=0;
  end;
end;

procedure TD2Memo.GoToTextEnd;
begin
  with FCaretPosition do
  begin
    Line:=Lines.Count - 1;
    if Line >= 0 then
      Pos:=Length(Lines[Line])
    else
      Pos:=0;
  end;
end;

procedure TD2Memo.GoToLineEnd;
begin
  with FCaretPosition do
  begin
    if Line <= Lines.Count-1 then
      Pos:=Length(GetLine(CaretPosition.Line));
  end;
end;

procedure TD2Memo.GoToLineBegin;
begin
  with FCaretPosition do
  begin
    Pos:=0;
  end;
end;

function TD2Memo.GetSelBeg:TCaretPosition;
begin
  if FSelStart.Line < FSelEnd.Line then
    Result:=FSelStart
  else
    if FSelEnd.Line < FSelStart.Line then
      Result:=FSelEnd
    else
      if FSelStart.Pos < FSelEnd.Pos then
        Result:=FSelStart
      else
        Result:=FSelEnd;
end;

function TD2Memo.GetSelEnd:TCaretPosition;
begin
  if FSelStart.Line > FSelEnd.Line then
    Result:=FSelStart
  else
    if FSelEnd.Line > FSelStart.Line then
      Result:=FSelEnd
    else
      if FSelStart.Pos > FSelEnd.Pos then
        Result:=FSelStart
      else
        Result:=FSelEnd;
end;

procedure TD2Memo.SelectAtPos(APos:TCaretPosition);
begin
  if not FSelected then begin
    FSelStart:=APos;
    FSelEnd:=APos;
    FSelected:=true;
  end else begin
    FSelEnd:=APos;
  end;
end;

function TD2Memo.GetPositionShift(APos:TCaretPosition;
  Delta:integer):TCaretPosition;
var
  LNewPos :TCaretPosition;
  LNewTextPos :integer;
  i :integer;
  CurLineText :WideString;
begin
  LNewPos:=APos;
  with LNewPos do
    if Delta >= 14 then begin
      LNewTextPos:=PosToTextPos(CaretPosition)+Delta;

      if Delta > 0 then begin
        if (LNewTextPos+1 <= Length(TextW)) and
           (TextW[LNewTextPos+1]=#10) then
          Inc(LNewTextPos);
      end else
        if Delta < 0 then begin
          if (LNewTextPos+1-1 >= Length(TextW)) and
             (TextW[LNewTextPos+1-1]=#10) then
          Dec(LNewTextPos);
        end;

      LNewPos:=TextPosToPos(LNewTextPos);
    end else begin
      CurLineText:=GetLineInternal(Line);
      if Delta > 0 then begin
        i:=1;
        while i <= Delta do begin
          Pos:=Pos+1;
          if (Pos+1 <= Length(CurLineText)) and (CurLineText[Pos+1]=#10) then begin
            Inc(Pos); Inc(i);
          end;
          if Pos+1 > Length(CurLineText) then begin
            if Line+1 <= Lines.Count-1 then begin
              Line:=Line+1;
              CurLineText:=GetLineInternal(Line);
              Pos:=0;
            end else
              Pos:=Length(CurLineText);
          end;
          Inc(i);
        end;
      end else begin {Delta < 0}
        i:=1;
        while i <= Abs(Delta) do begin
          if Pos-1 >= 0 then
            Pos:=Pos-1
          else begin
            if Line -1 >= 0 then begin
              Line:=Line-1;
              CurLineText:=GetLineInternal(Line);
              if CurLineText[Length(CurLineText)]=#10 then
                Pos:=Length(CurLineText)-2
              else
                Pos:=Length(CurLineText)-1;
            end;
          end;
          Inc(i);
        end;
      end;
    end;
  Result:=LNewPos;
end;

procedure TD2Memo.RestorePositions;
begin
  if FOldCaretPos >= 0 then
    CaretPosition:=TextPosToPos(FOldCaretPos);
  if FSelected and (FOldSelStartPos >= 0) then begin
    FSelStart:=TextPosToPos(FOldSelStartPos);
    FSelEnd:=TextPosToPos(FOldSelEndPos);
    FOldSelStartPos:=-1;
  end;
end;

procedure TD2Memo.StorePositions;
begin
  FOldCaretPos:=PosToTextPos(CaretPosition);
  if FSelected then begin
    FOldSelStartPos:=PosToTextPos(FSelStart);
    FOldSelEndPos:=PosToTextPos(FSelEnd);
  end;
end;

procedure TD2Memo.MoveCaretVertical(LineDelta:integer);
var
  NewLine,NewY,OldX :integer;
begin
  with FCaretPosition do
  begin
    NewLine:=Line+LineDelta;
    if NewLine < 0 then
      NewLine:=0
    else
      if NewLine > Lines.Count -1 then
        NewLine:=Lines.Count -1;

    NewY:=Round(GetPositionPoint(ComposeCaretPos(NewLine,Pos)).Y);
    OldX:=Round(GetPositionPoint(CaretPosition).X);
    Line:=NewLine;
    Pos:=Round(GetPointPosition(d2Point(OldX,NewY)).Pos);
  end;
end;

procedure TD2Memo.MoveCaretDown;
begin
  MoveCaretVertical(1);
end;

procedure TD2Memo.MoveCaretUp;
begin
  MoveCaretVertical(-1);
end;

procedure TD2Memo.MoveCaretPageDown;
begin
  MoveCaretVertical(round(GetPageSize));
end;

procedure TD2Memo.MoveCaretPageUp;
begin
  MoveCaretVertical(-round(GetPageSize));
end;

function TD2Memo.GetWidestLine:integer;
var
  i :integer;
  LWidth,LMaxWidth :single;
begin
  Result:=-1;
  LMaxWidth:=-1;
  for i:=0 to Lines.Count-1 do
  begin
    LWidth:=LineWidth[i];
    if LWidth > LMaxWidth then
    begin
      Result:=i;
      LMaxWidth:=LWidth;
    end;
  end;
end;

function TD2Memo.GetShowSelection:boolean;
begin
  Result:=IsFocused or not HideSelection;
end;

procedure TD2Memo.MouseWheel(Shift:TShiftState; WheelDelta:integer;
  var Handled:boolean);
begin
  inherited;
  if (VScrollBar <> nil) and (VScrollBar.Visible) then
  begin
    VScrollBar.Value:=VScrollBar.Value - (WheelDelta / 30);
  end;
end;

function TD2Memo.GetData:Variant;
begin
  Result:=TextW;
end;

procedure TD2Memo.SetData(const Value:Variant);
begin
  TextW:=Value;
end;

procedure TD2Memo.SetFont(const Value:TD2Font);
begin
  FFont.Assign(Value);
end;

procedure TD2Memo.SetTextAlign(const Value:TD2TextAlign);
begin
  FTextAlign:=Value;
end;

procedure TD2Memo.SetUpdateState(Updating:boolean);
begin
  FUpdating:=Updating;
  if not Updating then
    Realign;
end;

function TD2Memo.GetUnwrapLines:TD2WideStrings;
begin
  if FUnwrapLines=nil then
    FUnwrapLines:=TD2WideStringList.Create;
  FUnwrapLines.Text:=FText;
  Result:=FUnwrapLines;
end;
                                   
