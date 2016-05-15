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
  FxRadius:=0; //Added by GoldenFox
  FyRadius:=0; //Added by GoldenFox
  //Stored := true; //Added by GoldenFox
  //TextAlign := d2TextAlignNear;  //Deleted by GoldenFox
  {  //Deleted by GoldenFox
  DragMode := d2DragAutomatic;
  FSplitter := TD2VisualObject.Create(Self);
  FSplitter.Parent := Self;
  FSplitter.Width := 3;
  FSplitter.Align := vaRight;
  FSplitter.Locked := true;
  FSplitter.Stored := false;
  FSplitter.HitTest := true;
  FSplitter.AutoCapture := true;
  FSplitter.Cursor := crHSplit;
  FSplitter.OnMouseMove := DoSplitterMouseMove;
  FLeftSplitter := TD2VisualObject.Create(Self);
  FLeftSplitter.Parent := Self;
  FLeftSplitter.Width := 3;
  FLeftSplitter.Align := vaLeft;
  FLeftSplitter.Locked := true;
  FLeftSplitter.Stored := false;
  FLeftSplitter.HitTest := true;
  FLeftSplitter.AutoCapture := true;
  FLeftSplitter.Cursor := crHSplit;
  FLeftSplitter.OnMouseMove := DoLeftSplitterMouseMove;
  }
end;

procedure TD2HeaderItem.ApplyStyle; //procedure added by GoldenFox
var S: TD2Object;
begin
  inherited;
  S := FindResource('splitter');
  if (S <> nil) and (S is TD2VisualObject) then
  begin
    FSplitter := TD2VisualObject(S);
    //FSplitter.Stored := false;
    FSplitter.AutoCapture := true; //продолжать изменение размера при выходе курсора за пределы элемента пока нажата кнопка мыши
    FSplitter.OnMouseMove := DoSplitterMouseMove; //обработчик прерывания изменения размера
  end;
end;

procedure TD2HeaderItem.FreeStyle; //procedure added by GoldenFox
begin
  FSplitter:=nil;
  inherited;
end;

procedure TD2HeaderItem.DoSplitterMouseMove(Sender:TObject;
  Shift:TShiftState; X,Y,Dx,Dy:single);
var
  NewSize:single;
  H: TD2Header;	//Added by GoldenFox
begin
  if TD2HackVisualObject(FSplitter).FPressed then
  begin
    NewSize:=AbsoluteToLocal(FSplitter.LocalToAbsolute(d2Point(X,Y))).X;
    if NewSize < 0 then NewSize:=0;

    {//Deleted by GoldenFox
    if (Parent <> nil) and (Parent is TD2Header) then
    begin
      if Assigned(TD2Header(Parent).OnResizeItem) then
        TD2Header(Parent).OnResizeItem(Self, NewSize);
    end;
    Width := NewSize;
    if (Parent <> nil) and (Parent is TD2Header) then
      TD2Header(Parent).Realign;
    }
    H:=Header;                                    //Added by GoldenFox
    if (H <> nil) and (Assigned(H.OnResizeItem))  //если есть заголовок и у него установлен обраотчик прерывания изменения размера Added by GoldenFox
      then H.OnResizeItem(Self, NewSize);           //то вызвать прерывание Added by GoldenFox
    Width := NewSize;                             //установить новый размер после обработки прерывания Added by GoldenFox
    if H <> nil then Header.Realign;              //перестроить заголовок если есть Added by GoldenFox
  end;
end;

procedure TD2HeaderItem.MouseDown(Button: TMouseButton; Shift: TShiftState; X,  Y: single); //procedure added by GoldenFox
var
   i: integer;
   H: TD2Header;
begin
  inherited;
  H:=Header;
  if FPressed and (H <> nil) then  //если поле нажато и поле прикреплено к заголовку
    begin
      FMouseDownPos:=d2Point(X,Y); //сохраним позицию нажатия ЛКМ
      FOldIndex:=Index;            //сохраним старый индекс
      FNewIndex:=Index;            //установим текущий индекс
      H.FItemsWidth:=0;            //обнулим общую ширину полей заголовка
      for i := 0 to H.ItemsCount-1 do            //для всех полей заголовка
        with H.Items[i] do
        begin
          FLastPosition:=FPosition.GetPoint;    //сохранить позицию поля
          H.FItemsWidth:=H.FItemsWidth + Width; //вычисляем общую ширину полей заголовка
          Sides:=Sides+[d2SideLeft];            //у всех полей включим левую границу
        end;
      BringToFront;              //текущее поле переместим вверх
    end;
end;

procedure TD2HeaderItem.MouseMove(Shift: TShiftState; X, Y, Dx, Dy: single); //procedure added by GoldenFox
var
  idx,st,t: integer;
  H: TD2Header;
begin
  inherited;
  if not FPressed then exit;
  H:=Header;
  if H = nil then exit;
  Position.X:=Position.X+(X-FMouseDownPos.X); //новая позиция поля

  idx:=FNewIndex;  //текущий индекс поля заголовка
  if Position.X<H.FOffset+H.Margins.Left+Padding.Left-1 then          //если левая граница поля выходит за границу заголовка
    Position.X:=H.FOffset+H.Margins.Left+Padding.Left-1;       //выровнять поле влево по заголовку
  if Position.X>H.FOffset+H.FItemsWidth+H.Margins.Left-H.Margins.Right+Padding.Left-Width-Padding.Right then     //если правая граница поля выходит за границу заголовка
    Position.X:=H.FOffset+H.FItemsWidth+H.Margins.Left-H.Margins.Right+Padding.Left-Width-Padding.Right; //то выровнять поле по заголовку
  with H do
  begin
    if ItemsCount<0 then exit;                   //если кол-во полей < 0 то двигать нечего выходим
    if X-FMouseDownPos.X>0 then st:=1 else st:=-1;  //st=1 вдигаемся вправо, st=-1 вдигаемся влево
    while (0 <= idx) and (idx <= ItemsCount-1) do //перебираем все поля заголовка
    begin
      t:=0;
      if (0 < idx) and (st=-1) then
        with H.Items[idx-1] do
          if Self.Position.X<FLastPosition.X+Width/2 then
            begin
              FLastPosition.X:=FLastPosition.X+Self.Width;
              AnimateFloat('Position.X',FLastPosition.X,0.2);
              t:=-1;
            end;
      if (idx < ItemsCount-1) and (st=1) then
        with H.Items[idx] do
          if Self.Position.X>FLastPosition.X+Width/2-Self.Width then
            begin
              FLastPosition.X:=FLastPosition.X-Self.Width;
              AnimateFloat('Position.X',FLastPosition.X,0.2);
              t:=1;
            end;
      idx:=idx+st;
      FNewIndex:=FNewIndex+t;
    end;
  end;
