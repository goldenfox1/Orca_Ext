{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is extension part of Package pl_ORCA.pkg make by GoldenFox
 for CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//============== TD2DBTreeViewItem =============================

constructor TD2DBTreeViewItem.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
end;

//============== TD2DBTreeView =============================

function TD2DBTreeView.GetColumnCount: integer;
var
  i: integer;
begin
  Result := 0;
  if (FContent <> nil) and (FContent.ChildrenCount > 0) then
    for i := 0 to FContent.ChildrenCount - 1 do
      if FContent.Children[i] is TD2Column then
      begin
        Inc(Result);
      end;
end;

function TD2DBTreeView.GetColumn(Index: integer): TD2Column;
begin
  Result := ColumnByIndex(Index);
end;

procedure TD2DBTreeView.SetFocusedColumn(const Value: integer);
begin
  if FFocusedColumn <> Value then
  begin
    FFocusedColumn := Value;
    UpdateSelection;
    if Columns[FFocusedColumn].Position.X < HScrollBar.Value then
      HScrollBar.Value := Columns[FFocusedColumn].Position.X;
    if Columns[FFocusedColumn].Position.X + Columns[FFocusedColumn].Width > HScrollBar.Value + ClientWidth then
      HScrollBar.Value := Columns[FFocusedColumn].Position.X + Columns[FFocusedColumn].Width - ClientWidth;
  end;
end;

procedure TD2DBTreeView.SetShowHeader(const Value: boolean);
begin
  if FShowHeader <> Value then
  begin
    FShowHeader := Value;
    if FHeader <> nil then
      FHeader.Visible := FShowHeader;
  end;
end;

procedure TD2DBTreeView.ApplyStyle;
var
  T: TD2Object;
begin
  inherited;
  T := FindResource('header');
  if (T <> nil) and (T is TD2Header) then
  begin
    FHeader := TD2Header(T);
    FHeader.OnRealignItem := DoRealignItem;
    FHeader.OnResizeItem := DoResizeItem;
    FHeader.Visible := FShowHeader;
  end;
  T := FindResource('focus');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    FFocus := TD2VisualObject(T);
    FFocus.Visible := false;
  end;
  UpdateColumns;
  UpdateHeader;

  UpdateSelection;
end;

procedure TD2DBTreeView.FreeStyle;
begin
  inherited;
  FFocus := nil;
  FHeader := nil;
end;

procedure TD2DBTreeView.DoRealignItem(Sender: TObject; OldIndex,
  NewIndex: integer);
begin
  if FocusedColumn = Columns[OldIndex].Index then
    FocusedColumn := NewIndex;
  Columns[OldIndex].Index := NewIndex;
end;

procedure TD2DBTreeView.DoResizeItem(Sender: TObject; var NewSize: single);
begin
  if NewSize < 10 then NewSize := 10;
  Columns[TD2HeaderItem(Sender).Index].Width := NewSize;
end;

procedure TD2DBTreeView.UpdateColumns;
var
  i: integer;
begin
  for i := 0 to ColumnCount - 1 do
  begin
    //Columns[i].Position.Y := (TopRow * FRowHeight);
    Columns[i].UpdateColumn;
  end;
end;

procedure TD2DBTreeView.UpdateHeader;
var
  i: integer;
  Item: TD2HeaderItem;
begin
  if FHeader = nil then Exit;

  FHeader.FOffset := -HScrollBar.Value;
  FHeader.FChildren.Remove(FHeader.FLastItem);
  if FHeader.ChildrenCount < ColumnCount then
    for i := FHeader.ChildrenCount to ColumnCount - 1 do
    begin
      Item := TD2HeaderItem.Create(Self);
      Item.Parent := FHeader;
      Item.Locked := true;
      Item.Stored := false;
    end;
  FHeader.FChildren.Add(FHeader.FLastItem);

  for i := 0 to ColumnCount - 1 do
  begin
    TD2HeaderItem(FHeader.Children[i]).TextW := Columns[i].Header;
    TD2HeaderItem(FHeader.Children[i]).Width := Columns[i].Width;
  end;
  if FHeader <> nil then
    FHeader.Realign;
end;

constructor TD2DBTreeView.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FShowHeader := true;
end;

destructor TD2DBTreeView.Destroy;
begin
  inherited Destroy;
end;

function TD2DBTreeView.ItemClass: string;
begin
  //Result:='';
  Result := 'TD2DBColumn;TD2DBCheckColumn;TD2DBPopupColumn;TD2DBImageColumn;TD2DBProgressColumn';
end;

procedure TD2DBTreeView.AddObject(AObject: TD2Object);
begin
    if (FContent <> nil) and ((AObject is TD2Column) or (AObject is TD2SelectionItem)) then
  begin
    FContent.AddObject(AObject);
    if FUpdating = 0 then
      Realign;
  end
  else
    inherited;
end;

function TD2DBTreeView.ColumnByIndex(const Idx: integer): TD2Column;
var
  c, i: integer;
begin
  c := 0;
  if (FContent <> nil) and (FContent.ChildrenCount > 0) then
    for i := 0 to FContent.ChildrenCount - 1 do
      if FContent.Children[i] is TD2Column then
      begin
        if c = Idx then
        begin
          Result := TD2Column(FContent.Children[i]);
          Exit;
        end;
        Inc(c);
      end;
  Result := nil;
end;

function TD2DBTreeView.ColumnByPoint(const X, Y: single): TD2Column;
var
  i: integer;
  P: TD2Point;
  //P1: TD2Point;
begin
  P := LocalToAbsolute(d2Point(X, Y));
  if (FContent <> nil) and (FContent.ChildrenCount > 0) then
    for i := 0 to FContent.ChildrenCount - 1 do
      if FContent.Children[i] is TD2Column then
      begin
        if not TD2Column(FContent.Children[i]).Visible then Continue;
        if not d2IntersectRect(TD2Column(FContent.Children[i]).UpdateRect, UpdateRect) then Continue;
        if TD2Column(FContent.Children[i]).pointInObject(P.X, P.Y) then
        begin
          Result := TD2Column(FContent.Children[i]);
          Exit;
        end
      end;
  Result := nil;
end;
