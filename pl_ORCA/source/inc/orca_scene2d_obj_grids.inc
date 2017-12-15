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
  FxRadius:=0;
  FyRadius:=0;
end;

procedure TD2HeaderItem.ApplyStyle;
var S: TD2Object;
begin
  inherited;
  S := FindResource('splitter');
  if (S <> nil) and (S is TD2VisualObject) then
  begin
    FSplitter := TD2VisualObject(S);
    FSplitter.AutoCapture := true; //продолжать изменение размера при выходе курсора за пределы элемента пока нажата кнопка мыши
    FSplitter.OnMouseMove := DoSplitterMouseMove; //обработчик прерывания изменения размера
  end;
end;

procedure TD2HeaderItem.FreeStyle;
begin
  FSplitter:=nil;
  inherited;
end;

procedure TD2HeaderItem.DoSplitterMouseMove(Sender:TObject;
  Shift:TShiftState; X,Y,Dx,Dy:single);
var
  NewSize:single;
  H: TD2Header;
begin
  if TD2HackVisualObject(FSplitter).FPressed then
  begin
    NewSize:=AbsoluteToLocal(FSplitter.LocalToAbsolute(d2Point(X,Y))).X;
    if NewSize < 0 then NewSize:=0;
    H:=Header;
    if (H <> nil) and (Assigned(H.OnResizeItem))  //если есть заголовок и у него установлен обраотчик прерывания изменения размера
      then H.OnResizeItem(Self, NewSize);           //то вызвать прерывание
    Width := NewSize;                             //установить новый размер после обработки прерывания
    if H <> nil then Header.Realign;              //перестроить заголовок если есть
  end;
end;

procedure TD2HeaderItem.MouseDown(Button: TMouseButton; Shift: TShiftState; X,  Y: single);
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

procedure TD2HeaderItem.MouseMove(Shift: TShiftState; X, Y, Dx, Dy: single);
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

procedure TD2HeaderItem.MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: single);
var
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

function TD2HeaderItem.Header:TD2Header;
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
  if FContent<>nil
    then Result := TD2HeaderItem(FContent.Children[Index])
    else Result := nil;
end;

function TD2Header.GetItemsCount: integer;
begin
  if FContent<>nil
    then Result := FContent.ChildrenCount
    else Result := 0;
end;

procedure TD2Header.ApplyStyle;
var S: TD2Object;
begin
  inherited;
  S := FindResource('background');
  if (S <> nil) and (S.IsVisual) then
    FBackground := S.Visual;
end;

procedure TD2Header.FreeStyle;
begin
  FBackground:=nil;
  inherited;
end;

constructor TD2Header.Create(AOwner:TComponent);
begin
  inherited;
  ClipChildren:=true;
  Width := 100;
  Height := 21;
  Margins.Rect:=d2Rect(1,2,1,2);

  FContent := TD2Content.Create(Self);
  FContent.Parent := Self;
  FContent.Align := vaClient;
  FContent.ClipChildren := true;
  FContent.HitTest := false;
  FContent.Locked := true;
  FContent.Stored := false;
end;

destructor TD2Header.Destroy;
begin
  if FContent<>nil then
  begin
    FContent.Free;
    FContent:=nil;
  end;
  inherited;
end;

procedure TD2Header.AddObject(AObject: TD2Object);
begin
  if (FContent <> nil) and (AObject is TD2HeaderItem)
    then begin
           FContent.AddObject(AObject);
           if FUpdating = 0 then Realign;
         end
    else inherited;
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
      Control.SetBounds(Left, Top, Control.Width, Bottom-Top);
      Left:=Left + Control.Width;
    end;
  end;

begin
  if csDestroying in ComponentState then Exit;
  if FDisableAlign then Exit;
  inherited ;
  if ItemsCount <= 0 then Exit;
  FDisableAlign := true;
  try
    R := d2Rect(FOffset, 0, FWidth, FHeight);
    for i := 0 to ItemsCount - 1 do          //для всех полей заголовка
    begin
      DoPosition(TD2VisualObject(Items[i]));   //позиционируем поле согласно индекса
      Items[i].Sides := [d2SideRight];         //включаем только правую границу полей
    end;
  finally
    FDisableAlign:=false;
  end;
end;

function TD2Header.ItemClass: string;
begin
  Result := 'TD2HeaderItem';
end;

//=============== TD2Column =========================================