end;

procedure TD2HeaderItem.MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: single); //procedure added by GoldenFox
var
  //Old: integer;
  H: TD2Header;
begin
  inherited;
  Index:=FNewIndex;
  H:=Header;
  if H <> nil then
  begin
    //H.Realign;  //вызывается при присвоении индекса
    if Assigned(H.OnRealignItem)                                  //если у заголовка определено прерывание по изменению порядка полей
      then H.OnRealignItem(TD2Object(Self), FOldIndex, FNewIndex);  //то вызвать прерывание (эл-т, Старый индекс, новый индекс)
  end;
end;

{  //Deleted by GoldenFox
procedure TD2HeaderItem.DoLeftSplitterMouseMove(Sender: TObject;
  Shift: TShiftState; X, Y, Dx, Dy: single);
var
  P: TD2Point;
  LeftItem: TD2HeaderItem;
begin
  if (Index > 0) and TD2HackVisualObject(FLeftSplitter).FPressed then
  begin
    if (Parent <> nil) and (Parent is TD2Header) then
      LeftItem := TD2HeaderItem(TD2Header(Parent).Children[Index - 1]);
    if LeftItem = nil then Exit;

    P := FLeftSplitter.LocalToAbsolute(d2Point(X, Y));

    P := LeftItem.FSplitter.AbsoluteToLocal(P);
    TD2HackVisualObject(LeftItem.FSplitter).FPressed := true;
    LeftItem.DoSplitterMouseMove(Sender, Shift, P.X, P.Y, 0, 0);
    TD2HackVisualObject(LeftItem.FSplitter).FPressed := false;
  end;
end;

procedure TD2HeaderItem.DragDrop(const Data: TD2DragObject;
  const Point: TD2Point);
var
  NewIndex, OldIndex: integer;
begin
  inherited;
  NewIndex := Index;
  OldIndex := TD2Object(Data.Source).Index;
  TD2Object(Data.Source).Index := Index;
  if (Header <> nil) and Assigned(Header.OnRealignItem) then
    Header.OnRealignItem(TD2Object(Data.Source), OldIndex, NewIndex);
end;

procedure TD2HeaderItem.DragEnd;
begin
  inherited;
end;

procedure TD2HeaderItem.DragOver(const Data: TD2DragObject;
  const Point:TD2Point; var Accept:boolean);
begin
  Accept := (Data.Source is TD2HeaderItem) and (TD2HeaderItem(Data.Source).Header = Header) and (DragMode <> d2DragManual);
end;
}

function TD2HeaderItem.Header:TD2Header;
{ Deleted by GoldenFox
begin
  if (Parent <> nil) and (Parent is TD2Header) then
    Result := TD2Header(Parent)
  else
    Result := nil;
end;
}
{Added by GoldenFox}
var
  P: TD2Object;
begin
  P := Parent;
  while (P <> nil) do
  begin
    if (P is TD2Header) then
    begin
      Result := TD2Header(P);
      Exit;
    end;
    P := P.Parent;
  end;
  Result := nil;
end;

//=============== TD2Header ===================================================

function TD2Header.GetItem(Index: integer): TD2HeaderItem;
begin
  //Result := TD2HeaderItem(Children[Index]); //Deleted by GoldenFox
  if FContent<>nil                                            //Added by GoldenFox
    then Result := TD2HeaderItem(FContent.Children[Index])    //Added by GoldenFox
    else Result := nil;                                       //Added by GoldenFox
end;

function TD2Header.GetItemsCount: integer;  //procedure added by GoldenFox
begin
  if FContent<>nil
    then Result := FContent.ChildrenCount
    else Result := 0;
end;

procedure TD2Header.ApplyStyle;  //procedure added by GoldenFox
var S: TD2Object;
begin
  inherited;
  S := FindResource('background');
  //S := FindResource('headerbackground');
  if (S <> nil) and (S.IsVisual) then
    FBackground := S.Visual;
end;

procedure TD2Header.FreeStyle; //procedure added by GoldenFox
begin
  FBackground:=nil;
  inherited;
end;

constructor TD2Header.Create(AOwner:TComponent);
begin
  inherited;
  ClipChildren:=true;
  //FSides := AllSides;                       //Deleted by GoldenFox
  //FLastItem := TD2HeaderItem.Create(Self);  //Deleted by GoldenFox
  //FLastItem.Parent := Self;                 //Deleted by GoldenFox
  //FLastItem.Stored := false;                //Deleted by GoldenFox
  //FLastItem.Locked := true;                 //Deleted by GoldenFox
  //FLastItem.Width := 50;                    //Deleted by GoldenFox
  //FLastItem.DragMode := d2DragManual;       //Deleted by GoldenFox
  Width := 100;                        //Added by GoldenFox
  Height := 21;                        //Added by GoldenFox
  Margins.Rect:=d2Rect(1,2,1,2);       //Added by GoldenFox

  FContent := TD2Content.Create(Self); //Added by GoldenFox
  FContent.Parent := Self;             //Added by GoldenFox
  FContent.Align := vaClient;          //Added by GoldenFox
  FContent.ClipChildren := true;       //Added by GoldenFox
  FContent.HitTest := false;           //Added by GoldenFox
  FContent.Locked := true;             //Added by GoldenFox
  FContent.Stored := false;            //Added by GoldenFox
end;

destructor TD2Header.Destroy;  //procedure added by GoldenFox
begin

  if FContent<>nil then
  begin
    FContent.Free;
    FContent:=nil;
  end;
  inherited;
end;


procedure TD2Header.AddObject(AObject: TD2Object);  //procedure added by GoldenFox
begin

  if (FContent <> nil) and (AObject is TD2HeaderItem)
    then begin
           FContent.AddObject(AObject);
           if FUpdating = 0 then Realign;
         end
    else inherited;
end;

