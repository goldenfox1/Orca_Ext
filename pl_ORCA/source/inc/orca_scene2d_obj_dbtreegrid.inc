{%mainunit ../orca_scene2d.pas}

{**********************************************************************
 Package pl_ORCA.pkg
 This unit is extension part of Package pl_ORCA.pkg make by GoldenFox
 for CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}



{ TD2TreeDataController }

procedure TD2TreeDataController.ActiveChanged;
//вызывается при изменении состояния открыт/закрыт DataSet-a
begin
  if Active
    then begin
           FDataSet := DataSet;
           if DataSetName <> fDataSetName
             then begin
                    fDataSetName := DataSetName;
                    UpdateKeyField;
                    UpdateParentField;
                    if Assigned(OnNewDataSet) then OnNewDataSet(DataSet);
                  end
             else if Assigned(OnDataSetOpen) then OnDataSetOpen(DataSet);
         end
    else begin
           BufferCount := 0;
           if (DataSource = nil)
             then begin
                    if Assigned(OnInvalidDataSource) then OnInvalidDataSource(fDataSet);
                    fDataSet := nil;
                    fDataSetName := '[???]';
                  end
             else begin
                    if (DataSet=nil)or(csDestroying in DataSet.ComponentState)
                      then begin
                             if Assigned(OnInvalidDataSet) then OnInvalidDataSet(fDataSet);
                             fDataSet := nil;
                             fDataSetName := '[???]';
                           end
                      else begin
                             if Assigned(OnDataSetClose) then OnDataSetClose(DataSet);
                             if DataSet <> nil then FDataSetName := DataSetName;
                           end;
                  end;
         end;
end;

procedure TD2TreeDataController.SetKeyFieldName(const AValue: string);
//установить ключевое поле в соответствии c его имемем AValue
begin
  if FKeyFieldName <> AValue then
  begin
    FKeyFieldName := AValue;
    UpdateKeyField;
  end;
end;

procedure TD2TreeDataController.SetParentFieldName(const AValue: string);
//установить родительское поле в соответствии c его имемем AValue
begin
  if FParentFieldName <> AValue then
  begin
    FParentFieldName := AValue;
    UpdateParentField;
  end;
end;

procedure TD2TreeDataController.UpdateKeyField;
//обновить ключевое поле в соответствии с его именем заданным параметром FKeyFieldName
begin
  if Active and
     DataSet.Active and
     (FKeyFieldName <> '')
    then FKeyField := DataSet.FieldByName(FKeyFieldName)
    else FKeyField := nil;
  KeyChanged;
end;

procedure TD2TreeDataController.UpdateParentField;
//обновить родительское поле в соответствии с его именем заданным параметром FParentFieldName
begin
  if Active and
     DataSet.Active and
     (FParentFieldName <> '')
    then FParentField := DataSet.FieldByName(FParentFieldName)
    else FParentField := nil;
  ParentChanged;
end;

procedure TD2TreeDataController.KeyChanged;
//вызывается при изменении ключевого поля таблицы
begin
  if Assigned(FOnKeyChanged) then
    FOnKeyChanged(FKeyField);
end;

procedure TD2TreeDataController.ParentChanged;
//вызывается при изменении родительское поля таблицы
begin
  if Assigned(FOnParentChanged) then
    FOnParentChanged(FParentField);
end;

function TD2TreeDataController.Edit: Boolean;
begin
  If Not ReadOnly then
  DataSource.Edit;
  // Triggered event will set FEditing
  Result:=Editing;
end;

{ TD2DBTreeColumn }

procedure TD2DBTreeColumn.SetFieldName(const Value: String);
begin
  if FFieldName <> Value then
  begin
    if Header=FFieldName
      then Header:=Value;
    FField:=nil;
    FFieldName:=Value;
    UpdateColumn;
  end;
end;

function TD2DBTreeColumn.GetField: TField;
begin
  if FField = nil then LinkField;
  Result:=FField;
end;

procedure TD2DBTreeColumn.SetField(Value: TField);
begin
  if FField=Value then Exit;
  if Assigned(Value) and (csDestroying in Value.ComponentState)
    then Value:=nil;    // don't acquire references to fields being destroyed
  FField:=Value;
  if Assigned(Value)
    then FFieldName:=Value.FieldName;