constructor TD2Column.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=true;
  Width:=100;
  HitTest:=false;
  CanFocused:=false;
  FGrid:= GetGrid;
  FCellControls:= TList.Create;
  FClipChildren:= true;
end;

destructor TD2Column.Destroy;
var i: integer;
begin
  if FCellControls <> nil then
  begin
    for i:=0 to FCellControls.Count-1 do
      TD2Control(FCellControls.Items[i]).Free;
    FCellControls.Free;
  end;

  inherited Destroy;
end;

function TD2Column.CreateCellControl:TD2Control;
begin
  Result:=nil;
end;

procedure TD2Column.InitCellControl(ACellControl: TD2Control);
begin
  ACellControl.Parent:=Self;
  ACellControl.HitTest:=false;
  ACellControl.Visible:=false;
  ACellControl.Locked:=true;
  ACellControl.Stored:=false;
  ACellControl.CanFocused:=false;
  ACellControl.OnCanFocused:=DoCanFocused;
  ACellControl.OnEnterFocus:=DoEnterFocus;
  ACellControl.OnKillFocus:=DoKillFocus;
  ACellControl.OnKeyDown:=DoKeyDown;
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
  Grid.FocusedColumn:=Index;
  FSaveData:=TD2Object(Sender).Data;
end;

procedure TD2Column.DoKillFocus(Sender: TObject);
begin
  if TD2Object(Sender).IsVisual then
    with TD2VisualObject(Sender) do
    begin
      CanFocused:=false;
      HitTest:=false;
    end;
end;