procedure TD2Header.Paint;
//var R: TD2Rect; //Deleted by GoldenFox
begin
  inherited Paint;
  {//Deleted by GoldenFox
  if Assigned(Scene) and Scene.GetDesignTime and not Locked and not FInPaintTo then
  begin
    R := LocalRect;
    d2InflateRect(R, -0.5, -0.5);
    Canvas.StrokeThickness := 1;
    Canvas.StrokeDash := d2DashDash;
    Canvas.Stroke.Style := d2BrushSolid;
    Canvas.Stroke.SolidColor := $A0909090;
    Canvas.DrawRect(R, 0, 0, AllCorners, AbsoluteOpacity);
    Canvas.StrokeDash := d2DashSolid;
  end;
  }
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
      //Control.SetBounds(Left, Top, Control.Width, Height);   //Deleted by GoldenFox
      Control.SetBounds(Left, Top, Control.Width, Bottom-Top); //Added by GoldenFox
      //if Control <> FLastItem then //Deleted by GoldenFox
        Left:=Left + Control.Width;
    end;
  end;

  {Deleted by GoldenFox
  procedure DoAlign;
  var

    I: Integer;
    //J: Integer;
    //Control: TD2VisualObject;
  begin
    for I := 0 to ChildrenCount - 1 do
    begin
      if not (TD2VisualObject(Children[I]) is TD2HeaderItem) then Continue;
      DoPosition(TD2VisualObject(Children[I]));
      // sides
      TD2HeaderItem(Children[I]).Sides := FSides;
      // corners
      TD2HeaderItem(Children[I]).xRadius := Radius;
      TD2HeaderItem(Children[I]).yRadius := Radius;
      if I = 0 then
      begin
        TD2HeaderItem(Children[I]).Corners := [d2CornerTopLeft];
        TD2HeaderItem(Children[I]).Sides := TD2HeaderItem(Children[I]).Sides + (FSides * [d2SideLeft]) - [d2SideRight];
      end
      else
      if Children[I] = FLastItem then
      begin
        TD2HeaderItem(Children[I]).Corners := [d2CornerTopRight];
        TD2HeaderItem(Children[I]).Sides := TD2HeaderItem(Children[I]).Sides + [d2SideLeft];
      end
      else
      begin
        TD2HeaderItem(Children[I]).Corners := [];
        TD2HeaderItem(Children[I]).Sides := TD2HeaderItem(Children[I]).Sides + [d2SideLeft] - [d2SideRight];
      end;
    end;
  end;

     }
begin
  //inherited ;  //Deleted by GoldenFox
  if csDestroying in ComponentState then Exit;
  if FDisableAlign then Exit;
  inherited ;  //Added by GoldenFox
  //if ChildrenCount = 0 then Exit;              //Deleted by GoldenFox
  if ItemsCount <= 0 then Exit;        //Added by GoldenFox
  FDisableAlign := true;
  try
    //FChildren.Remove(FLastItem);               //Deleted by GoldenFox
    //FChildren.Add(FLastItem);                  //Deleted by GoldenFox
    R := d2Rect(FOffset, 0, FWidth, FHeight);
    //R := Margins.MarginRect(R);
    //DoAlign;                                   //Deleted by GoldenFox
    //if R.Left > R.Right then                   //Deleted by GoldenFox
    //  FLastItem.Width := 0                     //Deleted by GoldenFox
    //else                                       //Deleted by GoldenFox
    //  FLastItem.Width := R.Right - R.Left;     //Deleted by GoldenFox
    for i := 0 to ItemsCount - 1 do          //для всех полей заголовка Added by GoldenFox
    begin                                      //Added by GoldenFox
      DoPosition(TD2VisualObject(Items[i]));   //позиционируем поле согласно индекса  //Added by GoldenFox
      Items[i].Sides := [d2SideRight];         //включаем только правую границу полея//Added by GoldenFox
    end;
  finally
    FDisableAlign:=false;
  end;
end;

function TD2Header.ItemClass: string;
begin
  Result := 'TD2HeaderItem';
end;

{ Deleted by GoldenFox
procedure TD2Header.SetRadius(const Value: single);
begin
  if FRadius <> Value then
  begin
    FRadius := Value;
    Realign;
  end;
end;

procedure TD2Header.SetSides(const Value: TD2Sides);
begin
  if FSides <> Value then
  begin
    FSides := Value;
    Realign;
  end;
end;
}

//=============== TD2Column =========================================

constructor TD2Column.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=true;
  Width:=100;
  HitTest:=false;
  CanFocused:=false;
  FGrid:= GetGrid;
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

procedure TD2Column.SetWidth(const Value: single);  //Added by GoldenFox
begin
  inherited SetWidth(Value);
  if Grid<>nil then Grid.Realign;
end;

function TD2Column.GetGrid: TD2CustomGrid;
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

procedure TD2Column.Realign;
begin
  inherited Realign;
  if FGrid<>nil then FGrid.Realign;
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
    //if Grid.IsSelected(Grid.TopRow + i) and (Grid.ColumnIndex=Index) then //Deleted by GoldenFox
    if Grid.IsOneRowSelected and				//Added by GoldenFox
       Grid.IsSelected(Grid.TopRow + i) and		//Added by GoldenFox
      (Grid.ColumnIndex = Index) then			//Added by GoldenFox
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

procedure TD2Column.SetHeader(const Value:String);
begin
  if FHeader <> Value then
  begin
    FHeader := Value;
    //if (Grid <> nil) and (Grid.FHeader <> nil) then   //Deleted by GoldenFox
    //  Grid.FHeader.Items[Index].TextW := FHeader;     //Deleted by GoldenFox
    if (Grid <> nil) and (Grid.FHeader <> nil)                //Added by GoldenFox
      then Grid.FHeader.Items[Index].TextW := FHeader;     //Added by GoldenFox
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
  FScrollDuration:=0.3;
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
  //FItemHeight:=0;
  SetLength(FSelectedRows, 128);  //Added by GoldenFox
  FSelected := 0;                 //Added by GoldenFox
  FOldSelected := FSelected;      //Added by GoldenFox
  FSelectedRows[0] := 2;          //Added by GoldenFox
  FSelectedRows[1] := FSelected;  //Added by GoldenFox
  FSelectedRows[2] := FSelected;  //Added by GoldenFox
  FIsPreSelected := false;        //Added by GoldenFox
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
  Result:='TD2TextColumn;TD2CheckColumn;TD2ProgressColumn;TD2PopupColumn;TD2ImageColumn';
end;