end;

procedure TD2DBTreeColumn.LinkField;
var f: TField;
begin
  f:= nil;
  if (Length(FFieldName) > 0) and Assigned(Grid) and
      Assigned(TD2CustomDBTreeGrid(Grid).DataController.DataSet) then
    with TD2CustomDBTreeGrid(Grid).DataController.Dataset do
      if Active or (not DefaultFields)
        then f:=FindField(FieldName);
  SetField(f);
end;

function TD2DBTreeColumn.GetData: Variant;
begin
  if Assigned(Field)
    then Result:=Field.Value;
end;

procedure TD2DBTreeColumn.SetData(Value: Variant);
begin
  if Assigned(Field)
    then Field.Value:=Value;
end;

destructor TD2DBTreeColumn.Destroy;
begin
  FField:=nil;
  inherited;
end;


{ TD2DBTreeTextColumn }

function TD2DBTreeTextColumn.CreateCellControl: TD2Control;
begin
  Result:=inherited;
  with TD2TextCell(TD2TreeCellControl(Result).FControl) do
  begin
    HitTest:=false;
    CanFocused:=false;
    Locked:=true;
    OnChange:=DoTextChanged;
  end;
end;

procedure TD2DBTreeTextColumn.SetData(Value: Variant);
begin
  if Assigned(Field)
    then Field.Text:=UTF8Encode(Value);
end;

procedure TD2DBTreeTextColumn.NodeSetFocus(Node: PD2TreeNode);
// Установить фокус в видимую ячейку соответствующую узлу Node.
var C: TD2Control;
begin
  inherited;
  C:=CellControlByNode(Node);
  if (C <> nil) and (C is TD2TreeCellControl) and (TD2TreeCellControl(C).FControl <> nil) then
  begin
    TD2TreeCellControl(C).FControl.HitTest:=true;
    TD2TreeCellControl(C).FControl.CanFocused:=true;
    TD2TreeCellControl(C).FControl.SetFocus;
  end;
end;

function TD2DBTreeTextColumn.GetData: Variant;
begin
  if Assigned(Field) then
    Result:= UTF8Decode(Field.DisplayText);
end;

function TD2DBTreeTextColumn.GetCellClass: TD2CellClass;
begin
  Result:=TD2TextCell;
end;

procedure TD2DBTreeTextColumn.DoTextChanged(Sender: TObject);
begin
  if Grid=nil then Exit;
  if FUpdateColumn then Exit;
  if FDisableChange then Exit;
  with TD2TreeCellControl(TD2Control(Sender).Owner) do
  begin
    TD2TreeGrid(Grid).SetValue(FNode,FColumnIndex,TD2Control(Sender).Data);
    if Assigned(TD2TreeGrid(Grid).FOnEdititingDone) then
      TD2TreeGrid(Grid).FOnEdititingDone(Grid,FNode,FColumnIndex);
  end;
end;


{ TD2DBTreeCheckColumn }

function TD2DBTreeCheckColumn.CreateCellControl: TD2Control;
begin
  Result:=inherited;
  TD2CheckCell(TD2TreeCellControl(Result).FControl).OnChange:=DoCheckChanged;
end;

procedure TD2DBTreeCheckColumn.DoCheckChanged(Sender: TObject);
begin
  if Grid=nil then Exit;
  if FUpdateColumn then Exit;
  if FDisableChange then Exit;
  with TD2TreeCellControl(TD2Control(Sender).Owner) do
  begin
    TD2TreeGrid(Grid).SetValue(FNode,FColumnIndex,TD2Control(Sender).Data);
    if Assigned(TD2TreeGrid(Grid).FOnEdititingDone) then
      TD2TreeGrid(Grid).FOnEdititingDone(Grid,FNode,FColumnIndex);
  end;
end;

function TD2DBTreeCheckColumn.GetData: Variant;
begin
  if Assigned(Field)
    then Result:=Field.AsBoolean
end;

function TD2DBTreeCheckColumn.GetCellClass: TD2CellClass;
begin
  Result:=TD2CheckCell;
end;


{ TD2DBTreeProgressColumn }

