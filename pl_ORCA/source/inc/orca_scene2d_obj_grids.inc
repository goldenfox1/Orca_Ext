{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2HeaderItem ==================================================

constructor TD2HeaderItem.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=false;
  TextAlign:=d2TextAlignNear;
  DragMode:=d2DragAutomatic;
  FSplitter:=TD2VisualObject.Create(Self);
  FSplitter.Parent:=Self;
  FSplitter.Width:=3;
  FSplitter.Align:=vaRight;
  FSplitter.Locked:=true;
  FSplitter.Stored:=false;
  FSplitter.HitTest:=true;
  FSplitter.AutoCapture:=true;
  FSplitter.Cursor:=crHSplit;
  FSplitter.OnMouseMove:=DoSplitterMouseMove;
  FLeftSplitter:=TD2VisualObject.Create(Self);
  FLeftSplitter.Parent:=Self;
  FLeftSplitter.Width:=3;
  FLeftSplitter.Align:=vaLeft;
  FLeftSplitter.Locked:=true;
  FLeftSplitter.Stored:=false;
  FLeftSplitter.HitTest:=true;
  FLeftSplitter.AutoCapture:=true;
  FLeftSplitter.Cursor:=crHSplit;
  FLeftSplitter.OnMouseMove:=DoLeftSplitterMouseMove;
end;

procedure TD2HeaderItem.DoSplitterMouseMove(Sender:TObject;
  Shift:TShiftState; X,Y,Dx,Dy:single);
var
  NewSize:single;
begin
  if TD2HackVisualObject(FSplitter).FPressed then
  begin
    NewSize:=AbsoluteToLocal(FSplitter.LocalToAbsolute(d2Point(X,Y))).X;
    if NewSize < 0 then NewSize:=0;

    if (Parent <> nil) and (Parent is TD2Header) then
    begin
      if Assigned(TD2Header(Parent).OnResizeItem) then
        TD2Header(Parent).OnResizeItem(Self,NewSize);
    end;
    Width:=NewSize;
    if (Parent <> nil) and (Parent is TD2Header) then
      TD2Header(Parent).Realign;
  end;
end;

procedure TD2HeaderItem.DoLeftSplitterMouseMove(Sender:TObject;
  Shift:TShiftState; X,Y,Dx,Dy:single);
var
  P:TD2Point;
  LeftItem:TD2HeaderItem;
begin
  if (Index > 0) and TD2HackVisualObject(FLeftSplitter).FPressed then
  begin
    if (Parent <> nil) and (Parent is TD2Header) then
      LeftItem:=TD2HeaderItem(TD2Header(Parent).Children[Index - 1]);
    if LeftItem=nil then Exit;

    P:=FLeftSplitter.LocalToAbsolute(d2Point(X,Y));

    P:=LeftItem.FSplitter.AbsoluteToLocal(P);
    TD2HackVisualObject(LeftItem.FSplitter).FPressed:=true;
    LeftItem.DoSplitterMouseMove(Sender,Shift,P.X,P.Y,0,0);
    TD2HackVisualObject(LeftItem.FSplitter).FPressed:=false;
  end;
end;

procedure TD2HeaderItem.DragDrop(const Data:TD2DragObject;
  const Point:TD2Point);
var
  NewIndex,OldIndex:integer;
begin
  inherited;
  NewIndex:=Index;
  OldIndex:=TD2Object(Data.Source).Index;
  TD2Object(Data.Source).Index:=Index;
  if (Header <> nil) and Assigned(Header.OnRealignItem) then
    Header.OnRealignItem(TD2Object(Data.Source),OldIndex,NewIndex);
end;

procedure TD2HeaderItem.DragEnd;
begin
  inherited;
end;

procedure TD2HeaderItem.DragOver(const Data:TD2DragObject;
  const Point:TD2Point; var Accept:boolean);
begin
  Accept:=(Data.Source is TD2HeaderItem) and (TD2HeaderItem(Data.Source).Header=Header) and (DragMode <> d2DragManual);
end;

function TD2HeaderItem.Header:TD2Header;
begin
  if (Parent <> nil) and (Parent is TD2Header) then
    Result:=TD2Header(Parent)
  else
    Result:=nil;
end;

//=============== TD2Header ===================================================

constructor TD2Header.Create(AOwner:TComponent);
begin
  inherited;
  ClipChildren:=true;
  FSides:=AllSides;
  FLastItem:=TD2HeaderItem.Create(Self);
  FLastItem.Parent:=Self;
  FLastItem.Stored:=false;
  FLastItem.Locked:=true;
  FLastItem.Width:=50;
  FLastItem.DragMode:=d2DragManual;
end;

function TD2Header.GetItem(Index:integer):TD2HeaderItem;
begin
  Result:=TD2HeaderItem(Children[Index]);
end;

function TD2Header.ItemClass:string;
begin
  Result:='TD2HeaderItem';
end;

procedure TD2Header.Paint;
var
  R:TD2Rect;
begin
  inherited Paint;
  if Assigned(Scene) and Scene.GetDesignTime and not Locked and not FInPaintTo then
  begin
    R:=LocalRect;
    d2InflateRect(R,-0.5,-0.5);
    Canvas.StrokeThickness:=1;
    Canvas.StrokeDash:=d2DashDash;
    Canvas.Stroke.Style:=d2BrushSolid;
    Canvas.Stroke.SolidColor:=$A0909090;
    Canvas.DrawRect(R,0,0,AllCorners,AbsoluteOpacity);
    Canvas.StrokeDash:=d2DashSolid;
  end;
end;

procedure TD2Header.Realign;
var
  i:integer;
  R:TD2Rect;

  procedure DoPosition(Control:TD2VisualObject);
  begin
    if not Control.Visible then Exit;
    with R do
    begin
      Control.SetBounds(Left,Top,Control.Width,Height);
      if Control <> FLastItem then
        Left:=Left + Control.Width;
    end;
  end;

  procedure DoAlign;
  var
    I,J:integer;
    Control:TD2VisualObject;
  begin
    for I:=0 to ChildrenCount - 1 do
    begin
      if not (TD2VisualObject(Children[I]) is TD2HeaderItem) then Continue;
      DoPosition(TD2VisualObject(Children[I]));
      // sides
      TD2HeaderItem(Children[I]).Sides:=FSides;
      // corners
      TD2HeaderItem(Children[I]).xRadius:=Radius;
      TD2HeaderItem(Children[I]).yRadius:=Radius;
      if I=0 then
      begin
        TD2HeaderItem(Children[I]).Corners:=[d2CornerTopLeft];
        TD2HeaderItem(Children[I]).Sides:=TD2HeaderItem(Children[I]).Sides + (FSides * [d2SideLeft]) - [d2SideRight];
      end
      else
      if Children[I]=FLastItem then
      begin
        TD2HeaderItem(Children[I]).Corners:=[d2CornerTopRight];
        TD2HeaderItem(Children[I]).Sides:=TD2HeaderItem(Children[I]).Sides + [d2SideLeft];
      end
      else
      begin
        TD2HeaderItem(Children[I]).Corners:=[];
        TD2HeaderItem(Children[I]).Sides:=TD2HeaderItem(Children[I]).Sides + [d2SideLeft] - [d2SideRight];
      end;
    end;
  end;

begin
  inherited ;
  if csDestroying in ComponentState then Exit;
  if FDisableAlign then Exit;
  if ChildrenCount=0 then Exit;
  FDisableAlign:=true;
  try
    FChildren.Remove(FLastItem);
    FChildren.Add(FLastItem);

    R:=d2Rect(FOffset,0,FWidth,FHeight);
    R:=Margins.MarginRect(R);
    DoAlign;

    if R.Left > R.Right then
      FLastItem.Width:=0
    else
      FLastItem.Width:=R.Right - R.Left;
  finally
    FDisableAlign:=false;
  end;
end;

procedure TD2Header.SetRadius(const Value:single);
begin
  if FRadius <> Value then
  begin
    FRadius:=Value;
    Realign;
  end;
end;

procedure TD2Header.SetSides(const Value:TD2Sides);
begin
  if FSides <> Value then
  begin
    FSides:=Value;
    Realign;
  end;
end;

//=============== TD2Column =========================================

constructor TD2Column.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=true;
  Width:=100;
  HitTest:=false;
  CanFocused:=false;
end;

function TD2Column.CreateCellControl:TD2Control;
begin
  Result:=TD2TextCell.Create(Self);
  TD2TextCell(Result).OnChange:=DoTextChanged;
end;

procedure TD2Column.DoTextChanged(Sender:TObject);
begin
  if Grid=nil then Exit;
  if FUpdateColumn then Exit;
  if FDisableChange then Exit;
  with d2StringToPoint(TD2Object(Sender).TagString) do
  begin
    Grid.SetValue(trunc(x),trunc(y),TD2Control(Sender).Data);
    if Assigned(Grid.FOnEdititingDone) then
      Grid.FOnEdititingDone(Grid,trunc(x),trunc(y));
  end;
end;

procedure TD2Column.DoCanFocused(Sender:TObject; var ACanFocused:boolean);
begin
  if Grid=nil then Exit;
  ACanFocused:=Grid.CanEditModify;
  if ACanFocused and ReadOnly then
    ACanFocused:=false;
end;

procedure TD2Column.DoEnterFocus(Sender:TObject);
begin
  if Grid=nil then Exit;
  Grid.ColumnIndex:=Index;
  FSaveData:=TD2Object(Sender).Data;
end;

procedure TD2Column.DoKeyDown(Sender:TObject; var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  if (KeyChar <> #0) and (Grid <> nil) and not Grid.CanEditAcceptKey(KeyChar) then
    KeyChar:=#0;
  if (Key=VK_RETURN) and (Grid <> nil) then
    Grid.SetFocus;
  if (Key=VK_ESCAPE) and (Grid <> nil) then
  begin
    FDisableChange:=true;
    try
      TD2Object(Sender).Data:=FSaveData;
    finally
      FDisableChange:=false;
    end;
    Grid.Reset;
  end;
  if (Key=VK_UP) or (Key=VK_DOWN) and (Grid <> nil) then
  begin
    Grid.SetFocus;
    Grid.KeyDown(Key,Keychar,Shift);
  end;
end;

function TD2Column.Grid:TD2CustomGrid;
var
  P:TD2Object;
begin
  P:=Parent;
  while (P <> nil) do
  begin
    if P is TD2CustomGrid then
    begin
      Result:=TD2CustomGrid(P);
      Exit;
    end;
    P:=P.Parent;
  end;
  Result:=nil;
end;

procedure TD2Column.ClearColumn;
var
  i:integer;
begin
  for i:=0 to High(FCellControls) do
    FCellControls[i].Free;
  SetLength(FCellControls,0);
end;

function TD2Column.CellControlByPoint(X,Y:single):TD2Control;
var
  i:integer;
  P:TD2Point;
begin
  Result:=nil;
  if Grid=nil then Exit;

  P:=Grid.LocalToAbsolute(d2Point(X,Y));
  for i:=0 to High(FCellControls) do
    if FCellControls[i].Visible and (FCellControls[i].pointInObject(P.X,P.Y)) then
    begin
      Result:=FCellControls[i];
      Exit;
    end;
end;

function TD2Column.CellControlByRow(Row:integer):TD2Control;
var
  i:integer;
begin
  Result:=nil;
  if Grid=nil then Exit;

  for i:=0 to High(FCellControls) do
    if (FCellControls[i].Visible) and (trunc(d2StringToPoint(FCellControls[i].TagString).Y)=Row) then
    begin
      Result:=FCellControls[i];
      Exit;
    end;
end;

procedure TD2Column.UpdateColumn;
var
  i,C:integer;
  V:Variant;
begin
  if Grid=nil then Exit;
  FUpdateColumn:=true;
  try
    { Create controls }
    if Length(FCellControls) < Min(Grid.RowCount,Grid.VisibleRows) then
    begin
      C:=High(FCellControls);
      SetLength(FCellControls,Min(Grid.RowCount,Grid.VisibleRows));
      for i:=C + 1 to Min(Grid.RowCount,Grid.VisibleRows) - 1 do
      begin
        FCellControls[i]:=CreateCellControl;
        FCellControls[i].Parent:=Self;
        FCellControls[i].HitTest:=false;
        FCellControls[i].Visible:=false;
        FCellControls[i].Locked:=true;
        FCellControls[i].Stored:=false;
        FCellControls[i].OnCanFocused:=DoCanFocused;
        FCellControls[i].OnEnterFocus:=DoEnterFocus;
        FCellControls[i].OnKeyDown:=DoKeyDown;
      end;
    end;
    { Hide if need }
    if Length(FCellControls) > Min(Grid.RowCount,Grid.VisibleRows) then
    begin
      for i:=Min(Grid.RowCount,Grid.VisibleRows) to High(FCellControls) do
        FCellControls[i].Visible:=false;
    end;
    { Update Data }
    for i:=0 to Min(Grid.RowCount,Grid.VisibleRows) - 1 do
    begin
      if Grid.TopRow + i >= Grid.RowCount then Break;

      V:=Grid.GetValue(Index,Grid.TopRow + i);

      FCellControls[i].Visible:=true;
      FCellControls[i].SetBounds(0,i * Grid.RowHeight,Width,Grid.RowHeight);
      FCellControls[i].TagString:=d2PointToString(d2Point(Index,Grid.TopRow + i));
      FCellControls[i].Data:=V;
    end;
    UpdateSelected;
  finally
    FUpdateColumn:=false;
  end;
end;

procedure TD2Column.UpdateSelected;
var
  i:integer;
begin
  if Grid=nil then Exit;
  if Grid.ReadOnly then Exit;
  if ReadOnly then Exit;
  for i:=0 to Min(Grid.RowCount,Grid.VisibleRows) - 1 do
  begin
    if Grid.IsSelected(Grid.TopRow + i) and (Grid.ColumnIndex=Index) then
    begin
      FCellControls[i].CanFocused:=true;
      FCellControls[i].HitTest:=true
    end
    else
    begin
      FCellControls[i].CanFocused:=false;
      FCellControls[i].HitTest:=false;
      if FCellControls[i].IsFocused then
        Grid.SetFocus;
    end;
  end;
end;

procedure TD2Column.SetHeader(const Value:WideString);
begin
  if FHeader <> Value then
  begin
    FHeader:=Value;
    if (Grid <> nil) and (Grid.FHeader <> nil) then
      Grid.FHeader.Items[Index].TextW:=FHeader;
  end;
end;

//=============== TD2CheckColumn ================================================

constructor TD2CheckColumn.Create(AOwner:TComponent);
begin
  inherited;
end;

function TD2CheckColumn.CreateCellControl:TD2Control;
begin
  Result:=TD2CheckCell.Create(Self);
  TD2CheckCell(Result).OnChange:=DoCheckChanged;
end;

procedure TD2CheckColumn.DoCheckChanged(Sender:TObject);
begin
  if Grid=nil then Exit;
  if FUpdateColumn then Exit;
  with d2StringToPoint(TD2Object(Sender).TagString) do
  begin
    Grid.SetValue(trunc(x),trunc(y),TD2Control(Sender).Data);
    if Assigned(Grid.FOnEdititingDone) then
      Grid.FOnEdititingDone(Grid,trunc(x),trunc(y));
  end;
end;

//=============== TD2ProgressColumn ============================================

constructor TD2ProgressColumn.Create(AOwner:TComponent);
begin
  inherited;
  FMax:=100;
end;

function TD2ProgressColumn.CreateCellControl:TD2Control;
begin
  Result:=TD2ProgressCell.Create(Self);
  TD2ProgressCell(Result).Min:=FMin;
  TD2ProgressCell(Result).Max:=FMax;
end;

//=============== TD2PopupColumn ==============================================

constructor TD2PopupColumn.Create(AOwner:TComponent);
begin
  inherited;
  FItems:=TD2WideStringList.Create;
end;

destructor TD2PopupColumn.Destroy;
begin
  FItems.Free;
  inherited;
end;

function TD2PopupColumn.CreateCellControl:TD2Control;
begin
  Result:=TD2PopupCell.Create(Self);
  TD2PopupCell(Result).Items.Assign(FItems);
end;

procedure TD2PopupColumn.SetItems(const Value:TD2WideStrings);
begin
  FItems.Assign(Value);
end;

//=============== TD2ImageColumn =============================================

constructor TD2ImageColumn.Create(AOwner:TComponent);
begin
  inherited;
end;

function TD2ImageColumn.CreateCellControl:TD2Control;
begin
  Result:=TD2ImageCell.Create(Self);
  TD2ImageCell(Result).EnableOpenDialog:=false;
end;
         
//=============== TD2CustomGrid ==============================================

constructor TD2CustomGrid.Create(AOwner:TComponent);
begin
  inherited;
  FOddFill:=TD2Brush.Create(d2BrushSolid,$20000000);
  FLineFill:=TD2Brush.Create(d2BrushSolid,$FF202020);
  FShowSelectedCell:=true;
  FShowVertLines:=true;
  FShowHorzLines:=true;
  FShowHeader:=true;
  FRowHeight:=21;
  FRowCount:=100;
  CanFocused:=true;
  AutoCapture:=true;
  Width:=100;
  Height:=100;
  FItemHeight:=0;
end;

destructor TD2CustomGrid.Destroy;
begin
  if FSelections <> nil then
    FSelections.Free;
  FLineFill.Free;
  FOddFill.Free;
  inherited;
end;

function TD2CustomGrid.ItemClass:string;
begin
  Result:='TD2Column;TD2CheckColumn;TD2ProgressColumn;TD2PopupColumn;TD2ImageColumn';
end;

procedure TD2CustomGrid.FreeStyle;
begin
  inherited;
  FSelection:=nil;
  FFocus:=nil;
  FHeader:=nil;
end;

procedure TD2CustomGrid.ApplyStyle;
var
  T:TD2Object;
begin
  inherited;
  T:=FindResource('content');
  if (T <> nil) and (T.IsVisual) then
  begin
    TD2VisualObject(T).OnBeforePaint:=DoContentPaint;
    TD2VisualObject(T).OnPaint:=DoContentPaint2;
  end;
  T:=FindResource('header');
  if (T <> nil) and (T is TD2Header) then
  begin
    FHeader:=TD2Header(T);
    FHeader.OnRealignItem:=DoRealignItem;
    FHeader.OnResizeItem:=DoResizeItem;
    FHeader.Visible:=FShowHeader;
  end;
  T:=FindResource('selection');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    FSelection:=TD2VisualObject(T);
    FSelection.Visible:=false;
  end;
  T:=FindResource('focus');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    FFocus:=TD2VisualObject(T);
    FFocus.Visible:=false;
  end;
  T:=FindResource('AlternatingRowBackground');
  if (T <> nil) and (T is TD2BrushObject) then
  begin
    FOddFill.Assign(TD2BrushObject(T).Brush);
  end;
  T:=FindResource('LineFill');
  if (T <> nil) and (T is TD2BrushObject) then
  begin
    FLineFill.Assign(TD2BrushObject(T).Brush);
  end;
  UpdateColumns;
  UpdateHeader;
  UpdateSelection;
end;

procedure TD2CustomGrid.UpdateSelection;
var
  i:integer;
  P:TD2Point;
  R:TD2Rect;
  Sel:boolean;
  SelRects:array of TD2Rect;
  Clone:TD2VisualObject;
  Vis:boolean;
begin
  if FSelection=nil then Exit;
  if ColumnCount=0 then Exit;
  if RowCount=0 then
  begin
    if FSelections <> nil then
      for i:=0 to FSelections.Count - 1 do
        TD2VisualObject(FSelections[i]).Visible:=false;
    if FFocus <> nil then
      FFocus.Visible:=false;
    Exit;
  end;
  // calc rects
  Vis:=true;
  Sel:=false;
  SetLength(SelRects,0);
  for i:=0 to RowCount - 1 do
  begin
    if IsSelected(i) then
    begin
      P:=d2Point(0,i * FRowHeight);
      P:=FContent.LocalToAbsolute(P);
      if (FSelection.Parent <> nil) and FSelection.Parent.IsVisual then
        P:=FSelection.Parent.Visual.AbsoluteToLocal(P);
      if FContent.Width < ClientWidth then
        R:=d2Rect(P.X,P.Y,P.X + ClientWidth,P.Y + FRowHeight)
      else
        R:=d2Rect(P.X,P.Y,P.X + FContent.Width,P.Y + FRowHeight);
      if (Length(SelRects) > 0) and (i > 0) and (IsSelected(i - 1)) then
        SelRects[High(SelRects)]:=d2UnionRect(R,SelRects[High(SelRects)])
      else
      begin
        SetLength(SelRects,Length(SelRects) + 1);
        SelRects[High(SelRects)]:=R;
      end;
      Sel:=true;
    end;
  end;
  // Create selection list
  if FSelections=nil then
    FSelections:=TList.Create;
  // create selections
  if FSelections.Count < Length(SelRects) then
    for i:=FSelections.Count to Length(SelRects) - 1 do
    begin
      Clone:=TD2VisualObject(FSelection.Clone(Self));
      Clone.ResourceName:='';
      FSelections.Add(Clone);
      Clone.Parent:=FSelection.Parent;
      Clone.Stored:=false;
    end;
  // hide if not need
  if Length(SelRects) < FSelections.Count then
    for i:=Length(SelRects) to FSelections.Count - 1 do
    begin
      TD2VisualObject(FSelections[i]).Visible:=false;
    end;
  // align selections
  for i:=0 to High(SelRects) do
  begin
    TD2VisualObject(FSelections[i]).Visible:=Vis;
    if Vis then
    begin
      with SelRects[i] do
        TD2VisualObject(FSelections[i]).SetBounds(Left,Top,Right - Left,Bottom - Top);
      if (FFocus <> nil) and (FShowSelectedCell) then
      begin
        FFocus.Visible:=true;
        FFocus.BringToFront;
        P:=d2Point(Columns[ColumnIndex].Position.X,0);
        P:=FContent.LocalToAbsolute(P);
        if (FSelection.Parent <> nil) and FSelection.Parent.IsVisual then
          P:=FSelection.Parent.Visual.AbsoluteToLocal(P);
        with SelRects[i] do
          FFocus.SetBounds(P.X + FFocus.Padding.Left,SelRects[i].Top + FFocus.Padding.Top,
          Columns[FColumnIndex].Width - (FFocus.Padding.Left + FFocus.Padding.Right),
            SelRects[i].Bottom - SelRects[i].Top - (FFocus.Padding.Top + FFocus.Padding.Bottom));
      end;
    end;
  end;
  //
  for i:=0 to ColumnCount - 1 do
    Columns[i].UpdateSelected;
end;

procedure TD2CustomGrid.DoRealignItem(Sender:TObject; OldIndex,NewIndex:integer);
begin
  if ColumnIndex=Columns[OldIndex].Index then
    ColumnIndex:=NewIndex;
  Columns[OldIndex].Index:=NewIndex;
end;

procedure TD2CustomGrid.DoResizeItem(Sender:TObject;
  var NewSize:single);
begin
  if NewSize < 10 then NewSize:=10;
  Columns[TD2HeaderItem(Sender).Index].Width:=NewSize;
end;

procedure TD2CustomGrid.UpdateHeader;
var
  i:integer;
  Item:TD2HeaderItem;
begin
  if FHeader=nil then Exit;

  FHeader.FOffset:=-HScrollBar.Value;
  FHeader.FChildren.Remove(FHeader.FLastItem);
  if FHeader.ChildrenCount < ColumnCount then
    for i:=FHeader.ChildrenCount to ColumnCount - 1 do
    begin
      Item:=TD2HeaderItem.Create(Self);
      Item.Parent:=FHeader;
      Item.Locked:=true;
      Item.Stored:=false;
    end;
  FHeader.FChildren.Add(FHeader.FLastItem);

  for i:=0 to ColumnCount - 1 do
  begin
    TD2HeaderItem(FHeader.Children[i]).TextW:=Columns[i].Header;
    TD2HeaderItem(FHeader.Children[i]).Width:=Columns[i].Width;
  end;
  if FHeader <> nil then
    FHeader.Realign;
end;

procedure TD2CustomGrid.UpdateColumns;
var
  i:integer;
begin
  for i:=0 to ColumnCount - 1 do
  begin
    Columns[i].Position.Y:=(TopRow * FRowHeight);
    Columns[i].UpdateColumn;
  end;
end;

procedure TD2CustomGrid.DoContentPaint(Sender:TObject;
  const Canvas:TD2Canvas; const ARect:TD2Rect);
var
  i:integer;
  P:TD2Point;
begin
  if (FContent <> nil) and (ContentLayout <> nil) then
  begin
    if FAlternatingRowBackground then
    begin
      Canvas.Fill.Assign(FOddFill);
      for i:=0 to Min(RowCount,VisibleRows) - 1 do
      begin
        if Odd(TopRow + i) then
        begin
          P:=FContent.LocalToAbsolute(d2Point(0,(TopRow + i) * FRowHeight));
          P:=TD2VisualObject(Sender).AbsoluteToLocal(P);
          Canvas.FillRect(d2Rect(P.X,P.Y,P.X + FContent.Width,P.Y + FRowHeight),0,0,[],AbsoluteOpacity);
        end;
      end;
    end;
  end;
end;

procedure TD2CustomGrid.DoContentPaint2(Sender:TObject;
  const Canvas:TD2Canvas; const ARect:TD2Rect);
var
  i:integer;
  P:TD2Point;
  State:integer;
begin
  if (FContent <> nil) and (ContentLayout <> nil) and (FShowVertLines or FShowHorzLines) then
  begin
    State:=Canvas.SaveCanvas;
    Canvas.IntersectClipRect(ARect);

    Canvas.Stroke.Assign(FLineFill);
    if FShowHorzLines then
      for i:=0 to Min(RowCount,VisibleRows) do
      begin
        P:=FContent.LocalToAbsolute(d2Point(0,(TopRow + i) * FRowHeight));
        P:=TD2VisualObject(Sender).AbsoluteToLocal(P);
        Canvas.DrawLine(d2Point(P.X,P.Y - 0.5),d2Point(P.X + FContent.Width,P.Y - 0.5),AbsoluteOpacity);
      end;
    if FShowVertLines then
      for i:=0 to ColumnCount - 1 do
      begin
        P:=Columns[i].LocalToAbsolute(d2Point(Columns[i].Width,0));
        P:=TD2VisualObject(Sender).AbsoluteToLocal(P);
        Canvas.DrawLine(d2Point(P.X + 0.5,P.Y),d2Point(P.X + 0.5,P.Y + (Min(RowCount,VisibleRows) * FRowHeight)),AbsoluteOpacity);
      end;

    Canvas.RestoreCanvas(State);
  end;
end;

function TD2CustomGrid.GetContentBounds:TD2Rect;
var
  Col,R:TD2Rect;
  i,Idx:integer;
begin
  Result:=LocalRect;
  if FUpdating > 0 then Exit;
  if ContentLayout=nil then Exit;
  R:=ContentLayout.LocalRect;
  if ColumnCount > 0 then
  begin
    R.Right:=R.Left;
    R.Top:=R.Top;
    for i:=0 to ColumnCount - 1 do
    begin
      Col:=d2Rect(R.Right,R.Top,R.Right + Columns[i].Width,R.Bottom);
      R.Right:=R.Right + Columns[i].Width;
      Columns[i].SetBounds(Col.Left,(TopRow * FRowHeight),Columns[i].Width,ClientHeight);
      Columns[i].UpdateColumn;
    end;
    R.Bottom:=R.Top + (FRowCount * FRowHeight);
  end;
  if d2RectWidth(R) < ContentLayout.Width then
    R.Right:=R.Left + ContentLayout.Width;
  if d2RectHeight(R) < ContentLayout.Height then
    R.Bottom:=R.Top + ContentLayout.Height;
  Result:=R;
  UpdateColumns;
  UpdateHeader;
  UpdateSelection;
end;

procedure TD2CustomGrid.HScrollChange(Sender:TObject);
begin
  inherited;
  UpdateSelection;
  UpdateHeader;
end;

procedure TD2CustomGrid.VScrollChange(Sender:TObject);
begin
  inherited;
  UpdateColumns;
  UpdateSelection;
end;

function TD2CustomGrid.GetColumnCount:integer;
var
  i:integer;
begin
  Result:=0;
  if (FContent <> nil) and (FContent.ChildrenCount > 0) then
    for i:=0 to FContent.ChildrenCount - 1 do
      if FContent.Children[i] is TD2Column then
      begin
        Inc(Result);
      end;
end;

function TD2CustomGrid.RowByPoint(const X,Y:single):integer;
var
  i:integer;
  P,P1:TD2Point;
begin
  P:=LocalToAbsolute(d2Point(X,Y));
  if FContent <> nil then
  begin
    P:=FContent.AbsoluteToLocal(P);
    Result:=trunc(P.Y / FRowHeight);
    Exit;
  end;
  Result:=-1;
end;

function TD2CustomGrid.ColumnByIndex(const Idx:integer):TD2Column;
var
  c,i:integer;
begin
  c:=0;
  if (FContent <> nil) and (FContent.ChildrenCount > 0) then
    for i:=0 to FContent.ChildrenCount - 1 do
      if FContent.Children[i] is TD2Column then
      begin
        if c=Idx then
        begin
          Result:=TD2Column(FContent.Children[i]);
          Exit;
        end;
        Inc(c);
      end;
  Result:=nil;
end;

function TD2CustomGrid.ColumnByPoint(const X,Y:single):TD2Column;
var
  i:integer;
  P,P1:TD2Point;
begin
  P:=LocalToAbsolute(d2Point(X,Y));
  if (FContent <> nil) and (FContent.ChildrenCount > 0) then
    for i:=0 to FContent.ChildrenCount - 1 do
      if FContent.Children[i] is TD2Column then
      begin
        if not TD2Column(FContent.Children[i]).Visible then Continue;
        if not d2IntersectRect(TD2Column(FContent.Children[i]).UpdateRect,UpdateRect) then Continue;
        if TD2Column(FContent.Children[i]).pointInObject(P.X,P.Y) then
        begin
          Result:=TD2Column(FContent.Children[i]);
          Exit;
        end
      end;
  Result:=nil;
end;

function TD2CustomGrid.CanEditModify:boolean;
begin
  Result:=not ReadOnly;
end;

function TD2CustomGrid.CanEditAcceptKey(Key:System.WideChar):boolean;
begin
  Result:=true;
end;

procedure TD2CustomGrid.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  inherited ;
  if RowCount > 0 then
  begin
    if (KeyChar <> #0) and not (ReadOnly) and not (Columns[ColumnIndex].ReadOnly) then
    begin
      if (Columns[ColumnIndex] <> nil) and (Columns[ColumnIndex].CellControlByRow(Selected) <> nil) then
      begin
        Columns[ColumnIndex].CellControlByRow(Selected).SetFocus;
        TD2HackVisualObject(Columns[ColumnIndex].CellControlByRow(Selected)).KeyDown(Key,KeyChar,Shift);
      end;
    end;
    case Key of
      VK_F2:
        begin
          if (Columns[ColumnIndex] <> nil) and (Columns[ColumnIndex].CellControlByRow(Selected) <> nil) then
            Columns[ColumnIndex].CellControlByRow(Selected).SetFocus;
        end;
      VK_UP:if FSelected > 0 then Selected:=Selected - 1;
      VK_DOWN:if FSelected < FRowCount - 1 then Selected:=Selected + 1;
      VK_HOME:
        if ssCtrl in Shift then
          Selected:=0
        else
          ColumnIndex:=0;
      VK_END:
        if ssCtrl in Shift then
          Selected:=RowCount - 1
        else
          ColumnIndex:=ColumnCount - 1;
      VK_PRIOR:if FSelected > 0 then Selected:=Selected - Min(RowCount,VisibleRows);
      VK_NEXT:if FSelected < FRowCount - 1 then Selected:=Selected + Min(RowCount,VisibleRows);
      VK_LEFT:if ColumnIndex > 0 then ColumnIndex:=ColumnIndex - 1;
      VK_RIGHT:if ColumnIndex < ColumnCount - 1 then ColumnIndex:=ColumnIndex + 1;
    else
      Exit;
    end;
    Key:=0;
  end;
end;

procedure TD2CustomGrid.MouseDown(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
var
  Column:TD2Column;
begin
  inherited;
  if Button=mbLeft then
  begin
    Selected:=RowByPoint(X,Y);
    Column:=ColumnByPoint(X,Y);
    if Column <> nil then
      ColumnIndex:=Column.Index;
  end;
end;

procedure TD2CustomGrid.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
var
  Column:TD2Column;
begin
  inherited;
  if FPressed then
  begin
    Selected:=RowByPoint(X,Y);
    Column:=ColumnByPoint(X,Y);
    if Column <> nil then
      ColumnIndex:=Column.Index;
  end;
end;

procedure TD2CustomGrid.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
end;

procedure TD2CustomGrid.AddObject(AObject:TD2Object);
begin
  if (FContent <> nil) and ((AObject is TD2Column) or (AObject is TD2SelectionItem)) then
  begin
    FContent.AddObject(AObject);
    if FUpdating=0 then
      Realign;
  end
  else
    inherited;
end;

procedure TD2CustomGrid.RemoveObject(AObject:TD2Object);
begin
  inherited;
end;

procedure TD2CustomGrid.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
end;

function TD2CustomGrid.GetColumn(Index:integer):TD2Column;
begin
  Result:=ColumnByIndex(Index);
end;

procedure TD2CustomGrid.SetRowCount(const Value:integer);
begin
  if FRowCount <> Value then
  begin
    FRowCount:=Value;
    Realign;
    if (FSelected >= FRowCount) and (FRowCount > 0) then
      Selected:=FRowCount - 1;
  end;
end;

procedure TD2CustomGrid.SetRowHeight(const Value:single);
begin
  if FRowHeight <> Value then
  begin
    FRowHeight:=Value;
    if FRowHeight < 5 then FRowHeight:=5;
    Realign;
  end;
end;

function TD2CustomGrid.GetTopRow:integer;
begin
  if VScrollBar <> nil then
    Result:=trunc(VScrollBar.Value / FRowHeight)
  else
    Result:=0;
end;

function TD2CustomGrid.GetVisibleRows:integer;
begin
  Result:=trunc(ClientHeight / FRowHeight) + 2;
end;

function TD2CustomGrid.GetValue(Col,Row:integer):Variant;
begin
  Result:=NULL;
  if Assigned(FOnGetValue) then
    FOnGetValue(Self,Col,Row,Result);
end;

procedure TD2CustomGrid.SetValue(Col,Row:integer; const Value:Variant);
begin
  if Assigned(FOnSetValue) then
    FOnSetValue(Self,Col,Row,Value);
end;

procedure TD2CustomGrid.SetSelected(const Value:integer);
begin
  if FSelected <> Value then
  begin
    FSelected:=Value;
    if FSelected < 0 then FSelected:=0;
    if FSelected > FRowCount - 1 then FSelected:=FRowCount - 1;

    if FSelected <= TopRow then
      VScrollBar.Value:=FSelected * FRowHeight;
    if FSelected * FRowHeight > VScrollBar.Value + ClientHeight - FRowHeight then
      VScrollBar.Value:=FSelected * FRowHeight - ClientHeight + FRowHeight;
    UpdateSelection;
  end;
end;

function TD2CustomGrid.IsSelected(Row:integer):boolean;
begin
  Result:=Row=FSelected;
end;

procedure TD2CustomGrid.SetAlternatingRowBackground(const Value:boolean);
begin
  if FAlternatingRowBackground <> Value then
  begin
    FAlternatingRowBackground:=Value;
    Repaint;
  end;
end;

procedure TD2CustomGrid.SetShowHorzLines(const Value:boolean);
begin
  if FShowHorzLines <> Value then
  begin
    FShowHorzLines:=Value;
    Repaint;
  end;
end;

procedure TD2CustomGrid.SetShowVertLines(const Value:boolean);
begin
  if FShowVertLines <> Value then
  begin
    FShowVertLines:=Value;
    Repaint;
  end;
end;

procedure TD2CustomGrid.SetColumnIndex(const Value:integer);
begin
  if FColumnIndex <> Value then
  begin
    FColumnIndex:=Value;
    UpdateSelection;
    if Columns[FColumnIndex].Position.X < HScrollBar.Value then
      HScrollBar.Value:=Columns[FColumnIndex].Position.X;
    if Columns[FColumnIndex].Position.X + Columns[FColumnIndex].Width > HScrollBar.Value + ClientWidth then
      HScrollBar.Value:=Columns[FColumnIndex].Position.X + Columns[FColumnIndex].Width - ClientWidth;
  end;
end;

procedure TD2CustomGrid.SetShowHeader(const Value:boolean);
begin
  if FShowHeader <> Value then
  begin
    FShowHeader:=Value;
    if FHeader <> nil then
      FHeader.Visible:=FShowHeader;
  end;
end;

procedure TD2CustomGrid.Reset;
begin
  SetFocus;
end;

//=============== TD2StringColumn =======================================================

constructor TD2StringColumn.Create(AOwner:TComponent);
begin
  inherited;
end;

destructor TD2StringColumn.Destroy;
begin
  inherited;
end;

procedure TD2StringColumn.UpdateColumn;
begin
  if Grid=nil then Exit;
  SetLength(FCells,Grid.RowCount);
  inherited;
end;

//=============== TD2StringGrid =========================================================

constructor TD2StringGrid.Create(AOwner:TComponent);
begin
  inherited;
  FResource:='gridstyle';
end;

destructor TD2StringGrid.Destroy;
begin
  inherited;
end;

function TD2StringGrid.GetValue(Col,Row:integer):Variant;
var
  C:TD2Column;
begin
  C:=Columns[Col];
  if C <> nil then
  begin
    if Length(TD2StringColumn(C).FCells) <> RowCount then
      SetLength(TD2StringColumn(C).FCells,RowCount);
    Result:=TD2StringColumn(C).FCells[Row]
  end
  else
    Result:=NULL;
end;

procedure TD2StringGrid.SetValue(Col,Row:integer; const Value:Variant);
var
  C:TD2Column;
begin
  C:=Columns[Col];
  if (C <> nil) then
  begin
    if Length(TD2StringColumn(C).FCells) <> RowCount then
      SetLength(TD2StringColumn(C).FCells,RowCount);
    TD2StringColumn(C).FCells[Row]:=VarToWideStr(Value);
  end;
end;

function TD2StringGrid.GetCells(ACol,ARow:integer):WideString;
begin
  Result:=GetValue(ACol,ARow);
end;

procedure TD2StringGrid.SetCells(ACol,ARow:integer; const Value:WideString);
begin
  SetValue(ACol,ARow,Value)
end;

function TD2StringGrid.ItemClass:string;
begin
  Result:='TD2StringColumn';
end;

procedure TD2CustomGrid.SetShowSelectedCell(const Value:boolean);
begin
  if FShowSelectedCell <> Value then
  begin
    FShowSelectedCell:=Value;
    UpdateSelection;
  end;
end;

//=============== TD2PlotGrid ==================================

constructor TD2PlotGrid.Create(AOwner:TComponent);
begin
  inherited;
  FLineFill:=TD2Brush.Create(d2BrushSolid,$FF505050);
  FLineFill.OnChanged:=LineFillChanged;
  FMarks:=25;
  FFrequency:=5;
end;

destructor TD2PlotGrid.Destroy;
begin
  FLineFill.Free;
  inherited;
end;

procedure TD2PlotGrid.LineFillChanged(Sender:TObject);
begin
  Repaint;
end;

procedure TD2PlotGrid.Paint;
var
  x,y:single;
begin
  x:=0;
  y:=0;
  Canvas.Stroke.Assign(FLineFill);
  while x < Width / 2 do
  begin
    if (x=0) then
    begin
      Canvas.StrokeThickness:=2;
      Canvas.Stroke.SolidColor:=FLineFill.SolidColor
    end
    else
    begin
      if (frac(x)=0) and (frac(x / frequency / marks)=0) then
        Canvas.Stroke.SolidColor:=FLineFill.SolidColor
      else
        Canvas.Stroke.SolidColor:=d2Opacity(FLineFill.SolidColor,0.4);
      Canvas.StrokeThickness:=1;
    end;

    Canvas.DrawLine(d2Point(round(Width / 2) + x + (Canvas.StrokeThickness / 2),0),d2Point(round(Width / 2) + x + (Canvas.StrokeThickness / 2),Height),AbsoluteOpacity);
    if x <> 0 then
      Canvas.DrawLine(d2Point(round(Width / 2) - x + (Canvas.StrokeThickness / 2),0),d2Point(round(Width / 2) - x + (Canvas.StrokeThickness / 2),Height),AbsoluteOpacity);
    x:=x + FFrequency;
  end;
  while y < Height / 2 do
  begin
    if (y=0) then
    begin
      Canvas.StrokeThickness:=2;
      Canvas.Stroke.SolidColor:=FLineFill.SolidColor
    end
    else
    begin
      if (frac(y)=0) and (frac(y / frequency / marks)=0) then
        Canvas.Stroke.SolidColor:=FLineFill.SolidColor
      else
        Canvas.Stroke.SolidColor:=d2Opacity(FLineFill.SolidColor,0.4);
      Canvas.StrokeThickness:=1;
    end;

    Canvas.DrawLine(d2Point(0,round(Height / 2) + y + (Canvas.StrokeThickness / 2)),d2Point(Width,round(Height / 2) + y + (Canvas.StrokeThickness / 2)),AbsoluteOpacity);
    if y <> 0 then
      Canvas.DrawLine(d2Point(0,round(Height / 2) - y + (Canvas.StrokeThickness / 2)),d2Point(Width,round(Height / 2) - y + (Canvas.StrokeThickness / 2)),AbsoluteOpacity);
    y:=y + FFrequency;
  end;
end;

procedure TD2PlotGrid.SetFrequency(const Value:single);
begin
  if FFrequency <> Value then
  begin
    FFrequency:=Value;
    if FFrequency < 0.001 then
      FFrequency:=0.001;
    Repaint;
  end;
end;

procedure TD2PlotGrid.SetLineFill(const Value:TD2Brush);
begin
  FLineFill.Assign(Value);
end;

procedure TD2PlotGrid.SetMarks(const Value:single);
begin
  if FMarks <> Value then
  begin
    FMarks:=Value;
    Repaint;
  end;
end;
        

            
  