procedure TD2CustomGrid.FreeStyle;
begin
  //inherited;		//Deleted by GoldenFox
  FSelection:=nil;
  FPreSelection := nil;   //Added by GoldenFox
  FFocus:=nil;
  FHeader:=nil;
  inherited;		  //Added by GoldenFox
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
    FHeader.FGrid := Self; //Added by GoldenFox
  end;
  T:=FindResource('selection');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    FSelection:=TD2VisualObject(T);
    FSelection.Visible:=false;
  end;
  T := FindResource('preselection');                 //Added by GoldenFox
  if (T <> nil) and (T is TD2VisualObject) then      //Added by GoldenFox
  begin                                              //Added by GoldenFox
    FPreSelection := TD2VisualObject(T);             //Added by GoldenFox
    FPreSelection.Visible := false;                  //Added by GoldenFox
  end;                                               //Added by GoldenFox
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
  Realign;
  UpdateColumns;
  UpdateHeader;
  UpdateSelection;
end;

procedure TD2CustomGrid.UpdateSelection;
var
  i:integer;
  l,b: single;
  P:TD2Point;
  R:TD2Rect;
  //Sel:boolean;				//Deleted by GoldenFox
  SelRects:array of TD2Rect;
  Clone:TD2VisualObject;
  //Vis:boolean;				//Deleted by GoldenFox
begin
  if FSelection=nil then Exit;
  //if ColumnCount=0 then Exit;
  if (RowCount=0) or (ColumnCount=0) then
  begin
    FSelectedRows[0]:=0;			//Added by GoldenFox обнулить длину массива выделенных строк
    if FSelections <> nil then
      begin
        //FSelections.Free;
        for i:=0 to FSelections.Count - 1 do
          TD2VisualObject(FSelections[i]).Visible:=false;	//Deleted by GoldenFox  сркыть все маркеры выделения
          //TD2VisualObject(FSelections[i]).Free; //Added by GoldenFox  удалить все маркеры выделения
        //FreeAndNil(FSelections);                //Added by GoldenFox  уничтожить удалить все маркеры выделениясписок маркеров
      end;
    if FPreSelection <> nil then FPreSelection.Visible := false;  //Added by GoldenFox  скрыть маркер предвыделения
    if FFocus <> nil then
      FFocus.Visible:=false;
    Exit;
  end;
  // calc rects
  //Vis:=true;	//Deleted by GoldenFox
  //Sel:=false;	//Deleted by GoldenFox

  //делаем массив прямоугольников
  SetLength(SelRects,0);
  //for i:=0 to RowCount - 1 do	//Deleted by GoldenFox
  i := 1;                                                //Added by GoldenFox
  while (i <= FSelectedRows[0]) do                       //Added by GoldenFox
  begin
    //if IsSelected(i) then	//Deleted by GoldenFox
    if (FSelectedRows[i] <= FSelectedRows[i+1]) and      //Added by GoldenFox
       (FSelectedRows[i+1] < RowCount) then              //Added by GoldenFox
    begin
	  //P:=d2Point(0,i * FRowHeight);	//Deleted by GoldenFox
	  P := d2Point(0, FSelectedRows[i] * FRowHeight);  //верхняя левая точка //Added by GoldenFox
      P := FContent.LocalToAbsolute(P);                //преобразуем в координаты контента
      if (FSelection.Parent <> nil) and FSelection.Parent.IsVisual then
        P := FSelection.Parent.Visual.AbsoluteToLocal(P); //преобразуем в координаты владельца FSelection
      if FContent.Width < ClientWidth then
        //R:=d2Rect(P.X,P.Y,P.X + ClientWidth,P.Y + FRowHeight)  //Deleted by GoldenFox
        R:=d2Rect(P.X,P.Y,P.X + ClientWidth,P.Y + FRowHeight*(FSelectedRows[i+1]-FSelectedRows[i]+1) )	//Added by GoldenFox
      else
       // R:=d2Rect(P.X,P.Y,P.X + FContent.Width,P.Y + FRowHeight);	//Deleted by GoldenFox
       R := d2Rect(P.X, P.Y, P.X + FContent.Width, P.Y + FRowHeight*(FSelectedRows[i+1]-FSelectedRows[i]+1));	//Added by GoldenFox
       //if (Length(SelRects) > 0) and (i > 0) and (IsSelected(i - 1)) then	//Deleted by GoldenFox
       //   SelRects[High(SelRects)]:=d2UnionRect(R,SelRects[High(SelRects)])	//Deleted by GoldenFox
       // else	//Deleted by GoldenFox
       // begin	//Deleted by GoldenFox
      SetLength(SelRects,Length(SelRects) + 1);
      SelRects[High(SelRects)]:=R;
    end;
      //Sel:=true;	//Deleted by GoldenFox
    //end;			//Deleted by GoldenFox
    i:=i+2;	//Added by GoldenFox
  end;
  // Create selection list
  if FSelections=nil then		//создаем список маркеров выделения если его нет
    FSelections:=TList.Create;
  // create selections
  if FSelections.Count < Length(SelRects) then	//если кол-во маркеров меньше чем кол-во прямоуголькиков
    for i:=FSelections.Count to Length(SelRects) - 1 do	//то создадим недостающие маркеры и добавим их в список
    begin
      Clone:=TD2VisualObject(FSelection.Clone(Self));
      Clone.ResourceName:='';
      FSelections.Add(Clone);
      Clone.Parent:=FSelection.Parent;
      Clone.Stored:=false;
    end;
  // hide if not need
  if Length(SelRects) < FSelections.Count then 		//если кол-во маркеров больше чем кол-во прямоугольников
    for i:=Length(SelRects) to FSelections.Count - 1 do	//то лишние скроем
    begin
      TD2VisualObject(FSelections[i]).Visible:=false;
    end;
  // align selections  расставим маркеры выделения
  for i:=0 to High(SelRects) do
  begin
    //TD2VisualObject(FSelections[i]).Visible:=Vis;	//Deleted by GoldenFox
    TD2VisualObject(FSelections[i]).Visible := true; //Added by GoldenFox  влючим очередной маркер
    //if Vis then		//Deleted by GoldenFox
    //begin				//Deleted by GoldenFox
      with SelRects[i] do
        TD2VisualObject(FSelections[i]).SetBounds(Left, Top, Right - Left, Bottom - Top); //установим границы маркера как у соотв. прямоугольника
      //if (FFocus <> nil) and (FShowSelectedCell) then  //Deleted by GoldenFox
      if (FFocus <> nil) then          //Added by GoldenFox
      begin
        if FShowSelectedCell and               //Added by GoldenFox если разрешено выделять ячейку
           IsOneRowSelected then               //Added by GoldenFox и выделена только 1 строка
          begin                                              //Added by GoldenFox
	    FFocus.Visible:=true;
	    FFocus.BringToFront;
	    P:=d2Point(Columns[ColumnIndex].Position.X,0);
	    P:=FContent.LocalToAbsolute(P);
	    if (FSelection.Parent <> nil) and FSelection.Parent.IsVisual then
	      P:=FSelection.Parent.Visual.AbsoluteToLocal(P);
            if FShowHorzLines then b:=1 else b:=0;			  //Added by GoldenFox
            if (FShowVertLines) and (ColumnIndex>0) then l:=1 else l:=0;  //Added by GoldenFox
            with SelRects[i] do                                                                  //установить координаты маркера ячейки
              //FFocus.SetBounds(P.X + FFocus.Padding.Left,SelRects[i].Top + FFocus.Padding.Top,
              FFocus.SetBounds(P.X + FFocus.Padding.Left+l, SelRects[i].Top + FFocus.Padding.Top,
                              //Columns[FColumnIndex].Width - (FFocus.Padding.Left + FFocus.Padding.Right),
                              Columns[FColumnIndex].Width - (FFocus.Padding.Left + FFocus.Padding.Right)-l,
                              //SelRects[i].Bottom - SelRects[i].Top - (FFocus.Padding.Top + FFocus.Padding.Bottom));
                              SelRects[i].Bottom - SelRects[i].Top - (FFocus.Padding.Top + FFocus.Padding.Bottom)-b);
          //end;
          end                          //Added by GoldenFox
        else FFocus.Visible := false;  //Added by GoldenFox иначе скрыть маркер ячейки
    end;
  end;
  //установим маркер пред.выбора
  if FPreSelection <> nil then                                                        //Added by GoldenFox
    begin                                                                             //Added by GoldenFox
      if FIsPreSelected then                                                            //Added by GoldenFox
        begin                                                                         //Added by GoldenFox
          FPreSelection.Visible:=true;                                                //Added by GoldenFox
          FPreSelection.BringToFront;                                                 //Added by GoldenFox
          P := d2Point(0, FSelected * FRowHeight);                                    //Added by GoldenFox
          P := FContent.LocalToAbsolute(P);                                           //Added by GoldenFox
          if (FPreSelection.Parent <> nil) and FPreSelection.Parent.IsVisual          //Added by GoldenFox
            then P := FPreSelection.Parent.Visual.AbsoluteToLocal(P);                 //Added by GoldenFox
          if FContent.Width < ClientWidth                                             //Added by GoldenFox
            then R := d2Rect(P.X, P.Y, P.X + ClientWidth, P.Y + FRowHeight)           //Added by GoldenFox
            else R := d2Rect(P.X, P.Y, P.X + FContent.Width, P.Y + FRowHeight);       //Added by GoldenFox
          FPreSelection.SetBounds(R.Left, R.Top, R.Right - R.Left, R.Bottom - R.Top); //Added by GoldenFox //установим границы маркера пред.выбора
        end                                                                           //Added by GoldenFox
        else  FPreSelection.Visible:=false;                                           //Added by GoldenFox
    end;                                                                              //Added by GoldenFox

  for i := 0 to ColumnCount - 1 do   //обновить колонки
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
  c: integer; //Added by GoldenFox
  Item:TD2HeaderItem;