function TD2DBTreeProgressColumn.CreateCellControl: TD2Control;
begin
  Result:=inherited;
  with TD2ProgressCell(TD2TreeCellControl(Result).FControl) do
  begin
    Min:=FMin;
    Max:=FMax;
  end
end;

function TD2DBTreeProgressColumn.GetCellClass: TD2CellClass;
begin
  Result:=TD2ProgressCell;
end;

constructor TD2DBTreeProgressColumn.Create(AOwner: TComponent);
begin
  inherited;
  FMax:=100;
end;

{ TD2DBTreePopupColumn }

procedure TD2DBTreePopupColumn.SetItems(const Value: TD2WideStrings);
begin
  FItems.Assign(Value);
end;

procedure TD2DBTreePopupColumn.DoPopupChanged(Sender:TObject);
begin
  if Grid = nil then Exit;
  if FUpdateColumn then Exit;
  if FDisableChange then Exit;
  with TD2TreeCellControl(TD2Control(Sender).Owner) do
  begin
    TD2TreeGrid(Grid).SetValue(FNode,FColumnIndex,TD2Control(Sender).Data);
    if Assigned(TD2TreeGrid(Grid).FOnEdititingDone) then
      TD2TreeGrid(Grid).FOnEdititingDone(Grid,FNode,FColumnIndex);
  end;
end;

function TD2DBTreePopupColumn.CreateCellControl: TD2Control;
begin
  Result:=inherited;
  with TD2PopupCell(TD2TreeCellControl(Result).FControl) do
  begin
    Items.Assign(FItems);
    OnChange:=DoPopupChanged;
  end;
end;

function TD2DBTreePopupColumn.GetCellClass: TD2CellClass;
begin
  Result:=TD2PopupCell;
end;

constructor TD2DBTreePopupColumn.Create(AOwner: TComponent);
begin
  inherited;
  FItems:=TD2WideStringList.Create;
end;

destructor TD2DBTreePopupColumn.Destroy;
begin
  FItems.Free;
  inherited;
end;

{ TD2DBTreeImageColumn }

function TD2DBTreeImageColumn.CreateCellControl: TD2Control;
begin
  Result:=inherited;
  with TD2ImageCell(TD2TreeCellControl(Result).FControl) do
  begin
    OnChange:=DoImageChanged;
    EnableOpenDialog:=false;
  end;

end;

procedure TD2DBTreeImageColumn.DoImageChanged(Sender: TObject);
begin
  if Grid =nil then Exit;
  if FUpdateColumn then Exit;
  if FDisableChange then Exit;
  if not Grid.CanEditModify then Exit;
  with TD2TreeCellControl(TD2Control(Sender).Owner) do
  begin
    TD2TreeGrid(Grid).SetValue(FNode,FColumnIndex,TD2Control(Sender).Data);
    if Assigned(TD2TreeGrid(Grid).FOnEdititingDone) then
      TD2TreeGrid(Grid).FOnEdititingDone(Grid,FNode,FColumnIndex);
  end;
end;

procedure TD2DBTreeImageColumn.SetData(Value: Variant);
var
  P:TPicture;
begin
  if Assigned(Field) and Field.IsBlob and VarIsObject(Value)
    then  begin
            P:=TPicture.Create;
            P.Assign(TPersistent(VariantToObject(Value)));
            Field.Assign(P);
            P.Free;
          end
    else Field.Value:=NULL;
end;

function TD2DBTreeImageColumn.GetData: Variant;
begin
  if Assigned(Field) and Field.IsBlob and (TBlobField(Field).BlobSize > 0)
    then  begin
            if FCurrent=nil
               then FCurrent:=TD2Bitmap.Create(1,1);
            FCurrent.Assign(Field);
            Result:=ObjectToVariant(FCurrent);
          end
    else  Result:=NULL;
end;

function TD2DBTreeImageColumn.GetCellClass: TD2CellClass;
begin
  Result:=TD2ImageCell;
end;

destructor TD2DBTreeImageColumn.Destroy;
begin
  if FCurrent <> nil
    then FCurrent.Free;
  inherited;
end;


{ TDBDataNode }

constructor TD2DBDataNode.Create(aPosition: Integer; aKey: Integer;
  aParent: Integer; aNode: PD2TreeNode);
