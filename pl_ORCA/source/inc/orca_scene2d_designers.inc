{%mainunit ../orca_scene2d.pas}
            
{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=========== TD2Designer ===============================================
//This is the main Scene2D Designer

procedure TD2Designer.CallDesignSelect(AObject:TObject);
var
  i:integer;
begin
  if FScenes <> nil then
    for i:=0 to FScenes.Count - 1 do
    begin
      Id2Scene(FScenes[i]).DoDesignSelect(AObject);
      Id2Scene(FScenes[i]).AddUpdateRect(d2Rect(0,0,10000,10000));
    end;
end;

function TD2Designer.AddMethod(MethodName:string):TMethod;
begin
  Result.Data:=nil;
  Result.Code:=nil;
end;

function TD2Designer.GetMethodName(Method:TMethod):string;
begin
  Result:='';
end;

procedure TD2Designer.AddObject(AObject:TD2Object);
begin
end;

procedure TD2Designer.DeleteObject(AObject:TD2Object);
begin
end;

procedure TD2Designer.AddScene(const Scene:Id2Scene);
begin
  if FScenes=nil then FScenes:=TList.Create;

  if FScenes.IndexOf(Pointer(Scene)) < 0 then
    FScenes.Add(Pointer(Scene));
end;

procedure TD2Designer.RemoveScene(const Scene:Id2Scene);
begin
  FScenes.Remove(Pointer(Scene));
  if FScenes.Count=0 then FreeAndNil(FScenes);
end;

procedure TD2Designer.EditStyle(const Res:TD2Resources; const ASelected:string);
begin
end;
       
//=============== TD2StyleIDEDesigner ========================================

procedure TD2StyleIDEDesigner.Modified(ADesigner:TComponent);
begin
end;

function TD2StyleIDEDesigner.IsSelected(ADesigner:TComponent; const AObject:TObject):boolean;
begin
  Result:=false;
end;

procedure TD2StyleIDEDesigner.SelectObject(ADesigner:TComponent; AObject:TD2Object; MultiSelection:array of TD2Object);
begin
  if AObject=GvarD2StyleDesigner.DesignScene.Root then Exit;

  GvarD2StyleDesigner.Inspector.SelectedObject:=AObject;
  if GvarD2StyleDesigner.Inspector.SelectedObject is TD2Object then
    TD2Object(GvarD2StyleDesigner.Inspector.SelectedObject).AddFreeNotify(GvarD2StyleDesigner);
  if AObject.TagObject <> nil then
  begin
    GvarD2StyleDesigner.ObjectsTree.Selected:=TD2TreeViewItem(AObject.TagObject);
  end;
end;

function TD2StyleIDEDesigner.UniqueName(ADesigner:TComponent;
  ClassName:string):string;
begin
  Result:='';
end;

function DesignResources(AResource:TD2Resources; Current:string):boolean;
var
  SaveDesigner:TD2Designer;
  S:string;
begin
  Result:=false;
  if AResource <> nil then
  begin
    SaveDesigner:=GvarD2Designer;
    GvarD2Designer:=TD2StyleIDEDesigner.Create(Application);

    GvarD2StyleDesigner:=TD2StyleDesigner.Create(Application);
    {$IFDEF WINDOWS}
    with TRegistry.Create do
    begin
      RootKey:=HKEY_CURRENT_USER;
      if OpenKey('Software\ctorca\GvarD2StyleDesigner',true) then
      begin
        if ReadString('position') <> '' then
        begin
          GvarD2StyleDesigner.Position:=poDesigned;
          with d2StringToRect(ReadString('position')) do
          begin
            GvarD2StyleDesigner.Left:=round(left);
            GvarD2StyleDesigner.Top:=round(top);
            GvarD2StyleDesigner.Width:=round(right);
            GvarD2StyleDesigner.Height:=round(bottom);
          end;
        end;
        if ReadString('left') <> '' then
          GvarD2StyleDesigner.d2Scene1.width:=round(d2StrToFloat(ReadString('left')));
        if ReadString('right') <> '' then
          GvarD2StyleDesigner.d2Scene2.width:=round(d2StrToFloat(ReadString('right')));
      end;
      Free;
    end;
    {$ENDIF}
    Application.ProcessMessages;
    GvarD2StyleDesigner.DesignScene.DesignTime:=true;
    GvarD2StyleDesigner.FResource:=AResource;

    if GvarD2StyleDesigner.DesignScene.Root <> nil then
      GvarD2StyleDesigner.DesignScene.Root.DeleteChildren;

    AResource.FillStrings;
    GvarD2StyleDesigner.LoadFromStrings(AResource.Resource);

    GvarD2StyleDesigner.ObjectsTree.CollapseAll;

    if (GvarD2StyleDesigner.DesignScene.Root <> nil) and (GvarD2StyleDesigner.DesignScene.Root.FindResource(Current) <> nil) then
      GvarD2Designer.SelectObject(nil,GvarD2StyleDesigner.DesignScene.Root.FindResource(Current),[]);

    if GvarD2StyleDesigner.ShowModal=mrOk then
    begin
      GvarD2StyleDesigner.SaveToStrings(AResource.Resource);
      Result:=true;
    end;
    {$IFDEF WINDOWS}
    with TRegistry.Create do
    begin
      RootKey:=HKEY_CURRENT_USER;
      if OpenKey('Software\ctorca\GvarD2StyleDesigner',true) then
      begin
        WriteString('position',d2RectToString(d2Rect(GvarD2StyleDesigner.Left,GvarD2StyleDesigner.Top,GvarD2StyleDesigner.Width,GvarD2StyleDesigner.Height)));
        WriteString('left',d2FloatToStr(GvarD2StyleDesigner.d2Scene1.width));
        WriteString('right',d2FloatToStr(GvarD2StyleDesigner.d2Scene2.width));
      end;
      Free;
    end;
    {$ENDIF}
    GvarD2StyleDesigner.d2Scene1.Free;
    GvarD2StyleDesigner.DesignScene.Free;
    GvarD2StyleDesigner.d2Scene2.Free;
    GvarD2StyleDesigner.d2Scene3.Free;

    GvarD2StyleDesigner.Free;
    GvarD2StyleDesigner:=nil;

    GvarD2Designer.Free;

    GvarD2Designer:=SaveDesigner;
  end;
end;

procedure TD2StyleIDEDesigner.AddObject(AObject:TD2Object);
begin
  inherited;
  GvarD2StyleDesigner.UpdateTree;
end;

procedure TD2StyleIDEDesigner.DeleteObject(AObject:TD2Object);
begin
  inherited;
end;

//=============== TD2StyleDesigner =====================================

procedure TD2StyleDesigner.textFilterChangeTracking(Sender:TObject);
begin
  UpdateTree;
end;

procedure TD2StyleDesigner.UpdateTree;
 procedure ProcessObject(Obj:TD2Object; Node:TD2TreeViewItem);
 var
   i:integer;
   N:TD2TreeViewItem;
   S:AnsiString;
 begin
   S:=Obj.ResourceName;
   if S='' then
     S:=Obj.ClassName
   else
     S:=S + ':' + Obj.ClassName;

   if (textFilter.TextW <> '') and (Obj.Parent=DesignScene.Root) then
   begin
     if Pos(textFilter.TextW,S)=0 then Exit;
   end;

   if Obj=DesignScene.Root then
   begin
     N:=nil;
   end
   else
   if Node <> nil then
   begin
     N:=TD2TreeViewItem.Create(Self);
     N.Parent:=Node;
     N.TextW:=S;
     N.TagObject:=Obj;
     N.Resource:='objecttreeitemstyle';
     N.OnApplyResource:=TreeItemApplyResource;
     if Obj.IsVisual then
       N.IsChecked:=not Obj.Visual.Locked;
   end
   else
   begin
     N:=TD2TreeViewItem.Create(Self);
     N.Parent:=ObjectsTree;
     N.TextW:=S;
     N.TagObject:=Obj;
     N.Resource:='objecttreeitemstyle';
     N.OnApplyResource:=TreeItemApplyResource;
     if Obj.IsVisual then
       N.IsChecked:=not Obj.Visual.Locked;
   end;
   Obj.TagObject:=N;
   if Obj.isVisual and (Obj.Parent=DesignScene.Root) then
   begin
     Obj.Visual.DesignHide:=true;
     // center
     with Obj.Visual do
     begin
       SetBounds(Trunc((DesignScene.Width - Width) / 2),Trunc((DesignScene.Height - Height) / 2),Width,Height);
     end;
   end;
   if Obj.ChildrenCount > 0 then
     for i:=0 to Obj.ChildrenCount - 1 do
     begin
       if (DesignScene.Root is TD2Background) and (Obj.Children[i]=TD2HackBackground(DesignScene.Root).FResourceLink) then Continue;
       ProcessObject(Obj.Children[i],N);
     end;
 end;
begin
  if DesignScene.Root <> nil then
  begin
    ObjectsTree.Clear;
    ObjectsTree.BeginUpdate;
    ProcessObject(DesignScene.Root,nil);
    ObjectsTree.EndUpdate;
    DesignScene.AddUpdateRect(d2Rect(0,0,1000,1000));
  end;
end;

procedure TD2StyleDesigner.TreeItemApplyResource(Sender:TObject);
begin
  TD2TreeViewItem(Sender).Binding['deletebutton']:=EventToVariant(DoDeleteButton);
  TD2TreeViewItem(Sender).Binding['vischeck']:=EventToVariant(DoVisCheck);
  if TD2TreeViewItem(Sender).FindBinding('vischeck') <> nil then
  begin
    TD2Object(TD2TreeViewItem(Sender).FindBinding('vischeck')).TagObject:=TD2TreeViewItem(Sender).TagObject;
    TD2VisualObject(TD2TreeViewItem(Sender).FindBinding('vischeck')).Visible:=TD2TreeViewItem(Sender).TagObject is TD2VisualObject;
    if TD2TreeViewItem(Sender).TagObject is TD2VisualObject then
      TD2TreeViewItem(Sender).Binding['vischeck']:=TD2VisualObject(TD2TreeViewItem(Sender).TagObject).Visible;
  end;
  if not (TD2TreeViewItem(Sender).TagObject is TD2VisualObject) then
    TD2VisualObject(TD2TreeViewItem(Sender).FindResource('check')).Visible:=false;
end;

procedure TD2StyleDesigner.DoVisCheck(Sender:TObject);
begin
  TD2VisualObject(TD2Object(Sender).TagObject).Visible:=TD2Object(Sender).Data;
end;

procedure TD2StyleDesigner.ObjectsTreeChange(Sender:TObject);
var
  New,Old:TD2Object;
begin
  if (ObjectsTree.Selected <> nil) and (ObjectsTree.Selected.TagObject <> nil) then
  begin
    Old:=TD2Object(Inspector.SelectedObject);
    if (Old <> nil) and (Old.Parent <> nil) then
      while Old.Parent <> DesignScene.Root do
        Old:=Old.Parent;
    New:=TD2Object(ObjectsTree.Selected.TagObject);
    while New.Parent <> DesignScene.Root do
      New:=New.Parent;
    if New.IsVisual then
    begin
      // center
      New.Visual.Position.X:=round((DesignScene.Width - New.Visual.Width) / 2);
      New.Visual.Position.Y:=round((DesignScene.Height - New.Visual.Height) / 2);
      //
      if (Old <> nil) and (Old.IsVisual) then
      begin
        if Old <> DesignScene.Root then
          Old.Visual.DesignHide:=true
      end
      else
        if (Old <> nil) and (Old.Parent <> nil) and (Old.Parent.IsVisual) then
        begin
          if Old.Parent <> DesignScene.Root then
            Old.Parent.Visual.DesignHide:=true;
        end;
      if New <> DesignScene.Root then
        New.Visual.DesignHide:=false;
    end
    else
    begin
      if (New <> nil) and (New.Parent <> nil) and (New.Parent.IsVisual) then
      begin
        New.Parent.Visual.DesignHide:=false;
      end;
    end;
    Inspector.SelectedObject:=TD2Object(ObjectsTree.Selected.TagObject);
    if TD2Object(ObjectsTree.Selected.TagObject).isVisual then
      DesignScene.Selected:=TD2VisualObject(ObjectsTree.Selected.TagObject);
  end;
end;

procedure TD2StyleDesigner.Button1Click(Sender:TObject);
var
  S:TStrings;
begin
  if OpenDialog1.Execute then
  begin
    S:=TStringList.Create;
    S.LoadFromFile(OpenDialog1.FileName);
    LoadFromStrings(S);
    S.Free;
  end;
end;

procedure TD2StyleDesigner.Button2Click(Sender:TObject);
var
  S:TStrings;
begin
  if SaveDialog1.Execute then
  begin
    S:=TStringList.Create;
    SaveToStrings(S);
    S.SaveToFile(SaveDialog1.FileName);
    S.Free;
  end;
end;

procedure TD2StyleDesigner.LoadFromStrings(Str:TStrings);
var
  S:TStream;
  i:integer;
  Root:TD2Object;
begin
  S:=TMemoryStream.Create;
  try
    Inspector.SelectedObject:=nil;
    DesignScene.Selected:=nil;
    ObjectsTree.Selected:=nil;

    Str.SaveToStream(S);
    if S.Position > 0 then
    begin
      S.Position:=0;
      if DesignScene.Root <> nil then
        DesignScene.Root.Free;
      Root:=CreateObjectFromStream(nil,S);
      DesignScene.AddObject(Root);
      for i:=0 to Root.ChildrenCount - 1 do
        if Root.Children[i].isVisual then
        begin
        //  TD2VisualObject(Root.FChildren[i]).Position.X:=random(400);
        //  TD2VisualObject(Root.FChildren[i]).Position.Y:=random(400);
          TD2VisualObject(Root.Children[i]).Locked:=false;
        end;
      if Root.ChildrenCount > 0 then
        GvarD2Designer.SelectObject(nil,Root.Children[0],[]);
      UpdateTree;
      DesignScene.RealignRoot;

      if Root is TD2Background then
      begin
        btnBack.Visible:=true;
        rectBack.Fill.Assign(TD2Background(Root).Fill);
      end
      else
        btnBack.Visible:=false;
    end;
  finally
    S.Free;
  end;
end;

procedure TD2StyleDesigner.SaveToStrings(Str:TStrings);
var
  S:TStream;
  i:integer;
  Root:TD2Object;
begin
  // Save style
  S:=TMemoryStream.Create;
  try
    Root:=DesignScene.Root;
    Str.Clear;
    for i:=0 to Root.ChildrenCount - 1 do
      if Root.Children[i].isVisual then
      begin
        TD2VisualObject(Root.Children[i]).Locked:=false;
      end;
    Root.SaveToStream(S);
    S.Position:=0;
    Str.LoadFromStream(S);
  finally
    S.Free;
  end;
end;

procedure TD2StyleDesigner.Button3Click(Sender:TObject);
begin
  if FResource <> nil then
    SaveToStrings(FResource.Resource);
end;

procedure TD2StyleDesigner.btnClearClick(Sender:TObject);
begin
  btnClear.Enabled:=false;
  ClearTimer.Enabled:=true;
end;

procedure TD2StyleDesigner.ClearTimerTimer(Sender:TObject);
var
  R:TD2Background;
begin
  ClearTimer.Enabled:=false;
  Inspector.SelectedObject:=nil;
  DesignScene.Selected:=nil;
  ObjectsTree.Selected:=nil;

  DesignScene.Root.Free;
  R:=TD2Background.Create(nil);
  DesignScene.AddObject(R);

  UpdateTree;
  btnClear.Enabled:=true;
end;

procedure TD2StyleDesigner.btnLoadDefaultClick(Sender:TObject);
var
  i:integer;
  S:TStream;
  Root:TD2Object;
begin
  if GvarD2DefaultStyles=nil then Exit;

  Inspector.SelectedObject:=nil;
  DesignScene.Selected:=nil;
  ObjectsTree.Selected:=nil;
  Root:=nil;


  if System.FindResource(HInstance,PChar('DEFAULTORCASTYLE'),RT_RCDATA) <> 0 then

  begin
    S:=TResourceStream.Create(HInstance,'DEFAULTORCASTYLE',RT_RCDATA);
    Root:=CreateObjectFromBinStream(nil,S);
    S.Free;
  end;
  DesignScene.Root.Free;
  DesignScene.AddObject(Root);
  for i:=0 to Root.ChildrenCount - 1 do
    if Root.Children[i].isVisual then
    begin
      TD2VisualObject(Root.Children[i]).Locked:=false;
    end;

  if Root.ChildrenCount > 0 then
    GvarD2Designer.SelectObject(nil,Root.Children[0],[]);
  UpdateTree;

  DesignScene.RealignRoot;

  if Root is TD2Background then
  begin
    btnBack.Visible:=true;
    rectBack.Fill.Assign(TD2Background(Root).Fill);
  end
  else
    btnBack.Visible:=false;
end;

procedure TD2StyleDesigner.FormCreate(Sender:TObject);
begin
  OpenDialog1.Filter:='ORCA style|*.orcastyle';
  SaveDialog1.Filter:='ORCA style|*.orcastyle';
end;

procedure TD2StyleDesigner.DeleteObject(AObject:TD2Object; FreeObject:boolean);
var
  A,P:TD2Object;
  Sel:TD2TreeViewItem;
begin
  if AObject <> nil then
  begin
    AObject.RemoveFreeNotify(GvarD2StyleDesigner);

    Sel:=TD2TreeViewItem(AObject.TagObject);
    A:=AObject;
    P:=A.Parent;
    if P <> nil then
    begin
      Inspector.SelectedObject:=P;
      if P.TagObject <> nil then
        ObjectsTree.Selected:=TD2TreeViewItem(P.TagObject);
    end
    else
    begin
      Inspector.SelectedObject:=nil;
      if P.TagObject <> nil then
        ObjectsTree.Selected:=nil;
    end;
    Sel.Release;
    if FreeObject then
      A.Release
    else
      A.TagObject:=nil;
    ObjectsTree.Realign;
  end;
end;

procedure TD2StyleDesigner.btnBackClick(Sender:TObject);
begin
  d2BrushDialog1.Brush.Assign(rectBack.Fill);
  if d2BrushDialog1.Execute then
  begin
    rectBack.Fill.Assign(d2BrushDialog1.Brush);
    if (DesignScene.Root <> nil) and (DesignScene.Root is TD2Background) then
      TD2Background(DesignScene.Root).Fill.Assign(d2BrushDialog1.Brush);
    TD2Background(DesignScene.Root).Repaint;
  end;
end;

procedure TD2StyleDesigner.DoDeleteButton(Sender:TObject);
begin
  if ObjectsTree.Selected=nil then Exit;
  if (Sender is TD2SpeedButton) then
  begin// fixed fast click deletebutton
    TD2SpeedButton(Sender).Enabled:=False;
  end;
  DeleteObject(TD2Object(ObjectsTree.Selected.TagObject),true);
end;

procedure TD2StyleDesigner.ObjectsTreeMouseUp(Sender:TObject;
  Button:TMouseButton; Shift:TShiftState; X,Y:single);
begin
  if (Button=mbRight) then
  begin
    DesignScene.OpenDesignPopup;
  end;
end;

procedure TD2StyleDesigner.ObjectsTreeDragChange(SourceItem,
  DestItem:TD2TreeViewItem; Allow:boolean);
begin
  if DestItem=nil then
    TD2Object(SourceItem.TagObject).Parent:=DesignScene.Root
  else
    TD2Object(SourceItem.TagObject).Parent:=TD2Object(DestItem.TagObject)
end;

procedure TD2StyleDesigner.InspectorChangeProperty(Sender:TObject;
  PropertyName:String);
begin
  if GvarD2StyleDesigner.Inspector.SelectedObject=nil then Exit;
  if TD2TreeViewItem(TD2Object(GvarD2StyleDesigner.Inspector.SelectedObject).TagObject)=nil then Exit;

  if PropertyName='ResourceName' then
  begin
    TD2TreeViewItem(TD2Object(GvarD2StyleDesigner.Inspector.SelectedObject).TagObject).TextW :=
      GetPropValue(GvarD2StyleDesigner.Inspector.SelectedObject,PropertyName) + ':' + GvarD2StyleDesigner.Inspector.SelectedObject.ClassName;
  end;
end;

procedure TD2StyleDesigner.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  // Fixed to circumvent SIGSEGV en AV exceptions in the IDE.
//inherited ;           // Don't inherit first:object may become destroyed  *** DB ***
  if (Operation=opRemove)                                   and  
     (AComponent is TD2Object)                                and 
     Assigned(TD2Object(AComponent).Scene)                    and 
     (TD2Object(AComponent).Scene.GetComponent=DesignScene)     then
    DeleteObject(TD2Object(AComponent),false)                            // *** DB ***
  else                                                                    // *** DB ***
    inherited;          // In stead only inherit if object not deleted       *** DB ***             
end;

procedure TD2StyleDesigner.ObjectsTreeChangeCheck(Sender:TObject);
begin
  if (ObjectsTree.Selected <> nil) and (ObjectsTree.Selected.TagObject <> nil) and (TD2Object(ObjectsTree.Selected.TagObject).IsVisual) then
  begin
    TD2VisualObject(ObjectsTree.Selected.TagObject).Locked:=not ObjectsTree.Selected.IsChecked;
  end;
end;

procedure TD2StyleDesigner.DesignRootDragOver(Sender:TObject; const Data:TD2DragObject; const Point:TD2Point; var Accept:boolean);
begin
  Accept:=true;
end;

//==================== TfrmDsgnImageList ================================== 
             
procedure TfrmDsgnImageList.btnAddFilesClick(Sender:TObject);
var
  i:integer;
begin
  OpenDialog1.Filter:=GvarD2DefaultFilterClass.GetFileTypes;
  if OpenDialog1.Execute then
  begin
    for i:=0 to OpenDialog1.Files.Count - 1 do
      ImageList.AddFile(OpenDialog1.Files[i]);
  end;
end;

procedure TfrmDsgnImageList.btnClearClick(Sender:TObject);
begin
  ImageList.Clear;
end;

procedure TfrmDsgnImageList.btnDeleteClick(Sender:TObject);
begin
  if ImageList.Selected <> nil then
    ImageList.Selected.Free;
end;

//==================== TD2LangDesigner ==================================

procedure TD2LangDesigner.RebuildOriginalList;
var
  i:integer;
  Str:TD2WideStrings;
  Item:TD2ListboxItem;
begin
  OriginalList.Clear;
  if FLang.Original.Count=0 then
  begin
    // create original from Collection
    CollectLangStart;
    UpdateLang;
    FLang.Original.Assign(CollectLangStrings);
    CollectLangFinish;
  end;
  Str:=FLang.Original;
  for i:=0 to Str.Count - 1 do
  begin
    Item:=TD2ListboxItem.Create(Self);
    Item.AutoTranslate:=false;
    Item.Resource:='langitem';
    Item.TextW:=Str[i];
    Item.TextAlign:=d2TextAlignCenter;
    Item.Height:=22;
    Item.Parent:=OriginalList;
    if (FLang.Resources.Count > 0) and (langList.ItemIndex >= 0) then
    begin
      if FLang.LangStr[langList.TextW] <> nil then
      begin
        Item.Binding['translate']:=FLang.LangStr[langList.TextW].Values[Str[i]];
        Item.Binding['translate']:=EventToVariant(DoTranslateChanged);
      end;
    end else
    begin
      if Item.FindBinding('translate')<>nil then
       Item.FindBinding('translate').Visual.Visible:=false;
    end;
  end;
end;

procedure TD2LangDesigner.btnAddClick(Sender:TObject);
var
  List:TD2ListBox;
begin
  { add new lang }
  List:=TD2ListBox.Create(Self);
end;

procedure TD2LangDesigner.DoTranslateChanged(Sender:TObject);
begin
  if (FLang.LangStr[langList.TextW] <> nil) and (OriginalList.Selected <> nil) then
    with FLang.LangStr[langList.TextW] do
    begin
      Values[OriginalList.Selected.TextW]:=TD2TextBox(Sender).TextW;
    end;
end;

procedure TD2LangDesigner.btnAddLangClick(Sender:TObject);
var
  S:string;
begin
  if inputLang.TextW='' then Exit;
  S:=inputLang.TextW;
  if Length(S) > 2 then
    Delete(S,3,MaxInt);
  FLang.AddLang(S);
  langList.Items:=FLang.Resources;
  langList.ItemIndex:=langList.Items.IndexOf(S);
  RebuildOriginalList;

  layoutAdd.Visible:=false;
  layoutSelect.Visible:=true;
end;

procedure TD2LangDesigner.langListChange(Sender:TObject);
begin
  RebuildOriginalList;
end;

procedure TD2LangDesigner.btnAddNewLangClick(Sender:TObject);
begin
  layoutAdd.Visible:=true;
  layoutSelect.Visible:=false;
  btnCancalAdd.Visible:=langList.Items.Count > 0;

  inputLang.TextW:='';
  inputLang.SetFocus;
end;

procedure TD2LangDesigner.btnCancalAddClick(Sender:TObject);
begin
  if langList.Items.Count > 0 then
  begin
    layoutAdd.Visible:=false;
    layoutSelect.Visible:=true;
  end;
end;

procedure TD2LangDesigner.btnAddItemClick(Sender:TObject);
begin
  { Add Word }
  layoutAdd.Visible:=false;
  layoutSelect.Visible:=false;
  layoutAddText.Visible:=true;
  inputAddText.TextW:='';
  inputAddText.SetFocus;
  RebuildOriginalList;
end;

procedure TD2LangDesigner.btnRemoveItemClick(Sender:TObject);
begin
  { Remove Word }
  if OriginalList.ItemIndex >= 0 then
  begin
    FLang.Original.Delete(OriginalList.ItemIndex);
    RebuildOriginalList;
  end;
end;

procedure TD2LangDesigner.btnCancelAddTextClick(Sender:TObject);
begin
  layoutAdd.Visible:=langList.Items.Count=0;
  layoutSelect.Visible:=langList.Items.Count > 0;
  layoutAddText.Visible:=false;
end;

procedure TD2LangDesigner.btnAddTextClick(Sender:TObject);
begin
  btnCancelAddTextClick(Sender);
  FLang.Original.Add(inputAddText.TextW);
  RebuildOriginalList;
  OriginalList.ItemIndex:=OriginalList.Count - 1;
end;

procedure TD2LangDesigner.btnCollectClick(Sender:TObject);
var
  Str:TD2WideStrings;
  i:integer;
begin
  CollectLangStart;
  UpdateLang;
  Str:=TD2WideStringList.Create;
  Str.Assign(CollectLangStrings);
  for i:=0 to Str.Count - 1 do
    if FLang.Original.IndexOf(Str[i]) < 0 then
      FLang.Original.Add(Str[i]);
  Str.Free;
  CollectLangFinish;
  RebuildOriginalList;
end;

procedure TD2LangDesigner.btnCreateTemplateClick(Sender:TObject);
var
  Str:TD2WideStrings;
  i:integer;
begin
  if SaveDialog1.Execute then
  begin
    Str:=TD2WideStringList.Create;
    Str.Assign(FLang.Original);
    for i:=0 to Str.Count - 1 do
      Str[i]:=Str[i] + '=';
    Str.SaveToFile(SaveDialog1.FileName);
    Str.Free;
  end;
end;

procedure TD2LangDesigner.btnLoadTxtClick(Sender:TObject);
var
  Str:TD2WideStrings;
  i:integer;
begin
  if OpenDialog1.Execute then
  begin
    FLang.AddLang(inputLang.TextW);
    langList.Items:=FLang.Resources;
    langList.ItemIndex:=langList.Items.IndexOf(inputLang.TextW);
    RebuildOriginalList;

    layoutAdd.Visible:=false;
    layoutSelect.Visible:=true;

    Str:=TD2WideStringList.Create;
    Str.LoadFromFile(OpenDialog1.FileName);
    for i:=0 to Str.Count - 1 do
      if FLang.LangStr[langList.TextW].IndexOfName(Str.Names[i]) < 0 then
        FLang.LangStr[langList.TextW].Add(Str[i])
      else
        FLang.LangStr[langList.TextW].Values[Str.Names[i]]:=Str.Values[Str.Names[i]];
    Str.Free;
    RebuildOriginalList;
  end;
end;

procedure TD2LangDesigner.btnLoadLngClick(Sender:TObject);
begin
  if OpenDialog2.Execute then
  begin
    FLang.LoadFromFile(OpenDialog2.FileName);
    RebuildOriginalList;
  end;
end;

procedure TD2LangDesigner.btnSaveLngClick(Sender:TObject);
begin
  if SaveDialog2.Execute then
  begin
    FLang.SaveToFile(SaveDialog2.FileName);
  end;
end;

//======================== TD2PathDataDesigner ==========================

procedure TD2PathDataDesigner.PathDataChange(Sender:TObject);
begin
  if previewLayout.Visible then
    previewPath.Data.Data:=PathData.TextW;
end;

procedure TD2PathDataDesigner.Button2Click(Sender:TObject);
begin
  ModalResult:=mrOk;
end;

procedure TD2PathDataDesigner.Button3Click(Sender:TObject);
begin
  ModalResult:=mrCancel;
end;

procedure TD2PathDataDesigner.Button1Click(Sender:TObject);
begin
  pathData.SelectAll;
  pathData.ClearSelection;
  pathData.PasteFromClipboard;
end;
         
//=============== TD2BrushDesign ====================================================

procedure TD2BrushDesign.FormCreate(Sender:TObject);
begin
  //---
end;

procedure TD2BrushDesign.FormClose(Sender:TObject;
  var Action:TCloseAction);
begin
  Action:=caFree;
  GvarD2BrushDesign:=nil;
end;

procedure TD2BrushDesign.Notification(AComponent:TComponent;
  Operation:TOperation);
begin
  inherited;
  if (Operation=opRemove) and (AComponent=FComp) then
  begin
    Brush:=nil;
    Comp:=nil;
  end;
end;

procedure TD2BrushDesign.SetBrush(const Value:TD2Brush);
var
  i:integer;
begin
  FBrush:=Value;
  if FBrush <> nil then
  begin
    case FBrush.Style of
      d2BrushNone:
        begin
          brushTabControl.ItemIndex:=tabNone.Index;
        end;
      d2BrushSolid:
        begin
          solidPicker.Color:=FBrush.SolidColor;
          gradEditor.Gradient.Assign(FBrush.Gradient);
          brushTabControl.ItemIndex:=tabSolid.Index;
        end;
      d2BrushGradient:
        begin
          solidPicker.Color:=FBrush.SolidColor;
          gradEditor.Gradient.Assign(FBrush.Gradient);
          gradKind.ItemIndex:=Integer(gradEditor.Gradient.Style);
          if gradEditor.Gradient.StopPosition.X - gradEditor.Gradient.StartPosition.X <> 0 then
          begin
            if gradEditor.Gradient.StopPosition.X - gradEditor.Gradient.StartPosition.X > 0 then
              gradAngle.Value:=-d2RadToDeg(ArcTan((gradEditor.Gradient.StopPosition.Y - gradEditor.Gradient.StartPosition.Y) / (gradEditor.Gradient.StopPosition.X - gradEditor.Gradient.StartPosition.X)))
            else
              gradAngle.Value:=-d2RadToDeg(ArcTan((gradEditor.Gradient.StopPosition.Y - gradEditor.Gradient.StartPosition.Y) / (gradEditor.Gradient.StopPosition.X - gradEditor.Gradient.StartPosition.X))) - 180;
          end;
          gradAngle.Visible:=gradEditor.Gradient.Style=d2LinearGradient;
          gradQuadChange(Self);
          brushTabControl.ItemIndex:=tabGradient.Index;
        end;
      d2BrushVisual:;
      d2BrushBitmap:
        begin
          brushTabControl.ItemIndex:=tabBitmap.Index;
          bitmapImage.Bitmap.Assign(FBrush.Bitmap.Bitmap);
          tileModeList.ItemIndex:=Integer(FBrush.Bitmap.WrapMode);
        end;
      d2BrushResource:
        begin
          brushTabControl.ItemIndex:=tabRes.Index;
          rebuilResList;
        end;
    end;
    for i:=0 to brushList.Count - 1 do
      if (brushList.ItemByIndex(i) <> nil) and (TD2Brush(brushList.ItemByIndex(i).Tag)=Brush) then
        brushList.ItemIndex:=i;
  end;
end;

procedure TD2BrushDesign.SetComp(const Value:TPersistent);
var
  i:integer;
  BrushButton:TD2ListBoxItem;
  BrushText:TD2TextControl;
  PropCount:integer;
  PropList:PPropList;
  PropType:PTypeData;
begin
  FComp:=Value;
  if (FComp <> nil) and (FComp is TD2Scene) then
    FScene:=TD2Scene(FComp);

  if (FComp <> nil) and (FComp is TD2Object) then
    FScene:=TD2Object(FComp).Scene;
  { find all brushes }
  Brush:=nil;
  brushList.Clear;
  brushList.Height:=4;
  {$IFDEF KS_COMPILER5}
  PropCount:=GetPropList(FComp.ClassInfo,[tkClass],nil);
  GetMem(PropList,SizeOf(PPropInfo) * PropCount);
  PropCount:=GetPropList(FComp.ClassInfo,[tkClass],PropList);
  {$ELSE}
  PropCount:=GetPropList(FComp.ClassInfo,PropList);
  {$ENDIF}
  for i:=0 to PropCount - 1 do
  begin
    if PropList[i].PropType^.Kind <> tkClass then Continue;
    PropType:=GetTypeData(PropList[i].PropType);
    if PropType=nil then Continue;
    if not (PropType.ClassType.ClassName='TD2Brush') then Continue;
    BrushButton:=TD2ListBoxItem.Create(Self);
    BrushButton.Parent:=brushList;
    BrushButton.Height:=23;
    BrushButton.Tag:=Integer(GetObjectProp(FComp,PropList[i].Name));
    BrushText:=TD2Label.Create(Self);
    BrushText.Parent:=BrushButton;
    BrushText.Align:=vaClient;
    BrushText.HitTest:=false;
    BrushText.TextW:=PropList[i].Name;
    brushList.Height:=brushList.Height + BrushButton.Height;
    if Brush=nil then
      Brush:=TD2Brush(GetObjectProp(FComp,PropList[i].Name));
  end;
  brushList.ItemIndex:=0;
  FreeMem(PropList,SizeOf(PPropInfo) * PropCount);
end;

function TD2BrushDesign.UniqueName(S:string):string;
begin
  if (FComp <> nil) and
     (FComp is TComponent) and
     (TComponent(FComp).Owner <> nil) and
     (TComponent(FComp).Owner is TCustomForm) and
     (TCustomForm(TComponent(FComp).Owner).Designer<> nil) then
       Result:=TCustomForm(TComponent(FComp).Owner).Designer.UniqueName(S)
  else
  if Designer <> nil then
    Result:=Designer.UniqueName(S)
  else
  begin
    Tag:=Tag + 1;
    Result:=S + IntToStr(Tag);
  end;
end;

procedure TD2BrushDesign.solidQuadChange(Sender:TObject);
begin
  if FBrush=nil then Exit;
  solidQuad.Alpha:=((FBrush.SolidColor and $FF000000) shr 24) / $FF;
  FBrush.SolidColor:=(FBrush.SolidColor and not $FFFFFF) or ($00FFFFFF and d2HSLtoRGB(solidQuad.Hue,solidQuad.Sat,solidQuad.Lum));
  textSolidR.Value:=TD2ColorRec(FBrush.SolidColor).R;
  textSolidG.Value:=TD2ColorRec(FBrush.SolidColor).G;
  textSolidB.Value:=TD2ColorRec(FBrush.SolidColor).B;
  textSolidA.Value:=TD2ColorRec(FBrush.SolidColor).A;
  textSolidHex.TextW:=d2ColorToStr(FBrush.SolidColor);
end;

procedure TD2BrushDesign.textSolidHexChange(Sender:TObject);
begin
  { change solid hex }
  if FBrush=nil then Exit;
  FBrush.SolidColor:=d2StrToColor(textSolidHex.TextW);
  solidPicker.Color:=FBrush.SolidColor;
end;

procedure TD2BrushDesign.textSolidRChange(Sender:TObject);
var
  Color:TD2Color;
begin
  { solid textbox change }
  if FBrush=nil then Exit;
  Color:=FBrush.SolidColor;
  TD2ColorRec(Color).R:=trunc(textSolidR.Value);
  TD2ColorRec(Color).G:=trunc(textSolidG.Value);
  TD2ColorRec(Color).B:=trunc(textSolidB.Value);
  TD2ColorRec(Color).A:=trunc(textSolidA.Value);
  FBrush.SolidColor:=Color;
  solidPicker.Color:=FBrush.SolidColor;
end;

procedure TD2BrushDesign.gradEditorChange(Sender:TObject);
begin
  { change gradient }
  if FBrush=nil then Exit;
  FBrush.Gradient.Assign(gradEditor.Gradient);
end;

procedure TD2BrushDesign.gradQuadChange(Sender:TObject);
begin
  { chage color in current point }
  if FBrush=nil then Exit;
  gradEditor.Gradient.Points[gradEditor.CurrentPoint].IntColor :=
    (gradEditor.Gradient.Points[gradEditor.CurrentPoint].IntColor and $FF000000) or ($00FFFFFF and d2HSLtoRGB(gradQuad.Hue,gradQuad.Sat,gradQuad.Lum));
  FBrush.Gradient.Assign(gradEditor.Gradient);
  textGradR.Value:=TD2ColorRec(gradColorRect.Color).R;
  textGradG.Value:=TD2ColorRec(gradColorRect.Color).G;
  textGradB.Value:=TD2ColorRec(gradColorRect.Color).B;
  textGradA.Value:=TD2ColorRec(gradColorRect.Color).A;
  textGradHex.TextW:=d2ColorToStr(gradColorRect.Color);
  gradEditor.Repaint;
end;

procedure TD2BrushDesign.brushListChange(Sender:TObject);
begin
  if FScene=nil then Exit;
  if FComp=nil then Exit;
  if Sender <> nil then
    Brush:=TD2Brush(TD2ListBoxItem(Sender).Tag);
end;

procedure TD2BrushDesign.brushTabControlChange(Sender:TObject);
begin
  if FBrush=nil then Exit;
  if brushTabControl.ItemIndex=tabNone.Index then
    FBrush.Style:=d2BrushNone;
  if brushTabControl.ItemIndex=tabSolid.Index then
    FBrush.Style:=d2BrushSolid;
  if brushTabControl.ItemIndex=tabGradient.Index then
  begin
    FBrush.Style:=d2BrushGradient;
    gradQuadChange(Sender);
    gradAngleLabel.TextW:=InttoStr(Trunc(gradAngle.Value));
  end;
  if brushTabControl.ItemIndex=tabBitmap.Index then
    FBrush.Style:=d2BrushBitmap;
  if brushTabControl.ItemIndex=tabRes.Index then
    FBrush.Style:=d2BrushResource;

  btnMakeRes.Visible:=(brushTabControl.ItemIndex <> tabRes.Index) and (brushTabControl.ItemIndex <> tabNone.Index);
  if not btnMakeRes.Visible then
    rebuilResList;
end;

procedure TD2BrushDesign.textGradRChange(Sender:TObject);
var
  Color:TD2Color;
begin
  { change grad brush alpha }
  if FBrush=nil then Exit;
  Color:=gradEditor.Gradient.Points[gradEditor.CurrentPoint].IntColor;
  TD2ColorRec(Color).R:=trunc(textGradR.Value);
  TD2ColorRec(Color).G:=trunc(textGradG.Value);
  TD2ColorRec(Color).B:=trunc(textGradB.Value);
  TD2ColorRec(Color).A:=trunc(textGradA.Value);
  gradEditor.Gradient.Points[gradEditor.CurrentPoint].IntColor:=Color;
  gradEditor.UpdateGradient;
end;

procedure TD2BrushDesign.gradKindChange(Sender:TObject);
begin
  { change grad type }
  if FBrush=nil then Exit;
  gradEditor.Gradient.Style:=TD2GradientStyle(gradKind.ItemIndex);
  gradEditor.UpdateGradient;
  gradAngle.Visible:=gradEditor.Gradient.Style=d2LinearGradient;
end;

procedure TD2BrushDesign.gradAngleChange(Sender:TObject);
var
  Color:TD2Color;
  X,Y,Koef:single;
begin
  { change grad brush alpha }
  if FBrush=nil then Exit;
  if (Cos(d2DegToRad(gradAngle.Value)) <> 0) and (Abs(1 / Cos(d2DegToRad(gradAngle.Value))) >= 1) and (Abs(1 / Cos(d2DegToRad(gradAngle.Value))) <= 1.42) then
    X:=Abs(1 / Cos(d2DegToRad(gradAngle.Value)))
  else
    X:=1;
  if (Sin(d2DegToRad(gradAngle.Value)) <> 0) and (Abs(1 / Sin(d2DegToRad(gradAngle.Value))) >= 1) and (Abs(1 / Sin(d2DegToRad(gradAngle.Value))) <= 1.42) then
    Y:=Abs(1 / Sin(d2DegToRad(gradAngle.Value)))
  else
    Y:=1;
  Koef:=d2MaxFloat(X,Y);
  Koef:=Koef * 0.5;
  gradEditor.Gradient.StartPosition.Point:=d2Point(0.5 - (Cos(d2DegToRad(gradAngle.Value)) * Koef),0.5 + (Sin(d2DegToRad(gradAngle.Value)) * Koef));
  gradEditor.Gradient.StopPosition.Point:=d2Point(0.5 + (Cos(d2DegToRad(gradAngle.Value)) * Koef),0.5 - (Sin(d2DegToRad(gradAngle.Value)) * Koef));
  gradEditor.UpdateGradient;
  gradAngleLabel.TextW:=InttoStr(Trunc(gradAngle.Value));
end;

procedure TD2BrushDesign.textGradHexChange(Sender:TObject);
begin
  { change gradient hex }
  if FBrush=nil then Exit;
  gradEditor.Gradient.Points[gradEditor.CurrentPoint].IntColor:=d2StrToColor(textGradHex.TextW);
  gradEditor.UpdateGradient;
end;

procedure TD2BrushDesign.btnSelectBitmapClick(Sender:TObject);
begin
  if FBrush=nil then Exit;
  GvarD2BitmapEditor:=TD2BitmapEditor.Create(nil);
  GvarD2BitmapEditor.AssignFromBitmap(bitmapImage.Bitmap);
  if GvarD2BitmapEditor.ShowModal=mrOk then
  begin
    GvarD2BitmapEditor.AssignToBitmap(bitmapImage.Bitmap);
    FBrush.Bitmap.Bitmap.Assign(bitmapImage.Bitmap);
    if (FComp <> nil) and (FComp is TD2VisualObject) then
      TD2VisualObject(FComp).Repaint;
    bitmapImage.Repaint;
  end;
  GvarD2BitmapEditor.Free;
end;

procedure TD2BrushDesign.rebuilResList;
var
  i:integer;
  L:TList;
  item:TD2ListBoxItem;
  itemText:TD2TextControl;
  rect:TD2Rectangle;
begin
  if FScene=nil then Exit;
  if FScene.GetRoot=nil then Exit;
  if FBrush=nil then Exit;

  resList.Clear;

  L:=TList.Create;
  FScene.GetRoot.AddObjectsToList(L);
  for i:=0 to L.Count - 1 do
    if TD2Object(L[i]) is TD2BrushObject then
    begin
      item:=TD2ListBoxItem.Create(Self);
      item.Tag:=Integer(L[i]);
      item.Parent:=resList;
      itemText:=TD2Label.Create(Self);
      itemText.Parent:=item;
      itemText.Align:=vaClient;
      itemText.HitTest:=false;
      itemText.TextW:=TD2Object(L[i]).ResourceName;
      rect:=TD2Rectangle.Create(Self);
      rect.Parent:=item;
      rect.Align:=vaLeft;
      rect.HitTest:=false;
      rect.Padding.Rect:=d2Rect(2,2,2,2);
      rect.Fill.Style:=d2BrushResource;
      rect.Fill.Resource.Resource:=TD2BrushObject(L[i]);
      rect.Stroke.Color:='#80FFFFFF';
      if FBrush.Resource.Resource=TD2BrushObject(L[i]) then
        resList.ItemIndex:=resList.Count - 1;
    end;
  L.Free;
end;

procedure TD2BrushDesign.resListChange(Sender:TObject);
begin
  if FScene=nil then Exit;
  if FScene.GetRoot=nil then Exit;
  if FBrush=nil then Exit;
  if Sender=nil then Exit;

  FBrush.Assign(TD2BrushObject(TD2ListBoxItem(Sender).Tag).Brush);
  FBrush.Resource.Resource:=TD2BrushObject(TD2ListBoxItem(Sender).Tag);
  FBrush.Style:=d2BrushResource;
end;

procedure TD2BrushDesign.btnMakeResClick(Sender:TObject);
var
  S:string;
  B:TD2BrushObject;
begin
  if FBrush=nil then Exit;
  if FScene=nil then Exit;
  if FScene.GetRoot=nil then Exit;

  { make res }
  S:=UniqueName('Brush');
  if InputQuery('New TD2BrushObject','Enter resource name:',S) then
  begin
    B:=TD2BrushObject.Create(FScene.GetOwner);
    B.Parent:=FScene.GetRoot;
    B.ResourceName:=S;
    B.Name:=B.ResourceName;
    B.ResourceName:=S;
    B.Brush.Assign(FBrush);

    rebuilResList;
  end;
end;

procedure TD2BrushDesign.tileModeListChange(Sender:TObject);
begin
  if FBrush=nil then Exit;
  FBrush.Bitmap.WrapMode:=TD2WrapMode(tileModeList.ItemIndex);
  if (FComp <> nil) and (FComp is TD2VisualObject) then
    TD2VisualObject(FComp).Repaint;
end;

procedure TD2BrushDesign.btnOKClick(Sender:TObject);
begin
  ModalResult:=mrOk;
end;

procedure TD2BrushDesign.btnCancelClick(Sender:TObject);
begin
  ModalResult:=mrCancel;
end;

//=================== TD2BitmapEditor ===============================

procedure TD2BitmapEditor.FormCreate(Sender:TObject);
begin
  resizeLayout.Visible:=false;
  FBitmap:=TD2Bitmap.Create(1,1);
  FOldScale:=1;
end;

procedure TD2BitmapEditor.FormDestroy(Sender:TObject);
begin
  FreeAndNil(FBitmap);
end;

procedure TD2BitmapEditor.Button1Click(Sender:TObject);
var
  D:TOpenDialog;
begin
  D:=TOpenDialog.Create(Application);
  try
    D.Filter:='Images|' + GvarD2DefaultFilterClass.GetFileTypes;
    if D.Execute then
    begin
      FileName:=D.FileName;
      FBitmap.LoadFromFile(D.FileName);
      Preview.Repaint;
      FSourceRect:=d2Rect(0,0,FBitmap.Width,FBitmap.Height);
      Preview.Width:=FBitmap.Width * trackScale.Value;
      Preview.Height:=FBitmap.Height * trackScale.Value;
      ScrollBox1.Realign;

      editControl.Enabled:=true;
      btnOk.Enabled:=true;
    end;
  finally
    D.Free;
  end;
end;

procedure TD2BitmapEditor.PreviewPaint(Sender:TObject; const Canvas:TD2Canvas);
begin
  Canvas.DrawBitmap(FBitmap,FSourceRect,d2Rect(0,0,d2RectWidth(FSourceRect) * trackScale.Value,d2RectHeight(FSourceRect) * trackScale.Value),1);
end;

procedure TD2BitmapEditor.trackScaleChange(Sender:TObject);
begin
  Preview.Width:=FBitmap.Width * trackScale.Value;
  Preview.Height:=FBitmap.Height * trackScale.Value;
  ScrollBox1.Realign;

  if FCropRect <> nil then
  begin
    FCropRect.Position.X:=(FCropRect.Position.X / FOldScale) * trackScale.Value;
    FCropRect.Position.Y:=(FCropRect.Position.Y / FOldScale) * trackScale.Value;
    FCropRect.Width:=(FCropRect.Width / FOldScale) * trackScale.Value;
    FCropRect.Height:=(FCropRect.Height / FOldScale) * trackScale.Value;
  end;

  FOldScale:=trackScale.Value;
  labelScale.TextW:='Scale:' + IntToStr(Round(trackScale.Value * 100)) + '%';
end;

procedure TD2BitmapEditor.cropButtonClick(Sender:TObject);
begin
  if FCropRect=nil then
  begin
    btnFitClick(Self);

    FCropRect:=TD2Selection.Create(Self);
    FCropRect.Parent:=Preview;
    cropButton.TextW:='Full';
    FCropRect.SetBounds(Trunc(FBitmap.Width * 0.1 * trackScale.Value),Trunc(FBitmap.Height * 0.1 * trackScale.Value),
      (FBitmap.Width * 0.8 * trackScale.Value),Trunc(FBitmap.Height * 0.8 * trackScale.Value));
  end
  else
  begin
    FreeAndNil(FCropRect);
    Preview.Repaint;
    cropButton.TextW:='Crop';
  end;
end;

procedure TD2BitmapEditor.btnOkClick(Sender:TObject);
begin
  ModalResult:=mrOk;
end;

procedure TD2BitmapEditor.AssignFromBitmap(B:TD2Bitmap);
begin
  FileName:='';
  if B <> nil then
    FBitmap.Assign(B);
  FSourceRect:=d2Rect(0,0,FBitmap.Width,FBitmap.Height);
  Preview.Width:=FBitmap.Width * trackScale.Value;
  Preview.Height:=FBitmap.Height * trackScale.Value;
  ScrollBox1.Realign;
  if B.Width > 1 then
  begin
    editControl.Enabled:=true;
    btnOk.Enabled:=true;
  end;
end;

procedure TD2BitmapEditor.AssignToBitmap(B:TD2Bitmap);
var
  j:integer;
  CR:TD2Rect;
begin
  if B <> nil then
  begin
    if (FCropRect <> nil) and (FCropRect.Width > 0) and (FCropRect.Height > 0) then
    begin
      CR:=d2Rect(FCropRect.Position.X / trackScale.Value,FCropRect.Position.Y / trackScale.Value,
        (FCropRect.Position.X + FCropRect.Width) / trackScale.Value,(FCropRect.Position.Y + FCropRect.Height) / trackScale.Value);
      B.SetSize(Trunc(d2RectWidth(CR)),Trunc(d2RectHeight(CR)));
      for j:=0 to B.Height - 1 do
        d2MoveLongword(@FBitmap.Scanline[(Trunc(CR.Top) + j)][Trunc(CR.Left)],@B.Scanline[j][0],B.Width);
      B.BitmapChanged;
    end
    else
    begin
      B.Assign(FBitmap);
    end;
  end;
end;

procedure TD2BitmapEditor.Button2Click(Sender:TObject);
begin
  ModalResult:=mrCancel;
end;

procedure TD2BitmapEditor.trackScaleMouseDown(Sender:TObject;
  Button:TMouseButton; Shift:TShiftState; X,Y:single);
begin
  if ssDouble in Shift then trackScale.Value:=1;
end;

procedure TD2BitmapEditor.btnPasteClick(Sender:TObject);
var
  B:TBitmap;
begin
  B:=TBitmap.Create;
  B.PixelFormat:=pf32bit;
  B.HandleType:=bmDIB;
  B.Assign(Clipboard);
  FBitmap.Assign(B);
  B.Free;
end;

procedure TD2BitmapEditor.btnFitClick(Sender:TObject);
var
  R:TD2Rect;
begin
  R:=d2Rect(0,0,FBitmap.Width,FBitmap.Height);
  trackScale.Value:=1 / d2FitRect(R,ScrollBox1.LocalRect);
end;

procedure TD2BitmapEditor.btnOriginalClick(Sender:TObject);
begin
  trackScale.Value:=1;
end;

procedure TD2BitmapEditor.Button3Click(Sender:TObject);
begin
  editControl.Enabled:=false;
  btnOk.Enabled:=true;
  if FCropRect <> nil then cropButtonClick(Self);

  trackScale.Value:=1;
  FBitmap.SetSize(1,1);
  FSourceRect:=d2Rect(0,0,FBitmap.Width,FBitmap.Height);
  Preview.Width:=FBitmap.Width * trackScale.Value;
  Preview.Height:=FBitmap.Height * trackScale.Value;
  ScrollBox1.Realign;
end;

procedure TD2BitmapEditor.btnResizeClick(Sender:TObject);
begin
  newWidth.Value:=FBitmap.Width;
  newHeight.Value:=FBitmap.Height;

  resizeLayout.Visible:=true;
  resizeLayout.Position.Y:=-resizeLayout.Height;
  resizeLayout.AnimateFloat('Position.Y',0,0.3);
end;

procedure TD2BitmapEditor.Button4Click(Sender:TObject);
var
  tmp:TD2Bitmap;
  SaveFileName:string;
begin
  SaveFileName:=FileName;
  tmp:=TD2Bitmap.Create(trunc(newWidth.Value),trunc(newHeight.Value));
  tmp.Canvas.DrawBitmap(FBitmap,d2Rect(0,0,FBitmap.Width,FBitmap.Height),d2Rect(0,0,tmp.Width,tmp.Height),1);
  AssignFromBitmap(tmp);
  tmp.Free;
  FileName:=SaveFileName;

  resizeLayout.AnimateFloatWait('Position.Y',-resizeLayout.Height,0.3);
  resizeLayout.Visible:=false;
end;

procedure TD2BitmapEditor.Button5Click(Sender:TObject);
begin
  resizeLayout.AnimateFloatWait('Position.Y',-resizeLayout.Height,0.3);
  resizeLayout.Visible:=false;
end;

procedure TD2BitmapEditor.newWidthChange(Sender:TObject);
begin
  newHeight.Value:=Round(newWidth.Value * (FBitmap.Height / FBitmap.Width));
end;

procedure TD2BitmapEditor.newHeightChange(Sender:TObject);
begin
  newWidth.Value:=Round(newHeight.Value * (FBitmap.Width / FBitmap.Height));
end;

procedure TD2BitmapEditor.btnSaveClick(Sender:TObject);
begin
  SaveDialog1.FileName:=FFileName;
  if SaveDialog1.Execute then
    FBitmap.SaveToFile(SaveDialog1.FileName);
end;

//=============== TD2DesignFrame ==================================

constructor TD2DesignFrame.Create(AOwner:TComponent);
begin
  inherited;
end;

destructor TD2DesignFrame.Destroy;
begin
  inherited;
end;

procedure TD2DesignFrame.MouseDown(Button:TMouseButton;
  Shift:TShiftState; X,Y:single);
begin
  inherited;
end;

procedure TD2DesignFrame.MouseLeave;
begin
  inherited;
end;

procedure TD2DesignFrame.MouseMove(Shift:TShiftState; X,Y,Dx,
  Dy:single);
begin
  inherited;
end;

procedure TD2DesignFrame.MouseUp(Button:TMouseButton; Shift:TShiftState;
  X,Y:single);
begin
  inherited;
end;

procedure TD2DesignFrame.Paint;
begin

end;

              
             
                                 
