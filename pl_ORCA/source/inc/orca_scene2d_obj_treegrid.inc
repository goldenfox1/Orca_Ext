{%mainunit ../orca_scene2d.pas}

{**********************************************************************
 Package pl_ORCA.pkg
 This unit is extension part of Package pl_ORCA.pkg make by GoldenFox
 for CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

{ TD2TreeGridCell }

procedure TD2TreeGridCell.DoButtonClick(Sender: TObject);
begin
  IsExpanded:=not IsExpanded;
end;

procedure TD2TreeGridCell.DoCheckClick(Sender: TObject);
begin
  if FCheck <> nil then
    FIsChecked:=FCheck.IsChecked;
  if TreeGrid <> nil then
  begin
    TreeGrid.SetFocus;
    //if Assigned(TreeGrid.OnChangeCheck) then
    //  TreeGrid.OnChangeCheck(Self);
  end;
end;

function TD2TreeGridCell.GetHaveChildren: boolean;
begin

end;

procedure TD2TreeGridCell.SetIsChecked(const Value: boolean);
begin
  if FIsChecked <> Value then
  begin
    FIsChecked:=Value;
    if FCheck <> nil then
      FCheck.IsChecked:=FIsChecked;
  end;
end;

procedure TD2TreeGridCell.SetIsExpanded(const Value: boolean);
begin
  if FIsExpanded <> Value then
  begin
    FIsExpanded:=Value;
    if (FButton <> nil) and not (csLoading in ComponentState) then
    begin
      FButton.Visible:=GetHaveChildren;
      if FButton.Visible then
        FButton.StartTriggerAnimation(Self,'IsExpanded');
    end;
    if TreeGrid <> nil then
      TreeGrid.Realign;
  end;
end;

procedure TD2TreeGridCell.ApplyStyle;
var
  B:TD2Object;
begin
  inherited;
  B:=FindResource('button');
  if (B <> nil) and (B is TD2CustomButton) then
  begin
    FButton:=TD2CustomButton(B);
    FButton.OnClick:=DoButtonClick;
    FButton.Visible:=GetHaveChildren;
    if FButton.Visible then
    begin
      FButton.ApplyResource;
      FButton.StartTriggerAnimation(Self,'IsExpanded');
    end;
  end;
  B:=FindResource('check');
  if (B <> nil) and (B is TD2CheckBox) then
  begin
    FCheck:=TD2CheckBox(B);
    FCheck.IsChecked:=IsChecked;
    FCheck.OnChange:=DoCheckClick;
    if TreeGrid <> nil then
      FCheck.Visible:=TreeGrid.ShowCheckboxes;
  end;
  B:=FindResource('content');
  if (B <> nil) and (B is TD2Content) then
  begin
    FContent:=TD2Content(B);
  end;
  StartTriggerAnimation(Self,'IsSelected');
end;

procedure TD2TreeGridCell.FreeStyle;
begin
  inherited;
  FButton:=nil;
  FCheck:=nil;
  FContent:=nil;
end;

constructor TD2TreeGridCell.Create(AOwner: TComponent);
begin
  inherited;
end;

function TD2TreeGridCell.TreeGrid: TD2CustomTreeGrid;
begin

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
  Result:='TD2TGTextColumn;TD2TGCheckColumn;TD2TGPopupColumn;TD2TGImageColumn;TD2TGProgressColumn';
end;

{ TD2TGColumn }

function TD2TGColumn.CreateCellControl: TD2Control;
begin
  inherited;
end;