begin
  FPosition:=aPosition;
  FKey:=aKey;
  FParent:=aParent;
  FNode:=aNode;
end;

{ TD2CustomDBTreeGrid }

procedure TD2CustomDBTreeGrid.OnRecordChanged(aField: TField);
//прерывание после изменения записи в DataSet сразу после Post
begin
  if FDataController.DataSource.State <> dsInactive then
    UpdateColumns;
end;

procedure TD2CustomDBTreeGrid.OnDataSetChanged(aDataSet: TDataSet);
//прерывание при изменениях в DataSet
var i: integer;
begin
  //if not FBuildTree and  (FDataController.DataSet <> nil) then
  //begin
  //  //UpdateRowCount;
  //  FDisableMove:=true;
  //  with FDataController do
  //    if (DataSet.RecNo > 0) and (FKeyField<>nil)  then
  //    begin
  //      FIsPreSelected:=false;
  //      i:=0;
  //      FFocusedNode:=FindNode(FKeyField.AsInteger,i);
  //      SelectNode(FFocusedNode);
  //      ScrollIntoView(FFocusedNode,toCenterScrollIntoView in TreeOptions.FSelectionOptions,false);
  //    end;
  //  FDisableMove:=false;
  //  UpdateColumns;
  //end;
end;

procedure TD2CustomDBTreeGrid.OnDataSetOpen(aDataSet: TDataSet);
//прерывание при открытии DataSet
begin
  LinkActive(FDataController.Active);
  BuildTree;
  FIsPreSelected := false;
end;

procedure TD2CustomDBTreeGrid.OnDataSetClose(aDataSet: TDataSet);
//прерывание при закрытии DataSet
begin
  LinkActive(FDataController.Active);
  Clear;  //удалить все узлы из дерева
  FDataNodes.Clear;        //удалить данные узлов
end;

procedure TD2CustomDBTreeGrid.OnEditingChanged(aDataSet: TDataSet);
//прерывание при входе или выходе в/из режима редактирования данных в DataSet
begin
  LinkActive(FDataController.Active);
end;

procedure TD2CustomDBTreeGrid.OnInvalidDataSet(aDataSet: TDataSet);
//прерывание если не правильный DataSet
begin
  LinkActive(FDataController.Active);
  Clear;  //удалить все узлы из дерева
  FDataNodes.Clear;        //удалить данные узлов
end;

procedure TD2CustomDBTreeGrid.OnInvalidDataSource(aDataSet: TDataset);
//прерывание если не правильный DataSource
begin
  LinkActive(FDataController.Active);
  Clear;  //удалить все узлы из дерева
  FDataNodes.Clear;        //удалить данные узлов
end;

procedure TD2CustomDBTreeGrid.OnKeyChanged(aField: TField);
begin
  if FDataController.Active
    then BuildTree;
end;

procedure TD2CustomDBTreeGrid.OnParentChanged(aField: TField);
begin
  if FDataController.Active
    then BuildTree;
end;

procedure TD2CustomDBTreeGrid.OnLayoutChanged(aDataSet: TDataSet);
//прерывание при изменении состава или порядка полей в DataSet
begin
  LinkActive(FDataController.Active);
  if FDataController.Active
    then BuildTree;
end;

procedure TD2CustomDBTreeGrid.OnNewDataSet(aDataSet: TDataset);
//прерывание или первом открытии нового DataSet
begin
  LinkActive(FDataController.Active);
  BuildTree;
  FIsPreSelected := false;
end;

procedure TD2CustomDBTreeGrid.OnDataSetScrolled(aDataSet: TDataSet; Distance: Integer);
//прерывание при перемещении на другую запись в DataSet
var i: integer;
begin
  if not FBuildTree and  (FDataController.DataSet <> nil) then
  begin
    //UpdateRowCount;
    FDisableMove:=true;
    with FDataController do
      if (DataSet.RecNo > 0) and (FKeyField<>nil)  then
      begin
        FIsPreSelected:=false;
        i:=0;
        FFocusedNode:=FindNode(FKeyField.AsInteger,i);
        SelectNode(FFocusedNode);
        ScrollIntoView(FFocusedNode,toCenterScrollIntoView in TreeOptions.FSelectionOptions,false);
      end;
    FDisableMove:=false;
    UpdateColumns;
  end;
