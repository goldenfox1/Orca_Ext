{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//=============== TD2Inspector =============================================

constructor TD2Inspector.Create(AOwner:TComponent);
begin
  inherited;
  FShowProperties:=true;
  FShowEvents:=false;
  FResource:='treeviewstyle';
  HideSelectionUnfocused:=false;
  FDisabledProperties:=TStringList.Create;
  FEditButton:=TD2Button.Create(Self);
  FEditButton.ResourceName:='inspectorEditor';
  FEditButton.TagString:='self';
  FEditButton.DisableFocusEffect:=true;
  FEditButton.Visible:=false;
  FEditButton.Stored:=false;
  FEditButton.Parent:=Self;
  FEditButton.OnClick:=DoEditorChange;
  FEditButton.Visible:=false;
  FEditButton.Position.X:=2000;
  FEditBox:=TD2TextBox.Create(Self);
  FEditBox.ResourceName:='inspectorEditor';
  FEditBox.TagString:='self';
  FEditBox.DisableFocusEffect:=true;
  FEditBox.Padding.Rect:=d2Rect(0,0,0,0);
  FEditBox.Stored:=false;
  FEditBox.Parent:=Self;
  FEditBox.OnChange:=DoEditorChange;
  FEditBox.Visible:=false;
  FEditBox.Position.X:=2000;
  FComboBox:=TD2PopupBox.Create(Self);
  FComboBox.ResourceName:='inspectorEditor';
  FComboBox.TagString:='self';
  FComboBox.DisableFocusEffect:=true;
  FComboBox.Stored:=false;
  FComboBox.Parent:=Self;
  FComboBox.TextAlign:=d2TextAlignNear;
  FComboBox.OnChange:=DoEditorChange;
  FComboBox.Visible:=false;
  FcomboBox.Position.X:=2000;
end;

destructor TD2Inspector.Destroy;
begin
  if (FSelectedObject <> nil) and (FSelectedObject is TD2Object) then
    TD2Object(FSelectedObject).RemoveFreeNotify(Self);
  FDisabledProperties.Free;
  inherited;
end;

procedure TD2Inspector.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FSelectedObject) then
  begin
    SelectedObject:=nil;
  end;
end;

procedure TD2Inspector.InsAddObject(ItemRoot:TD2Object; Root:TObject);
var
  PropList:PPropList;
  PropInfo:PPropInfo;
  //i,j,PropCount:integer;	//Deleted by GoldenFox
  i,PropCount:integer;		//Added by GoldenFox
  Obj:TObject;
  Item:TD2TreeViewItem;
  Value:TD2Label;
  //Cls:TClass;	//Deleted by GoldenFox