begin
  if FHeader = nil then Exit; //если нет заголовка, то выйти
  FHeader.FOffset := -HScrollBar.Value;           //установить сдвиг заголовка
  //FHeader.FChildren.Remove(FHeader.FLastItem);              //Deleted by GoldenFox
  //if FHeader.ChildrenCount < ColumnCount then               //Deleted by GoldenFox
  //  for i := FHeader.ChildrenCount to ColumnCount - 1 do    //Deleted by GoldenFox
  c:=FHeader.ItemsCount;                 //кол-во полей в заголовке  Added by GoldenFox
  if c < ColumnCount then                //добавим поля в заголовок, если не хватает
    for i := c to ColumnCount - 1 do
    begin
      Item:=TD2HeaderItem.Create(Self);
      Item.Parent:=FHeader;
      Item.Locked:=true;
      Item.Stored:=false;
    end;
  if (c > ColumnCount) and (FHeader.FContent <> nil) then  //удалим лишние поля из заголовка Added by GoldenFox
    for i := ColumnCount to c - 1 do                       //Added by GoldenFox
        begin                                              //Added by GoldenFox
          Item := FHeader.GetItem(i);                      //Added by GoldenFox
          FHeader.FContent.RemoveObject(Item);             //Added by GoldenFox
          Item.Free;                                       //Added by GoldenFox
        end;                                               //Added by GoldenFox
  //FHeader.FChildren.Add(FHeader.FLastItem); //Deleted by GoldenFox

  for i := 0 to ColumnCount - 1 do //обновим ширину и надписи полей заголовка
  begin
    //TD2HeaderItem(FHeader.Children[i]).TextW := Columns[i].Header;  //Deleted by GoldenFox
    //TD2HeaderItem(FHeader.Children[i]).Width := Columns[i].Width;   //Deleted by GoldenFox
    FHeader.Items[i].TextW := Columns[i].Header; //пропишем текст поля заголовка   Added by GoldenFox
    FHeader.Items[i].Width := Columns[i].Width;  //установим ширину полея заголовка Added by GoldenFox
  end;
  //if FHeader <> nil then  //Deleted by GoldenFox
  FHeader.Realign;  //перестроим заголовок
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
  i{,Idx}:integer;
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
  //i: integer;
  P: TD2Point;
  //P1: TD2Point;
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
  P: TD2Point;
  //P1: TD2Point;
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

function TD2CustomGrid.CanEditModify: Boolean;
begin
  Result:=not ReadOnly;
end;

function TD2CustomGrid.CanEditAcceptKey(Key: System.WideChar): Boolean;
begin
  Result:=true;
end;