end;

//procedure TD2CustomDBTreeGrid.OnDataSourceChanged(aDataSource: TDataSource);
//begin
//  LinkActive(FDataController.Active);
//  if FDataController.Active
//    then BuildTree;
//end;

procedure TD2CustomDBTreeGrid.OnUpdateData(aDataSet: TDataSet);
var C: TD2DBTreeColumn;
begin
  if FNeedUpdate then
  begin
    C:=TD2DBTreeColumn(Columns[FocusedColumn]);
    if Assigned(C)
      then C.SetData(FEditValue);
    FEditValue:=NULL;
    FNeedUpdate:=false;
  end;
end;

procedure TD2CustomDBTreeGrid.SetDataController(AValue: TD2TreeDataController);
//установить ссылку на DataController
begin
  FDataController.Assign(AValue);
end;

procedure TD2CustomDBTreeGrid.UpdateRowCount;
//установить кол-во строк в гриде в соответствии с DataSet
begin
  //if FDataController.Active then
  //begin
  //  FDataController.BufferCount:=VisibleRows * 2;
  //  if FDataController.BufferCount < FDataController.DataSet.RecordCount then
  //    RowCount:=FDataController.DataSet.RecordCount - FDataController.BufferCount + FDataController.RecordCount
  //  else
  //    RowCount:=FDataController.RecordCount;
  //end
  //else RowCount:=0;

  if FDataController.Active
    then FDataController.BufferCount:=FDataController.DataSet.RecordCount
    else FDataController.BufferCount:=0;
end;

function TD2CustomDBTreeGrid.CanEditModify: Boolean;
begin
  Result:=False;
  if not ReadOnly and FDataController.Active and not FDataController.Readonly then
  with TD2DBTreeColumn(Columns[FocusedColumn]) do
  begin
    if (not ReadOnly) and Assigned(Field) then
      Result:=Field.CanModify;
    if (not ReadOnly) and Assigned(Field) and Field.CanModify then
    begin
      FDataController.Edit;
      Result:=FDataController.Editing;
      if Result then FDataController.Modified:=True;
    end;
  end;
end;

procedure TD2CustomDBTreeGrid.DoFocusNode(Node: PD2TreeNode; Ask: Boolean);
//Передать фокус узлу Value
var Old: PD2TreeNode;
begin
  Old := FFocusedNode;
  inherited;
  if (FFocusedNode <> nil) and (Old <> FFocusedNode) and
     (not FDisableMove) and
     (FDataController.Active) then
    if (Old <> nil)
      then FDataController.MoveBy(TD2DBDataNode(GetNodeData(FFocusedNode)^).FPosition-
                                  TD2DBDataNode(GetNodeData(Old)^).FPosition )
      else with FDataController do
             DataSet.Locate(FKeyFieldName, TD2DBDataNode(GetNodeData(FFocusedNode)^).FKey,[]);
end;

function TD2CustomDBTreeGrid.GetValue(Node: PD2TreeNode; Col: integer): Variant;
//получить значение ячейки в колонке Col для узла Node
var
  C: TD2DBTreeColumn;
  D: TD2DBDataNode;
begin
    if (not Assigned(Node)) or (Col < 0) or
       (not Assigned(FDataController.DataSet)) or
       (not FDataController.DataSet.Active) then exit;
    C:=TD2DBTreeColumn(Columns[Col]);
    D:=TD2DBDataNode(GetNodeData(Node)^);
    if Assigned(D) and Assigned(C) and Assigned(C.FField)
      then Result := FDataController.DataSet.Lookup(FDataController.FKeyFieldName,
                                                    D.Key, C.FFieldName)
      else Result := Null;
  if Assigned(FOnGetValue) then
    FOnGetValue(Self, Node, Col, Result);
end;

procedure TD2CustomDBTreeGrid.SetValue(Node: PD2TreeNode; Col: integer; const Value: Variant);
//сохранить значение ячейки Value в колонке Col для узла Node
var
  C: TD2DBTreeColumn;
