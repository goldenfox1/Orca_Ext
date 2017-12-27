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
    TD2TreeGrid(Grid).SetValue(FNode,FColumnIndex,Data);
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
    TD2TreeGrid(Grid).SetValue(FNode,FColumnIndex,Data);
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