procedure TD2CustomGrid.KeyDown(var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  inherited ;
  if RowCount > 0 then
  begin
    if IsOneRowSelected then	//Added by GoldenFox
    begin			//Added by GoldenFox
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
        VK_LEFT:
          begin
            if (Shift = []) and (ColumnIndex > 0) then ColumnIndex := ColumnIndex - 1;
            if (Shift = [ssCtrl]) and (ColumnIndex <> 0) then ColumnIndex := 0;
          end;
        VK_RIGHT:
          begin
            if (Shift = []) and (ColumnIndex < ColumnCount - 1)
              then ColumnIndex := ColumnIndex + 1;
            if (Shift = [ssCtrl]) and (ColumnIndex <> ColumnCount - 1)
               then ColumnIndex := ColumnCount - 1;
          end;
      end;
    end;
    case Key of
      VK_SPACE:
        if MultiSelect and (Shift = [ssCtrl])  then
        begin
          ChangeSelectionRow(FSelected);
          UpdateSelection;
        end;
      VK_A:
        if (Shift = [ssCtrl]) and MultiSelect then //Ctrl + A
          begin
            FSelectedRows[0]:=2;
            FSelectedRows[1]:=0;
            FSelectedRows[2]:=RowCount - 1;
            FIsPreSelected:=false;
            UpdateSelection;
          end;
      VK_UP:
        if FSelected > 0 then
        begin
          if Shift = [] then SetSelected(FSelected - 1);  //нажата только вверх
          if Shift = [ssShift] then SetSelectedMoreRow(Selected - 1); //если Shift + ввех
          if Shift = [ssCtrl]  then SetPreSelected(FSelected - 1);    //Ctrl + ввех  и выбрана не первая строка
        end;
      VK_DOWN:
        if FSelected < RowCount-1 then
        begin
          if Shift = [] then SetSelected(FSelected + 1);  //нажата только вниз
          if Shift = [ssShift] then SetSelectedMoreRow(FSelected + 1); //если Shift + вниз
          if Shift = [ssCtrl]  then SetPreSelected(FSelected + 1);     //Ctrl + вниз и выбрана не последняя строка
        end;
      VK_HOME:
        if FSelected > 0 then
        begin
          if Shift = [] then SetSelected(0);  //нажата только Home
          if Shift = [ssShift] then SetSelectedMoreRow(0);   //если Shift + Home
          if Shift = [ssCtrl]  then SetPreSelected(0);  //Ctrl + Home и выбрана не первая строка
        end;
      VK_END:
        if FSelected < RowCount-1 then
        begin
          if Shift = [] then SetSelected(RowCount - 1);  //нажата только End
          if Shift = [ssShift] then SetSelectedMoreRow(RowCount - 1);   //если Shift + End
          if Shift = [ssCtrl]  then SetPreSelected(RowCount - 1);  //Ctrl + End и выбрана не последняя строка
        end;
      VK_PRIOR:
        if FSelected > 0 then
        begin
          if Shift = [] then SetSelected(FSelected - Min(RowCount, VisibleRows-3));  //нажата только End
          if Shift = [ssShift] then SetSelectedMoreRow(FSelected - Min(RowCount, VisibleRows-3));   //если Shift + End
          if Shift = [ssCtrl]  then SetPreSelected(FSelected - Min(RowCount, VisibleRows-3));  //Ctrl + End и выбрана не последняя строка
        end;
      VK_NEXT:
        if FSelected < RowCount-1 then
        begin
          if Shift = [] then SetSelected(FSelected + Min(RowCount, VisibleRows-3));  //нажата только End
          if Shift = [ssShift] then SetSelectedMoreRow(FSelected + Min(RowCount, VisibleRows-3));   //если Shift + End
          if Shift = [ssCtrl]  then SetPreSelected(FSelected + Min(RowCount, VisibleRows-3));  //Ctrl + End и выбрана не последняя строка
        end;
		{Block End added by GoldenFox}
      else Exit;
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
    //Selected:=RowByPoint(X,Y); //Deleted by GoldenFox
    if Shift = [ssLeft] then Selected := RowByPoint(X, Y);                  //нажата только ЛКМ 	//Added by GoldenFox
    if Shift = [ssLeft, ssShift] then SetSelectedMoreRow(RowByPoint(X, Y)); //если нажаты ЛКМ + Shift   //Added by GoldenFox
    if Shift = [ssLeft, ssCtrl] then ChangeSelectionRow(RowByPoint(X, Y));  //если нажаты ЛКМ + Ctrl	//Added by GoldenFox
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
    //Selected:=RowByPoint(X,Y);	//Deleted by GoldenFox
    //if (Shift = [ssLeft]) then SetSelectedMoreRow(RowByPoint(X, Y));	//Added by GoldenFox
    Column:=ColumnByPoint(X,Y);
    if Column <> nil then
      ColumnIndex:=Column.Index;
  end;
end;

function TD2CustomGrid.ChangeSelectionRow(Idx: integer):boolean; //Инвертировать выделение строки Idx. Результат: true - строка выделена, false - развыделена //Added by GoldenFox
var L, M, R: integer; //левая граница, середина, правая граница
    b: boolean;       //

  procedure DelRange(t: integer);  //удаление строк t и t+1 из массива
  var i: integer;
  begin
    if FSelectedRows[0] < 2 then exit;        //если массив пустой то выходим
    if FSelectedRows[0] > 2 then              //если в массиве больше одной строки
      for i:=t to FSelectedRows[0]-2 do          //то все что ниже удаляемой строки
        FSelectedRows[i]:=FSelectedRows[i+2];    //сдвигаем на одну строку вверх
    FSelectedRows[0]:=FSelectedRows[0]-2;     //уменьшаем длину массва на 1
  end;

  procedure AddRange(t: integer);  //добавить 2 строки в массив начиная с t
  var i: integer;
  begin
    if FSelectedRows[0]+3 > Length(FSelectedRows)            //если увеличиваться некуда
      then SetLength(FSelectedRows, FSelectedRows[0]+129);     //то добавим 128 эл.тов к массиву
    for i:=FSelectedRows[0] downto t do                      //начиная с последней строки до t
      FSelectedRows[i+2]:=FSelectedRows[i];                     //сдвигаем все на одну строку вниз
    FSelectedRows[0]:=FSelectedRows[0]+2;                    //увеличим последнее значение массва на 1 строку
  end;

begin
  if not FMultiSelect then
  begin
    FIsPreSelected:= false;
    Selected:=Idx;
    exit;
  end;
  if Idx < 0 then Idx := 0;
  if Idx > FRowCount - 1 then Idx := FRowCount - 1;
  Result:=true;
  FIsPreSelected:= not Result;
  if FSelectedRows[0]<2 then //если массив выделения пустой
    begin
      FSelectedRows[0]:= 2;    //последний эл-т массива (1 диапазон)
      FSelectedRows[1]:= Idx;  //начало выделения
      FSelectedRows[2]:= Idx;  //конец выделения
      FOldSelected := FSelected;
      FSelected := Idx;                                 //Added by GoldenFox
      ScrollToRow(Idx);
      UpdateSelection;
      exit;                    //выход
    end;
  //поиск позиции в массиве
  L:=1;
  R:=FSelectedRows[0];
  b:=true;
  while b and (L <= R) do    //делать пока не найдено искомое или левая граница левее правой
  begin
    M:= L + ((R - L) div 2);   //найдем середину диапазона
    if Idx < FSelectedRows[M]          //если искомое меньше среднего
      then R:=M-1                         //то установим правую границу перед средним значением
      else if Idx > FSelectedRows[M]      //иначе если искомое больше среднего
             then L:=M+1                       //то установим левую границу после среднего значения
             else b:=false;                    //иначе значение равно искомому
  end;

  if b then //найдено не точно: строка находится внутри диапазона
    begin
      if Idx < FSelectedRows[M]   //если искомое меньше среднего
        then begin                  //то
               L:=M-1;                 //левая граница диапазона меньше среднего
               //if L<0 then L:=0;
               if (M and 1) = 0        //проверим M на четность
                 then b:=true             //М - четное: нижняя граница выделения; строка внутри выделения
                 else b:=false;           //М - нечетное: верхняя граница выделения; строка вне выделения
             end
        else begin                  //иначе искомое больше среднего
               L:=M;                   //левая граница диапазона = среднему
               //if L > FSelectedRows[0] then L:=FSelectedRows[0];
               if (M and 1) = 0        //проверим M на четность
                 then b:=false            //М - четное: нижняя граница выделения; строка вневыделения
                 else b:=true;            //М - нечетное: верхняя граница выделения; строка внутри выделения
             end;
      if b
        then begin     //если значение внутри выделения
               AddRange(L+1);                    //разделим диапазон на 2
               FSelectedRows[L+1]:=Idx-1;        //исключим строку из обоих диапазонов
               FSelectedRows[L+2]:=Idx+1;
               Result:=false;
               FIsPreSelected:= not Result;
             end
        else begin    //если значение вне выделения
               if ((L = 0) and (Idx < FSelectedRows[1]-1)) or                  //если строка выше первого интервала и не предыдущая перед выделением или
                  ((L = FSelectedRows[0]) and (Idx > FSelectedRows[L]+1)) or   //строка ниже последнего интервала и не следующая за выделеннойили
                  ((L > 0)  and (Idx > FSelectedRows[L]+1) and                 //ниже первого и выше последнего интервала и не следующая/предыдущая за/перед выделением
                   (L < FSelectedRows[0]) and (Idx < FSelectedRows[L+1]-1))
                 then  begin                                                   //то
                         AddRange(L+1);                                        //добавим диапазон
                         FSelectedRows[L+1]:=Idx;                              //пропишем строку как выделенную
                         FSelectedRows[L+2]:=Idx;
                         FOldSelected := FSelected;
                         FSelected := Idx;
                         ScrollToRow(Idx);
                         UpdateSelection;
                         exit;
                       end;

               if (L > 0) and (Idx = FSelectedRows[L]+1) //если строка ниже первого интервала и следующая за выделенной
                 then FSelectedRows[L]:=Idx;               //добавим строку к диапазону
               if (L < FSelectedRows[0]) and (Idx = FSelectedRows[L+1]-1)
                 then FSelectedRows[L+1]:=Idx; //если строка предыдущая перед выделенной  то добавим строку к диапазону
               if (L > 0) and (L < FSelectedRows[0]) and
                  (FSelectedRows[L]>=FSelectedRows[L+1]-1) then  //если 2 выделенных диапазона пересеклись
                 begin
                   FSelectedRows[L]:=FSelectedRows[L+2];
                   DelRange(L+1);
                 end;
             end;
    end
  else    //найдено точно: строка отмечена и находится на границе диапазона
    begin
      if (M and 1) = 0   //проверим M на четность
        then if FSelectedRows[M-1]=FSelectedRows[M]       //М - четное: нижняя граница
               then DelRange(M-1)                              //если это одна строка удаляем её;
               else FSelectedRows[M]:=FSelectedRows[M]-1     //иначе это несколько строк - уменьшим нижнюю границу
        else if FSelectedRows[M]=FSelectedRows[M+1]       //М - нечетное: верхняя граница
               then DelRange(M)                                //если это одна строка удаляем ее;
               else FSelectedRows[M]:=FSelectedRows[M]+1;     //иначе это несколько строк - увеличим верхнюю границу
      Result:=false;
      FIsPreSelected:= not Result;
    end;
  FOldSelected := FSelected;
  FSelected := Idx;                                 //Added by GoldenFox
  ScrollToRow(Idx);
  UpdateSelection;
end;

procedure TD2CustomGrid.SetSelectedMoreRow(Idx: integer);  //добавить выделенных строк procedure added by GoldenFox
begin
  if FMultiSelect then
    begin
      if Idx < 0 then Idx := 0;
      if Idx > FRowCount - 1 then Idx := FRowCount - 1;
      if (FSelectedRows[0]<>2) or
         ((FSelectedRows[1] < FSelected) and (FSelected < FSelectedRows[2]))
       then begin
              FSelectedRows[0]:=2; //установим длину массива для 1 строки
              if Idx >= FSelected
                then  begin
                        FSelectedRows[1]:= FSelected;  //начало выделения
                        FSelectedRows[2]:= Idx;     //конец выделения
                      end
                else  begin
                      FSelectedRows[1]:= Idx;     //начало выделения
                      FSelectedRows[2]:= FSelected;  //конец выделения
                    end;
            end
       else begin
              if FSelected <= FSelectedRows[1]  //выделение cнизу вверх
                then if Idx <= FSelectedRows[2]
                        then FSelectedRows[1]:=Idx
                        else begin
                               FSelectedRows[1]:=FSelectedRows[2];
                               FSelectedRows[2]:=Idx;
                             end
                else if FSelected >= FSelectedRows[2]  //выделение сверху вниз
                        then if Idx >= FSelectedRows[1]
                                then FSelectedRows[2]:=Idx
                                else begin
                                       FSelectedRows[2]:=FSelectedRows[1];
                                       FSelectedRows[1]:=Idx;
                                     end
                        else begin
                             end;
            end;
      FIsPreSelected:=false;
      FOldSelected := FSelected;
      FSelected := Idx;
      ScrollToRow(FSelected);
      UpdateSelection;
    end
    else SetSelected(Idx);
end;

{Deleted by GoldenFox
procedure TD2CustomGrid.MouseUp(Button:TMouseButton; Shift:TShiftState; X,
  Y:single);
begin
  inherited;
end;
 }

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
  if AObject is TD2Column then
    begin
      TD2Column(AObject).FGrid:=TD2Column(AObject).GetGrid;
      if Assigned(FScene) and (FScene.GetDesignTime) then
        begin
          FSelected:=0;
          FOldSelected := FSelected;
          FSelectedRows[0]:=2;                                           //Added by GoldenFox
          FSelectedRows[1]:=FSelected;                                   //Added by GoldenFox
          FSelectedRows[2]:=FSelected;                                   //Added by GoldenFox
          FIsPreSelected:=false;                                        //Added by GoldenFox
          UpdateSelection;
        end;
    end;
end;

procedure TD2CustomGrid.ApplyResource;  //procedure Added by GoldenFox
begin
  if FNeedResource and (FSelections <> nil) then FreeAndNil(FSelections);
  inherited;
end;

procedure TD2CustomGrid.ScrollToRow(ARow: integer);  //procedure Added by GoldenFox
begin
  if ARow <= TopRow then VScrollBar.Value := ARow * FRowHeight;
  if ARow * FRowHeight > VScrollBar.Value + ClientHeight - FRowHeight
    then VScrollBar.Value := ARow * FRowHeight - ClientHeight + FRowHeight;
end;

{Deleted by GoldenFox
procedure TD2CustomGrid.RemoveObject(AObject:TD2Object);
begin
  inherited;
end;

procedure TD2CustomGrid.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
end;
}

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
  if FVScrollBar = nil
    then Result:=0
    else Result:=trunc(FVScrollBar.Value / FRowHeight);
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

procedure TD2CustomGrid.SetPreSelected(const Value: integer);
var S:integer;
begin
  if (FSelected <> Value) then
  begin
    FOldSelected := FSelected;
    S := Value;
    if S < 0 then S:=0;
    if S > FRowCount - 1 then S:=FRowCount - 1;
    if MultiSelect
      then begin
             FSelected:=S;
             ScrollToRow(FSelected);
             if IsOneRowSelected
               then FIsPreSelected:=false
               else FIsPreSelected:=true;
             UpdateSelection;
           end
      else SetSelected(S);
  end;
end;

procedure TD2CustomGrid.SetSelected(const Value:integer);
begin
  //if FSelected <> Value then		//Deleted by GoldenFox
  if (FSelected <> Value) or				//Added by GoldenFox
     (FSelectedRows[0] <> 2) or				//Added by GoldenFox
     (FSelectedRows[1] <> FSelectedRows[2]) then	//Added by GoldenFox
  begin
    FOldSelected := FSelected;
    FSelected:=Value;
    if FSelected < 0 then FSelected:=0;
    if FSelected > FRowCount - 1 then FSelected:=FRowCount - 1;
    FSelectedRows[0]:=2;                                           //Added by GoldenFox
    FSelectedRows[1]:=FSelected;                                   //Added by GoldenFox
    FSelectedRows[2]:=FSelected;                                   //Added by GoldenFox
    FIsPreSelected:=false;                                        //Added by GoldenFox
    ScrollToRow(FSelected);                                       //Added by GoldenFox
    UpdateSelection;
  end;
end;

function TD2CustomGrid.IsSelected(Row:integer):boolean; //проверить выбрана строка или нет
var i: integer;                                                  //Added by GoldenFox
begin
  //Result := Row = FSelected;                            //Deleted by GoldenFox
  if FSelectedRows[0] < 2                                        //Added by GoldenFox
    then begin                                                   //Added by GoldenFox
           Result := false;                                      //Added by GoldenFox
           exit;                                                 //Added by GoldenFox
         end;                                                    //Added by GoldenFox
  Result := true;                                                //Added by GoldenFox
  i:=1;                                                          //Added by GoldenFox
  while i < FSelectedRows[0] do                                  //Added by GoldenFox
    if (FSelectedRows[i] <= Row) and (Row <=FSelectedRows[i+1])  //Added by GoldenFox
      then exit                                                  //Added by GoldenFox
      else i:=i+2;                                               //Added by GoldenFox
  Result := false;                                               //Added by GoldenFox
end;

function TD2CustomGrid.IsOneRowSelected: boolean; //function Added by GoldenFox
begin
  if (FSelectedRows[0] = 2) and
     (FSelectedRows[1] = FSelectedRows[2]) and
     (FSelected = FSelectedRows[1])
    then Result:=true
    else Result:=false;
end;

procedure TD2CustomGrid.SetAlternatingRowBackground(const Value:boolean);
begin
  if FAlternatingRowBackground <> Value then
  begin
    FAlternatingRowBackground:=Value;
    Repaint;
  end;
end;

procedure TD2CustomGrid.SetMultiSelect(const Value: boolean);  //procedure Added by GoldenFox
begin
  if FMultiSelect <> Value then
  begin
    FMultiSelect := Value;
    if not FMultiSelect then
    begin
      FSelectedRows[0]:=2;
      FSelectedRows[1]:=FSelected;
      FSelectedRows[2]:=FSelected;
    end;
    UpdateSelection;
  end;
end;

procedure TD2CustomGrid.SetShowSelectedCell(const Value:boolean);
begin
  if FShowSelectedCell <> Value then
  begin
    FShowSelectedCell:=Value;
    UpdateSelection;
  end;
end;

procedure TD2CustomGrid.ContentRemoveObject(AObject: TD2Object);  //procedure Adedd by GoldenFox
begin
  inherited;
  Realign;
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
  Realign;
  UpdateSelection;
end;

procedure TD2CustomGrid.Reset;
begin
  SetFocus;
end;

//=============== TD2StringColumn =======================================================

//constructor TD2StringColumn.Create(AOwner:TComponent);
//begin
//  inherited;
//end;
//
//destructor TD2StringColumn.Destroy;
//begin
//  inherited;
//end;

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