procedure TD2Column.DoKeyDown(Sender:TObject; var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  if (KeyChar <> #0) and (Grid <> nil) and not Grid.CanEditAcceptKey(KeyChar) then
    KeyChar:=#0;
  if (Key=VK_RETURN) and (Grid <> nil) then
  begin
    Key:=VK_UNKNOWN;
    Grid.SetFocus;
  end;
  if (Key=VK_ESCAPE) and (Grid <> nil) then
  begin
    FDisableChange:=true;
    try
      TD2Object(Sender).Data:=FSaveData;
    finally
      FDisableChange:=false;
    end;
    Key:=VK_UNKNOWN;
    Grid.Reset;
  end;
  if (Key=VK_UP) or (Key=VK_DOWN) and (Grid <> nil) then
  begin
    Grid.SetFocus;
    Grid.KeyDown(Key,Keychar,Shift);
  end;
end;

procedure TD2Column.CellSetFocus(ARow: integer);
begin
  //виртуальный метод определяется в потомках
end;

procedure TD2Column.SetWidth(const Value: single);
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
  for i:=0 to FCellControls.Count - 1 do
    TD2Control(FCellControls.Items[i]).Free;
  FCellControls.Clear;
end;

function TD2Column.CellControlByPoint(X,Y:single):TD2Control;
var
  i:integer;
  P:TD2Point;
begin
  Result:=nil;
  if Grid=nil then Exit;

  P:=Grid.LocalToAbsolute(d2Point(X,Y));
  for i:=0 to FCellControls.Count - 1 do
    with TD2Control(FCellControls.Items[i]) do
      if Visible and (pointInObject(P.X,P.Y)) then
      begin
        Result:=TD2Control(FCellControls.Items[i]);
        Exit;
      end;
end;

function TD2Column.CellControlByRow(Row:integer):TD2Control;
var
  i:integer;
begin
  Result:=nil;
  if Grid=nil then Exit;
  for i:=0 to FCellControls.Count - 1 do
    with TD2Control(FCellControls.Items[i]) do
      if Visible and (trunc(d2StringToPoint(TagString).Y)=Row) then
      begin
        Result:=TD2Control(FCellControls.Items[i]);
        Exit;
      end;
end;

procedure TD2Column.Realign;
begin
  inherited Realign;
  if FGrid<>nil then FGrid.Realign;
end;

function TD2CustomGrid.GetTopRow:integer;
begin
  if FVScrollBar = nil
    then Result:=0
    else Result:=trunc(FVScrollBar.Value / FRowHeight);
end;

function TD2CustomGrid.GetTopRowY: single;
begin
  Result:= TopRow * FRowHeight;
end;

procedure TD2Column.UpdateColumn;
var
  i,C,m:integer;
  V:Variant;
  cc: TD2Control;
begin
  if Grid=nil then Exit;
  FUpdateColumn:=true;
  try
    m:= Min(Grid.RowCount,Grid.VisibleRows);
    C:= FCellControls.Count;
    { Create controls }
    if C < m
       then begin
              for i:= C to m - 1 do
              begin
                cc:=CreateCellControl;
                InitCellControl(cc);
                FCellControls.Add(cc);
              end;
            end
    else
    { Hide if need }
    for i:=m to FCellControls.Count-1 do
      TD2Control(FCellControls.Items[i]).Visible:=false;

    { Update Data }
    for i:=0 to m - 1 do
    begin
      if Grid.TopRow + i >= Grid.RowCount then Break;
      TD2Control(FCellControls.Items[i]).Visible:=true;
      TD2Control(FCellControls.Items[i]).SetBounds(0, i * Grid.RowHeight, Width,Grid.RowHeight);
      TD2Control(FCellControls.Items[i]).TagString:=d2PointToString(d2Point(Index,Grid.TopRow + i));
      V:=Grid.GetValue(Index,Grid.TopRow + i);
      TD2Control(FCellControls.Items[i]).Data:=V;
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
  if Assigned(FCellControls) and  (FCellControls.Count > 0) then
    for i:=0 to Min(Grid.RowCount,Grid.VisibleRows) - 1 do
      with TD2Control(FCellControls.Items[i]) do
        begin
          HitTest:=false;
          if IsFocused
            then Grid.SetFocus;
        end;
end;

procedure TD2Column.SetHeader(const Value:String);
begin
  if FHeader <> Value then
  begin
    FHeader := Value;
    if (Grid <> nil) and (Grid.FHeader <> nil)
      then Grid.FHeader.Items[Index].TextW := FHeader;
  end;
end;

//=============== TD2TextColumn ================================================
function TD2TextColumn.CreateCellControl:TD2Control;
begin
  Result:=TD2TextCell.Create(Self);
end;

procedure TD2TextColumn.InitCellControl(ACellControl: TD2Control);
begin
  inherited;
  TD2TextCell(ACellControl).OnChange:=DoTextChanged;
end;

procedure TD2TextColumn.DoTextChanged(Sender:TObject);
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

procedure TD2TextColumn.DoEnterFocus(Sender: TObject);
begin
  inherited;
  with TD2TextCell(Sender) do
  begin
    SelLength:=0;
    CaretPosition:=Length(TextW);
  end;
end;

procedure TD2TextColumn.DoKillFocus(Sender: TObject);
begin
  inherited;
  with TD2TextCell(Sender) do
  begin
    CaretPosition:=0;
  end;
end;

procedure TD2TextColumn.CellSetFocus(ARow: integer);
var C: TD2Control;
begin
  inherited;
  C:=CellControlByRow(ARow);
  if C <> nil then
  begin
    C.HitTest:=true;
    C.CanFocused:=true;
    C.SetFocus;
  end;
end;

//=============== TD2CheckColumn ================================================

function TD2CheckColumn.CreateCellControl:TD2Control;
begin
  Result:=TD2CheckCell.Create(Self);

end;

procedure TD2CheckColumn.InitCellControl(ACellControl: TD2Control);
begin
  inherited;
  TD2CheckCell(ACellControl).OnChange:=DoCheckChanged;
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
end;

procedure TD2ProgressColumn.InitCellControl(ACellControl: TD2Control);
begin
  inherited;
  TD2ProgressCell(ACellControl).Min:=FMin;
  TD2ProgressCell(ACellControl).Max:=FMax;
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
end;

procedure TD2PopupColumn.InitCellControl(ACellControl: TD2Control);
begin
  inherited;
  TD2PopupCell(ACellControl).Items.Assign(FItems);
end;

procedure TD2PopupColumn.SetItems(const Value:TD2WideStrings);
begin
  FItems.Assign(Value);
end;

//=============== TD2ImageColumn =============================================

function TD2ImageColumn.CreateCellControl:TD2Control;
begin
  Result:=TD2ImageCell.Create(Self);
end;

procedure TD2ImageColumn.InitCellControl(ACellControl: TD2Control);
begin
  inherited;
  TD2ImageCell(ACellControl).EnableOpenDialog:=false;
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
  Width:=200;
  Height:=100;
  //FItemHeight:=0;
  SetLength(FSelectedRows, 128);
  FSelected := 0;
  FOldSelected := FSelected;
  FSelectedRows[0] := 2;
  FSelectedRows[1] := FSelected;
  FSelectedRows[2] := FSelected;
  FIsPreSelected := false;
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
  FSelection:=nil;
  FPreSelection := nil;
  FFocus:=nil;
  FHeader:=nil;
  inherited;
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
end;

procedure TD2CustomGrid.UpdateSelection;   //обновить маркеры выбора строк
var
  i:integer;
  l,b: single;
  P:TD2Point;
  R:TD2Rect;
  SelRects:array of TD2Rect;
  Clone:TD2VisualObject;
begin
  if FSelection=nil then Exit;
  if (RowCount=0) or (ColumnCount=0) then
  begin
    FSelectedRows[0]:=0;			//обнулить длину массива выделенных строк
    if FSelections <> nil then
      begin
        for i:=0 to FSelections.Count - 1 do
          TD2VisualObject(FSelections[i]).Visible:=false;	//сркыть все маркеры выделения
      end;
    if FPreSelection <> nil then FPreSelection.Visible := false;  //скрыть маркер предвыделения
    if FFocus <> nil then
      FFocus.Visible:=false;
    Exit;
  end;
  //делаем массив прямоугольников
  SetLength(SelRects,0);
  i := 1;
  while (i <= FSelectedRows[0]) do
  begin
    if (FSelectedRows[i] <= FSelectedRows[i+1]) and
       (FSelectedRows[i+1] < RowCount) then
    begin
      P := d2Point(0, FSelectedRows[i] * FRowHeight);  //верхняя левая точка
      P := FContent.LocalToAbsolute(P);                //преобразуем в координаты контента
      if (FSelection.Parent <> nil) and FSelection.Parent.IsVisual then
        P := FSelection.Parent.Visual.AbsoluteToLocal(P); //преобразуем в координаты владельца FSelection
      if FContent.Width < ClientWidth
        then R:= d2Rect(P.X, P.Y, P.X + ClientWidth,    P.Y + FRowHeight*(FSelectedRows[i+1] - FSelectedRows[i] + 1) )
        else R:= d2Rect(P.X, P.Y, P.X + FContent.Width, P.Y + FRowHeight*(FSelectedRows[i+1] - FSelectedRows[i] + 1) );
      SetLength(SelRects,Length(SelRects) + 1);
      SelRects[High(SelRects)]:=R;
    end;
    i:=i+2;
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
    TD2VisualObject(FSelections[i]).Visible := true; //влючим очередной маркер
      with SelRects[i] do
        TD2VisualObject(FSelections[i]).SetBounds(Left, Top, Right - Left, Bottom - Top); //установим границы маркера как у соотв. прямоугольника
      if (FFocus <> nil) then
      begin
        if FShowSelectedCell and               // если разрешено выделять ячейку
           IsOneRowSelected and                // и выделена только 1 строка
           (FocusedColumn > NoColumn) then       // и есть выделенная колонка
          begin
	    FFocus.Visible:=true;
	    FFocus.BringToFront;
	    P:=d2Point(Columns[FocusedColumn].Position.X,0);
	    P:=FContent.LocalToAbsolute(P);
	    if (FSelection.Parent <> nil) and FSelection.Parent.IsVisual then
	      P:=FSelection.Parent.Visual.AbsoluteToLocal(P);
            if FShowHorzLines then b:=1 else b:=0;
            if (FShowVertLines) and (FocusedColumn>0) then l:=1 else l:=0;
            with SelRects[i] do                                                                  //установить координаты маркера ячейки
              FFocus.SetBounds(P.X + FFocus.Padding.Left+l, SelRects[i].Top + FFocus.Padding.Top,
                              Columns[FFocusedColumn].Width - (FFocus.Padding.Left + FFocus.Padding.Right)-l,
                              SelRects[i].Bottom - SelRects[i].Top - (FFocus.Padding.Top + FFocus.Padding.Bottom)-b);
          end
        else FFocus.Visible := false;  // иначе скрыть маркер ячейки
    end;
  end;
  //установим маркер пред.выбора
  if FPreSelection <> nil then
    begin
      if FIsPreSelected then
        begin
          FPreSelection.Visible:=true;
          FPreSelection.BringToFront;
          P := d2Point(0, FSelected * FRowHeight);
          P := FContent.LocalToAbsolute(P);
          if (FPreSelection.Parent <> nil) and FPreSelection.Parent.IsVisual
            then P := FPreSelection.Parent.Visual.AbsoluteToLocal(P);
          if FContent.Width < ClientWidth
            then R := d2Rect(P.X, P.Y, P.X + ClientWidth, P.Y + FRowHeight)
            else R := d2Rect(P.X, P.Y, P.X + FContent.Width, P.Y + FRowHeight);
          FPreSelection.SetBounds(R.Left, R.Top, R.Right - R.Left, R.Bottom - R.Top); //установим границы маркера пред.выбора
        end
        else  FPreSelection.Visible:=false;
    end;

  for i := 0 to ColumnCount - 1 do   //обновить колонки
    Columns[i].UpdateSelected;
end;

procedure TD2CustomGrid.DoRealignItem(Sender:TObject; OldIndex,NewIndex:integer);
begin
  if FocusedColumn=Columns[OldIndex].Index then
    FocusedColumn:=NewIndex;
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
  FHeader.FOffset := -HScrollBar.Value;  //установить сдвиг заголовка
  c:=FHeader.ItemsCount;                 //кол-во полей в заголовке
  if c < ColumnCount then                //добавим поля в заголовок, если не хватает
    for i := c to ColumnCount - 1 do
    begin
      Item:=TD2HeaderItem.Create(Self);
      Item.Parent:=FHeader;
      Item.Locked:=true;
      Item.Stored:=false;
    end;
  if (c > ColumnCount) and (FHeader.FContent <> nil) then  //удалим лишние поля из заголовка
    for i := ColumnCount to c - 1 do
        begin
          Item := FHeader.GetItem(i);
          FHeader.FContent.RemoveObject(Item);
          Item.Free;
        end;

  for i := 0 to ColumnCount - 1 do //обновим ширину и надписи полей заголовка
  begin
    FHeader.Items[i].TextW := Columns[i].Header; //пропишем текст поля заголовка
    FHeader.Items[i].Width := Columns[i].Width;  //установим ширину полея заголовка
  end;
  FHeader.Realign;  //перестроим заголовок
end;

procedure TD2CustomGrid.UpdateColumns;
var
  R:TD2Rect;
  i: integer;
  Y: single;
begin
  if (ContentLayout <> nil) then
  begin
    R:=ContentLayout.LocalRect;
    Y:= GetTopRowY;
    for i:=0 to ColumnCount - 1 do
    begin
      Columns[i].SetBounds(R.Left, Y, Columns[i].Width, d2RectHeight(R) + FVScrollBar.Value - Y);
      Columns[i].UpdateColumn;
      R.Left:=R.Left + Columns[i].Width;
    end;
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

procedure TD2CustomGrid.GetColumnBounds(iColumn: integer; out sLeft, sRight: Single);
// Возвращает левую и правую границу столбца Column. Если Column = NoColumn, то возвращается вся ширина клиетской части грида.
// Returns the left and right bound of the given column. If Column is NoColumn then the entire client width is returned.
begin
  if iColumn <= NoColumn
     then begin
            sLeft := 0;
            sRight := ClientWidth;
          end
     else with GetColumn(iColumn).GetBoundsRect do
          begin
            sLeft:=Left;
            sRight := Right;
            //if UseRightToLeftAlignment then
            //begin
            //  Inc(Left, ComputeRTLOffset(True));
            //  Inc(Right, ComputeRTLOffset(True));
            //end;
          end;
end;

function TD2CustomGrid.GetContentBounds:TD2Rect;
begin
  Result:= LocalRect;
  if FUpdating > 0 then Exit;
  if ContentLayout=nil then Exit;
  Result:= ContentLayout.LocalRect;
  Result.Right:= Result.Left + Max(GetContentWidth, ContentLayout.Width);
  Result.Bottom:= Result.Top + Max(GetContentHeight, ContentLayout.Height);
end;

function TD2CustomGrid.GetContentHeight: Single;
begin
  Result:= Max(0, FRowCount * FRowHeight);
end;

function TD2CustomGrid.GetContentWidth: Single;
var i: integer;
begin
  Result:=0;
  if ColumnCount > 0 then
    for i:=0 to ColumnCount - 1 do
      Result:= Result + Columns[i].Width;
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

function TD2CustomGrid.RowByPoint(const X, Y:single):integer;
begin
  if FContent <> nil
    then Result:=trunc(Y / FRowHeight)
    else Result:=-1;
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

procedure TD2CustomGrid.Realign;
begin
  if csDestroying in ComponentState then Exit;
  if csLoading in ComponentState then Exit;
  if ContentLayout=nil then Exit;
  if FDisableAlign then Exit;
  if FUpdating > 0 then Exit;
  inherited;
  UpdateColumns;
  UpdateHeader;
  UpdateSelection;
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
    if IsOneRowSelected then
    begin
      if (KeyChar <> #0) and not (ReadOnly) and not (Columns[FocusedColumn].ReadOnly) then
      begin
        if (Columns[FocusedColumn] <> nil) and (Columns[FocusedColumn].CellControlByRow(Selected) <> nil) then
        begin
          Columns[FocusedColumn].CellSetFocus(Selected);
          TD2HackVisualObject(Columns[FocusedColumn].CellControlByRow(Selected)).KeyDown(Key,KeyChar,Shift);
        end;
      end;
      case Key of
        VK_F2:
          begin
            if Columns[FocusedColumn] <> nil
               then Columns[FocusedColumn].CellSetFocus(Selected);
          end;
        VK_LEFT:
          begin
            if (Shift = []) and (FocusedColumn > 0)
              then FocusedColumn := FocusedColumn - 1;
            if (Shift = [ssCtrl]) and (FocusedColumn <> 0)
              then FocusedColumn := 0;
            Key:=0;
          end;
        VK_RIGHT:
          begin
            if (Shift = []) and (FocusedColumn < ColumnCount - 1)
              then FocusedColumn := FocusedColumn + 1;
            if (Shift = [ssCtrl]) and (FocusedColumn <> ColumnCount - 1)
               then FocusedColumn := ColumnCount - 1;
            Key:=0;
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
      else Exit;
    end;
    Key:=0;
  end;
end;

procedure TD2CustomGrid.MouseDown(Button:TMouseButton; Shift:TShiftState; X, Y:single);
var
  Column:TD2Column;
  P: TD2Point;
begin
  inherited;
  if FContent = nil then exit;
  P:=LocalToAbsolute(d2Point(X,Y));
  P:=FContent.AbsoluteToLocal(P);

  if Button=mbLeft then
  begin
    if (Shift = [ssLeft]) and not IsSelected(P.X, P.Y) //если нажата только ЛКМ и строка еще не выбрана
      then SetSelectedByPoint(P.X, P.Y);                 //то выбрать эту строку
    if Shift = [ssLeft, ssShift] then SetSelectedMoreRowByPoint(P.X, P.Y);          //если нажаты ЛКМ + Shift
    if Shift = [ssLeft, ssShift, ssCtrl] then SetSelectedExtraRowByPoint(P.X, P.Y); //если нажаты ЛКМ + Shift + Ctrl
    //if Shift = [ssLeft, ssCtrl] then ChangeSelectionRowByPoint(P.X, P.Y);           //если нажаты ЛКМ + Ctrl
    Column:=ColumnByPoint(X, Y);
    if Column <> nil then
      FocusedColumn:=Column.Index;
  end;
end;

procedure TD2CustomGrid.MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: single);
//обработка отпускания кнопок мыши
var
  P: TD2Point;
begin
  inherited;
  if FContent = nil then exit;
  P:=LocalToAbsolute(d2Point(X,Y));
  P:=FContent.AbsoluteToLocal(P);

  if Button=mbLeft then     //отпущена ЛКМ
  begin
    if (Shift = []) and IsSelected(P.X, P.Y) //больше ни чего не нажато и строка уже выбрана
      then SetSelectedByPoint(P.X, P.Y);
    if Shift = [ssCtrl]
      then ChangeSelectionRowByPoint(P.X, P.Y);//если отпущена ЛКМ и нажат Ctrl
  end;

end;

procedure TD2CustomGrid.MouseMove(Shift:TShiftState; X,Y,Dx,Dy:single);
var
  Column:TD2Column;
begin
  inherited;
  if FPressed then
  begin
    Column:=ColumnByPoint(X,Y);
    if Column <> nil then
      FocusedColumn:=Column.Index;
  end;
end;

function TD2CustomGrid.ChangeSelectionRow(Idx: integer):boolean;  //Added by GoldenFox
//Инвертировать выделение строки c индексом Idx. Результат: true - строка выделена, false - развыделена
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
      FSelected := Idx;
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
  FSelected := Idx;
  ScrollToRow(Idx);
  UpdateSelection;
end;

function TD2CustomGrid.ChangeSelectionRowByPoint(const X, Y: single): boolean;
begin
  Result:=ChangeSelectionRow(RowByPoint(X, Y));
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

procedure TD2CustomGrid.SetSelectedExtraRow(Idx: integer);
//добавить к выбранным строки начиная с текущей до Idx без очистки ранее выбранных строк
begin
  SetSelectedMoreRow(Idx);
end;

procedure TD2CustomGrid.SetSelectedMoreRowByPoint(const X, Y: single);
//добавить к выбранным строки начиная с текущей до строки с координатами X, Y с очисткой ранее выбранных строк
begin
  SetSelectedMoreRow(RowByPoint(X, Y));
end;

procedure TD2CustomGrid.SetSelectedExtraRowByPoint(const X, Y: single);
//добавить к выбранным строки начиная с текущей до строки с координатами X, Y без очистки ранее выбранных строк
begin
  SetSelectedExtraRow(RowByPoint(X, Y));
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
  if AObject is TD2Column then
    begin
      TD2Column(AObject).FGrid:=TD2Column(AObject).GetGrid;
      if Assigned(FScene) and (FScene.GetDesignTime) then
        begin
          FSelected:=0;
          FOldSelected := FSelected;
          FSelectedRows[0]:=2;
          FSelectedRows[1]:=FSelected;
          FSelectedRows[2]:=FSelected;
          FIsPreSelected:=false;
          UpdateSelection;
        end;
    end;
end;

procedure TD2CustomGrid.ApplyResource;
begin
  if FNeedResource and (FSelections <> nil) then FreeAndNil(FSelections);
  inherited;
end;

procedure TD2CustomGrid.ScrollToRow(ARow: integer);
begin
  if ARow <= TopRow then VScrollBar.Value := ARow * FRowHeight;
  if ARow * FRowHeight > VScrollBar.Value + ClientHeight - FRowHeight
    then VScrollBar.Value := ARow * FRowHeight - ClientHeight + FRowHeight;
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
//сделать выбранной строку с номером Value
begin
  if (FSelected <> Value) or
     (FSelectedRows[0] <> 2) or
     (FSelectedRows[1] <> FSelectedRows[2]) then
  begin
    FOldSelected := FSelected;
    FSelected:=Value;
    if FSelected < 0 then FSelected:=0;
    if FSelected > FRowCount - 1 then FSelected:=FRowCount - 1;
    FSelectedRows[0]:=2;
    FSelectedRows[1]:=FSelected;
    FSelectedRows[2]:=FSelected;
    FIsPreSelected:=false;
    ScrollToRow(FSelected);
    UpdateSelection;
  end;
end;

procedure TD2CustomGrid.SetSelectedByPoint(const X, Y: single);
//сделать выбранной строку по координатам X, Y
begin
  SetSelected(RowByPoint(X, Y)); //выбрать строку
end;

function TD2CustomGrid.IsSelected(Row:integer):boolean; //проверить выбрана строка или нет
var i: integer;
begin
  if FSelectedRows[0] < 2
    then begin
           Result := false;
           exit;
         end;
  Result := true;
  i:=1;
  while i < FSelectedRows[0] do
    if (FSelectedRows[i] <= Row) and (Row <=FSelectedRows[i+1])
      then exit
      else i:=i+2;
  Result := false;
end;

function TD2CustomGrid.IsSelected(const X, Y: single): boolean;
begin
  Result:=IsSelected(RowByPoint(X, Y));
end;

function TD2CustomGrid.IsOneRowSelected: boolean;
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

procedure TD2CustomGrid.SetMultiSelect(const Value: boolean);
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

procedure TD2CustomGrid.DblClick;
begin
  inherited DblClick;
  if Columns[FocusedColumn] <> nil
    then Columns[FocusedColumn].CellSetFocus(Selected);
end;

procedure TD2CustomGrid.ContentRemoveObject(AObject: TD2Object);
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

procedure TD2CustomGrid.SetFocusedColumn(const Value:integer);
begin
  if FFocusedColumn <> Value then
  begin
    FFocusedColumn:=Value;
    UpdateSelection;
    if Columns[FFocusedColumn].Position.X < HScrollBar.Value then
      HScrollBar.Value:=Columns[FFocusedColumn].Position.X;
    if Columns[FFocusedColumn].Position.X + Columns[FFocusedColumn].Width > HScrollBar.Value + ClientWidth then
      HScrollBar.Value:=Columns[FFocusedColumn].Position.X + Columns[FFocusedColumn].Width - ClientWidth;
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
