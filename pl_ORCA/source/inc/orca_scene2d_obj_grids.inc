{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2HeaderItem ==================================================
{
//Deleted by GoldenFox
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
}

//----------- Block added by GoldenFox -----------
constructor TD2HeaderItem.Create(AOwner:TComponent);
begin
  inherited;
  CanFocused:=false;
  FxRadius:=0;
  FyRadius:=0;
  FIsSortedUp:=false;        //true - сортировка от меньшего к большему
  FIsSortedDown:=false;      //true - сортировка от большего к меньшему
end;

procedure TD2HeaderItem.SetIsSortedDown(AValue: boolean);
begin
  if FIsSortedDown=AValue then Exit;
  FIsSortedDown:=AValue;
  if AValue
    then IsSortedUp:=not AValue;
  StartTriggerAnimation(Self,'IsSortedDown');
end;

procedure TD2HeaderItem.SetIsSortedUp(AValue: boolean);
begin
  if FIsSortedUp=AValue then Exit;
  FIsSortedUp:=AValue;
  if AValue
    then IsSortedDown:=not AValue;
  StartTriggerAnimation(Self,'IsSortedUp');
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
//-------- End block added by GoldenFox ----------

procedure TD2HeaderItem.DoSplitterMouseMove(Sender:TObject;
  Shift:TShiftState; X,Y,Dx,Dy:single);
var
  NewSize:single;
begin
  if TD2HackVisualObject(FSplitter).FPressed then
  begin
    NewSize:=AbsoluteToLocal(FSplitter.LocalToAbsolute(d2Point(X,Y))).X;
    if NewSize < 0 then NewSize:=0;
	{
	//Deleted by GoldenFox
	if (Parent <> nil) and (Parent is TD2Header) then
    begin
      if Assigned(TD2Header(Parent).OnResizeItem) then
        TD2Header(Parent).OnResizeItem(Self,NewSize);
    end;
    Width:=NewSize;
    if (Parent <> nil) and (Parent is TD2Header) then
      TD2Header(Parent).Realign;
	}
	//----------- Block added by GoldenFox -----------
    if (Header <> nil) and (Assigned(Header.OnResizeItem))  //если есть заголовок и у него установлен обраотчик прерывания изменения размера
      then Header.OnResizeItem(Self, NewSize);           //то вызвать прерывание
    Width := NewSize;                             //установить новый размер после обработки прерывания
    if Header <> nil then Header.Realign;              //перестроить заголовок если есть
	//-------- End block added by GoldenFox ----------
  end;
end;

{	
//Deleted by GoldenFox
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
}
															//----------- Block added by GoldenFox -----------			  
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
  FIsPositioning:=true;                        //начат процесс изменения позиции
  BringToFront;              //текущее поле переместим вверх
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
  Index:=FNewIndex;
  H:=Header;
  if H <> nil then
  begin
    //H.Realign;  //вызывается при присвоении индекса
    if Assigned(H.OnRealignItem)                                  //если у заголовка определено прерывание по изменению порядка полей
      then H.OnRealignItem(TD2Object(Self), FOldIndex, FNewIndex);  //то вызвать прерывание (эл-т, Старый индекс, новый индекс)
  end;
  inherited;
  FIsPositioning:=false;                        //закончен процесс изменения позиции
end;

function TD2HeaderItem.Header:TD2Header;	//function Added by GoldenFox
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

procedure TD2HeaderItem.Click;
begin
  if not FIsPositioning and Assigned(Header.FGrid.FOnHeaderClick) //если закончен процесс изменения позиции
    then Header.FGrid.FOnHeaderClick(Self);
  inherited;
end;
procedure TD2HeaderItem.DblClick;	//procedure Added by GoldenFox
begin
  if not FIsPositioning and Assigned(Header.FGrid.FOnHeaderDblClick) //если закончен процесс изменения позиции
    then Header.FGrid.FOnHeaderDblClick(Self);
  inherited;
end;
//-------- End block added by GoldenFox ----------

//=============== TD2Header ===================================================

constructor TD2Header.Create(AOwner:TComponent);
begin
  inherited;
  ClipChildren:=true;
  {
  //Deleted by GoldenFox
  FSides:=AllSides;	
  FLastItem:=TD2HeaderItem.Create(Self);
  FLastItem.Parent:=Self;
  FLastItem.Stored:=false;
  FLastItem.Locked:=true;
  FLastItem.Width:=50;
  FLastItem.DragMode:=d2DragManual;
  }
  //----------- Block added by GoldenFox -----------
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
  //-------- End block added by GoldenFox ----------
end;

function TD2Header.GetItem(Index: integer): TD2HeaderItem;
begin
  //Result:=TD2HeaderItem(Children[Index]);	//Deleted by GoldenFox
  if FContent<>nil	//Added by GoldenFox
    then Result := TD2HeaderItem(FContent.Children[Index])	//Added by GoldenFox
    else Result := nil;	//Added by GoldenFox
end;

function TD2Header.ItemClass: string;
begin
  Result := 'TD2HeaderItem';
end;

{
// Deleted by GoldenFox
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
}

//----------- Block added by GoldenFox -----------
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
//-------- End block added by GoldenFox ----------

procedure TD2Header.Realign;
var
  i:integer;
  R:TD2Rect;

  procedure DoPosition(Control:TD2VisualObject);
  begin
    if not Control.Visible then Exit;
    with R do
    begin
      //Control.SetBounds(Left,Top,Control.Width,Height);	//Deleted by GoldenFox
	  Control.SetBounds(Left, Top, Control.Width, Bottom-Top);	//Added by GoldenFox
	  //if Control <> FLastItem then  	//Deleted by GoldenFox
      Left:=Left + Control.Width;
    end;
  end;
  
{																//Deleted by GoldenFox	
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
  }		
  
begin
  //inherited ; 	//Deleted by GoldenFox
  if csDestroying in ComponentState then Exit;
  if FDisableAlign then Exit;
  inherited;	//Added by GoldenFox
  //if ChildrenCount=0 then Exit;	//Deleted by GoldenFox
  if ItemsCount <= 0 then Exit;	//Added by GoldenFox
  FDisableAlign := true;
  try
	//FChildren.Remove(FLastItem);	//Deleted by GoldenFox
    //FChildren.Add(FLastItem);		//Deleted by GoldenFox
    R := d2Rect(FOffset, 0, FWidth, FHeight);
	{
	//Deleted by GoldenFox
	R:=Margins.MarginRect(R);
    DoAlign;
    if R.Left > R.Right then
      FLastItem.Width:=0
    else
      FLastItem.Width:=R.Right - R.Left;x
	}
	//----------- Block added by GoldenFox -----------
	for i := 0 to ItemsCount - 1 do          //для всех полей заголовка
    begin
      DoPosition(TD2VisualObject(Items[i]));   //позиционируем поле согласно индекса
      Items[i].Sides := [d2SideRight];         //включаем только правую границу полей  
    end;
	//-------- End block added by GoldenFox ----------
  finally
    FDisableAlign:=false;
  end;
end;

{
// Deleted by GoldenFox
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
}

//=============== TD2Column =========================================

constructor TD2Column.Create(AOwner:TComponent);
begin
  inherited;
  FAutoTranslate:=true;
  Width:=100;
  HitTest:=false;
  CanFocused:=false;
  FGrid:= GetGrid;				//Added by GoldenFox
  FCellControls:= TList.Create;	//Added by GoldenFox
  FClipChildren:= true;			//Added by GoldenFox
end;

//----------- Block added by GoldenFox -----------
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
//-------- End block added by GoldenFox ----------

function TD2Column.CreateCellControl:TD2Control;
begin
  //Result:=TD2TextCell.Create(Self);				//Deleted by GoldenFox
  //TD2TextCell(Result).OnChange:=DoTextChanged;	//Deleted by GoldenFox 
//----------- Block added by GoldenFox -----------  
  Result:=GetCellClass.Create(Self);
  Result.Parent:=Self;
  Result.HitTest:=false;
  Result.Visible:=false;
  Result.Locked:=true;
  Result.Stored:=false;
  Result.CanFocused:=false;
  Result.OnCanFocused:=DoCanFocused;
  Result.OnEnterFocus:=DoEnterFocus;
  Result.OnKillFocus:=DoKillFocus;
  Result.OnKeyDown:=DoKeyDown;
//-------- End block added by GoldenFox ----------
end;

{
//Deleted by GoldenFox
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
}

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
  //Grid.ColumnIndex:=Index;	//Deleted by GoldenFox
  Grid.FocusedColumn:=Index;	//Added by GoldenFox
  FSaveData:=TD2Object(Sender).Data;
end;

//----------- Block added by GoldenFox -----------
procedure TD2Column.DoKillFocus(Sender: TObject);
begin
  if TD2Object(Sender).IsVisual then
    with TD2VisualObject(Sender) do
    begin
      CanFocused:=false;
      HitTest:=false;
    end;
end;
//-------- End block added by GoldenFox ----------

procedure TD2Column.DoKeyDown(Sender:TObject; var Key:Word; var KeyChar:System.WideChar;
  Shift:TShiftState);
begin
  if (KeyChar <> #0) and (Grid <> nil) and not Grid.CanEditAcceptKey(KeyChar) then
    KeyChar:=#0;
  if (Key=VK_RETURN) and (Grid <> nil) then
  begin					//Added by GoldenFox
    Key:=VK_UNKNOWN;	//Added by GoldenFox
    Grid.SetFocus;
  end;					//Added by GoldenFox
  if (Key=VK_ESCAPE) and (Grid <> nil) then
  begin
    FDisableChange:=true;
    try
      TD2Object(Sender).Data:=FSaveData;
    finally
      FDisableChange:=false;
    end;
    Key:=VK_UNKNOWN;	//Added by GoldenFox
    Grid.Reset;
  end;
  if (Key=VK_UP) or (Key=VK_DOWN) and (Grid <> nil) then
  begin
    Grid.SetFocus;
    Grid.KeyDown(Key,Keychar,Shift);
  end;
end;

//----------- Block added by GoldenFox -----------
procedure TD2Column.CellSetFocus(ARow: integer);
begin
  //виртуальный метод определяется в потомках
end;

procedure TD2Column.SetWidth(const Value: single);
begin
  inherited SetWidth(Value);
  if Grid<>nil then Grid.Realign;
end;

function TD2Column.GetCellClass: TD2CellClass;
begin
  Result:=TD2Control;
end;
//-------- End block added by GoldenFox ----------

//function TD2Column.Grid:TD2CustomGrid;	//Deleted by GoldenFox
function TD2Column.GetGrid: TD2CustomGrid;	//Added by GoldenFox
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
  {
  //Deleted by GoldenFox
  for i:=0 to High(FCellControls) do  
    FCellControls[i].Free;    
  SetLength(FCellControls,0);
  }
  //----------- Block added by GoldenFox -----------
  for i:=0 to FCellControls.Count - 1 do
    TD2Control(FCellControls.Items[i]).Free;
  FCellControls.Clear;
  //-------- End block added by GoldenFox ----------
end;

function TD2Column.CellControlByPoint(X,Y:single):TD2Control;
var
  i:integer;
  P:TD2Point;
begin
  Result:=nil;
  if Grid=nil then Exit;

  P:=Grid.LocalToAbsolute(d2Point(X,Y));
  {
  //Deleted by GoldenFox
  for i:=0 to High(FCellControls) do		
    if FCellControls[i].Visible and (FCellControls[i].pointInObject(P.X,P.Y)) then
    begin
      Result:=FCellControls[i];
  	    Exit;
      end;
  }
  //----------- Block added by GoldenFox -----------
  for i:=0 to FCellControls.Count - 1 do
    with TD2Control(FCellControls.Items[i]) do
	  if Visible and (pointInObject(P.X,P.Y)) then
      begin
        Result:=TD2Control(FCellControls.Items[i]);
        Exit;
      end;
  //-------- End block added by GoldenFox ----------
end;

function TD2Column.CellControlByRow(Row:integer):TD2Control;
var
  i:integer;
begin
  Result:=nil;
  if Grid=nil then Exit;
  {
  //Deleted by GoldenFox
  for i:=0 to High(FCellControls) do	
    if (FCellControls[i].Visible) and 	(trunc(d2StringToPoint(FCellControls[i].TagString).Y)=Row) then
    begin	
      Result:=FCellControls[i];
	  Exit;
    end;
  }
//----------- Block added by GoldenFox -----------
  for i:=0 to FCellControls.Count - 1 do
    with TD2Control(FCellControls.Items[i]) do
      if Visible and (trunc(d2StringToPoint(TagString).Y)=Row) then
      begin
        Result:=TD2Control(FCellControls.Items[i]);
        Exit;
      end;
//-------- End block added by GoldenFox ----------
end;

//----------- Block added by GoldenFox -----------
procedure TD2Column.Realign;	//procedure Added by GoldenFox
begin
  inherited Realign;
  if FGrid<>nil then FGrid.Realign;
end;
//-------- End block added by GoldenFox ----------

procedure TD2Column.UpdateColumn;
var
  i,C:integer;
  m:integer;	//Added by GoldenFox
  //V:Variant;	//Deleted by GoldenFox
begin
  if Grid=nil then Exit;
  FUpdateColumn:=true;
  try
    m:= Min(Grid.RowCount,Grid.VisibleRows);	//Added by GoldenFox
    C:= FCellControls.Count;	//Added by GoldenFox
    { Create controls }
	{
	//Deleted by GoldenFox
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
	}
	//----------- Block added by GoldenFox -----------
	if C < m then 
	begin							   			
	  for i:= C to m - 1 do
	    FCellControls.Add(CreateCellControl);
    end
    else
	//-------- End block added by GoldenFox ----------
    { Hide if need }
	{
	//Deleted by GoldenFox
	if Length(FCellControls) > Min(Grid.RowCount,Grid.VisibleRows) then
    begin
      for i:=Min(Grid.RowCount,Grid.VisibleRows) to High(FCellControls) do
        FCellControls[i].Visible:=false;
    end;
	}
	  for i:=m to FCellControls.Count-1 do	//Added by GoldenFox
	    TD2Control(FCellControls.Items[i]).Visible:=false;	//Added by GoldenFox

    { Update Data }
    //for i:=0 to Min(Grid.RowCount,Grid.VisibleRows) - 1 do	//Deleted by GoldenFox
	for i:=0 to m - 1 do	//Added by GoldenFox
    begin
      if Grid.TopRow + i >= Grid.RowCount then Break;
	  {
	  //Deleted by GoldenFox
	  V:=Grid.GetValue(Index,Grid.TopRow + i);	
      FCellControls[i].Visible:=true;
      FCellControls[i].SetBounds(0,i * Grid.RowHeight,Width,Grid.RowHeight); 
	  FCellControls[i].TagString:=d2PointToString(d2Point(Index,Grid.TopRow + i));	  
	  FCellControls[i].Data:=V;
	  }
	  //----------- Block added by GoldenFox -----------
      TD2Control(FCellControls.Items[i]).Visible:=true;
      TD2Control(FCellControls.Items[i]).SetBounds(0, i * Grid.RowHeight, Width,Grid.RowHeight);
      TD2Control(FCellControls.Items[i]).TagString:=d2PointToString(d2Point(Index,Grid.TopRow + i));
      TD2Control(FCellControls.Items[i]).Data:=Grid.GetValue(Index,Grid.TopRow + i);
	  //-------- End block added by GoldenFox ----------
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
  {
  //Deleted by GoldenFox
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
  }
  //----------- Block added by GoldenFox -----------
  if Assigned(FCellControls) and  (FCellControls.Count > 0) then
  begin
    for i:=0 to Min(Grid.RowCount,Min(FCellControls.Count,Grid.VisibleRows)) - 1 do
      with TD2Control(FCellControls.Items[i]) do
	  begin
	    HitTest:=false;
	    if IsFocused then
	      Grid.SetFocus;
	  end;
  end;
  //-------- End block added by GoldenFox ----------  
end;

//procedure TD2Column.SetHeader(const Value:WideString);	//Deleted by GoldenFox
procedure TD2Column.SetHeader(const Value:String);	//Added by GoldenFox
begin
  if FHeader <> Value then
  begin
    FHeader := Value;
    if (Grid <> nil) and (Grid.FHeader <> nil) then
       Grid.FHeader.Items[Index].TextW := FHeader;
  end;
end;

//----------- Block added by GoldenFox -----------

//=============== TD2TextColumn ================================================
function TD2TextColumn.CreateCellControl:TD2Control;
begin
  Result:=inherited;
  TD2TextCell(Result).OnChange:=DoTextChanged;
end;

function TD2TextColumn.GetCellClass: TD2CellClass;
begin
  Result:=TD2TextCell;
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
//-------- End block added by GoldenFox ----------

//=============== TD2CheckColumn ================================================

{
// Deleted by GoldenFox
constructor TD2CheckColumn.Create(AOwner:TComponent);	
begin
  inherited;
end;
}

function TD2CheckColumn.CreateCellControl:TD2Control;
begin
  //Result:=TD2CheckCell.Create(Self);	//Deleted by GoldenFox
  Result:=inherited;	//Added by GoldenFox
  TD2CheckCell(Result).OnChange:=DoCheckChanged;
end;

procedure TD2CheckColumn.DoCheckChanged(Sender:TObject);
begin
  if Grid=nil then Exit;
  if FUpdateColumn then Exit;
  if FDisableChange then Exit;	//Added by GoldenFox
  with d2StringToPoint(TD2Object(Sender).TagString) do
  begin
    Grid.SetValue(trunc(x),trunc(y),TD2Control(Sender).Data);
    if Assigned(Grid.FOnEdititingDone) then
      Grid.FOnEdititingDone(Grid,trunc(x),trunc(y));
  end;
end;

//----------- Block added by GoldenFox -----------
function TD2CheckColumn.GetCellClass: TD2CellClass;
begin
  Result:=TD2CheckCell;
end;
//-------- End block added by GoldenFox ----------

//=============== TD2ProgressColumn ============================================

constructor TD2ProgressColumn.Create(AOwner:TComponent);
begin
  inherited;
  FMax:=100;
end;

function TD2ProgressColumn.CreateCellControl:TD2Control;
begin
  //Result:=TD2ProgressCell.Create(Self);	//Deleted by GoldenFox
  Result:=inherited;	//Added by GoldenFox
  TD2ProgressCell(Result).Min:=FMin;
  TD2ProgressCell(Result).Max:=FMax;
end;

//----------- Block added by GoldenFox -----------
function TD2ProgressColumn.GetCellClass: TD2CellClass;
begin
  Result:=TD2ProgressCell;
end;
//-------- End block added by GoldenFox ----------

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
  //Result:=TD2PopupCell.Create(Self);	//Deleted by GoldenFox
  Result:=inherited;	//Added by GoldenFox
  TD2PopupCell(Result).Items.Assign(FItems);
end;

//----------- Block added by GoldenFox -----------
function TD2PopupColumn.GetCellClass: TD2CellClass;
begin
  Result:=TD2PopupCell;
end;
//-------- End block added by GoldenFox ----------

procedure TD2PopupColumn.SetItems(const Value:TD2WideStrings);
begin
  FItems.Assign(Value);
end;

//=============== TD2ImageColumn =============================================
{
// Deleted by GoldenFox
constructor TD2ImageColumn.Create(AOwner:TComponent);	
begin
  inherited;											   
end;
}

function TD2ImageColumn.CreateCellControl:TD2Control;
begin
  //Result:=TD2ImageCell.Create(Self);	//Deleted by GoldenFox
  Result:=inherited;	//Added by GoldenFox
  TD2ImageCell(Result).EnableOpenDialog:=false;
end;

//----------- Block added by GoldenFox -----------
function TD2ImageColumn.GetCellClass: TD2CellClass;	
begin
  Result:=TD2ImageCell;
end;
//-------- End block added by GoldenFox ----------

//=============== TD2CustomGrid ==============================================

constructor TD2CustomGrid.Create(AOwner:TComponent);
begin
  inherited;
  FScrollDuration:=0.3;	//Added by GoldenFox
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
  //Width:=100;	//Deleted by GoldenFox
  Width:=200;	//Added by GoldenFox
  Height:=100;
  //FItemHeight:=0;	//Deleted by GoldenFox
  //----------- Block added by GoldenFox -----------
  SetLength(FSelectedRows, 128);
  FSelected := 0;
  FOldSelected := FSelected;
  FSelectedRows[0] := 2;
  FSelectedRows[1] := FSelected;
  FSelectedRows[2] := FSelected;
  FIsPreSelected := false;
  FSortColumn := NoColumn;
  FSortDirection := sdAscending;
  //-------- End block added by GoldenFox ----------
end;

destructor TD2CustomGrid.Destroy;
var i: integer;	//Added by GoldenFox
begin
  if FSelections <> nil then
  begin									//Added by GoldenFox
    for i:=0 to FSelections.Count-1 do	//Added by GoldenFox
      TObject(FSelections[i]).Free;		//Added by GoldenFox
    FSelections.Free;
  end;									//Added by GoldenFox
  FLineFill.Free;
  FOddFill.Free;
  inherited;
end;

//----------- Block added by GoldenFox -----------
procedure TD2CustomGrid.ApplyResource;
begin
  if FNeedResource and (FSelections <> nil) then FreeAndNil(FSelections);
  inherited;
end;
//-------- End block added by GoldenFox ----------

function TD2CustomGrid.ItemClass:string;
begin
  //Result:='TD2Column;TD2CheckColumn;TD2ProgressColumn;TD2PopupColumn;TD2ImageColumn';	//Deleted by GoldenFox
  Result:='TD2TextColumn;TD2CheckColumn;TD2ProgressColumn;TD2PopupColumn;TD2ImageColumn';	//Added by GoldenFox
end;

procedure TD2CustomGrid.FreeStyle;
begin
  //inherited;	//Deleted by GoldenFox			
  FSelection:=nil;
  FPreSelection := nil;	//Added by GoldenFox
  FFocus:=nil;
  FHeader:=nil;
  inherited;	//Added by GoldenFox
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
    FHeader.FGrid := Self; 		//Added by GoldenFox
  end;
  T:=FindResource('selection');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    FSelection:=TD2VisualObject(T);
    FSelection.Visible:=false;
  end;
  //----------- Block added by GoldenFox -----------
  T := FindResource('preselection');
  if (T <> nil) and (T is TD2VisualObject) then
  begin
    FPreSelection := TD2VisualObject(T);
    FPreSelection.Visible := false;
  end;
  //-------- End block added by GoldenFox ----------
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
  //UpdateColumns;		//Deleted by GoldenFox
  //UpdateHeader;		//Deleted by GoldenFox
  //UpdateSelection;	//Deleted by GoldenFox
  Realign;			//Added by GoldenFox
end;

procedure TD2CustomGrid.UpdateSelection;   //обновить маркеры выбора строк
var
  i:integer;
  l,b: single;	//Added by GoldenFox
  P:TD2Point;
  R:TD2Rect;
  //Sel:boolean;	//Deleted by GoldenFox
  SelRects:array of TD2Rect;
  Clone:TD2VisualObject;
  //Vis:boolean;	//Deleted by GoldenFox			  
begin
  if FSelection=nil then Exit;
  //if ColumnCount=0 then Exit;	//Deleted by GoldenFox
  //if RowCount=0 then	//Deleted by GoldenFox
  if (RowCount=0) or (ColumnCount=0) then	//Added by GoldenFox
  begin
    FSelectedRows[0]:=0;			//обнулить длину массива выделенных строк	//Added by GoldenFox
    if FSelections <> nil then
        for i:=0 to FSelections.Count - 1 do
          TD2VisualObject(FSelections[i]).Visible:=false;	//сркыть все маркеры выделения
    if FPreSelection <> nil then FPreSelection.Visible := false;  //скрыть маркер предвыделения	//Added by GoldenFox
    if FFocus <> nil then
      FFocus.Visible:=false;
    Exit;
  end;
  // calc rects  делаем массив прямоугольников
  //Vis:=true;	//Deleted by GoldenFox
  //Sel:=false; //Deleted by GoldenFox
  SetLength(SelRects,0);
  //for i:=0 to RowCount - 1 do	//Deleted by GoldenFox
  i := 1;	//Added by GoldenFox
  while (i <= FSelectedRows[0]) do	//Added by GoldenFox
  begin
    //if IsSelected(i) then	//Deleted by GoldenFox
    if (FSelectedRows[i] <= FSelectedRows[i+1]) and	//Added by GoldenFox
       (FSelectedRows[i+1] < RowCount) then			//Added by GoldenFox
    begin
	  //P:=d2Point(0,i * FRowHeight);	//Deleted by GoldenFox
      P := d2Point(0, FSelectedRows[i] * FRowHeight);  //верхняя левая точка	//Added by GoldenFox
      P := FContent.LocalToAbsolute(P);                //преобразуем в координаты контента
      if (FSelection.Parent <> nil) and FSelection.Parent.IsVisual then
        P := FSelection.Parent.Visual.AbsoluteToLocal(P); //преобразуем в координаты владельца FSelection
      if FContent.Width < ClientWidth then
	    //R:=d2Rect(P.X,P.Y,P.X + ClientWidth,P.Y + FRowHeight)	//Deleted by GoldenFox
        R:= d2Rect(P.X, P.Y, P.X + ClientWidth,    P.Y + FRowHeight*(FSelectedRows[i+1] - FSelectedRows[i] + 1) )	//Added by GoldenFox		  
      else 
		//R:=d2Rect(P.X,P.Y,P.X + FContent.Width,P.Y + FRowHeight);	//Deleted by GoldenFox
		R:= d2Rect(P.X, P.Y, P.X + FContent.Width, P.Y + FRowHeight*(FSelectedRows[i+1] - FSelectedRows[i] + 1) );	//Added by GoldenFox

      //if (Length(SelRects) > 0) and (i > 0) and (IsSelected(i - 1)) //then	//Deleted by GoldenFox
      //  SelRects[High(SelRects)]:=d2UnionRect(R,SelRects[High(SelRects)])	//Deleted by GoldenFox
      //else		//Deleted by GoldenFox
      //begin		//Deleted by GoldenFox
      SetLength(SelRects,Length(SelRects) + 1);
      SelRects[High(SelRects)]:=R;
	  //end;		//Deleted by GoldenFox
	  //Sel:=true;	//Deleted by GoldenFox
    end;
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
	TD2VisualObject(FSelections[i]).Visible := true; //влючим очередной маркер	//Added by GoldenFox
	//if Vis then	//Deleted by GoldenFox
    //begin		   	//Deleted by GoldenFox
      with SelRects[i] do
        TD2VisualObject(FSelections[i]).SetBounds(Left, Top, Right - Left, Bottom - Top); //установим границы маркера как у соотв. прямоугольника
      if (FFocus <> nil) then
      begin
        if FShowSelectedCell and               // если разрешено выделять ячейку	//Added by GoldenFox
           IsOneRowSelected and                // и выделена только 1 строка	//Added by GoldenFox
           (FocusedColumn > NoColumn) then       // и есть выделенная колонка	//Added by GoldenFox
          begin
	    FFocus.Visible:=true;
	    FFocus.BringToFront;
		//P:=d2Point(Columns[ColumnIndex].Position.X,0);	//Deleted by GoldenFox
	    P:=d2Point(Columns[FocusedColumn].Position.X,0);	//Added by GoldenFox
	    P:=FContent.LocalToAbsolute(P);
	    if (FSelection.Parent <> nil) and FSelection.Parent.IsVisual then
	      P:=FSelection.Parent.Visual.AbsoluteToLocal(P);
            if FShowHorzLines then b:=1 else b:=0;							//Added by GoldenFox
            if (FShowVertLines) and (FocusedColumn>0) then l:=1 else l:=0;	//Added by GoldenFox
            with SelRects[i] do 	//установить координаты маркера ячейки
			  //FFocus.SetBounds(P.X + FFocus.Padding.Left,SelRects[i].Top + FFocus.Padding.Top,	//Deleted by GoldenFox
              //Columns[FColumnIndex].Width - (FFocus.Padding.Left + FFocus.Padding.Right),	//Deleted by GoldenFox
              //SelRects[i].Bottom - SelRects[i].Top - (FFocus.Padding.Top + FFocus.Padding.Bottom));	//Deleted by GoldenFox
			  FFocus.SetBounds(P.X + FFocus.Padding.Left+l, SelRects[i].Top + FFocus.Padding.Top,						//Added by GoldenFox
                              Columns[FFocusedColumn].Width - (FFocus.Padding.Left + FFocus.Padding.Right)-l,			//Added by GoldenFox
                              SelRects[i].Bottom - SelRects[i].Top - (FFocus.Padding.Top + FFocus.Padding.Bottom)-b);	//Added by GoldenFox
          end								//Added by GoldenFox
        else FFocus.Visible := false;  // иначе скрыть маркер ячейки	//Added by GoldenFox
    end;
  end;
  //----------- Block added by GoldenFox -----------
  //установим маркер пред.выбора
  if FPreSelection <> nil then
  begin
    if FIsPreSelected then
	begin
	  FPreSelection.Visible:=true;
	  FPreSelection.BringToFront;
	  P := d2Point(0, FSelected * FRowHeight);
	  P := FContent.LocalToAbsolute(P);
	  if (FPreSelection.Parent <> nil) and FPreSelection.Parent.IsVisual then 
	    P := FPreSelection.Parent.Visual.AbsoluteToLocal(P);
	  if FContent.Width < ClientWidth then 
	    R := d2Rect(P.X, P.Y, P.X + ClientWidth, P.Y + FRowHeight)
	  else 
	    R := d2Rect(P.X, P.Y, P.X + FContent.Width, P.Y + FRowHeight);
	  FPreSelection.SetBounds(R.Left, R.Top, R.Right - R.Left, R.Bottom - R.Top); 	//установим границы маркера пред.выбора
	end	
	else FPreSelection.Visible:=false;	
//-------- End block added by GoldenFox ----------
  end;

  for i := 0 to ColumnCount - 1 do   //обновить колонки
    Columns[i].UpdateSelected;
end;

procedure TD2CustomGrid.DoRealignItem(Sender:TObject; OldIndex,NewIndex:integer);
begin
  //if ColumnIndex=Columns[OldIndex].Index then	//Deleted by GoldenFox
  //  ColumnIndex:=NewIndex;	//Deleted by GoldenFox
//----------- Block added by GoldenFox -----------
  if FocusedColumn=OldIndex then 
    FocusedColumn:=NewIndex
  else 
    if (OldIndex<FocusedColumn) and (FocusedColumn<=NewIndex) then 
	  FocusedColumn:=FocusedColumn-1
    else 
	  if (OldIndex>FocusedColumn) and (FocusedColumn>=NewIndex) then 
	    FocusedColumn:=FocusedColumn+1;

  if SortColumn=OldIndex then 
    SortColumn:=NewIndex
  else 
    if (OldIndex<SortColumn) and (SortColumn<=NewIndex) then 
	  SortColumn:=SortColumn-1
    else 
	  if (OldIndex>SortColumn) and (SortColumn>=NewIndex) then 
	    SortColumn:=SortColumn+1;
//-------- End block added by GoldenFox ----------
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
  c: integer;	//Added by GoldenFox
  Item: TD2HeaderItem;
  b: boolean;	//Added by GoldenFox
begin
  if FHeader = nil then Exit; //если нет заголовка, то выйти
  FHeader.FOffset:=-HScrollBar.Value;  //установить сдвиг заголовка
  c:=FHeader.ItemsCount;	//Added by GoldenFox                 //кол-во полей в заголовке
  if c < ColumnCount then	//Added by GoldenFox                //добавим поля в заголовок, если не хватает
    //for i:=FHeader.ChildrenCount to ColumnCount - 1 do	//Deleted by GoldenFox
	for i := c to ColumnCount - 1 do	//Added by GoldenFox
    begin
      Item:=TD2HeaderItem.Create(Self);
      Item.Parent:=FHeader;
      Item.Locked:=true;
      Item.Stored:=false;
    end;
  //FHeader.FChildren.Add(FHeader.FLastItem);	//Deleted by GoldenFox
  //----------- Block added by GoldenFox -----------
  if (c > ColumnCount) and (FHeader.FContent <> nil) then  //удалим лишние поля из заголовка
    for i := ColumnCount to c - 1 do
	begin
	  Item := FHeader.GetItem(i);
	  FHeader.FContent.RemoveObject(Item);
	  Item.Free;
	end;
  b:=false;
  //-------- End block added by GoldenFox ----------
  
  for i := 0 to ColumnCount - 1 do //обновим ширину и надписи полей заголовка
  begin
  {
  //Deleted by GoldenFox
    TD2HeaderItem(FHeader.Children[i]).TextW:=Columns[i].Header;
    TD2HeaderItem(FHeader.Children[i]).Width:=Columns[i].Width;
  end;
  if FHeader <> nil then
    FHeader.Realign;
}		
  //----------- Block added by GoldenFox -----------
	FHeader.Items[i].TextW := Columns[i].Header; //пропишем текст поля заголовка
    FHeader.Items[i].Width := Columns[i].Width;  //установим ширину поля заголовка
    if i=FSortColumn then
	begin
	  case FSortDirection of
		sdAscending:
		  if (FHeader.Items[i].IsSortedUp<>true) or
			 (FHeader.Items[i].IsSortedDown<>false) then
  		  begin
			FHeader.Items[i].IsSortedUp:=true;
			FHeader.Items[i].IsSortedDown:=false;
			b:=true;
		  end;
		sdDescending:
		  if (FHeader.Items[i].IsSortedUp<>false) or
			 (FHeader.Items[i].IsSortedDown<>true) then
		  begin
			FHeader.Items[i].IsSortedUp:=false;
			FHeader.Items[i].IsSortedDown:=true;
			b:=true;
		  end;
	  end;
	end	
    else 
	  if (FHeader.Items[i].IsSortedUp<>false) or
		  (FHeader.Items[i].IsSortedDown<>false) then 
	  begin
		FHeader.Items[i].IsSortedUp:=false;
		FHeader.Items[i].IsSortedDown:=false;
		b:=true;
	  end;
  end;
  if b	//если требуется пересортировка,
    then SortGrid        //то запустим сортировку с полным пререстроением грида
    else FHeader.Realign;  //иначе перестроим только заголовок
  //-------- End block added by GoldenFox ----------	
end;

procedure TD2CustomGrid.UpdateColumns;
var
  R:TD2Rect;	//Added by GoldenFox
  i: integer;
  Y: single;	//Added by GoldenFox
begin
  if (ContentLayout <> nil) then	//Added by GoldenFox
  begin	//Added by GoldenFox
    R:=ContentLayout.LocalRect;	//Added by GoldenFox
    Y:= GetTopRowY;	//Added by GoldenFox
    for i:=0 to ColumnCount - 1 do
    begin
      //Columns[i].Position.Y:=(TopRow * FRowHeight);	//Deleted by GoldenFox
	  Columns[i].SetBounds(R.Left, Y, Columns[i].Width, d2RectHeight(R) + FVScrollBar.Value - Y);	//Added by GoldenFox
      Columns[i].UpdateColumn;
      R.Left:=R.Left + Columns[i].Width;	//Added by GoldenFox
    end;	//Added by GoldenFox
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

//----------- Block added by GoldenFox -----------
procedure TD2CustomGrid.GetColumnBounds(iColumn: integer; out sLeft, sRight: Single);	//procedure Added by GoldenFox
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
//-------- End block added by GoldenFox ----------

function TD2CustomGrid.GetContentBounds:TD2Rect;
//var				//Deleted by GoldenFox
//  Col,R:TD2Rect;	//Deleted by GoldenFox
//  i,Idx:integer;	//Deleted by GoldenFox
begin
  Result:= LocalRect;
  if FUpdating > 0 then Exit;
  if ContentLayout=nil then Exit;
  {
  //Deleted by GoldenFox
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
  }
  //----------- Block added by GoldenFox -----------
  Result:= ContentLayout.LocalRect;
  Result.Right:= Result.Left + Max(GetContentWidth, ContentLayout.Width);
  Result.Bottom:= Result.Top + Max(GetContentHeight, ContentLayout.Height);
  //-------- End block added by GoldenFox ----------
end;

//----------- Block added by GoldenFox -----------
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
//-------- End block added by GoldenFox ----------

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
{
//Deleted by GoldenFox
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
  }
//----------- Block added by GoldenFox -----------
begin
  if FContent <> nil			   
    then Result:=trunc(Y / FRowHeight)
    else Result:=-1;
//-------- End block added by GoldenFox ----------
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
  //P,P1:TD2Point;	//Deleted by GoldenFox
  P: TD2Point;		//Added by GoldenFox
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

//----------- Block added by GoldenFox -----------
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
//-------- End block added by GoldenFox ----------

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
  {
   //Deleted by GoldenFox
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
  }
  
  //----------- Block added by GoldenFox -----------
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
//-------- End block added by GoldenFox ----------
end;

procedure TD2CustomGrid.MouseDown(Button:TMouseButton; Shift:TShiftState; X, 
  Y:single);
var
  Column:TD2Column;
  P: TD2Point;	//Added by GoldenFox
begin
  inherited;
  if FContent = nil then exit;	//Added by GoldenFox
  P:=LocalToAbsolute(d2Point(X,Y));	//Added by GoldenFox
  P:=FContent.AbsoluteToLocal(P);	//Added by GoldenFox

  if Button=mbLeft then
  begin
    //Selected:=RowByPoint(X,Y);	//Deleted by GoldenFox
	//----------- Block added by GoldenFox -----------
    if (Shift = [ssLeft]) and not IsSelected(P.X, P.Y) //если нажата только ЛКМ и строка еще не выбрана
      then SetSelectedByPoint(P.X, P.Y);                 //то выбрать эту строку
    if Shift = [ssLeft, ssShift] then SetSelectedMoreRowByPoint(P.X, P.Y);          //если нажаты ЛКМ + Shift
    if Shift = [ssLeft, ssShift, ssCtrl] then SetSelectedExtraRowByPoint(P.X, P.Y); //если нажаты ЛКМ + Shift + Ctrl
    if Shift = [ssLeft, ssCtrl] then ChangeSelectionRowByPoint(P.X, P.Y);           //если нажаты ЛКМ + Ctrl
	//-------- End block added by GoldenFox ----------
    Column:=ColumnByPoint(X, Y);
    if Column <> nil then
      //ColumnIndex:=Column.Index;	//Deleted by GoldenFox
	  FocusedColumn:=Column.Index;	//Added by GoldenFox
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
    Column:=ColumnByPoint(X,Y);
    if Column <> nil then
      //ColumnIndex:=Column.Index;	//Deleted by GoldenFox
	  FocusedColumn:=Column.Index;	//Added by GoldenFox
  end;
end;

procedure TD2CustomGrid.MouseUp(Button: TMouseButton; Shift: TShiftState; X, 
  Y: single);
//обработка отпускания кнопок мыши
var				//Added by GoldenFox
  P: TD2Point;	//Added by GoldenFox
begin
  inherited;
  //----------- Block added by GoldenFox -----------
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
  //-------- End block added by GoldenFox ----------
end;

//----------- Block added by GoldenFox -----------
function TD2CustomGrid.ChangeSelectionRow(Idx: integer):boolean;
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

procedure TD2CustomGrid.SetSelectedMoreRow(Idx: integer);  //добавить выделенных строк
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
//-------- End block added by GoldenFox ----------

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
  //----------- Block added by GoldenFox -----------
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
  //-------- End block added by GoldenFox ----------
end;

{
//Deleted by GoldenFox
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
  {
  //Deleted by GoldenFox
  if VScrollBar <> nil then
    Result:=trunc(VScrollBar.Value / FRowHeight)
  else
    Result:=0;
  }
  if FVScrollBar = nil		//Added by GoldenFox
    then Result:=0			//Added by GoldenFox
    else Result:=trunc(FVScrollBar.Value / FRowHeight);	//Added by GoldenFox
end;

//----------- Block added by GoldenFox -----------
function TD2CustomGrid.GetTopRowY: single;
begin
  Result:= TopRow * FRowHeight;
end;

procedure TD2CustomGrid.ScrollToRow(ARow: integer);
begin
  if ARow <= TopRow then VScrollBar.Value := ARow * FRowHeight;
  if ARow * FRowHeight > VScrollBar.Value + ClientHeight - FRowHeight
    then VScrollBar.Value := ARow * FRowHeight - ClientHeight + FRowHeight;
end;
//-------- End block added by GoldenFox ----------

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

//----------- Block added by GoldenFox -----------
procedure TD2CustomGrid.SortGrid;
//отсортировать грид в соответствии со значениями SortColumn и SortDirection
begin
  Realign;
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
//-------- End block added by GoldenFox ----------

procedure TD2CustomGrid.SetSelected(const Value:integer);
//сделать выбранной строку с номером Value
begin
  //if FSelected <> Value then	//Deleted by GoldenFox
  if (FSelected <> Value) or						//Added by GoldenFox
     (FSelectedRows[0] <> 2) or						//Added by GoldenFox
     (FSelectedRows[1] <> FSelectedRows[2]) then	//Added by GoldenFox
  begin
    FOldSelected := FSelected;	//Added by GoldenFox
    FSelected:=Value;
    if FSelected < 0 then FSelected:=0;
    if FSelected > FRowCount - 1 then FSelected:=FRowCount - 1;
	{													//Deleted by GoldenFox						 
	if FSelected <= TopRow then
      VScrollBar.Value:=FSelected * FRowHeight;
    if FSelected * FRowHeight > VScrollBar.Value + ClientHeight - FRowHeight then	
      VScrollBar.Value:=FSelected * FRowHeight - ClientHeight + FRowHeight;	
    }
	//----------- Block added by GoldenFox -----------
	FSelectedRows[0]:=2;			//Added by GoldenFox
    FSelectedRows[1]:=FSelected;	//Added by GoldenFox
    FSelectedRows[2]:=FSelected;	//Added by GoldenFox
    FIsPreSelected:=false;			//Added by GoldenFox
    ScrollToRow(FSelected);			//Added by GoldenFox
	//-------- End block added by GoldenFox ----------
	UpdateSelection;
  end;
end;

//----------- Block added by GoldenFox -----------
procedure TD2CustomGrid.SetSelectedByPoint(const X, Y: single);	//procedure Added by GoldenFox
//сделать выбранной строку по координатам X, Y
begin
  SetSelected(RowByPoint(X, Y)); //выбрать строку
end;
//-------- End block added by GoldenFox ----------

{
//Deleted by GoldenFox
function TD2CustomGrid.IsSelected(Row:integer):boolean;
begin							  
  Result:=Row=FSelected;
end;
}

//----------- Block added by GoldenFox -----------
function TD2CustomGrid.IsSelected(Row:integer):boolean;
//проверить выбрана строка или нет
var i: integer;	//Added by GoldenFox
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
//-------- End block added by GoldenFox ----------

procedure TD2CustomGrid.SetAlternatingRowBackground(const Value:boolean);
begin
  if FAlternatingRowBackground <> Value then
  begin
    FAlternatingRowBackground:=Value;
    Repaint;
  end;
end;

//----------- Block added by GoldenFox -----------
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
//-------- End block added by GoldenFox ----------

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

{
//Deleted by GoldenFox
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
}

//----------- Block added by GoldenFox -----------
procedure TD2CustomGrid.SetSortColumn(AValue: integer);
begin
  if AValue > ColumnCount-1 then AValue:=ColumnCount-1;
  if AValue < 0 then  AValue:=NoColumn;
  if FSortColumn=AValue then Exit;
  FSortColumn:=AValue;
  SortGrid;
end;

procedure TD2CustomGrid.SetSortDirection(AValue: TD2SortDirection);
begin
  if FSortDirection=AValue then Exit;
  FSortDirection:=AValue;
  SortGrid;
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
//-------- End block added by GoldenFox ----------

procedure TD2CustomGrid.SetShowHeader(const Value:boolean);
begin
  if FShowHeader <> Value then
  begin
    FShowHeader:=Value;
    if FHeader <> nil then
      FHeader.Visible:=FShowHeader;
  end;
  Realign;			//Added by GoldenFox
  UpdateSelection;	//Added by GoldenFox
end;

procedure TD2CustomGrid.Reset;
begin
  SetFocus;
end;

//=============== TD2StringColumn =======================================================

{
//Deleted by GoldenFox
constructor TD2StringColumn.Create(AOwner:TComponent);
begin
  inherited;
end;

destructor TD2StringColumn.Destroy;
begin
  inherited;
end;
}

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