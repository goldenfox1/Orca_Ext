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