begin
  if Assigned(FDataController) and FDataController.Active and not FDataController.ReadOnly then
  begin
    C:=TD2DBTreeColumn(Columns[Col]);
    if (C <> nil) and (C.Field <> nil) and C.Field.CanModify then
    begin
      FDataController.Modified:=True;
      FEditValue:=Value;
      FNeedUpdate:=true;
      FDataController.UpdateData;
    end;
  end;
end;

procedure TD2CustomDBTreeGrid.ColumnsLinkFields;
//переопределить Field для каждой колонки
var i: integer;
begin
  for i:=0 to ColumnCount - 1 do
    TD2DBTreeColumn(Columns[i]).LinkField;
end;

procedure TD2CustomDBTreeGrid.LinkActive(Value: Boolean);
//переинициализация дерева при изменении свойста FDataController.Active
begin
  ColumnsLinkFields;
  UpdateRowCount;
  UpdateSelection;
end;

procedure TD2CustomDBTreeGrid.BuildTree;
//перестроить структуру дерева в соответствии с данными в DataSet

var
  i,s: Integer;
  P: TD2DBDataNode;
  N: PD2TreeNode;
begin
  if FBuildTree then exit; //если уже идет построение дерева то выйти
  FBuildTree:=true;        //установить флаг построения дерева
  BeginUpdate;             //запретить обновление отображения дерева
  Clear;                   //удалить все узлы дерева
  FDataNodes.Clear;        //удалить узлы данных
  with DataController do
    if (FKeyField <> nil) and    //если определено ключевое поле и
       (DataSet <> nil) and      //есть таблица и
       (DataSet.Active) and      //таблиц открыта и
       (DataSet.RecordCount > 0) then  //в таблице есть записи
      begin
        DataSet.DisableControls;               //отключить данные от контрола
        DataSet.First;                         //встать на первую запись таблицы
        for i:=0 to DataSet.RecordCount-1 do     //для всех записей таблицы
        begin
          P:=TD2DBDataNode.Create(i, FKeyField.AsInteger); //создать узел данных с ключевым полем
          if FParentField <> nil                           //если поле родителя определено
             then P.FParent:=FParentField.AsInteger;         //то добавим его в узел данных
          P.Node:=AddChild(nil,P);                         //создать узел дерева, указатель на него пишем в узел данных
          FDataNodes.Add(P);                               //добавим узел данных в дерево список данных
          DataSet.Next;                                    //следующий
        end;
        FDataNodes.Sort(DBDataNodeKeyCompare);          //Сортируем данные по ключевому полю (сортировка в DataSet-e может быть другой)
        s:=0;
        if FParentField <> nil then            //если есть ParentField
          for i:=0 to FDataNodes.Count-1 do      //делаем для всех узлов
            MoveTo(TD2DBDataNode(FDataNodes.Items[i]).FNode, //подключаем все дочерние узлы к их родителям
                   FindNode(TD2DBDataNode(FDataNodes.Items[i]).FParent, s),
                   amAddChildLast, False );

        P:=TD2DBDataNode(GetNodeData(GetFirstVisible)^);
        if Assigned(P)
          then FDataController.DataSet.Locate( FDataController.FKeyFieldName, P.Key, []); //встать на первую видимый узел дерева

        DataSet.EnableControls;                //подключить данные к контролу
      end;
  EndUpdate;          //разрешаем обновление отображения дерева
  FBuildTree:=false;  //снимаем флаг построения дерева
end;

function TD2CustomDBTreeGrid.FindNode(aKey: integer; var aStart: integer): PD2TreeNode;
//Поиск узла, соответствующего ключу aKey в списке DataNodes начиная с позиции aStart.
//На выходе найденный узел или Root; в aStart - № позиции узла в списке DataNodes.
var
  verh: integer; { верхняя граница поиска }
  niz: integer; { нижняя граница поиска }
  found: boolean; { TRUE — совпадение образца с элементом массива }