begin
  if Root=nil then Exit;
  {$IFDEF KS_COMPILER5}
  PropCount:=GetPropList(PTypeInfo(Root.ClassInfo),[tkUnknown,tkInteger,tkChar,tkEnumeration,tkFloat,
    tkString,tkSet,tkClass,tkMethod,tkWChar,tkLString,tkWString,
    tkVariant,tkArray,tkRecord,tkInterface,tkInt64,tkDynArray],nil);
  GetMem(PropList,SizeOf(PPropInfo) * PropCount);
  PropCount:=GetPropList(PTypeInfo(Root.ClassInfo),[tkUnknown,tkInteger,tkChar,tkEnumeration,tkFloat,
    tkString,tkSet,tkClass,tkMethod,tkWChar,tkLString,tkWString,
    tkVariant,tkArray,tkRecord,tkInterface,tkInt64,tkDynArray],PropList);
  {$ELSE}
  PropCount:=GetPropList(PTypeInfo(Root.ClassInfo),PropList);
  {$ENDIF}
  if PropCount=0 then Exit;

  try
    {$IFDEF KS_COMPILER6_UP}
    SortPropList(PropList,PropCount);
    {$ENDIF}
    for i:=0 to PropCount - 1 do
    begin
      with PropList[i]^ do
      begin
        if FDisabledProperties.IndexOf(Name) >= 0 then Continue;
        PropInfo:=GetPropInfo(Root,Name);
        if PropInfo=nil then Continue;
        if PropInfo^.SetProc=nil then Continue;

        if (PropType^.Kind in [tkMethod]) and ((not FShowEvents) or (Root <> FSelectedObject)) then Continue;
        if not (PropType^.Kind in [tkMethod]) and (not FShowProperties) then Continue;

        Item:=TD2TreeViewItem.Create(Self);
        Item.Parent:=ItemRoot;
        Item.TextW:=Name;
        Item.Locked:=true;
        Item.Stored:=false;
        Value:=TD2Label.Create(Self);
        Value.ResourceName:='inspectorEditor';
        Value.Locked:=true;
        Value.Stored:=false;
        Value.Parent:=Item;
        Value.SetBounds(Width / 2,0,Width,Item.Height + 2);
        Value.TextAlign:=d2TextAlignNear;
        Value.WordWrap:=false;
        Item.TagObject:=Root;
        Item.TagString:=Name;
        Item.Tag:=PtrInt(Value);
        case PropType^.Kind of
          tkInteger:
            begin
              Value.TextW:=IntToStr(GetOrdProp(Root,Name));
            end;
          tkFloat:
            begin
              Value.TextW:=FloatToStr(GetFloatProp(Root,Name));
            end;
          tkClass:
            begin
              Obj:=GetObjectProp(Root,Name);
              if (Obj is TD2Object) then
              begin
                TD2Label(Value).TextW:=TComponent(Obj).Name;
                if TComponent(Obj).Name='' then
                  TD2Label(Value).TextW:=TD2Object(Obj).ResourceName;
              end
              else
              if Obj <> nil then
              begin
                Item.IsExpanded:=false;
                TD2Label(Value).TextW:=Obj.ClassName;
                if not (Obj is TD2Object) then
                  InsAddObject(Item,Obj);
              end;
            end;
           {$IFDEF KS_COMPILER11}tkUString,{$ENDIF}tkWString:
            begin
              {$IFDEF KS_COMPILER6_UP}
              Value.TextW:=GetWideStrProp(Root,Name);
              {$ELSE}
              Value.TextW:=GetStrProp(Root,Name);
              {$ENDIF}
            end;
          tkString,tkLString:
            begin
              Value.TextW:=GetStrProp(Root,Name);
            end;
          tkMethod:
            begin
              Value.TextW:=GetStrProp(Root,Name);
            end;
          tkEnumeration:
            begin
              Value.TextW:=GetEnumProp(Root,Name);
            end;

          tkBool:
            begin
              Value.TextW:=GetEnumProp(Root,Name);
            end;
          else
            Value.TextW:=GetStrProp(Root,Name);
        end;
      end;
    end;
  finally
    FreeMem(PropList,SizeOf(PPropInfo) * PropCount);
  end;
end;

procedure TD2Inspector.RebuildList;
begin
  BeginUpdate;
  try
    FEditButton.TagObject:=nil;
    FEditBox.TagObject:=nil;
    FComboBox.TagObject:=nil;
    Clear;
    if FSelectedObject <> nil then
      InsAddObject(Self,FSelectedObject);
  finally
    EndUpdate;
  end;
end;

procedure TD2Inspector.RebuildEditor;
var
  PropInfo:PPropInfo;
  TypeData:PTypeData;
  Editor:TD2Control;
  //L:TList;	//Deleted by GoldenFox
  i:integer;
  Obj:TObject;
  M:TMethod;
begin
  FEditButton.Visible:=false;
  FEditBox.Visible:=false;
  FComboBox.Visible:=false;
  if (Selected=nil) or (Selected.TagObject=nil) then Exit;

  PropInfo:=GetPropInfo(Selected.TagObject,Selected.TagString);
  if PropInfo=nil then Exit;

  TypeData:=GetTypeData(PropInfo.PropType);
  if TypeData=nil then Exit;

  if FEditButton.TagObject <> nil then
    TD2VisualObject(FEditButton.TagObject).Visible:=true;
  if FEditBox.TagObject <> nil then
    TD2VisualObject(FEditBox.TagObject).Visible:=true;
  if FComboBox.TagObject <> nil then
    TD2VisualObject(FComboBox.TagObject).Visible:=true;
  with PropInfo^ do
  begin
    case PropType^.Kind of
      tkMethod:
        begin
          M:=GetMethodProp(Selected.TagObject,Selected.TagString);
          if GvarD2Designer <> nil then
            FEditBox.TextW:=GvarD2Designer.GetMethodName(M);
          FEditBox.SelectAll;
          Editor:=FEditBox;
        end;
      tkClass:
        begin
          Obj:=GetObjectProp(Selected.TagObject,Selected.TagString);
          if (Obj is TD2Gradient) or (Obj is TD2Brush) or (Obj is TD2Bitmap) or (Obj is TD2PathData) or (Obj is TD2Font) then
          begin
            FEditButton.TextW:='Edit...';
            Editor:=FEditButton;
          end
          else
            Editor:=nil;
        end;
      tkInteger:
        begin
          FEditBox.TextW:=IntToStr(GetOrdProp(Selected.TagObject,Selected.TagString));
          Editor:=FEditBox;
        end;
      tkFloat:
        begin
          FEditBox.TextW:=FloatToStr(GetFloatProp(Selected.TagObject,Selected.TagString));
          Editor:=FEditBox;
        end;
      tkWString:
        begin
          {$IFDEF KS_COMPILER6_UP}
          FEditBox.TextW:=GetWideStrProp(Selected.TagObject,Selected.TagString);
          {$ELSE}
          FEditBox.TextW:=GetStrProp(Selected.TagObject,Selected.TagString);
          {$ENDIF}
          Editor:=FEditBox;
        end;
      tkString,tkLString:
        begin
          FEditBox.TextW:=GetStrProp(Selected.TagObject,Selected.TagString);
          FEditBox.SelectAll;
          Editor:=FEditBox;
        end;
      tkEnumeration:
        begin
          FComboBox.Items.Clear;
          FComboBox.ItemIndex:=-1;
          for i:=TypeData.MinValue to TypeData.MaxValue do
            FComboBox.Items.Add(GetEnumName(PropInfo.PropType,i));
          FComboBox.ItemIndex:=GetOrdProp(Selected.TagObject,Selected.TagString);
          Editor:=FComboBox;
        end;

      tkBool:
        begin
          FComboBox.Items.Clear;
          for i:=TypeData.MinValue to TypeData.MaxValue do
            FComboBox.Items.Add(GetEnumName(PropInfo.PropType,i));
          FComboBox.ItemIndex:=GetOrdProp(Selected.TagObject,Selected.TagString);
          Editor:=FComboBox;
        end;

    else
      FEditBox.TextW:=GetStrProp(Selected.TagObject,Selected.TagString);
      Editor:=FEditBox;
    end;
  end;
  if Editor <> nil then
  begin
    Editor.Visible:=true;
    Editor.Locked:=true;
    Editor.Stored:=false;
    Editor.TagObject:=TObject(Selected.Tag);
    if Editor.TagObject <> nil then
      TD2VisualObject(Editor.TagObject).Visible:=false;
    UpdateEditorPos;
    if Editor.Visible then
      Editor.SetFocus;
  end;
