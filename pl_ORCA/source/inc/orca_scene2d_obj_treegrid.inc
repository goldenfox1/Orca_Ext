{%mainunit ../orca_scene2d.pas}

{**********************************************************************
 Package pl_ORCA.pkg
 This unit is extension part of Package pl_ORCA.pkg make by GoldenFox
 for CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

{ TD2TreeCell }

procedure TD2TreeCell.DoExpanderClick(Sender: TObject);
begin
  IsExpanded:=not IsExpanded;
end;

procedure TD2TreeCell.DoCheckClick(Sender: TObject);
begin
  if FCheck <> nil then
    FIsChecked:=FCheck.IsChecked;
  if Assigned(FOnChangeCheck) then
    FOnChangeCheck(Self);
end;

function TD2TreeCell.GetHaveChildren: boolean;
begin
  if Assigned(FOnGetHaveChildren)
    then Result:=FOnGetHaveChildren(Self)
    else Result:=true;
end;

procedure TD2TreeCell.SetIsChecked(const Value: boolean);
begin
  if FIsChecked <> Value then
  begin
    FIsChecked:=Value;
    if FCheck <> nil then
      FCheck.IsChecked:=FIsChecked;
  end;
end;

procedure TD2TreeCell.SetIsExpanded(const Value: boolean);
begin
  if FIsExpanded <> Value then
  begin
    FIsExpanded:=Value;
    if (FExpander <> nil) and not (csLoading in ComponentState) then
    begin
      FExpander.Visible:=GetHaveChildren;
      if FExpander.Visible then
        FExpander.StartTriggerAnimation(Self,'IsExpanded');
    end;
  end;
end;

procedure TD2TreeCell.ApplyStyle;
var
  B:TD2Object;
begin
  inherited;

  B:=FindResource('expander');
  if (B <> nil) and (B is TD2CustomButton) then
  begin
    FExpander:=TD2CustomButton(B);
    FExpander.OnClick:=DoExpanderClick;
    FExpander.Visible:=GetHaveChildren;
    if FExpander.Visible then
    begin
      FExpander.ApplyResource;
      FExpander.StartTriggerAnimation(Self,'IsExpanded');
    end;
  end;
  B:=FindResource('check');
  if (B <> nil) and (B is TD2CheckBox) then
  begin
    FCheck:=TD2CheckBox(B);
    FCheck.IsChecked:=IsChecked;
    FCheck.OnChange:=DoCheckClick;
  end;
  B:=FindResource('content');
  if (B <> nil) and (B is TD2Content) then
    FContent:=TD2Content(B);
  StartTriggerAnimation(Self,'IsSelected');
end;

procedure TD2TreeCell.FreeStyle;
begin
  inherited;
  if FControl<>nil then FControl.Parent:=Self;
  FContent:=nil;
  FExpander:=nil;
  FCheck:=nil;
end;

procedure TD2TreeCell.AddObject(AObject: TD2Object);
begin
  inherited;
  if (FContent<> nil) and (AObject is TD2Control) then
  begin
    TD2Control(AObject).Parent:=FContent;
    TD2Control(AObject).Align:=vaClient;
    TD2Control(AObject).Visible:=true;
  end;
end;

destructor TD2TreeCell.Destroy;
begin
  FreeAndNil(FControl);
  inherited Destroy;
end;

{ TD2TreeColumn }

function TD2TreeColumn.CreateCellControl: TD2Control;
begin
  Result:=TD2TreeCell.Create(Self);
end;

{ TD2TreeTextColumn }

function TD2TreeTextColumn.CreateCellControl: TD2Control;
begin
  Result:=inherited;
  TD2TreeCell(Result).FControl:=TD2TextCell.Create(Result);
  TD2TextCell(TD2TreeCell(Result).FControl).OnChange:=DoTextChanged;
end;

procedure TD2TreeTextColumn.DoTextChanged(Sender: TObject);
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


{ TD2CustomTreeGrid }

procedure TD2CustomTreeGrid.SetShowCheckboxes(const Value: boolean);
begin

end;

constructor TD2CustomTreeGrid.Create(AOwner: TComponent);
begin
  inherited;
  FResource:='gridstyle';
end;

function TD2CustomTreeGrid.ItemClass: string;
begin
  Result:='TD2TreeTextColumn;TD2TreeCheckColumn;TD2TreePopupColumn;TD2TreeImageColumn;TD2TreeProgressColumn';
end;