begin
  if FDataNodes.Count <= 0 then //если нет узлов данных
  begin
    aStart:=-1;       //позицию поиска установить -1
    Result:=RootNode; //вернуть корень
    exit;             //выйти
  end;
  if aStart > FDataNodes.Count - 1      //если позиция поиска больше индекса последнего узла
    then aStart := FDataNodes.Count-1;    //то установить позицию поиска в индекс последнего узла
  if aStart < 0 then aStart := 0;       //если позиция поиска отрицательное то установить в 0

  if aKey = TD2DBDataNode(FDataNodes.Items[aStart]).FKey        //если искомое равно текущему
    then found := True                                          //то установить флаг "найдено"
    else begin                                                  //иначе
           found := False;                                        //снять флаг "найдено"
           if aKey < TD2DBDataNode(FDataNodes.Items[aStart]).FKey   //если искомое меньше текущего
             then begin                                             //то
                    verh := 0;                                        //начало ставим в 0
                    niz := aStart - 1;                                //конечную позицию сдвигаем на 1 выше текущей
                  end
             else begin                                              //иначе если больше
                    verh := aStart + 1;                                //начальную позицию сдвигаем на 1 ниже текущей
                    niz := FDataNodes.Count-1;                         //конечную позицию ставим на последий узел
                  end;
           while (verh <= niz) and (not found) do                      //пока верх меньше или равно низ и нет флага "найдено"
           begin
             aStart := ((niz - verh) div 2) + verh;                   //текущая позиция в середину диапазона
             if aKey = TD2DBDataNode(FDataNodes.Items[aStart]).FKey        //если искомое равно текущему
               then found := True                                          //то установить флаг "найдено"
               else if aKey < TD2DBDataNode(FDataNodes.Items[AStart]).FKey   //иначе если искомое меньше текущего
                      then niz := aStart - 1                                 //конечную позицию сдвигаем на 1 выше текущей
                      else verh := aStart + 1;                               //иначе начальную позицию сдвигаем на 1 ниже текущей
           end;
         end;
  if found then Result:=TD2DBDataNode(FDataNodes.Items[AStart]).FNode //если найдено, вернуть узел
           else Result:=RootNode;                                     //иначе вернуть корень
end;

constructor TD2CustomDBTreeGrid.Create(aOwner: TComponent);
begin
  inherited;
  FBuildTree:=false;
  NodeDataSize := SizeOf(TD2DBDataNode);
  FDataController := TD2TreeDataController.Create;//(Self);
  FDataNodes:=TList.Create;
  FDataController.OnRecordChanged:=OnRecordChanged;
  FDataController.OnDatasetChanged:=OnDataSetChanged;
  FDataController.OnDataSetOpen:=OnDataSetOpen;
  FDataController.OnDataSetClose:=OnDataSetClose;
  FDataController.OnNewDataSet:=OnNewDataSet;
  FDataController.OnInvalidDataSet:=OnInvalidDataset;
  FDataController.OnInvalidDataSource:=OnInvalidDataSource;
  FDataController.OnKeyChanged:=OnKeyChanged;
  FDataController.OnParentChanged:=OnParentChanged;
  FDataController.OnDataSetScrolled:=OnDataSetScrolled;
  FDataController.OnLayoutChanged:=OnLayoutChanged;
  FDataController.OnEditingChanged:=OnEditingChanged;
  FDataController.OnUpdateData:=OnUpdateData;
end;

destructor TD2CustomDBTreeGrid.Destroy;
begin
  FDataController.OnRecordChanged:=nil;
  FDataController.OnDatasetChanged:=nil;
  FDataController.OnDataSetOpen:=nil;
  FDataController.OnDataSetClose:=nil;
  FDataController.OnNewDataSet:=nil;
  FDataController.OnInvalidDataSet:=nil;
  FDataController.OnInvalidDataSource:=nil;
  FDataController.OnKeyChanged:=nil;
  FDataController.OnParentChanged:=nil;
  FDataController.OnDataSetScrolled:=nil;
  FDataController.OnLayoutChanged:=nil;
  FDataController.OnEditingChanged:=nil;
  FDataController.OnUpdateData:=nil;
  FDataNodes.Free;
  FDataController.Free;
  inherited;
end;

function TD2CustomDBTreeGrid.ItemClass: string;
begin
  Result:='TD2DBTreeTextColumn;TD2DBTreeCheckColumn;TD2DBTreePopupColumn;TD2DBTreeImageColumn;TD2DBTreeProgressColumn';
end;


