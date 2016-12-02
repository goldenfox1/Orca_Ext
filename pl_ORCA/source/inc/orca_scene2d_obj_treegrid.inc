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
  if Assigned(FOnChangeExpander) then
    FOnChangeExpander(Sender);
end;

procedure TD2TreeCell.DoCheckClick(Sender: TObject);
begin
  if FCheck <> nil then
    FIsChecked:=FCheck.IsChecked;
  if Assigned(FOnChangeCheck) then
    FOnChangeCheck(Sender);
end;

function TD2TreeCell.GetHaveChildren: boolean;
begin
  if Assigned(FOnGetHaveChildren)
    then Result:=FOnGetHaveChildren(Self)
    else Result:=false;
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
      begin
        FExpander.StartTriggerAnimation(Self,'IsExpanded');
      end;
    end;
  end;
end;

function TD2TreeCell.GetData: Variant;
begin
  if FControl<>nil
    then Result:=FControl.GetData
    else Result:=Null;
end;

procedure TD2TreeCell.SetData(const Value: Variant);
begin
  if FControl<>nil
    then FControl.SetData(Value);
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
  if (B <> nil) and (B is TD2Layout) then
  begin
    FContent:=TD2Content(B);
    if FControl<> nil then
      begin
        FControl.Parent:=FContent;
        FControl.Align:=vaClient;
        FControl.Visible:=true;
      end;
  end;
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

procedure TD2TreeCell.KeyDown(var Key: Word; var KeyChar: System.WideChar;
  Shift: TShiftState);
begin
  inherited;
  if FControl <> nil then
    TD2HackVisualObject(FControl).KeyDown(Key,KeyChar,Shift);
end;

procedure TD2TreeCell.AddObject(AObject: TD2Object);
begin
  inherited;
  if (FContent<> nil) then
    AObject.Parent:=FContent;
end;

constructor TD2TreeCell.Create(AOwner: TComponent);
begin
  inherited;
  FControl:=nil;
end;

destructor TD2TreeCell.Destroy;
begin
  FreeAndNil(FControl);
  inherited Destroy;
end;

{ TD2TreeColumn }

procedure TD2TreeColumn.DoChangeCheck(Sender: TObject);
begin

end;

procedure TD2TreeColumn.DoChangeExpander(Sender: TObject);
begin

end;

function TD2TreeColumn.DoGetHaveChildren(Sender: TObject): boolean;
begin
  Result:=true;
end;

function TD2TreeColumn.CreateCellControl: TD2Control;
begin
  Result:=TD2TreeCell.Create(Self);
end;

procedure TD2TreeColumn.InitCellControl(ACellControl: TD2Control);
begin
  inherited;
  if (ACellControl <> nil) and
     (ACellControl is TD2TreeCell) and
     (TD2TreeCell(ACellControl).FControl <> nil) and
     (TD2TreeCell(ACellControl).FControl is TD2Control) then
    begin
      ACellControl.OnEnterFocus:=nil;
      ACellControl.OnKillFocus:=nil;
      ACellControl.OnKeyDown:=nil;
      ACellControl.OnCanFocused:=nil;
      TD2TreeCell(ACellControl).FControl.OnCanFocused:=DoCanFocused;
      TD2TreeCell(ACellControl).FControl.OnEnterFocus:=DoEnterFocus;
      TD2TreeCell(ACellControl).FControl.OnKillFocus:=DoKillFocus;
      TD2TreeCell(ACellControl).FControl.OnKeyDown:=DoKeyDown;
      TD2TreeCell(ACellControl).OnChangeCheck:=DoChangeCheck;
      TD2TreeCell(ACellControl).OnChangeExpander:=DoChangeExpander;
      TD2TreeCell(ACellControl).OnGetHaveChildren:=DoGetHaveChildren;
    end;
end;

{ TD2TreeTextColumn }

function TD2TreeTextColumn.CreateCellControl: TD2Control;
begin
  Result:=inherited;
  if Result <> nil then
    begin
      TD2TreeCell(Result).FControl:=TD2TextCell.Create(Result);
      if TD2TreeCell(Result).FControl <> nil then
        with TD2TreeCell(Result).FControl do
        begin
          HitTest:=false;
          CanFocused:=false;
          Locked:=true;
        end;
    end;
end;

procedure TD2TreeTextColumn.InitCellControl(ACellControl: TD2Control);
begin
  inherited;
  if TD2TreeCell(ACellControl).FControl <> nil then
    TD2TextCell(TD2TreeCell(ACellControl).FControl).OnChange:=DoTextChanged;
end;

procedure TD2TreeTextColumn.DoTextChanged(Sender: TObject);
begin
  if Grid=nil then Exit;
  if FUpdateColumn then Exit;
  if FDisableChange then Exit;
  if TD2Object(Sender).Parent=nil then Exit;
  with d2StringToPoint(TD2Object(Sender).Parent.TagString) do
  begin
    Grid.SetValue(trunc(x),trunc(y),TD2Control(Sender).Data);
    if Assigned(Grid.FOnEdititingDone) then
      Grid.FOnEdititingDone(Grid,trunc(x),trunc(y));
  end;
end;

procedure TD2TreeTextColumn.CellSetFocus(ARow: integer);
var C: TD2Control;
begin
  inherited;
  C:=CellControlByRow(ARow);
  if (C <> nil) and (C is TD2TreeCell) and (TD2TreeCell(C).FControl <> nil) then
  begin
    TD2TreeCell(C).FControl.HitTest:=true;
    TD2TreeCell(C).FControl.CanFocused:=true;
    TD2TreeCell(C).FControl.SetFocus;
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