end;

procedure TD2Inspector.DoEditorChange(Sender:TObject);
var
  Obj:TObject;
  F:TFontDialog;
  //S:string;	//Deleted by GoldenFox
  EditDlg:TD2PathDataDesigner;
  //i:integer;	//Deleted by GoldenFox
  //L:TList;	//Deleted by GoldenFox
  PropInfo:PPropInfo;
  M:TMethod;
begin
  if FUpdating > 0 then Exit;

  if (Selected <> nil) and (Selected.TagObject <> nil) then
  begin
    if (Sender=FEditButton) and FEditButton.Visible then
    begin
      { Designer }
      Obj:=GetObjectProp(Selected.TagObject,Selected.TagString);
      if Obj is TD2Brush then
      begin
        SelectInDesign(Obj,TPersistent(SelectedObject));
      end;
      if Obj is TD2Gradient then
      begin
        ShowGradientDialog(TD2Gradient(Obj));
      end;
      if Obj is TD2Bitmap then
      begin
        GvarD2BitmapEditor:=TD2BitmapEditor.Create(nil);
        GvarD2BitmapEditor.AssignFromBitmap(TD2Bitmap(Obj));
        if GvarD2BitmapEditor.ShowModal=mrOk then
        begin
          GvarD2BitmapEditor.AssignToBitmap(TD2Bitmap(Obj));
        end;
        GvarD2BitmapEditor.Free;
      end;
      if Obj is TD2PathData then
      begin
        EditDlg:=TD2PathDataDesigner.Create(Self);
        EditDlg.PathData.Lines.Text:=TD2PathData(Obj).Data;
        if EditDlg.ShowModal=mrOk then
        begin
          TD2PathData(Obj).Data:=EditDlg.PathData.Lines.Text;
        end;
        EditDlg.Free;
      end;
      if Obj is TD2Font then
      begin
        F:=TFontDialog.Create(nil);
        F.Font.Assign(TD2Font(Obj));
        if F.Execute then
        begin
          TD2Font(Obj).Assign(F.Font);
        end;
        F.Free;
      end;
    end;
    if (Sender=FEditBox) and FEditBox.Visible then
    begin
      PropInfo:=GetPropInfo(Selected.TagObject,Selected.TagString);
      if PropInfo^.PropType^.Kind=tkMethod then
      begin
        if GvarD2Designer <> nil then
        begin
          M:=GvarD2Designer.AddMethod(TD2TextBox(Sender).TextW);
          SetMethodProp(Selected.TagObject,PropInfo,M);
          if (M.Code <> nil) and (TD2Object(Sender).TagObject <> nil) then
            TD2Label(TD2Object(Sender).TagObject).TextW:=TD2TextBox(Sender).TextW;
        end;
      end
      else
      begin
        SetPropValue(Selected.TagObject,Selected.TagString,TD2TextBox(Sender).TextW);
        if TD2Object(Sender).TagObject <> nil then
          TD2Label(TD2Object(Sender).TagObject).TextW:=TD2TextBox(Sender).TextW;
      end;
    end;
    if (Sender=FComboBox) and FComboBox.Visible then
    begin
      SetPropValue(Selected.TagObject,Selected.TagString,TD2PopupBox(Sender).ItemIndex);
      if TD2Object(Sender).TagObject <> nil then
        TD2Label(TD2Object(Sender).TagObject).TextW:=TD2PopupBox(Sender).TextW;
    end;
    if Assigned(FOnChangeProperty) then
      FOnChangeProperty(Self,Selected.TagString);
  end;
