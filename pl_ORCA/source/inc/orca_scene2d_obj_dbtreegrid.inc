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
    then Field.Value:=Value
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
    //HitTest:=false;
    //CanFocused:=false;
    //Locked:=true;
    OnChange:=DoTextChanged;
  end;
end;

procedure TD2DBTreeTextColumn.SetData(Value: Variant);
begin
  if Assigned(Field)
    then Field.Text:=UTF8Encode(Value);
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

constructor TDBDataNode.Create(AKey: Integer; AParent: Integer;
  ANode: PD2TreeNode);
begin
  FKey:=AKey;
  FParent:=AParent;
  FNode:=ANode;
end;

{ TD2CustomDBTreeGrid }

procedure TD2CustomDBTreeGrid.SetDataController(AValue: TD2TreeDataController);
begin
  FDataController.Assign(AValue);
end;

constructor TD2CustomDBTreeGrid.Create(AOwner: TComponent);
begin
  inherited;
  FDataController := TD2TreeDataController.Create;//(Self);
  //FDataController.OnRecordChanged:=OnRecordChanged;
  //FDataController.OnDatasetChanged:=OnDataSetChanged;
  //FDataController.OnDataSetOpen:=OnDataSetOpen;
  //FDataController.OnDataSetClose:=OnDataSetClose;
  //FDataController.OnNewDataSet:=OnNewDataSet;
  //FDataController.OnInvalidDataSet:=OnInvalidDataset;
  //FDataController.OnInvalidDataSource:=OnInvalidDataSource;
  //FDataController.OnDataSetScrolled:=OnDataSetScrolled;
  //FDataController.OnLayoutChanged:=OnLayoutChanged;
  //FDataController.OnEditingChanged:=OnEditingChanged;
  //FDataController.OnUpdateData:=OnUpdateData;
  //FDataController.VisualControl:= True;

  //FSelectedRows := TBookmarkList.Create(Self);
  //RowCount:=0;
  //FDisableMove:=false;
end;

destructor TD2CustomDBTreeGrid.Destroy;
begin
  FDataController.OnDataSetChanged:=nil;
  FDataController.OnRecordChanged:=nil;
  FDataController.Free;
  inherited;
end;

function TD2CustomDBTreeGrid.ItemClass: string;
begin
  Result:='TD2DBTreeTextColumn;TD2DBTreeCheckColumn;TD2DBTreePopupColumn;TD2DBTreeImageColumn;TD2DBTreeProgressColumn';
end;