end;

procedure TD2Inspector.SetSelectedObject(const Value:TComponent);
begin
  if FSelectedObject <> Value then
  begin
    if (Editor <> nil) and (Editor.Visible) and (Editor <> FEditButton) then
      DoEditorChange(Editor);
    if (FSelectedObject <> nil) and (FSelectedObject is TD2Object) then
      TD2Object(FSelectedObject).RemoveFreeNotify(Self);
    FSelectedObject:=Value;
    if (FSelectedObject <> nil) and (FSelectedObject is TD2Object) then
      TD2Object(FSelectedObject).AddFreeNotify(Self);
    RebuildList;
    RebuildEditor;
  end;
end;

procedure TD2Inspector.SetSelected(const Value:TD2TreeViewItem);
begin
  if Value <> Selected then
  begin
    if (Editor <> nil) and (Editor.Visible) and (Editor <> FEditButton) then
      DoEditorChange(Editor);
    inherited;
    RebuildEditor;
  end;
end;

procedure TD2Inspector.SetDisabledProperties(const Value:TStrings);
begin
  FDisabledProperties.Assign(Value);
end;

function TD2Inspector.Editor:TD2Control;
begin
  if FComboBox.Visible then
    Result:=FComboBox
  else
  if FEditBox.Visible then
    Result:=FEditBox
  else
    Result:=FEditButton;
end;

procedure TD2Inspector.VScrollChange(Sender:TObject);
begin
  inherited;
  UpdateEditorPos;
end;

procedure TD2Inspector.UpdateEditorPos;
var
  R:TD2Rect;
begin
  if (Editor <> nil) and (Selected <> nil) and (ContentLayout <> nil) then
  begin
    R:=GetItemRect(Selected);
    R.TopLeft:=FContent.LocalToAbsolute(R.TopLeft);
    R.BottomRight:=FContent.LocalToAbsolute(R.BottomRight);
    R.TopLeft:=AbsoluteToLocal(R.TopLeft);
    R.BottomRight:=AbsoluteToLocal(R.BottomRight);
    with R do
    begin
      Editor.SetBounds(Width / 2 + (Selected.Level * Selected.Height),Top + 1,
        ContentLayout.Width - (Width / 2) - FContent.Position.X + ContentLayout.Position.X - (Selected.Level * Selected.Height) - 1,
        Selected.Height - 2);
    end;
    if Editor.Position.Y < 0 then
      Editor.Opacity:=0
    else
    if Editor.Position.Y + Editor.Height > Height then
      Editor.Opacity:=0
    else
      Editor.Opacity:=1
  end;
end;

procedure TD2Inspector.DialogKey(var Key:Word; Shift:TShiftState);
begin
  inherited;
  if (Count > 0) and (Selected <> nil) and (Scene <> nil) and
     (Scene.GetFocused=Editor) then
  begin
    case Key of
      VK_ADD:Selected.IsExpanded:=true;
      VK_SUBTRACT:Selected.IsExpanded:=false;
      VK_HOME:
        if Editor <> FEditBox then
          Selected:=ItemByGlobalIndex(0)
        else
          Exit;
      VK_END:
        if Editor <> FEditBox then
          Selected:=ItemByGlobalIndex(GlobalCount - 1)
        else
          Exit;
      VK_UP:
        if Selected.GlobalIndex > 0 then
          Selected:=ItemByGlobalIndex(Selected.GlobalIndex - 1);
      VK_DOWN:
        if Selected.GlobalIndex < GlobalCount - 1 then
          Selected:=ItemByGlobalIndex(Selected.GlobalIndex + 1);
    else
      Exit;
    end;
    Key:=0;
  end;
end;

procedure TD2Inspector.SetShowEvents(const Value:boolean);
begin
  if FShowEvents <> Value then
  begin
    FShowEvents:=Value;
    RebuildList
  end;
end;

procedure TD2Inspector.SetShowProperties(const Value:boolean);
begin
  if FShowProperties <> Value then
  begin
    FShowProperties:=Value;
    RebuildList
  end;
end;
         

            
  
